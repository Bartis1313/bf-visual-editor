#include "textures.h"
#include "texgen.h"
#include "surfacepick.h"
#include "../lights/lights.h"
#include "../../hooks/functions.h"
#include "../../utils/log.h"

#include "../editor_context.h"

#include <Windows.h>
#include <d3d11.h>
#include <imgui.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cctype>
#include <cstring>
#include <atomic>
#include <deque>
#include <format>
#include <fstream>
#include <filesystem>
#include <functional>
#include <mutex>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>

#include "textures_internal.h"
namespace editor::textures
{
    using namespace detail;
    namespace detail
    {
        uint8_t* liveInstance(uint64_t setKey, uint32_t materialIndex);
        bool materialHasColor(const MaterialEntry& m);
        std::string materialLabel(const MaterialEntry& m);
    }

    namespace detail
    {
#if !defined(BFVE_GAME_BF4)
        std::unordered_map<std::string, void*> g_bf3TextureByName; // lowercase path -> DxTexture*
        std::unordered_map<std::string, void*> g_bf3ResourceByName;
        void* g_bf3MeshSetVtable = nullptr; // most common "_mesh" vtable
#endif

        bool isDxTexture(const void* p)
        {
            return p && static_cast<const fb::DxTexture*>(p)->m_vtable == fb::DxTexture::VTable();
        }

        std::unordered_set<const void*> streamedTextureSet()
        {
            std::unordered_set<const void*> out;
            fb::TextureStreamingManager* mgr = fb::TextureStreamingManager::Singleton();
            if (!mgr)
                return out;
#if defined(BFVE_GAME_BF4)
            for (const fb::TextureStreamingEntry& e : mgr->m_entries)
                if (e.m_texture)
                    out.insert(e.m_texture);
#else
            if (mgr->m_begin && mgr->m_end >= mgr->m_begin)
            {
                const size_t n = (std::min)(size_t(mgr->m_end - mgr->m_begin), size_t(0x2000));
                for (size_t i = 0; i < n; ++i)
                    if (mgr->m_begin[i].m_texture)
                        out.insert(mgr->m_begin[i].m_texture);
            }
#endif
            return out;
        }

        bool copyEngineString(const char* src, char* dst, size_t cap)
        {
            if (!src)
            {
                dst[0] = 0;
                return false;
            }
            strncpy_s(dst, cap, src, _TRUNCATE);
            return dst[0] != 0;
        }

        const char* srvDimensionName(void* srv, uint32_t& format)
        {
            format = 0;
            if (!srv)
                return "no shader view";

            D3D11_SHADER_RESOURCE_VIEW_DESC d{};
            static_cast<ID3D11ShaderResourceView*>(srv)->GetDesc(&d);
            format = uint32_t(d.Format);
            switch (d.ViewDimension)
            {
            case D3D11_SRV_DIMENSION_TEXTURE2D: return "TEXTURE2D";
            case D3D11_SRV_DIMENSION_TEXTURE2DARRAY: return "TEXTURE2DARRAY";
            case D3D11_SRV_DIMENSION_TEXTURE2DMS: return "TEXTURE2DMS";
            case D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY: return "TEXTURE2DMSARRAY";
            case D3D11_SRV_DIMENSION_TEXTURECUBE: return "TEXTURECUBE";
            case D3D11_SRV_DIMENSION_TEXTURECUBEARRAY: return "TEXTURECUBEARRAY";
            case D3D11_SRV_DIMENSION_TEXTURE3D: return "TEXTURE3D";
            case D3D11_SRV_DIMENSION_TEXTURE1D: return "TEXTURE1D";
            case D3D11_SRV_DIMENSION_BUFFER: return "BUFFER";
            default: return "unknown";
            }
        }

        bool srvIsDrawable2D(void* srv)
        {
            if (!srv)
                return false;

            D3D11_SHADER_RESOURCE_VIEW_DESC desc{};
            static_cast<ID3D11ShaderResourceView*>(srv)->GetDesc(&desc);
            return desc.ViewDimension == D3D11_SRV_DIMENSION_TEXTURE2D;
        }

        void addRefSrv(void* srv)
        {
            if (srv)
                static_cast<IUnknown*>(srv)->AddRef();
        }

        void releaseSrv(void* srv)
        {
            if (srv)
                static_cast<IUnknown*>(srv)->Release();
        }

        void* createOwnSrv(const TextureEntry& e)
        {
            if (!g_pDevice || !e.resource)
                return nullptr;

            ID3D11Texture2D* tex2d = nullptr;
            const HRESULT hr = static_cast<IUnknown*>(e.resource)->QueryInterface(
                __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&tex2d));
            if (FAILED(hr) || !tex2d)
                return nullptr;

            D3D11_TEXTURE2D_DESC td{};
            tex2d->GetDesc(&td);

            // imgui: single-slice non-msaa 2d only
            if (td.ArraySize != 1 || td.SampleDesc.Count != 1)
            {
                tex2d->Release();
                return nullptr;
            }

            D3D11_SHADER_RESOURCE_VIEW_DESC sd{};
            sd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            sd.Texture2D.MipLevels = td.MipLevels ? td.MipLevels : 1;
            // typeless: view format is m_shaderFormat
            sd.Format = (e.shaderFormat != 0) ? DXGI_FORMAT(e.shaderFormat) : td.Format;

            ID3D11ShaderResourceView* srv = nullptr;
            if (FAILED(g_pDevice->CreateShaderResourceView(tex2d, &sd, &srv)))
            {
                sd.Format = td.Format;
                if (FAILED(g_pDevice->CreateShaderResourceView(tex2d, &sd, &srv)))
                    srv = nullptr;
            }

            tex2d->Release();
            return srv;
        }

        void* resolveAssetTexture(void* textureAsset);
        bool liveAsset(const void* textureAsset);
        void readTextureMeta(TextureEntry& e);
        void ensureDrawable(TextureEntry& e, bool markShown);

        // handles are 15 bits, 0xFFFF = not streamed
        bool streamingTarget(void* dxTexture, fb::TextureStreamingManager*& mgr, uint16_t& handle)
        {
            mgr = nullptr;
            handle = 0xFFFF;

            if (!dxTexture)
                return false;
            mgr = fb::TextureStreamingManager::Singleton();
            if (!mgr)
                return false;

            handle = asTex(dxTexture)->m_handle;
            return (handle & 0x7FFF) == handle;
        }

        // 0 = not loaded, 1 = loading, 2 = loaded, 3 = not an on-demand texture
        int onDemandStatus(void* dxTexture)
        {
            fb::TextureStreamingManager* mgr = nullptr;
            uint16_t handle = 0;
            if (!streamingTarget(dxTexture, mgr, handle))
                return -1;

            return mgr->getOnDemandStatus(handle);
        }

        std::mutex pinMutex;
        std::unordered_set<uint16_t> pinnedHandles;
        std::deque<uint16_t> pinOrder;

        uint32_t frameCounter = 0;
        std::unordered_map<uint16_t, int> handleToEntry;

        std::mutex loadMutex;
        std::vector<uint16_t> loadQueue;
        std::unordered_set<uint16_t> loadQueued;
        std::atomic<uint32_t> loadsAsked{ 0 };
        std::atomic<uint32_t> loadsDone{ 0 };
        std::atomic<bool> budgetHit{ false };

        bool liveTexture(const void* texture)
        {
            static std::unordered_set<const void*> live;
            static uint32_t builtFrame = ~0u;
            if (builtFrame != frameCounter)
            {
                live = streamedTextureSet();
                builtFrame = frameCounter;
            }
            return texture && live.count(texture) != 0;
        }

        bool callLoad(fb::TextureStreamingManager* mgr, uint16_t handle)
        {
            return mgr->loadOnDemand(handle, true);
        }

        bool callUnload(fb::TextureStreamingManager* mgr, uint16_t handle)
        {
            return mgr->unloadOnDemand(handle, true);
        }

        uint32_t originalBudgetCap = 0;

        uint32_t* budgetCapSlot(uint32_t& scale)
        {
#if defined(BFVE_GAME_BF4)
            scale = 1;
            fb::TextureStreamingManager* mgr = fb::TextureStreamingManager::Singleton();
            return mgr ? &mgr->m_onDemandBudgetCap : nullptr;
#else
            scale = 1024u * 1024u;
            fb::TextureStreamingSettings* st = streamingSettings();
            return st ? &st->m_OnDemandPoolSize : nullptr;
#endif
        }

        bool readBudget(uint32_t& used, uint32_t& cap)
        {
            fb::TextureStreamingManager* mgr = fb::TextureStreamingManager::Singleton();
            uint32_t scale = 1;
            uint32_t* slot = budgetCapSlot(scale);
            if (!mgr || !slot)
                return false;
            used = mgr->m_onDemandBudgetUsed;
            cap = *slot * scale;
            return true;
        }

        bool setBudgetCap(uint32_t bytes)
        {
            uint32_t scale = 1;
            uint32_t* slot = budgetCapSlot(scale);
            if (!slot)
                return false;

            const uint32_t cur = *slot * scale;
            if (!originalBudgetCap)
                originalBudgetCap = cur;

            *slot = bytes / scale;

            budgetHit = false;
            logger::info("[textures] on-demand budget cap {} MB -> {} MB",
                cur / (1024 * 1024), bytes / (1024 * 1024));
            return true;
        }

        void restoreBudgetCap()
        {
            if (!originalBudgetCap)
                return;
            uint32_t scale = 1;
            if (uint32_t* slot = budgetCapSlot(scale))
                *slot = originalBudgetCap / scale;
            originalBudgetCap = 0;
        }

        bool releaseHandle(uint16_t handle)
        {
            fb::TextureStreamingManager* mgr = fb::TextureStreamingManager::Singleton();
            if (!mgr)
                return false;

            if (auto it = handleToEntry.find(handle); it != handleToEntry.end() &&
                it->second >= 0 && it->second < int(entries.size()) &&
                entries[it->second].texture && gen::hasOverride(entries[it->second].texture))
            {
                std::lock_guard<std::mutex> lock(pinMutex);
                pinnedHandles.erase(handle);
                return false;
            }

            const bool unloaded = callUnload(mgr, handle);

            std::lock_guard<std::mutex> lock(pinMutex);
            pinnedHandles.erase(handle);
            return unloaded;
        }

        bool loadHandle(uint16_t handle)
        {
            fb::TextureStreamingManager* mgr = fb::TextureStreamingManager::Singleton();
            if (!mgr)
                return false;

            if (!callLoad(mgr, handle))
                return false;

            uint16_t evict = 0xFFFF;
            {
                std::lock_guard<std::mutex> lock(pinMutex);
                if (pinnedHandles.insert(handle).second)
                {
                    pinOrder.push_back(handle);
                    while (!keepAllLoaded && pinOrder.size() > size_t(maxPins))
                    {
                        const uint16_t old = pinOrder.front();
                        pinOrder.pop_front();
                        if (old != handle && pinnedHandles.count(old))
                        {
                            evict = old;
                            break;
                        }
                    }
                }
            }
            if (evict != 0xFFFF)
                releaseHandle(evict);

            return true;
        }

        std::atomic<bool> bulkLoadActive{ false };
        std::unordered_set<uint16_t> bulkAsked;

        uint32_t residentTextures = 0;

        void refreshResidency()
        {
            uint32_t live = 0;
            for (TextureEntry& e : entries)
            {
                if (!e.texture)
                    continue;

                void* res = asTex(e.texture)->m_resource;
                if (!res)
                    continue;

                e.resource = res;
                e.loaded = true;
                ++live;
            }
            residentTextures = live;
        }

        bool isPinned(uint16_t handle)
        {
            std::lock_guard<std::mutex> lock(pinMutex);
            return pinnedHandles.count(handle) != 0;
        }

        void queueLoad(uint16_t handle)
        {
            if (budgetHit.load())
                return;

            if (auto it = handleToEntry.find(handle); it != handleToEntry.end() &&
                it->second >= 0 && it->second < int(entries.size()) &&
                entries[it->second].texture && gen::hasOverride(entries[it->second].texture))
            {
                static std::unordered_set<uint16_t> said;
                if (said.insert(handle).second)
                    logger::info("[textures] not streaming {} on demand - it carries an edit",
                                 entries[it->second].lowerPath);
                return;
            }

            std::lock_guard<std::mutex> lock(loadMutex);
            if (!loadQueued.insert(handle).second)
                return;
            loadQueue.push_back(handle);
        }

        // drained on a worker
        void pumpLoads()
        {
            constexpr size_t LOAD_BATCH = 16;

            std::vector<uint16_t> batch;
            {
                std::lock_guard<std::mutex> lock(loadMutex);
                if (loadQueue.empty())
                    return;

                const size_t n = (std::min)(loadQueue.size(), LOAD_BATCH);
                batch.assign(loadQueue.begin(), loadQueue.begin() + ptrdiff_t(n));
                loadQueue.erase(loadQueue.begin(), loadQueue.begin() + ptrdiff_t(n));
                for (uint16_t h : batch)
                    loadQueued.erase(h);
            }

            uint32_t refused = 0;
            for (uint16_t h : batch)
            {
                if (!loadHandle(h))
                    ++refused;
                ++loadsDone;
            }

            // whole batch refused = streaming budget spent
            if (refused == batch.size() && batch.size() >= 8)
            {
                budgetHit = true;
                const bool wasBulk = bulkLoadActive.exchange(false);

                static uint64_t lastReport = 0;
                const uint64_t now = GetTickCount64();
                if (now - lastReport > 5000)
                {
                    lastReport = now;
                    uint32_t used = 0, cap = 0;
                    readBudget(used, cap);
                    logger::warning("[textures] on-demand budget full: {} / {} MB - {}",
                                    used / (1024 * 1024), cap / (1024 * 1024),
                                    wasBulk ? "bulk load stopped; raise the on-demand "
                                              "pool in TextureStreamingSettings to fit more"
                                            : "recycling least recently shown textures");
                }
            }
        }

        void raiseBudgetIfWanted()
        {
            if (!autoRaiseBudget)
                return;

            uint32_t used = 0, cap = 0;
            if (!readBudget(used, cap))
                return;

            const uint32_t want = uint32_t(budgetTargetMb) * 1024u * 1024u;
            if (cap >= want)
                return;

            setBudgetCap(want);
        }

        enum class QueueResult { Skip, Queued, Resolved, NoTexture };

        QueueResult queueEntry(TextureEntry& e)
        {
            if (e.resource || e.srvLinear)
                return QueueResult::Skip;

            bool justResolved = false;
            if (!e.texture && e.asset)
            {
                if (void* tex = resolveAssetTexture(e.asset))
                {
                    e.texture = tex;
                    e.loaded = true;
                    readTextureMeta(e);
                    if (!e.lowerPath.empty())
                        textureNames.emplace(tex, e.lowerPath);
                    justResolved = true;
                }
            }

            fb::TextureStreamingManager* mgr = nullptr;
            uint16_t handle = 0;
            if (!streamingTarget(e.texture, mgr, handle))
                return QueueResult::NoTexture;

            e.handle = handle;
            handleToEntry[handle] = int(&e - entries.data());
            if (isPinned(handle))
                return QueueResult::Skip;

            if (budgetHit.load())
                return QueueResult::Skip;

            if (!bulkAsked.insert(handle).second)
                return QueueResult::Skip;

            queueLoad(handle);
            return justResolved ? QueueResult::Resolved : QueueResult::Queued;
        }

        void queueEveryMissing()
        {
            budgetHit = false;
            raiseBudgetIfWanted();

            uint32_t queued = 0, resolved = 0, unresolvable = 0;
            bulkAsked.clear();
            bulkLoadActive = true;

            for (TextureEntry& e : entries)
            {
                switch (queueEntry(e))
                {
                case QueueResult::Queued: ++queued; break;
                case QueueResult::Resolved: ++resolved; ++queued; break;
                case QueueResult::NoTexture: ++unresolvable; break;
                default: break;
                }
            }

            loadsDone = 0;
            loadsAsked = queued;
            logger::info("[textures] queued {} of {} catalogued texture(s) for loading "
                         "({} asset(s) resolved on the way, {} with no runtime texture)",
                queued, entries.size(), resolved, unresolvable);
        }

        void cancelLoads()
        {
            size_t dropped = 0;
            {
                std::lock_guard<std::mutex> lock(loadMutex);
                dropped = loadQueue.size();
                loadQueue.clear();
                loadQueued.clear();
            }
            loadsAsked = loadsDone.load();
            bulkLoadActive = false;
            bulkAsked.clear();
            if (dropped)
                logger::info("[textures] cancelled {} queued load(s)", dropped);
        }

        void releaseAllPins()
        {
            {
                std::lock_guard<std::mutex> lock(loadMutex);
                loadQueue.clear();
                loadQueued.clear();
            }

            handleToEntry.clear();

            std::vector<uint16_t> pins;
            {
                std::lock_guard<std::mutex> lock(pinMutex);
                pins.assign(pinnedHandles.begin(), pinnedHandles.end());
                pinnedHandles.clear();
                pinOrder.clear();
            }
            if (pins.empty())
                return;

            fb::TextureStreamingManager* mgr = fb::TextureStreamingManager::Singleton();
            uint32_t released = 0;
            if (mgr)
                for (uint16_t h : pins)
                    if (callUnload(mgr, h))
                        ++released;

            restoreBudgetCap();
            budgetHit = false;
            loadsDone = 0;
            logger::debug("[textures] released {} of {} pinned texture(s)", released, pins.size());
        }

        void recyclePins(uint32_t keepWithinFrames = 30, size_t maxEvict = 64)
        {
            std::vector<uint16_t> stale;
            {
                std::lock_guard<std::mutex> lock(pinMutex);
                for (size_t i = 0; i < pinOrder.size() && stale.size() < maxEvict; ++i)
                {
                    const uint16_t h = pinOrder[i];
                    if (!pinnedHandles.count(h))
                        continue;

                    auto it = handleToEntry.find(h);
                    if (it != handleToEntry.end() && it->second >= 0 &&
                        it->second < int(entries.size()))
                    {
                        const TextureEntry& e = entries[it->second];
                        if (frameCounter - e.lastDrawn < keepWithinFrames)
                            continue;
                    }
                    stale.push_back(h);
                }
            }

            for (uint16_t h : stale)
            {
                auto it = handleToEntry.find(h);
                if (it != handleToEntry.end() && it->second >= 0 &&
                    it->second < int(entries.size()))
                {
                    TextureEntry& e = entries[it->second];
                    if (e.drawable)
                        releaseSrv(e.srvLinear);
                    e.srvLinear = nullptr;
                    e.resource = nullptr;
                    e.drawable = false;
                    e.viewChecked = false;
                    e.ownsSrv = false;
                }
                releaseHandle(h);
            }

            {
                std::lock_guard<std::mutex> lock(pinMutex);
                for (auto it = pinOrder.begin(); it != pinOrder.end(); )
                    it = pinnedHandles.count(*it) ? it + 1 : pinOrder.erase(it);
            }

            if (!stale.empty())
                budgetHit = false;
        }


        void ensureDrawable(TextureEntry& e, bool markShown)
        {
            if (markShown)
                e.lastDrawn = frameCounter;

            if (e.drawable)
                return;

            if (!e.texture && e.asset)
            {
                if (void* tex = resolveAssetTexture(e.asset))
                {
                    e.texture = tex;
                    e.loaded = true;
                    readTextureMeta(e);
                    if (!e.lowerPath.empty())
                        textureNames.emplace(tex, e.lowerPath);
                }
            }
            if (!e.texture)
                return;

            if (!liveTexture(e.texture))
            {
                e.texture = nullptr;
                e.loaded = false;
                return;
            }
            if (e.vtable != fb::DxTexture::VTable())
                return;

            const fb::DxTexture* tp = asTex(e.texture);
            if (tp->m_shaderViews[0])
                e.srvLinear = tp->m_shaderViews[0];
            if (tp->m_resource)
                e.resource = tp->m_resource;
            e.srvGamma = tp->m_shaderViews[1];
            e.width = tp->m_width;
            e.height = tp->m_height;
            e.mips = tp->m_mipmapCount;
            e.shaderFormat = tp->m_shaderFormat;

            if (srvIsDrawable2D(e.srvLinear))
            {
                addRefSrv(e.srvLinear);
                e.drawable = true;
                e.viewChecked = true;
                return;
            }

            // no srv bind flag = no engine views
            if (void* own = createOwnSrv(e))
            {
                e.srvLinear = own;
                e.ownsSrv = true;
                e.drawable = true;
                e.viewChecked = true;
                return;
            }

            if (autoLoadMissing && !e.resource && !e.srvLinear)
            {
                fb::TextureStreamingManager* mgr = nullptr;
                uint16_t handle = 0;
                if (streamingTarget(e.texture, mgr, handle))
                {
                    e.handle = handle;
                    handleToEntry[handle] = int(&e - entries.data());
                    if (!isPinned(handle))
                        queueLoad(handle);
                }
            }

        }

        void releaseHeldSrvs()
        {
            for (TextureEntry& e : entries)
                if (e.drawable)
                {
                    releaseSrv(e.srvLinear);
                    e.drawable = false;
                }
        }

        uint32_t shaderParamHandle(const char* s)
        {
            uint32_t h = 5381;
            for (; *s; ++s)
            {
                uint32_t c = static_cast<uint8_t>(*s);
                if (c - 65u <= 25u)
                    c += 32;
                h = c ^ (33u * h);
            }
            return h;
        }

        bool paramInfo(const void* block, uint32_t slot, uint32_t& handle, uint16_t& offset)
        {
            if (!block)
                return false; // no declared params = no block, sub_140C1C360
            const auto* b = static_cast<const fb::ShaderParameterBlock*>(block);
            handle = b->m_entries[slot].m_handle;
            offset = b->m_entries[slot].m_offset;
            return true;
        }

        uint8_t* paramValue(void* block, uint16_t offset)
        {
            return reinterpret_cast<uint8_t*>(static_cast<fb::ShaderParameterBlock*>(block)->values()) + offset;
        }

        void writeVec(void* block, uint16_t offset, const float* v) { std::memcpy(paramValue(block, offset), v, 16); }
        void readVec(void* block, uint16_t offset, float* v) { std::memcpy(v, paramValue(block, offset), 16); }
        void writeTex(void* block, uint16_t offset, void* t) { *reinterpret_cast<void**>(paramValue(block, offset)) = t; }
        void* readTex(void* block, uint16_t offset) { return *reinterpret_cast<void**>(paramValue(block, offset)); }

        bool blockIsSane(const void* block, uint8_t& vecOut, uint8_t& texOut, uint8_t& boolOut)
        {
            const auto* b = static_cast<const fb::ShaderParameterBlock*>(block);
            const uint8_t vec = b->m_vectorCount, tex = b->m_textureCount, bol = b->m_boolCount;

            const uint32_t total = uint32_t(vec) + tex + bol;
            if (total == 0 || total > 128)
                return false;

#if defined(BFVE_GAME_BF4)
            uint32_t head = 8u * total + 16u;
            if (vec)
                head = (8u * total + 31u) & ~0xFu;

            const uint32_t size = uint32_t(bol) + 8u * (uint32_t(tex) + 2u * vec) + head;
            if (b->size() != size)
                return false;
#else
            // bf3: values after the info array
            if (b->size() < 16u + 8u * total + 16u * vec + 4u * tex + bol)
                return false;
#endif

            vecOut = vec;
            texOut = tex;
            boolOut = bol;
            return true;
        }

        struct SetRef { uint8_t* set; uint64_t key; };

        // set: count +0x18, instances +0x10 stride 0x50, block +0x08
        uint32_t setMaterialCount(const uint8_t* set)
        {
            return *reinterpret_cast<const uint32_t*>(set + layout.countOffset);
        }

        uint8_t* setMaterials(const uint8_t* set)
        {
            return *reinterpret_cast<uint8_t* const*>(set + layout.materialsOffset);
        }

        uint8_t* setBlock(const uint8_t* materials, uint32_t index)
        {
            return *reinterpret_cast<uint8_t* const*>(
                materials + 1ull * index * layout.instanceStride + layout.blockOffset);
        }

        void collectSets(std::vector<SetRef>& out)
        {
            fb::MeshVariationManager* mgr = fb::MeshVariationManager::Singleton();
            if (!mgr)
            {
                logger::warning("[textures] g_meshVariationManager is null - not in a level?");
                return;
            }

            stats.elementCount = mgr->m_elementCount;

            if (!mgr->m_buckets || mgr->m_bucketCount == 0)
                return;

            for (uint32_t i = 0; i < mgr->m_bucketCount; ++i)
            {
                uint32_t guard = 0;
                for (fb::MeshVariationNode* node = mgr->m_buckets[i]; node && guard++ < 4096;
                     node = node->m_next)
                {
                    if (node->m_set)
                        out.push_back({ static_cast<uint8_t*>(node->m_set), node->m_key });
                }
            }
        }

        uint32_t addName(const char* name)
        {
            char buf[128];
            if (!copyEngineString(name, buf, sizeof(buf)))
                return 0;

            const uint32_t handle = shaderParamHandle(buf);
            handleNames.emplace(handle, buf);
            return handle;
        }

        void addBoolNames(fb::Array<fb::BoolShaderParameter>& a)
        {
            for (fb::BoolShaderParameter& p : a)
                addName(p.m_ParameterName);
        }

        void addVectorNames(fb::Array<fb::VectorShaderParameter>& a,
                            std::vector<uint32_t>* handlesOut = nullptr)
        {
            for (fb::VectorShaderParameter& p : a)
            {
                const uint32_t handle = addName(p.m_ParameterName);
                if (!handle)
                    continue;
                if (handlesOut)
                    handlesOut->push_back(handle);
                if (uint32_t(p.m_ParameterType) == 6u)
                    colorHandles.insert(handle);
            }
        }

        void addTextureNames(fb::Array<fb::TextureShaderParameter>& a)
        {
            for (fb::TextureShaderParameter& p : a)
                addName(p.m_ParameterName);
        }

        void addShaderNames(fb::SurfaceShaderInstanceDataStruct& s,
                            std::vector<uint32_t>* vectorHandlesOut = nullptr)
        {
            addBoolNames(s.m_BoolParameters);
            addVectorNames(s.m_VectorParameters, vectorHandlesOut);
            addTextureNames(s.m_TextureParameters);
        }

        struct EbxMaterial
        {
            std::vector<std::pair<uint32_t, std::string>> textures;
            std::string shaderName;
            void* material = nullptr;
            void* dbTexParams = nullptr;
            uint32_t dbTexCount = 0;
            std::vector<uint32_t> variationHandles;
            bool hasVariation = false;
        };
        struct EbxEntry { std::string meshName; std::vector<EbxMaterial> materials; };

        std::unordered_map<uint64_t, EbxEntry> g_ebx;

        std::string assetName(const void* asset)
        {
            if (!asset)
                return {};
            char buf[256];
            if (!copyEngineString(static_cast<const fb::Asset*>(asset)->m_Name, buf, sizeof(buf)))
                return {};
            return buf;
        }

        void harvestEbx()
        {
            g_ebx.clear();
            variationNames.clear();
            colorHandles.clear();
            uint32_t materialsSeen = 0;
            uint32_t variationsSeen = 0;

            fb::ResourceManager* rm = fb::ResourceManager::GetInstance();
            if (!rm)
                return;

            for (const auto& comp : rm->m_compartments)
            {
                if (!comp) continue;

                for (const auto& obj : comp->m_objects)
                {
                    if (!obj) continue;

                    fb::ClassInfo* classInfo = fb::classOf(obj);
                    if (!classInfo)
                        continue;

                    const char* className = classInfo->m_InfoData ? classInfo->m_InfoData->m_Name : nullptr;
                    if (!className)
                        continue;

                    if (strcmp(className, "ObjectVariation") == 0)
                    {
                        auto* ov = reinterpret_cast<fb::ObjectVariation*>(obj);
                        if (ov->m_NameHash)
                        {
                            std::string name = assetName(obj);
                            if (!name.empty())
                                variationNames.emplace(ov->m_NameHash, std::move(name));
                        }
                        continue;
                    }

                    if (strcmp(className, "MeshVariationDatabase") != 0)
                        continue;

                    auto* db = reinterpret_cast<fb::MeshVariationDatabase*>(obj);
#if defined(BFVE_GAME_BF4)
                    for (fb::MeshVariationDatabaseEntry& entry : db->m_Entries)
                    {
#else
                    for (fb::MeshVariationDatabaseEntry* entryPtr : db->m_Entries)
                    {
                        if (!entryPtr)
                            continue;
                        fb::MeshVariationDatabaseEntry& entry = *entryPtr;
#endif
                        fb::MeshAsset* mesh = entry.m_Mesh;
                        if (!mesh || !mesh->m_NameHash)
                            continue;

                        const uint64_t key = uint64_t(entry.m_VariationAssetNameHash)
                                           | (uint64_t(mesh->m_NameHash) << 32);

                        EbxEntry e;
                        e.meshName = assetName(mesh);

                        for (fb::MeshVariationDatabaseMaterial& mat : entry.m_Materials)
                        {
                            EbxMaterial em;

                            // MeshMaterial is not a partition primary
                            if (mat.m_Material)
                            {
                                addShaderNames(mat.m_Material->m_Shader);
                                ++materialsSeen;
                                if (fb::SurfaceShaderBaseAsset* graph = mat.m_Material->m_Shader.m_Shader)
                                    em.shaderName = assetName(graph);
                                em.material = mat.m_Material;
                                em.dbTexParams = mat.m_TextureParameters.m_firstElement;
                                em.dbTexCount = mat.m_TextureParameters.m_firstElement ? mat.m_TextureParameters.size() : 0;
                            }

                            // variation carries the lamp ColorTint
                            if (mat.m_MaterialVariation)
                            {
                                em.hasVariation = true;
                                addShaderNames(mat.m_MaterialVariation->m_Shader, &em.variationHandles);
                                ++variationsSeen;
                            }

                            for (fb::TextureShaderParameter& tp : mat.m_TextureParameters)
                            {
                                if (!tp.m_Value)
                                    continue;

                                char nameBuf[128];
                                if (!copyEngineString(tp.m_ParameterName, nameBuf, sizeof(nameBuf)))
                                    continue;

                                std::string path = assetName(tp.m_Value);
                                if (!path.empty())
                                    em.textures.emplace_back(shaderParamHandle(nameBuf), std::move(path));
                            }
                            e.materials.push_back(std::move(em));
                        }

                        g_ebx.emplace(key, std::move(e));
                    }
                }
            }

            stats.ebxEntries = uint32_t(g_ebx.size());
            stats.namesFound = uint32_t(handleNames.size());
            logger::info("[textures] ebx: {} entries, {} MeshMaterials, {} variations, "
                          "{} param names ({} colors), {} variation names",
                g_ebx.size(), materialsSeen, variationsSeen, handleNames.size(),
                colorHandles.size(), variationNames.size());
        }

        constexpr const char* KNOWN_PARAMS[] = {
            "CamoBackground", "CamoColor1", "CamoColor2", "CamoColor3", "CamoColor4",
            "ColorTint", "DiffuseTint", "Color", "TintColor", "BaseColor", "DiffuseColor",
            "Lasercolor",
            "Emissive", "EmissiveColor", "SpecularColor", "GlowColor", "MainColor",
            "AccentColor", "PaintColor", "UntintedColor", "FacadeColor",
            "TeamColor1", "TeamColor2",
            "Diffuse", "Normal", "SpecSmooth", "Specular", "Camo", "Mask",
            "Opacity", "Alpha", "Roughness", "Metallic",
            "UVScale", "UVOffset", "UVTiling", "DetailScale", "NormalScale",
        };

        void harvestNames()
        {
            handleNames.clear();

            for (const char* n : KNOWN_PARAMS)
                handleNames.emplace(shaderParamHandle(n), n);

            fb::ResourceManager* rm = fb::ResourceManager::GetInstance();
            if (!rm)
            {
                logger::warning("[textures] no ResourceManager - parameter names unavailable");
                return;
            }

            uint32_t seen = 0;
            for (const auto& comp : rm->m_compartments)
            {
                if (!comp) continue;

                for (const auto& obj : comp->m_objects)
                {
                    if (!obj) continue;

                    fb::ClassInfo* classInfo = fb::classOf(obj);
                    if (!classInfo)
                        continue;

                    const char* className = classInfo->m_InfoData ? classInfo->m_InfoData->m_Name : nullptr;
                    if (!className || strcmp(className, "MeshMaterial") != 0)
                        continue;

                    ++seen;
                    addShaderNames(reinterpret_cast<fb::MeshMaterial*>(obj)->m_Shader);
                }
            }

            stats.namesFound = uint32_t(handleNames.size());
            logger::debug("[textures] harvestNames: {} names from {} MeshMaterial primaries (0 is expected)",
                handleNames.size(), seen);
        }

        const char* nameForHandle(uint32_t handle)
        {
            auto it = handleNames.find(handle);
            return it == handleNames.end() ? nullptr : it->second.c_str();
        }

        const char* typeName(uint32_t t);
        const TextureEntry* findTextureEntry(void* texture);
        bool matchesSearch(const std::string& haystack, const char* needleRaw);
        void ensureNameCache(TextureEntry& e)
        {
            if (e.nameCached)
                return;
            e.nameCached = true;

            auto it = textureNames.find(e.texture);
            if (it != textureNames.end())
            {
                e.lowerPath = it->second;
                const size_t slash = e.lowerPath.find_last_of("/\\");
                e.shortName = (slash == std::string::npos) ? e.lowerPath
                                                           : e.lowerPath.substr(slash + 1);
            }
            else
            {
                e.shortName = std::format("{}x{} {}", e.width, e.height, typeName(e.type));
                e.lowerPath = e.shortName;
            }
            std::transform(e.lowerPath.begin(), e.lowerPath.end(), e.lowerPath.begin(),
                [](unsigned char c) { return char(std::tolower(c)); });
        }

        const char* textureLabel(const TextureEntry& e);
        std::string exportNameFor(const TextureEntry& e);
        void selectTextureIndex(int index);
        void revertAll();
        int catalogueClone(void* clone, const TextureEntry& src);
        void renderMaterialTextureSlots(const MaterialEntry& m);
        void renderSlotPicker();
        void noteUncatalogued(void* dxTexture);
        void forgetTexBackup(void* block, uint32_t slot);
        bool paramIsColor(uint32_t handle, const char* name);
        bool materialHasColor(const MaterialEntry& m);
        void holdVecOverride(const MaterialEntry& m, uint32_t handle, const float* value);
        void holdTexOverride(const MaterialEntry& m, uint32_t handle, void* texture);
        void dropOverride(uint64_t setKey, uint32_t material, uint32_t handle,
                                 void* block);
        void applyVecOverrides();
        void rebuildMaterialIndex();
        std::string shortLabel(const TextureEntry& e);


        int skyResPicker = -1;
        char skyPickSearch[128] = {};

        std::unordered_map<uint32_t, std::string> meshNames;

        void harvestMeshNames()
        {
            meshNames.clear();

            fb::ResourceManager* rm = fb::ResourceManager::GetInstance();
            if (!rm)
                return;

            auto* const base = reinterpret_cast<fb::ClassInfo*>(fb::MeshAsset::ClassInfoPtr());
            if (!base)
                return;

            for (const auto& comp : rm->m_compartments)
            {
                if (!comp)
                    continue;

                for (const auto& obj : comp->m_objects)
                {
                    if (!obj)
                        continue;

                    fb::ClassInfo* ci = fb::classOf(obj);
                    if (!ci || !ci->isSubclassOf(base))
                        continue;

                    const uint32_t hash = reinterpret_cast<fb::MeshAsset*>(obj)->m_NameHash;
                    if (!hash)
                        continue;

                    std::string name = assetName(obj);
                    if (!name.empty())
                        meshNames.emplace(hash, std::move(name));
                }
            }

            logger::info("[textures] {} mesh name(s) by hash", meshNames.size());
        }

        uint64_t compartmentFingerprint()
        {
            fb::ResourceManager* rm = fb::ResourceManager::GetInstance();
            if (!rm)
                return 0;
            uint64_t h = 1469598103934665603ull;
            for (const auto& comp : rm->m_compartments)
            {
                if (!comp) continue;
                h = (h ^ reinterpret_cast<uintptr_t>(comp)) * 1099511628211ull;
                h = (h ^ comp->m_objects.size()) * 1099511628211ull;
            }
            return h;
        }

        std::unordered_set<const void*> g_liveAssets;
        uint64_t g_assetsFingerprint = 0;

        // only read while its compartment lists it
        bool liveAsset(const void* textureAsset)
        {
            static uint32_t checkedFrame = ~0u;
            if (checkedFrame != frameCounter)
            {
                checkedFrame = frameCounter;
                if (compartmentFingerprint() != g_assetsFingerprint)
                    harvestTextureAssets();
            }
            return textureAsset && g_liveAssets.count(textureAsset) != 0;
        }

        void harvestTextureAssets()
        {
            textureAssets.clear();
            g_liveAssets.clear();
            g_assetsFingerprint = compartmentFingerprint();

            fb::ResourceManager* rm = fb::ResourceManager::GetInstance();
            if (!rm)
                return;

            for (const auto& comp : rm->m_compartments)
            {
                if (!comp) continue;

                for (const auto& obj : comp->m_objects)
                {
                    if (!obj) continue;

                    fb::ClassInfo* classInfo = fb::classOf(obj);
                    if (!classInfo)
                        continue;

                    const char* cn = classInfo->m_InfoData ? classInfo->m_InfoData->m_Name : nullptr;
                    if (!cn || strcmp(cn, "TextureAsset") != 0)
                        continue;

                    g_liveAssets.insert(obj);
                    std::string name = assetName(obj);
                    if (!name.empty())
                        textureAssets.emplace_back(std::move(name), static_cast<void*>(obj));
                }
            }

            std::sort(textureAssets.begin(), textureAssets.end(),
                [](const auto& a, const auto& b) { return _stricmp(a.first.c_str(), b.first.c_str()) < 0; });
            textureAssets.erase(std::unique(textureAssets.begin(), textureAssets.end(),
                [](const auto& a, const auto& b) { return a.second == b.second; }), textureAssets.end());

            logger::debug("[textures] {} TextureAssets loaded", textureAssets.size());
        }

#if defined(BFVE_GAME_BF4)
        constexpr uint32_t RES_STATE_OFFSET[SKY_SLOT_COUNT] = {
            0x188, // SkyGradientTexture, SkyComponentData +0xB8
            0x198, // PanoramicTexture +0xE0
            0x1A8, // PanoramicAlphaTexture +0xE8
            0x1C8, // CloudLayerMaskTexture +0xF0
            0x1B8, // CloudLayer1Texture +0x118
            0x1C0, // CloudLayer2Texture +0x140
            0x1D8, // StaticEnvmapTexture +0x148
            0x1E8, // CustomEnvmapTexture +0x158
        };
#else
        // VisualEnvironmentState::resources +0xB4
        constexpr uint32_t RES_STATE_OFFSET[SKY_SLOT_COUNT] = {
            offsetof(fb::VisualEnvironmentState, resources) + offsetof(fb::VisualEnvironmentResources, skyGradientTexture),
            offsetof(fb::VisualEnvironmentState, resources) + offsetof(fb::VisualEnvironmentResources, panoramicSkyTexture),
            offsetof(fb::VisualEnvironmentState, resources) + offsetof(fb::VisualEnvironmentResources, panoramicSkyAlphaTexture),
            offsetof(fb::VisualEnvironmentState, resources) + offsetof(fb::VisualEnvironmentResources, cloudLayerMaskTexture),
            offsetof(fb::VisualEnvironmentState, resources) + offsetof(fb::VisualEnvironmentResources, cloudLayer0Texture),
            offsetof(fb::VisualEnvironmentState, resources) + offsetof(fb::VisualEnvironmentResources, cloudLayer1Texture),
            offsetof(fb::VisualEnvironmentState, resources) + offsetof(fb::VisualEnvironmentResources, staticEnvmapTexture),
            offsetof(fb::VisualEnvironmentState, resources) + offsetof(fb::VisualEnvironmentResources, customEnvmapTexture),
        };
#endif

        void* resolveAssetTexture(void* textureAsset)
        {
#if defined(BFVE_GAME_BF4)
            if (!liveAsset(textureAsset))
                return nullptr;

            const uintptr_t ref = static_cast<const fb::TextureAsset*>(textureAsset)->m_Resource;
            void* p = reinterpret_cast<void*>(ref & ~uintptr_t(3));
            if (ref == ~uintptr_t(0) || !fb::isValidPtr(p))
                return nullptr;

            return isDxTexture(p) ? p : nullptr;
#else
            if (!liveAsset(textureAsset))
                return nullptr;
            const char* name = static_cast<fb::Asset*>(textureAsset)->m_Name;
            if (!name || !*name)
                return nullptr;
            std::string key = name;
            std::transform(key.begin(), key.end(), key.begin(),
                [](unsigned char c) { return char(std::tolower(c)); });
            auto it = g_bf3TextureByName.find(key);
            return it != g_bf3TextureByName.end() && isDxTexture(it->second) ? it->second : nullptr;
#endif
        }

        void forEachVeState(const std::function<void(uint8_t*)>& fn)
        {
            fb::VisualEnvironmentManager* mgr = fb::VisualEnvironmentManager::GetInstance();
            if (!mgr)
                return;
            for (fb::VisualEnvironmentState* st : mgr->m_states)
                if (st)
                    fn(reinterpret_cast<uint8_t*>(st));
        }

        // ITexture vfunc[1] = addRef
        void addRefTexture(void* tex)
        {
            if (!tex)
                return;
#if defined(BFVE_GAME_BF4)
            void** vt = *reinterpret_cast<void***>(tex);
            reinterpret_cast<void(__fastcall*)(void*)>(vt[1])(tex);
#else
            // RefCountBase release is a plain decrement
            if (isDxTexture(tex))
                InterlockedIncrement(reinterpret_cast<volatile LONG*>(&asTex(tex)->m_refCount));
#endif
        }

        // ITexture vfunc[2] = release, BF4 0x140C0A7E0
        void releaseTexture(void* tex)
        {
            if (!tex)
                return;
#if defined(BFVE_GAME_BF4)
            void** vt = *reinterpret_cast<void***>(tex);
            reinterpret_cast<void(__fastcall*)(void*)>(vt[2])(tex);
#else
            if (isDxTexture(tex))
                InterlockedDecrement(reinterpret_cast<volatile LONG*>(&asTex(tex)->m_refCount));
#endif
        }

        void*& skySlotRef(uint8_t* state, int slot)
        {
            return *reinterpret_cast<void**>(state + RES_STATE_OFFSET[slot]);
        }

        // one override ref per state
        // states release their slots before unloadLevel
        struct SkyHeld
        {
            void* ours[SKY_SLOT_COUNT] = {};
            void* orig[SKY_SLOT_COUNT] = {};
            bool held[SKY_SLOT_COUNT] = {};
        };
        std::unordered_map<uint8_t*, SkyHeld> g_skyHeld;

        void skyPruneHeld()
        {
            std::unordered_set<uint8_t*> live;
            forEachVeState([&live](uint8_t* st) { live.insert(st); });
            for (auto it = g_skyHeld.begin(); it != g_skyHeld.end();)
            {
                if (live.count(it->first))
                {
                    ++it;
                    continue;
                }
                for (int i = 0; i < SKY_SLOT_COUNT; ++i)
                    if (it->second.held[i])
                        releaseTexture(it->second.orig[i]);
                it = g_skyHeld.erase(it);
            }
        }

        // skyless states hold null, e.g. XP4_Titan
        void* skyCurrent(int slot)
        {
            void* cur = nullptr;
            forEachVeState([&cur, slot](uint8_t* st)
            {
                if (cur)
                    return;
                const auto it = g_skyHeld.find(st);
                void* v = (it != g_skyHeld.end() && it->second.held[slot]) ? it->second.orig[slot] : skySlotRef(st, slot);
                if (v)
                    cur = v;
            });
            return cur;
        }

        void skyCaptureOriginal(int slot)
        {
            if (slot < 0 || slot >= SKY_SLOT_COUNT || skyOriginalCaptured[slot])
                return;
            skyOriginal[slot] = skyCurrent(slot);
            skyOriginalCaptured[slot] = true;
        }

        void skyRevertSlot(int slot)
        {
            if (slot < 0 || slot >= SKY_SLOT_COUNT)
                return;

            skyOverride[slot] = {};
            skyOriginal[slot] = nullptr;
            skyOriginalCaptured[slot] = false;

            skyPruneHeld();
            for (auto it = g_skyHeld.begin(); it != g_skyHeld.end();)
            {
                SkyHeld& h = it->second;
                if (h.held[slot])
                {
                    void*& ref = skySlotRef(it->first, slot);
                    if (ref == h.ours[slot])
                    {
                        releaseTexture(h.ours[slot]);
                        ref = h.orig[slot];
                    }
                    else
                        releaseTexture(h.orig[slot]); // the game rewrote the slot
                    h.held[slot] = false;
                }
                if (std::none_of(std::begin(h.held), std::end(h.held), [](bool b) { return b; }))
                    it = g_skyHeld.erase(it);
                else
                    ++it;
            }
        }

        void skyRevertAll()
        {
            for (int i = 0; i < SKY_SLOT_COUNT; ++i)
                skyRevertSlot(i);
            g_skyHeld.clear();
        }

        void applyResourceOverrides()
        {
            skyPruneHeld();
            forEachVeState([](uint8_t* st)
            {
                SkyHeld* h = nullptr;
                for (int i = 0; i < SKY_SLOT_COUNT; ++i)
                {
                    if (!skyOverride[i].enabled)
                        continue;
                    void*& ref = skySlotRef(st, i);
                    void* const want = skyOverride[i].asset;
                    if (ref == want)
                        continue;
                    if (!h)
                        h = &g_skyHeld[st];
                    if (h->held[i] && ref == h->ours[i])
                        releaseTexture(h->ours[i]);
                    else
                    {
                        if (h->held[i])
                            releaseTexture(h->orig[i]); // the game rewrote the slot
                        h->orig[i] = ref;
                        h->held[i] = true;
                    }
                    addRefTexture(want);
                    ref = want;
                    h->ours[i] = want;
                }
            });
        }

        bool looksSkyRelevant(const std::string& path)
        {
            static const char* kHints[] = {
                "sky", "cloud", "gradient", "panoramic", "envmap", "horizon",
                "lighting", "sun", "moon", "star",
            };
            std::string low = path;
            std::transform(low.begin(), low.end(), low.begin(),
                [](unsigned char c) { return char(std::tolower(c)); });
            for (const char* h : kHints)
                if (low.find(h) != std::string::npos)
                    return true;
            return false;
        }



        const char* typeName(uint32_t t)
        {
            switch (t)
            {
            case 0: return "2D";
            case 1: return "Cube";
            case 2: return "3D";
            case 3: return "Array";
            case 5: return "1D";
            default: return "?";
            }
        }

        void fillTextureFields(TextureEntry& e, const fb::DxTexture* tp)
        {
            e.vtable = tp->m_vtable;
            e.width = tp->m_width;
            e.height = tp->m_height;
            e.depth = tp->m_depth;
            e.mips = tp->m_mipmapCount;
            e.type = tp->m_type;
            e.format = tp->m_format;
            e.resourceFormat = tp->m_resFormat;
            e.shaderFormat = tp->m_shaderFormat;
            e.resource = tp->m_resource;
            e.srvLinear = tp->m_shaderViews[0];
            e.srvGamma = tp->m_shaderViews[1];
            e.srgb = tp->m_srgb;
        }

        void readTextureMeta(TextureEntry& e)
        {
            if (!e.texture)
                return;

            const fb::DxTexture* tp = asTex(e.texture);
            e.vtable = tp->m_vtable;
            if (e.vtable != fb::DxTexture::VTable())
            {
                e.texture = nullptr;
                return;
            }
            fillTextureFields(e, tp);
        }

        void catalogueAssetTextures()
        {
            if (textureAssets.empty())
                harvestTextureAssets();

            std::unordered_map<void*, size_t> byTexture;
            std::unordered_set<void*> haveAsset;
            for (size_t i = 0; i < entries.size(); ++i)
            {
                if (entries[i].texture)
                    byTexture.emplace(entries[i].texture, i);
                if (entries[i].asset)
                    haveAsset.insert(entries[i].asset);
            }

            uint32_t added = 0, named = 0, unresolved = 0;

            for (const auto& [name, asset] : textureAssets)
            {
                if (haveAsset.count(asset))
                    continue;

                void* tex = resolveAssetTexture(asset);

                if (tex)
                {
                    if (auto it = byTexture.find(tex); it != byTexture.end())
                    {
                        entries[it->second].loaded = true;
                        if (!entries[it->second].asset)
                            entries[it->second].asset = asset;
                        if (textureNames.emplace(tex, name).second)
                            ++named;
                        haveAsset.insert(asset);
                        continue;
                    }
                    if (textureNames.emplace(tex, name).second)
                        ++named;
                }
                else
                {
                    ++unresolved;
                }

                TextureEntry e{};
                e.asset = asset;
                e.texture = tex;
                e.loaded = tex != nullptr;
                readTextureMeta(e);

                e.nameCached = true;
                e.lowerPath = name;
                const size_t slash = e.lowerPath.find_last_of("/\\");
                e.shortName = (slash == std::string::npos) ? e.lowerPath
                                                           : e.lowerPath.substr(slash + 1);
                std::transform(e.lowerPath.begin(), e.lowerPath.end(), e.lowerPath.begin(),
                    [](unsigned char c) { return char(std::tolower(c)); });

                if (tex)
                    byTexture.emplace(tex, entries.size());
                haveAsset.insert(asset);
                entries.push_back(std::move(e));
                ++added;
            }

            stats.assetTextures = added;
            stats.texNamed += named;
            logger::debug("[textures] catalogued {} from {} assets ({} named, {} not resident yet)",
                added, textureAssets.size(), named, unresolved);
        }

        std::vector<void*> pendingCatalogue;

        void noteUncatalogued(void* dxTexture)
        {
            if (!dxTexture || pendingCatalogue.size() >= 64)
                return;

            for (void* p : pendingCatalogue)
                if (p == dxTexture)
                    return;

            pendingCatalogue.push_back(dxTexture);
        }

#if !defined(BFVE_GAME_BF4)
        struct StrNode
        {
            const char* part;
            StrNode* next;
            uintptr_t child;
            StrNode* parent;
        };

        void walkStringTree(const StrNode* node, std::string& prefix, uint32_t depth,
                            uint32_t& seen, const std::unordered_set<const void*>& live)
        {
            for (const StrNode* n = reinterpret_cast<const StrNode*>(node->child & ~uintptr_t(3));
                 n && depth < 64 && seen < 200000; n = n->next)
            {
                ++seen;
                const size_t mark = prefix.size();
                if (n->child & 1)
                {
                    char inl[5] = { };
                    std::memcpy(inl, &n->part, 4);
                    prefix += inl;
                }
                else if (n->part)
                {
                    prefix += n->part;
                }

                if (n->child & 2)
                {
                    void* const handle = *reinterpret_cast<void* const*>(
                        reinterpret_cast<const uint8_t*>(n) + sizeof(StrNode));
                    const bool aligned = handle && (reinterpret_cast<uintptr_t>(handle) & 3) == 0;
                    void* const obj = aligned ? *static_cast<void**>(handle) : nullptr;
                    if (obj && (reinterpret_cast<uintptr_t>(obj) & 3) == 0)
                    {
                        g_bf3ResourceByName[prefix] = obj;
                        if (live.count(obj) && isDxTexture(obj))
                        {
                            g_bf3TextureByName[prefix] = obj;
                            textureNames.emplace(obj, prefix);
                        }
                    }
                }

                walkStringTree(n, prefix, depth + 1, seen, live);
                prefix.resize(mark);
            }
        }

        const StrNode* compartmentHandlesRoot(const fb::ResourceManager::Compartment* comp)
        {
            const StrNode* root = static_cast<const StrNode*>(comp->m_handles.m_root);
            if (!root || (reinterpret_cast<uintptr_t>(root) & 3) != 0 || root->next || root->parent)
                return nullptr;
            return root;
        }

        uint32_t harvestResourceNames()
        {
            fb::ResourceManager* rm = fb::ResourceManager::GetInstance();
            if (!rm)
                return 0;

            g_bf3TextureByName.clear();
            g_bf3ResourceByName.clear();
            g_bf3MeshSetVtable = nullptr;
            const std::unordered_set<const void*> live = streamedTextureSet();
            uint32_t seen = 0;
            std::string prefix;
            for (fb::ResourceManager::Compartment* comp : rm->m_compartments)
            {
                if (!comp)
                    continue;
                if (const StrNode* root = compartmentHandlesRoot(comp))
                {
                    prefix.clear();
                    walkStringTree(root, prefix, 0, seen, live);
                }
            }

            // a name can hold another type or stale data
            std::unordered_map<void*, uint32_t> vtables;
            uint32_t meshes = 0, best = 0;
            for (const auto& [name, obj] : g_bf3ResourceByName)
                if (name.size() > 5 && name.compare(name.size() - 5, 5, "_mesh") == 0)
                {
                    ++vtables[*static_cast<void**>(obj)];
                    ++meshes;
                }
            for (const auto& [vt, n] : vtables)
                if (n > best) { best = n; g_bf3MeshSetVtable = vt; }

            logger::info("[textures] resource names: {} texture(s) named from {} tree node(s), MeshSet vtable {} ({} of {} meshes)",
                g_bf3TextureByName.size(), seen, g_bf3MeshSetVtable, best, meshes);
            return uint32_t(g_bf3TextureByName.size());
        }
#endif

        // bf3: catalogue = TextureInfo::texture, 8192 slots
        uint32_t catalogueStreamedTextures()
        {
#if defined(BFVE_GAME_BF4)
            return 0;
#else
            harvestResourceNames();

            fb::TextureStreamingManager* mgr = fb::TextureStreamingManager::Singleton();
            if (!mgr || !mgr->m_begin || mgr->m_end < mgr->m_begin)
                return 0;

            const size_t count = (std::min)(size_t(mgr->m_end - mgr->m_begin), size_t(0x2000));

            std::unordered_map<std::string, size_t> assetByName;
            for (size_t i = 0; i < entries.size(); ++i)
                if (entries[i].asset && !entries[i].texture && !entries[i].lowerPath.empty())
                    assetByName.emplace(entries[i].lowerPath, i);

            uint32_t added = 0, attached = 0;
            for (size_t i = 0; i < count; ++i)
            {
                fb::DxTexture* tex = mgr->m_begin[i].m_texture;
                if (!tex || !isDxTexture(tex) || findTextureEntry(tex))
                    continue;

                std::string lower, shortName;
                bool named = false;
                if (auto it = textureNames.find(tex); it != textureNames.end())
                {
                    named = true;
                    lower = it->second;
                    std::transform(lower.begin(), lower.end(), lower.begin(),
                        [](unsigned char c) { return char(std::tolower(c)); });
                    const size_t slash = lower.find_last_of("/\\");
                    shortName = (slash == std::string::npos) ? lower : lower.substr(slash + 1);
                }

                if (named)
                    if (auto a = assetByName.find(lower); a != assetByName.end())
                    {
                        TextureEntry& e = entries[a->second];
                        e.texture = tex;
                        e.loaded = tex->m_resource != nullptr;
                        readTextureMeta(e);
                        assetByName.erase(a);
                        ++attached;
                        continue;
                    }

                TextureEntry e;
                e.texture = tex;
                e.loaded = tex->m_resource != nullptr;
                readTextureMeta(e);
                if (named)
                {
                    e.nameCached = true;
                    e.lowerPath = std::move(lower);
                    e.shortName = std::move(shortName);
                }

                entries.push_back(std::move(e));
                ++added;
            }
            if (attached)
                logger::info("[textures] {} streamed texture(s) attached to their asset rows", attached);

            for (TextureEntry& e : entries)
            {
                if (e.nameCached || !e.texture)
                    continue;
                auto it = textureNames.find(e.texture);
                if (it == textureNames.end())
                    continue;
                e.nameCached = true;
                e.lowerPath = it->second;
                const size_t slash = e.lowerPath.find_last_of("/\\");
                e.shortName = (slash == std::string::npos) ? e.lowerPath : e.lowerPath.substr(slash + 1);
                std::transform(e.lowerPath.begin(), e.lowerPath.end(), e.lowerPath.begin(),
                    [](unsigned char c) { return char(std::tolower(c)); });
            }

            if (added)
                logger::info("[textures] catalogued {} streamed texture(s)", added);
            return added;
#endif
        }

        void drainPendingCatalogue()
        {
            if (pendingCatalogue.empty())
                return;

            uint32_t added = 0;
            for (void* tex : pendingCatalogue)
            {
                if (findTextureEntry(tex) || !isDxTexture(tex))
                    continue;

                TextureEntry e;
                e.texture = tex;
                e.loaded = true;
                readTextureMeta(e);

                if (auto it = textureNames.find(tex); it != textureNames.end())
                {
                    e.nameCached = true;
                    e.lowerPath = it->second;
                    const size_t slash = e.lowerPath.find_last_of("/\\");
                    e.shortName = (slash == std::string::npos) ? e.lowerPath
                                                               : e.lowerPath.substr(slash + 1);
                    std::transform(e.lowerPath.begin(), e.lowerPath.end(), e.lowerPath.begin(),
                        [](unsigned char c) { return char(std::tolower(c)); });
                }

                entries.push_back(std::move(e));
                ++added;
            }

            pendingCatalogue.clear();

            if (added)
            {
                ++catalogGeneration;
                logger::info("[textures] catalogued {} texture(s) found behind a parameter "
                             "block", added);
            }
        }

        struct SetCache { uint8_t* set; std::vector<MaterialEntry> materials; };
        std::unordered_map<uint64_t, SetCache> g_setCache;

        void catalogueSet(const SetRef& ref, std::unordered_map<void*, size_t>& seen)
        {
            const size_t first = materials.size();
            uint8_t* const set = ref.set;
            const uint32_t count = setMaterialCount(set);
            uint8_t* const materialArray = setMaterials(set);

            if (!materialArray)
            {
                ++stats.setsNoMat;
                return;
            }
            if (count == 0 || count > 64)
            {
                ++stats.setsBadCnt;
                return;
            }

            ++stats.setsOk;

            for (uint32_t i = 0; i < count; ++i)
            {
                ++stats.materials;

                uint8_t* block = setBlock(materialArray, i);
                uint8_t vec = 0, tex = 0, bol = 0;
                if (block && !blockIsSane(block, vec, tex, bol))
                    continue;

                ++stats.blocksOk;

                MaterialEntry me{};
                me.set = set;
                me.setKey = ref.key;
                me.index = i;
                me.block = block;
                me.vecCount = vec;
                me.texCount = tex;
                me.boolCount = bol;

                const EbxMaterial* ebxMat = nullptr;
                if (auto it = g_ebx.find(ref.key); it != g_ebx.end())
                {
                    me.meshName = it->second.meshName;
                    if (i < it->second.materials.size())
                        ebxMat = &it->second.materials[i];
                    if (!me.meshName.empty())
                        ++stats.matsNamed;
                }
                if (ebxMat)
                {
                    me.variationOverride = ebxMat->hasVariation;
                    me.variationHandles = ebxMat->variationHandles;

                    me.ebxTextures = ebxMat->textures;
                    me.shaderName = ebxMat->shaderName;
                    me.meshMaterial = ebxMat->material;
                    me.dbTexParams = ebxMat->dbTexParams;
                    me.dbTexCount = ebxMat->dbTexCount;
                }

                if (me.meshName.empty())
                    if (auto mn = meshNames.find(uint32_t(ref.key >> 32));
                        mn != meshNames.end())
                    {
                        me.meshName = mn->second;
                        ++stats.matsNamed;
                    }

                if (me.meshName.empty())
                    me.meshName = std::format("<unnamed {:016X}>", ref.key);

                // low half of the set key = ObjectVariation name hash
                if (auto vit = variationNames.find(uint32_t(ref.key & 0xFFFFFFFFull));
                    vit != variationNames.end())
                    me.variationName = vit->second;

                me.lowerName = me.meshName;
                me.searchKey = me.meshName + ' ' + me.variationName;
                for (std::string* str : { &me.lowerName, &me.searchKey })
                    std::transform(str->begin(), str->end(), str->begin(),
                        [](unsigned char c) { return char(std::tolower(c)); });

                me.hasColor = materialHasColor(me);
                me.label = materialLabel(me);
                materials.push_back(std::move(me));

                for (uint32_t t = 0; t < tex; ++t)
                {
                    uint32_t slotHandle = 0;
                    uint16_t valueOffset = 0;
                    paramInfo(block, uint32_t(vec) + t, slotHandle, valueOffset);

                    void* texture = readTex(block, valueOffset);
                    if (!texture)
                        continue;

                    ++stats.texSlots;

                    if (ebxMat && !textureNames.count(texture))
                    {
                        for (const auto& [h, path] : ebxMat->textures)
                            if (h == slotHandle)
                            {
                                textureNames.emplace(texture, path);
                                ++stats.texNamed;
                                break;
                            }
                    }

                    if (auto it = seen.find(texture); it != seen.end())
                    {
                        ++entries[it->second].refs;
                        entries[it->second].loaded = true;
                        continue;
                    }

                    TextureEntry e{};
                    e.texture = texture;
                    e.refs = 1;
                    e.loaded = true;

                    const fb::DxTexture* tp = asTex(texture);
                    e.vtable = tp->m_vtable;
                    if (e.vtable == fb::DxTexture::VTable())
                    {
                        fillTextureFields(e, tp);
                        ++stats.vtableMatch;
                    }

                    if (srvIsDrawable2D(e.srvLinear))
                    {
                        addRefSrv(e.srvLinear);
                        e.drawable = true;
                    }

                    seen.emplace(texture, entries.size());
                    entries.push_back(e);
                }
            }
            g_setCache[ref.key] = { ref.set, std::vector<MaterialEntry>(materials.begin() + ptrdiff_t(first), materials.end()) };
        }

        void scan();

        void scanIncremental()
        {
            std::vector<SetRef> sets;
            collectSets(sets);
            stats.sets = uint32_t(sets.size());

            uint64_t selKey = 0; uint32_t selIndex = 0; bool hadSel = false;
            if (selectedMaterial >= 0 && selectedMaterial < int(materials.size()))
            {
                hadSel = true;
                selKey = materials[selectedMaterial].setKey;
                selIndex = materials[selectedMaterial].index;
            }

            std::unordered_map<void*, size_t> seen;
            for (size_t i = 0; i < entries.size(); ++i)
                seen.emplace(entries[i].texture, i);

            bool anyFresh = false;
            for (const SetRef& ref : sets)
            {
                auto it = g_setCache.find(ref.key);
                if (it == g_setCache.end() || it->second.set != ref.set)
                {
                    anyFresh = true;
                    break;
                }
            }
            if (anyFresh)
            {
                harvestEbx();
                harvestMeshNames();
            }

            materials.clear();
            size_t fresh = 0;
            std::unordered_set<uint64_t> liveKeys;
            for (const SetRef& ref : sets)
            {
                liveKeys.insert(ref.key);
                auto it = g_setCache.find(ref.key);
                if (it != g_setCache.end() && it->second.set == ref.set)
                    materials.insert(materials.end(), it->second.materials.begin(), it->second.materials.end());
                else
                {
                    catalogueSet(ref, seen);
                    ++fresh;
                }
            }
            for (auto it = g_setCache.begin(); it != g_setCache.end();)
                it = liveKeys.count(it->first) ? std::next(it) : g_setCache.erase(it);

            stats.texUnique = uint32_t(entries.size());
            ++catalogGeneration;
            rebuildMaterialIndex();
            selectedMaterial = -1;
            if (hadSel)
                for (size_t i = 0; i < materials.size(); ++i)
                    if (materials[i].setKey == selKey && materials[i].index == selIndex)
                    {
                        selectedMaterial = int(i);
                        break;
                    }
            pickerSlot = -1;
            usagesFor = -1;
            logger::info("[textures] incremental scan: {} set(s), {} new, {} materials, {} textures",
                         sets.size(), fresh, materials.size(), entries.size());
        }

        void scan()
        {
            cancelLoads();
            bulkLoadActive = false;
            bulkAsked.clear();
            handleToEntry.clear();
            loadsAsked = 0;
            loadsDone = 0;
            residentTextures = 0;

            releaseHeldSrvs();
            if (!keepUnloaded)
            {
                releaseHeldSrvs();
                releaseAllPins();
                entries.clear();
                textureNames.clear();
            }
            uint32_t stale = 0;
            const std::unordered_set<const void*> live = streamedTextureSet();
            for (TextureEntry& e : entries)
            {
                e.loaded = false;
                e.refs = 0;
                e.handle = 0xFFFF;

                if (e.texture && live.find(e.texture) == live.end())
                {
                    e.texture = nullptr;
                    e.resource = nullptr;
                    e.viewChecked = false;
                    ++stale;
                }
            }
            if (stale)
                logger::info("[textures] {} carried entry/entries no longer resolve to a "
                             "live texture", stale);

            materials.clear();
            vecBackups.clear();
            texBackups.clear();
            const uint32_t carried = uint32_t(entries.size());
            stats = {};
            selectedMaterial = -1;
            pickerSlot = -1;
            usagesFor = -1;

            harvestNames();
            harvestEbx();
            harvestMeshNames();

            std::vector<SetRef> sets;
            collectSets(sets);
            stats.sets = uint32_t(sets.size());

            std::unordered_map<void*, size_t> seen;
            for (size_t i = 0; i < entries.size(); ++i)
                seen.emplace(entries[i].texture, i);

            g_setCache.clear();
            for (const SetRef& ref : sets)
                catalogueSet(ref, seen);

            if (includeAllAssets)
                catalogueAssetTextures();

            catalogueStreamedTextures();

            stats.texUnique = uint32_t(entries.size());
            ++catalogGeneration;
            rebuildMaterialIndex();

            if (autoLoadAfterScan)
                queueEveryMissing();
            uint32_t liveNow = 0;
            for (const TextureEntry& e : entries)
                if (e.loaded) ++liveNow;
            logger::debug("[textures] catalog: {} total ({} live, {} carried)",
                entries.size(), liveNow, carried);

            // handlesNamed > 0: BF4 uses BF3's DJB2-xor hash
            for (const MaterialEntry& m : materials)
            {
                const uint32_t total = uint32_t(m.vecCount) + m.texCount + m.boolCount;
                for (uint32_t s = 0; s < total; ++s)
                {
                    uint32_t handle = 0;
                    uint16_t offset = 0;
                    if (!paramInfo(static_cast<const uint8_t*>(m.block), s, handle, offset))
                        continue;

                    ++stats.handlesTotal;
                    if (nameForHandle(handle))
                        ++stats.handlesNamed;
                }
            }

            uint32_t resident = 0;
            for (const TextureEntry& e : entries)
                if (e.texture) ++resident;
            logger::info("[textures] scan: {} textures ({} resident), {} materials, {} named",
                stats.texUnique, resident, stats.blocksOk, stats.texNamed);
            logger::debug("[textures] sets {}/{} (mgr {}), slots {}, vtable {}/{}",
                stats.setsOk, stats.sets, stats.elementCount, stats.texSlots,
                stats.vtableMatch, stats.texUnique);
#if defined(BFVE_GAME_BF3)
            {
                // SurfaceShader +0 -> solution pairs +0x44 -> pixel permutation
                size_t realized = 0, withShader = 0, initialized = 0, withPairs = 0, perms = 0, shown = 0;
                for (const MaterialEntry& m : materials)
                {
                    auto* inst = reinterpret_cast<fb::SurfaceShaderInstance*>(liveInstance(m.setKey, m.index));
                    if (!inst) continue;
                    ++realized;
                    auto* sh = static_cast<fb::SurfaceShader*>(inst->m_shader);
                    if (!sh) continue;
                    ++withShader;
                    if (sh->m_initialized) ++initialized;
                    if (sh->m_solutionPairs && sh->m_solutionPairCount) ++withPairs;
                    for (uint32_t p = 0; sh->m_solutionPairs && p < sh->m_solutionPairCount && p < 1024; ++p)
                    {
                        fb::DxShaderSolution* sol = sh->m_solutionPairs[p].m_solution;
                        if (sol && sol->m_pixelPermutation && sol->m_pixelPermutation->m_shader) ++perms;
                    }
                    if (shown < 3)
                    {
                        ++shown;
                        logger::info("[shaders] sample {} [{}] {}: instance {} shader {} vtable {} initialized {} pairs {} x {} block {}",
                            m.meshName, m.index, m.shaderName, static_cast<const void*>(inst), static_cast<const void*>(sh),
                            *reinterpret_cast<void**>(sh), int(sh->m_initialized), static_cast<const void*>(sh->m_solutionPairs),
                            sh->m_solutionPairCount, inst->m_block);
                    }
                }
                logger::info("[shaders] coverage: {} materials, {} realized, {} with a shader, {} initialized, {} with solution pairs, {} pixel permutations",
                             materials.size(), realized, withShader, initialized, withPairs, perms);
            }
#endif
        }
    }

    void init()
    {
        clear();
        gen::setAssetResolvers(
            [](void* dxTexture) -> std::string
            {
                if (auto it = textureNames.find(dxTexture); it != textureNames.end())
                    return it->second;
                for (const TextureEntry& e : entries)
                    if (e.texture == dxTexture && liveAsset(e.asset))
                        return assetName(e.asset);
                return std::string();
            },
            [](const std::string& path) -> void*
            {
                std::string lower = path;
                std::transform(lower.begin(), lower.end(), lower.begin(),
                    [](unsigned char c) { return char(std::tolower(c)); });
                for (const TextureEntry& e : entries)
                {
                    if (!e.asset || e.lowerPath != lower)
                        continue;
                    if (void* now = resolveAssetTexture(e.asset))
                        return now;
                    break;
                }
                return textureByPath(path);
            });
    }

    void shutdown()
    {
        gen::shutdown();
        clear();
    }

    namespace detail
    {
        int rescanCountdown = -1; // < 0 = idle
        int autoScanRetryIn = 0;
        int emptyScans = 0;
        bool rescanIncremental = false;
    }

    void requestRescan(int delayFrames)
    {
        rescanCountdown = delayFrames;
        rescanIncremental = false;
    }

    namespace detail
    {
        void requestRescanSoon()
        {
            if (rescanCountdown < 0)
                requestRescan(60);
        }
    }

    static bool g_autoScanned = false;

    uint32_t g_lastScanElements = 0;
    static uint32_t g_elementCheckIn = 0;

    namespace detail { void holdShaderPatches(); void applyPendingShader(); void clearShaderPatches(); }

    void tick()
    {
        ++frameCounter;

        if (!g_autoScanned && autoScan)
        {
            fb::MeshVariationManager* mgr = fb::MeshVariationManager::Singleton();
            const uint32_t elements = mgr ? mgr->m_elementCount : 0;

            if (elements)
            {
                g_autoScanned = true;
                requestRescan(60);
                logger::info("[textures] world realized ({} mesh variation(s)) - scanning",
                    elements);
            }
        }

        if (autoScan && g_autoScanned && rescanCountdown < 0 && ++g_elementCheckIn >= 60)
        {
            g_elementCheckIn = 0;

            fb::MeshVariationManager* mgr = fb::MeshVariationManager::Singleton();
            const uint32_t elements = mgr ? mgr->m_elementCount : 0;

            if (mgr && elements != g_lastScanElements)
            {
                logger::info("[textures] {} mesh variation(s) realized, was {} - incremental scan",
                    elements, g_lastScanElements);
                rescanCountdown = 30;
                rescanIncremental = true;
            }
        }

        // render thread
        gen::tickBatch();

        applyPendingConfig();

        applyVecOverrides();
        holdShaderPatches();
        if ((frameCounter % 30) == 0)
        {
            gen::replayPending();
            applyPendingShader();
        }

        if (holdEdits)
            gen::reassertOverrides();

        if (budgetHit.load() && !gen::batchActive() && !bulkLoadActive.load())
            recyclePins();

        // only safe point to grow entries
        drainPendingCatalogue();

        // 16 loadOnDemand per frame
        pumpLoads();

        {
            static uint32_t lastResidency = 0;
            const bool loading = loadsAsked.load() > loadsDone.load();
            if (loading && frameCounter - lastResidency > 30)
            {
                lastResidency = frameCounter;
                refreshResidency();
            }
        }

        if (autoScan && entries.empty() && rescanCountdown < 0 && emptyScans < 4 &&
            isSafeToOperate() && --autoScanRetryIn <= 0)
        {
            autoScanRetryIn = 900;
            requestRescan(120);
        }

        if (rescanCountdown < 0)
            return;
        if (--rescanCountdown > 0)
            return;

        rescanCountdown = -1;

        const bool incremental = rescanIncremental;
        rescanIncremental = false;
        if (incremental)
            scanIncremental();
        else
            scan();
        g_lastScanElements = stats.elementCount;
        emptyScans = entries.empty() ? emptyScans + 1 : 0;
        if (!incremental)
            logger::info("[textures] auto-scan: {} texture(s) catalogued", entries.size());
    }

    void clear()
    {
        ++catalogGeneration;
        clearShaderPatches();
#if !defined(BFVE_GAME_BF4)
        g_bf3TextureByName.clear();
        g_bf3ResourceByName.clear();
        g_bf3MeshSetVtable = nullptr;
#endif

        releaseClonedParamBlocks();

        revertAll();

        gen::revertAll();
        releaseHeldSrvs();
        releaseAllPins();

        bulkLoadActive = false;
        bulkAsked.clear();
        residentTextures = 0;
        pendingCatalogue.clear();
        clearPendingConfig();
        g_autoScanned = false;
        g_lastScanElements = 0;

        skyRevertAll();
        skyEditSlot = -1;
        skyResPicker = -1;
        textureAssets.clear();

        entries.clear();
        materials.clear();
        paramOverrides.clear();
        handleNames.clear();
        colorHandles.clear();
        variationNames.clear();
        textureNames.clear();
        meshNames.clear();
        vecBackups.clear();
        texBackups.clear();
        stats = {};
        selected = -1;
        selectedMaterial = -1;
        pickerSlot = -1;
    }

    namespace detail
    {
        VecBackup* findVecBackup(void* block, uint32_t index)
        {
            for (VecBackup& b : vecBackups)
                if (b.block == block && b.index == index)
                    return &b;
            return nullptr;
        }

        TexBackup* findTexBackup(void* block, uint32_t slot)
        {
            for (TexBackup& b : texBackups)
                if (b.block == block && b.index == slot)
                    return &b;
            return nullptr;
        }

        ParamOverride* findOverride(uint64_t setKey, uint32_t material, uint32_t handle,
                                    void* block)
        {
            for (ParamOverride& o : paramOverrides)
            {
                if (o.setKey != setKey || o.material != material || o.handle != handle)
                    continue;
                if (setKey == 0 && o.block != block)
                    continue;
                return &o;
            }
            return nullptr;
        }

        std::unordered_map<uint64_t, int> materialBySlot;

        uint64_t slotKey(uint64_t setKey, uint32_t material)
        {
            return setKey * 1099511628211ull + material;
        }

        void rebuildMaterialIndex()
        {
            materialBySlot.clear();
            materialBySlot.reserve(materials.size());
            for (int i = 0; i < int(materials.size()); ++i)
                materialBySlot.emplace(slotKey(materials[i].setKey, materials[i].index), i);
        }

        void holdVecOverride(const MaterialEntry& m, uint32_t handle, const float* value)
        {
            if (!handle || !m.block)
                return;

            if (ParamOverride* o = findOverride(m.setKey, m.index, handle, m.block))
            {
                std::memcpy(o->value, value, sizeof(o->value));
                o->isTexture = false;
                return;
            }

            ParamOverride o;
            o.setKey = m.setKey;
            o.material = m.index;
            o.handle = handle;
            o.isTexture = false;
            o.block = m.setKey ? nullptr : m.block;
            std::memcpy(o.value, value, sizeof(o.value));
            paramOverrides.push_back(o);
        }

        void holdTexOverride(const MaterialEntry& m, uint32_t handle, void* texture)
        {
            if (!handle || !m.block)
                return;

            if (ParamOverride* o = findOverride(m.setKey, m.index, handle, m.block))
            {
                o->texture = texture;
                o->isTexture = true;
                return;
            }

            ParamOverride o;
            o.setKey = m.setKey;
            o.material = m.index;
            o.handle = handle;
            o.isTexture = true;
            o.texture = texture;
            o.block = m.setKey ? nullptr : m.block;
            paramOverrides.push_back(o);
        }

        void dropOverride(uint64_t setKey, uint32_t material, uint32_t handle, void* block)
        {
            for (size_t i = 0; i < paramOverrides.size(); ++i)
            {
                const ParamOverride& o = paramOverrides[i];
                if (o.setKey != setKey || o.material != material || o.handle != handle)
                    continue;
                if (setKey == 0 && o.block != block)
                    continue;
                {
                    paramOverrides.erase(paramOverrides.begin() + ptrdiff_t(i));
                    return;
                }
            }
        }

        bool slotForHandle(const MaterialEntry& m, uint32_t handle, bool wantTexture,
                           uint16_t& offset)
        {
            const uint32_t first = wantTexture ? m.vecCount : 0u;
            const uint32_t last = wantTexture ? uint32_t(m.vecCount) + m.texCount : m.vecCount;

            for (uint32_t slot = first; slot < last; ++slot)
            {
                uint32_t h = 0;
                if (!paramInfo(static_cast<const uint8_t*>(m.block), slot, h, offset))
                    continue;
                if (h == handle)
                    return true;
            }
            return false;
        }

        void requestRescanSoon();

        enum class Resolve
        {
            Ok, NoManager, NoBuckets, KeyNotFound, IndexOutOfRange, BlockInsane
        };

        const char* resolveName(Resolve r)
        {
            switch (r)
            {
            case Resolve::Ok: return "ok";
            case Resolve::NoManager: return "no MeshVariationManager";
            case Resolve::NoBuckets: return "no bucket array";
            case Resolve::KeyNotFound: return "set key not in the manager";
            case Resolve::IndexOutOfRange: return "material index past the set's count";
            case Resolve::BlockInsane: return "block failed its sanity check";
            }
            return "?";
        }

        void* liveBlock(uint64_t setKey, uint32_t materialIndex, MaterialEntry& out,
                        Resolve& why)
        {
            why = Resolve::NoManager;

            fb::MeshVariationManager* mgr = fb::MeshVariationManager::Singleton();
            if (!mgr)
                return nullptr;

            why = Resolve::NoBuckets;
            if (!mgr->m_buckets || !mgr->m_bucketCount)
                return nullptr;

            why = Resolve::KeyNotFound;

            // bucket = (u32)variationNameHash % bucketCount
            const uint32_t bucket = uint32_t(setKey & 0xFFFFFFFFull) % mgr->m_bucketCount;

            uint32_t guard = 0;
            for (fb::MeshVariationNode* node = mgr->m_buckets[bucket]; node && guard < 4096;
                 node = node->m_next, ++guard)
            {
                if (node->m_key != setKey || !node->m_set)
                    continue;

                uint8_t* set = static_cast<uint8_t*>(node->m_set);

                why = Resolve::IndexOutOfRange;
                uint8_t* const materialArray = setMaterials(set);
                if (!materialArray || materialIndex >= setMaterialCount(set))
                    return nullptr;

                why = Resolve::BlockInsane;
                out.set = set;
                out.index = materialIndex;
                uint8_t* block = setBlock(materialArray, materialIndex);
                if (!block)
                    return nullptr;

                uint8_t vec = 0, tex = 0, bol = 0;
                if (!blockIsSane(block, vec, tex, bol))
                    return nullptr;

                why = Resolve::Ok;

                out.set = set;
                out.index = materialIndex;
                out.block = block;
                out.vecCount = vec;
                out.texCount = tex;
                out.boolCount = bol;
                return block;
            }

            return nullptr;
        }

        // read by the block-setter hooks, game thread
        struct HeldVec { uint32_t handle; float value[4]; };
        std::mutex g_heldMutex;
        std::unordered_map<const void*, std::vector<HeldVec>> g_heldVec;
        std::atomic<uint32_t> g_heldCount{ 0 };

        void applyVecOverrides()
        {
            {
                std::lock_guard<std::mutex> lock(g_heldMutex);
                g_heldVec.clear();
                g_heldCount.store(0, std::memory_order_relaxed);
            }

            if (!holdEdits || paramOverrides.empty())
                return;

            static std::unordered_map<uint64_t, int> lastState;

            for (const ParamOverride& o : paramOverrides)
            {
                Resolve why = Resolve::Ok;
                MaterialEntry live{};
                bool resolved = false;

                if (o.block)
                {
                    uint8_t vec = 0, tex = 0, bol = 0;
                    if (blockIsSane(static_cast<const uint8_t*>(o.block), vec, tex, bol))
                    {
                        live.block = o.block;
                        live.vecCount = vec;
                        live.texCount = tex;
                        live.boolCount = bol;
                        resolved = true;
                    }
                    else
                    {
                        why = Resolve::BlockInsane;
                    }
                }
                else
                {
                    resolved = liveBlock(o.setKey, o.material, live, why) != nullptr;
                }

                bool wrote = false;
                uint16_t offset = 0;

                if (!resolved && why == Resolve::KeyNotFound && !o.block && (o.setKey >> 32) != 0)
                {
                    int twins = 0;
                    if (fb::MeshVariationManager* mgr = fb::MeshVariationManager::Singleton())
                    {
                        if (mgr->m_buckets && mgr->m_bucketCount)
                            for (uint32_t b = 0; b < mgr->m_bucketCount; ++b)
                            {
                                uint32_t guard = 0;
                                for (fb::MeshVariationNode* node = mgr->m_buckets[b]; node && guard < 4096;
                                     node = node->m_next, ++guard)
                                {
                                    if ((node->m_key >> 32) != (o.setKey >> 32) || !node->m_set)
                                        continue;
                                    MaterialEntry twin{};
                                    Resolve twhy = Resolve::Ok;
                                    if (!liveBlock(node->m_key, o.material, twin, twhy))
                                        continue;
                                    uint16_t toff = 0;
                                    if (!slotForHandle(twin, o.handle, o.isTexture, toff))
                                        continue;
                                    if (o.isTexture)
                                        writeTex(twin.block, toff, o.texture);
                                    else
                                    {
                                        writeVec(twin.block, toff, o.value);
                                        std::lock_guard<std::mutex> lock(g_heldMutex);
                                        HeldVec h{ o.handle, { o.value[0], o.value[1], o.value[2], o.value[3] } };
                                        g_heldVec[twin.block].push_back(h);
                                        g_heldCount.fetch_add(1, std::memory_order_relaxed);
                                    }
                                    ++twins;
                                }
                            }
                    }
                    if (twins)
                    {
                        wrote = true;
                        resolved = true;
                    }
                }

                if (o.added && !o.isTexture && !o.block && live.set &&
                    (resolved ? !slotForHandle(live, o.handle, false, offset)
                              : why == Resolve::BlockInsane))
                {
                    queueGrow(o.setKey, o.material, o.handle, o.value);
                    continue;
                }
                if (resolved)
                {
                    if (slotForHandle(live, o.handle, o.isTexture, offset))
                    {
                        if (o.isTexture)
                            writeTex(live.block, offset, o.texture);
                        else
                        {
                            writeVec(live.block, offset, o.value);
                            std::lock_guard<std::mutex> lock(g_heldMutex);
                            HeldVec h{ o.handle, { o.value[0], o.value[1], o.value[2], o.value[3] } };
                            g_heldVec[live.block].push_back(h);
                            g_heldCount.fetch_add(1, std::memory_order_relaxed);
                        }
                        wrote = true;
                    }
                }

                const int state = wrote ? 0 : (resolved ? -1 : int(why));
                const uint64_t id = o.block
                    ? (reinterpret_cast<uintptr_t>(o.block) ^ o.handle)
                    : (slotKey(o.setKey, o.material) ^ o.handle);

                auto it = lastState.find(id);
                if (it == lastState.end() || it->second != state)
                {
                    lastState[id] = state;
                    if (wrote)
                        logger::info("[textures] hold {:016X}[{}] {:08X}: applying",
                            o.setKey, o.material, o.handle);
                    else if (resolved)
                        logger::warning("[textures] hold {:016X}[{}] {:08X}: block found but "
                            "no slot carries that handle", o.setKey, o.material, o.handle);
                    else
                        logger::warning("[textures] hold {:016X}[{}] {:08X}: {}",
                            o.setKey, o.material, o.handle, resolveName(why));
                }
            }
        }

        void revertAll()
        {
            for (const VecBackup& b : vecBackups)
            {
                uint32_t handle = 0;
                uint16_t offset = 0;
                if (paramInfo(b.block, b.index, handle, offset))
                    writeVec(b.block, offset, b.value);
            }
            for (const TexBackup& b : texBackups)
            {
                uint32_t handle = 0;
                uint16_t offset = 0;
                if (paramInfo(b.block, b.index, handle, offset))
                    writeTex(b.block, offset, b.texture);
            }

            const size_t views = gen::overrideCount();
            gen::revertAll();

            logger::info("[textures] reverted {} vector, {} texture and {} view edit(s)",
                vecBackups.size(), texBackups.size(), views);
            vecBackups.clear();
            texBackups.clear();
            paramOverrides.clear();
            releaseGrownBlocks();
            freeRetiredBlocks();
        }

        void forgetTexBackup(void* block, uint32_t slot)
        {
            for (size_t i = 0; i < texBackups.size(); ++i)
            {
                if (texBackups[i].block == block && texBackups[i].index == slot)
                {
                    texBackups.erase(texBackups.begin() + ptrdiff_t(i));
                    return;
                }
            }
        }

        const TextureEntry* findTextureEntry(void* texture)
        {
            for (const TextureEntry& e : entries)
                if (e.texture == texture)
                    return &e;
            return nullptr;
        }

        const char* textureLabel(const TextureEntry& e)
        {
            auto it = textureNames.find(e.texture);
            return it == textureNames.end() ? nullptr : it->second.c_str();
        }

        std::string shortLabel(const TextureEntry& e)
        {
            if (e.nameCached)
                return e.shortName;

            if (const char* full = textureLabel(e))
            {
                std::string n = full;
                const size_t slash = n.find_last_of("/\\");
                if (slash != std::string::npos)
                    n = n.substr(slash + 1);
                return n;
            }
            return std::format("{}x{} {}", e.width, e.height, typeName(e.type));
        }

        bool matchesSearch(const std::string& haystack, const char* needleRaw)
        {
            if (!needleRaw || !needleRaw[0])
                return true;
            std::string hay = haystack, needle = needleRaw;
            std::transform(hay.begin(), hay.end(), hay.begin(),
                [](unsigned char c) { return char(std::tolower(c)); });
            std::transform(needle.begin(), needle.end(), needle.begin(),
                [](unsigned char c) { return char(std::tolower(c)); });
            return hay.find(needle) != std::string::npos;
        }

        void rebuildUsages()
        {
            usages.clear();
            usagesFor = selected;
            replaceSlot = -1;

            if (selected < 0 || selected >= int(entries.size()))
                return;

            void* want = entries[selected].texture;
            for (int mi = 0; mi < int(materials.size()); ++mi)
            {
                const MaterialEntry& m = materials[mi];
                for (uint32_t i = 0; i < m.texCount; ++i)
                {
                    const uint32_t slot = uint32_t(m.vecCount) + i;
                    uint32_t handle = 0;
                    uint16_t offset = 0;
                    if (!paramInfo(static_cast<const uint8_t*>(m.block), slot, handle, offset))
                        continue;

                    if (readTex(m.block, offset) == want)
                        usages.push_back({ mi, slot, handle });
                }
            }
        }

        void writeTextureSlot(const Usage& u, void* newTexture)
        {
            const MaterialEntry& m = materials[u.material];
            uint32_t handle = 0;
            uint16_t offset = 0;
            if (!paramInfo(static_cast<const uint8_t*>(m.block), u.slot, handle, offset))
                return;

            if (!findTexBackup(m.block, u.slot))
                texBackups.push_back({ m.block, u.slot, readTex(m.block, offset) });
            writeTex(m.block, offset, newTexture);
            holdTexOverride(m, handle, newTexture);
        }

        std::vector<int> g_visible;
        std::string g_visibleKey;
        uint32_t g_liveCount = 0;

        void refreshVisible()
        {
            const std::string key = std::format("{}|{}|{}|{}|{}|{}",
                search, minSize, only2D ? 1 : 0, showUnloaded ? 1 : 0,
                entries.size(), catalogGeneration);
            if (key == g_visibleKey)
                return;
            g_visibleKey = key;

            std::string needle = search;
            std::transform(needle.begin(), needle.end(), needle.begin(),
                [](unsigned char c) { return char(std::tolower(c)); });

            g_visible.clear();
            g_visible.reserve(entries.size());
            g_liveCount = 0;

            for (int i = 0; i < int(entries.size()); ++i)
            {
                TextureEntry& e = entries[i];
                ensureNameCache(e);
                if (e.loaded)
                    ++g_liveCount;

                const bool resolved = e.texture != nullptr;
                if (resolved && (int(e.width) < minSize || int(e.height) < minSize))
                    continue;
                if (only2D && resolved && e.type != 0)
                    continue;
                if (!showUnloaded && !e.loaded)
                    continue;
                if (!needle.empty() && e.lowerPath.find(needle) == std::string::npos)
                    continue;

                g_visible.push_back(i);
            }

            std::sort(g_visible.begin(), g_visible.end(), [](int a, int b)
            {
                const TextureEntry& ea = entries[a];
                const TextureEntry& eb = entries[b];
                const int c = ea.lowerPath.compare(eb.lowerPath);
                return c ? (c < 0) : (a < b);
            });
        }




        bool containsCI(const char* haystack, const char* needle)
        {
            if (!haystack) return false;
            std::string h = haystack;
            std::transform(h.begin(), h.end(), h.begin(),
                [](unsigned char c) { return char(std::tolower(c)); });
            return h.find(needle) != std::string::npos;
        }

        int effectiveMode(uint32_t handle, const char* name, const float* v)
        {
            if (auto it = paramMode.find(handle); it != paramMode.end() && it->second != ParamAuto)
                return it->second;

            if (colorHandles.count(handle))
                return ParamColor;

            if (name)
            {
                if (containsCI(name, "color") || containsCI(name, "colour") ||
                    containsCI(name, "tint") || containsCI(name, "albedo") ||
                    containsCI(name, "emissive") || containsCI(name, "specular") ||
                    containsCI(name, "background"))
                    return ParamColor;

                if (containsCI(name, "uv") || containsCI(name, "offset") ||
                    containsCI(name, "scale") || containsCI(name, "tile") ||
                    containsCI(name, "pos") || containsCI(name, "size") ||
                    containsCI(name, "rect") || containsCI(name, "range") ||
                    containsCI(name, "factor") || containsCI(name, "param"))
                    return ParamNumeric;
            }

            for (int i = 0; i < 4; ++i)
                if (v[i] < 0.0f || v[i] > 1.0f)
                    return ParamNumeric;

            return asColor ? ParamColor : ParamNumeric;
        }

        const std::vector<void*>* g_mirrorBlocks = nullptr;

        EditRedirect g_editRedirect = nullptr;
        void* g_editRedirectUser = nullptr;

        void* redirectEdit(void* block)
        {
            if (!g_editRedirect)
                return block;

            void* target = g_editRedirect(block, g_editRedirectUser);
            return target ? target : block;
        }

        void mirrorWrite(void* edited, uint32_t handle, bool isTexture,
                         const float* value, void* texture)
        {
            if (!g_mirrorBlocks)
                return;

            for (void* block : *g_mirrorBlocks)
            {
                if (block == edited)
                    continue;

                MaterialEntry mm{};
                uint8_t vec = 0, tex = 0, bol = 0;
                if (!looksLikeParamBlock(block, vec, tex, bol))
                    continue;

                mm.block = block;
                mm.vecCount = vec;
                mm.texCount = tex;
                mm.boolCount = bol;

                uint16_t offset = 0;
                if (!slotForHandle(mm, handle, isTexture, offset))
                    continue;

                if (isTexture)
                {
                    writeTex(block, offset, texture);
                    holdTexOverride(mm, handle, texture);
                }
                else
                {
                    writeVec(block, offset, value);
                    holdVecOverride(mm, handle, value);
                }
            }
        }


        int catalogueClone(void* clone, const TextureEntry& src)
        {
            if (!clone)
                return -1;

            TextureEntry e = src;
            e.texture = clone;
            e.asset = nullptr;
            e.drawable = false;
            e.viewChecked = false;
            e.loaded = true;
            e.refs = 0;
            e.handle = 0xFFFF;

            const char* full = textureLabel(src);
            std::string base = full ? full : src.shortName;
            if (base.empty())
                base = "texture";

            int copyIndex = 1;
            for (const TextureEntry& other : entries)
                if (other.texture && gen::isClonedTexture(other.texture))
                    ++copyIndex;

            const std::string name = base + "  (copy " + std::to_string(copyIndex) + ")";
            textureNames[clone] = name;

            e.nameCached = true;
            e.lowerPath = name;
            const size_t slash = e.lowerPath.find_last_of("/\\");
            e.shortName = (slash == std::string::npos) ? e.lowerPath
                                                       : e.lowerPath.substr(slash + 1);
            std::transform(e.lowerPath.begin(), e.lowerPath.end(), e.lowerPath.begin(),
                [](unsigned char c) { return char(std::tolower(c)); });

            void* srv = asTex(clone)->m_shaderViews[0];
            if (srvIsDrawable2D(srv))
            {
                addRefSrv(srv);
                e.srvLinear = srv;
                e.drawable = true;
                e.viewChecked = true;
            }

            entries.push_back(std::move(e));
            ++catalogGeneration;
            return int(entries.size()) - 1;
        }

        void selectTextureIndex(int index)
        {
            if (index < 0 || index >= int(entries.size()))
                return;
            selected = index;
            usagesFor = -1;
            focusTab = true;
        }

        SlotPick slotPick;
        char slotPickSearch[128] = {};

        void assignSlot(void* block, uint32_t slot, uint16_t offset, uint32_t handle,
                        uint64_t setKey, uint32_t material, void* current, void* replacement)
        {
            block = redirectEdit(block);

            if (!findTexBackup(block, slot))
                texBackups.push_back({ block, slot, current });

            writeTex(block, offset, replacement);

            MaterialEntry key;
            key.setKey = setKey;
            key.index = material;
            key.block = block;
            holdTexOverride(key, handle, replacement);
            mirrorWrite(block, handle, true, nullptr, replacement);
        }

        void revertSlot(void* block, uint32_t slot, uint16_t offset, uint32_t handle,
                        uint64_t setKey, uint32_t material)
        {
            TexBackup* b = findTexBackup(block, slot);
            if (!b)
                return;

            writeTex(block, offset, b->texture);

            dropOverride(setKey, material, handle, block);
            forgetTexBackup(block, slot);
        }




        std::string exportNameFor(const TextureEntry& e)
        {
            if (const char* full = textureLabel(e))
                return full;
            return shortLabel(e);
        }

        void exportAllLoaded(bool asDds)
        {
            std::vector<gen::BatchItem> items;
            items.reserve(entries.size());

            for (const TextureEntry& e : entries)
            {
                if (!e.texture || !e.resource)
                    continue;

                gen::BatchItem item;
                item.dxTexture = e.texture;
                item.name = exportNameFor(e);
                items.push_back(std::move(item));
            }

            if (items.empty())
            {
                logger::warning("[textures] export all: no texture is resident yet");
                return;
            }

            std::string map = sanitizeMapName(getCurrentMapName());
            if (map.empty())
                map = "unknown_level";

            gen::beginBatch(std::move(items), asDds, map);
        }





        bool paramIsColor(uint32_t handle, const char* name)
        {
            if (colorHandles.count(handle))
                return true;

            return name && (containsCI(name, "color") || containsCI(name, "colour") ||
                            containsCI(name, "tint") || containsCI(name, "emissive") ||
                            containsCI(name, "glow") || containsCI(name, "selfillum") ||
                            containsCI(name, "flare"));
        }

        void renderMaterialTextureSlots(const MaterialEntry& m);

        bool materialHasColor(const MaterialEntry& m)
        {
            for (uint32_t i = 0; i < m.vecCount; ++i)
            {
                uint32_t handle = 0;
                uint16_t offset = 0;
                if (!paramInfo(static_cast<const uint8_t*>(m.block), i, handle, offset))
                    continue;
                if (paramIsColor(handle, nameForHandle(handle)))
                    return true;
            }
            return false;
        }

        int tintMaterial(const MaterialEntry& m, const float* rgb, float scale,
                         const std::vector<uint32_t>* only)
        {
            const float pick = (std::max)((std::max)(rgb[0], rgb[1]), rgb[2]);
            if (pick <= 0.0f)
                return 0;

            int written = 0;
            for (uint32_t i = 0; i < m.vecCount; ++i)
            {
                uint32_t handle = 0;
                uint16_t offset = 0;
                if (!paramInfo(static_cast<const uint8_t*>(m.block), i, handle, offset))
                    continue;
                if (only && std::find(only->begin(), only->end(), handle) == only->end())
                    continue;
                if (!paramIsColor(handle, nameForHandle(handle)))
                    continue;

                float orig[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
                readVec(m.block, offset, orig);

                const float magnitude = (std::max)(
                    (std::max)(orig[0], orig[1]), (std::max)(orig[2], 1e-4f));

                float value[4];
                for (int c = 0; c < 3; ++c)
                    value[c] = (rgb[c] / pick) * magnitude * scale;
                value[3] = orig[3];

                if (!findVecBackup(m.block, i))
                    vecBackups.push_back({ m.block, i,
                        { orig[0], orig[1], orig[2], orig[3] } });

                writeVec(m.block, offset, value);
                holdVecOverride(m, handle, value);
                ++written;
            }
            return written;
        }

        std::string materialLabel(const MaterialEntry& m)
        {
            if (m.variationName.empty())
                return m.meshName;
            return m.meshName + "   [" + m.variationName + "]";
        }

    }

    // the manager rewrites ve->sky every blend
    void applySkyOverrides(fb::VisualEnvironment* ve)
    {
        if (!ve)
            return;

        applyResourceOverrides();
    }

    int skySlotCount() { return SKY_SLOT_COUNT; }

    const char* skySlotLabel(int slot)
    {
        return (slot >= 0 && slot < SKY_SLOT_COUNT) ? SKY_SLOTS[slot].label : "";
    }

    int skySlotByLabel(const char* label)
    {
        if (!label)
            return -1;
        for (int i = 0; i < SKY_SLOT_COUNT; ++i)
            if (std::strcmp(SKY_SLOTS[i].label, label) == 0)
                return i;
        return -1;
    }

    void setSkySlot(int slot, void* texture)
    {
        if (slot < 0 || slot >= SKY_SLOT_COUNT)
            return;

        skyCaptureOriginal(slot);
        skyOverride[slot].enabled = true;
        skyOverride[slot].asset = texture;
    }

    void revertSkySlot(int slot) { skyRevertSlot(slot); }

    std::string texturePath(void* dxTexture)
    {
        if (!dxTexture)
            return {};

        if (auto it = textureNames.find(dxTexture); it != textureNames.end())
            return it->second;

        return {};
    }

    void* textureByPath(const std::string& path)
    {
        if (path.empty())
            return nullptr;

        std::string lower = path;
        std::transform(lower.begin(), lower.end(), lower.begin(),
            [](unsigned char c) { return char(std::tolower(c)); });

        for (TextureEntry& e : entries)
            if (e.lowerPath == lower)
            {
                if (!e.texture && e.asset)
                    ensureDrawable(e, false);
                if (e.texture)
                    return e.texture;
            }

        const auto inAssets = [&path]() -> void*
        {
            for (const auto& [name, asset] : textureAssets)
                if (_stricmp(name.c_str(), path.c_str()) == 0)
                    return resolveAssetTexture(asset);
            return nullptr;
        };

        if (textureAssets.empty())
            harvestTextureAssets();

        if (void* const tex = inAssets())
            return tex;

        static uint32_t lastHarvest = 0;
        if (frameCounter - lastHarvest > 300)
        {
            lastHarvest = frameCounter;
            harvestTextureAssets();

            void* const tex = inAssets();

            if (tex)
                requestRescanSoon();

            return tex;
        }

        return nullptr;
    }

    const char* paramName(uint32_t handle)
    {
        auto it = handleNames.find(handle);
        return it == handleNames.end() ? nullptr : it->second.c_str();
    }

    void catalogueCloneOf(void* clone, void* source)
    {
        if (!clone || !source)
            return;

        if (const TextureEntry* src = findTextureEntry(source))
        {
            catalogueClone(clone, *src);
            ++catalogGeneration;
        }
    }

    void rescanNow()
    {
        scan();
        g_lastScanElements = stats.elementCount;
        usagesFor = -1;
        emptyScans = 0;
        autoScanRetryIn = 0;
    }

    namespace detail
    {
#if defined(BFVE_GAME_BF4)
        void* callGetContainer(void* mgr, void* typeInfo)
        {
            using Fn = void* (__fastcall*)(void*, void*);
            return reinterpret_cast<Fn>(OFF_SettingsManager_getSettings)(mgr, typeInfo);
        }

        void callApplySettings(void* mgr)
        {
            using Fn = void (__fastcall*)(void*);
            reinterpret_cast<Fn>(OFF_SettingsManager_apply)(mgr);
        }
#else
        void* callGetContainer(void* mgr, void* typeInfo)
        {
            using Fn = void* (__thiscall*)(void*, const void*);
            return reinterpret_cast<Fn>(OFF_SettingsManager_getSettings)(mgr, typeInfo);
        }

        void callApplySettings(void*) { }
#endif

        void* settingsManagerInstance()
        {
            return *reinterpret_cast<void**>(OFF_g_settingsManager);
        }
    }

    fb::TextureStreamingSettings* streamingSettings()
    {
        void* mgr = settingsManagerInstance();
        if (!mgr)
            return nullptr;

        return static_cast<fb::TextureStreamingSettings*>(callGetContainer(
            mgr, reinterpret_cast<void*>(fb::TextureStreamingSettings::ClassInfoPtr())));
    }

    void applyStreamingSettings()
    {
        if (void* mgr = settingsManagerInstance())
            callApplySettings(mgr);
    }

    void loadAllMissing() { queueEveryMissing(); }

    namespace detail
    {
        std::string lowerOf(const char* s)
        {
            std::string out = s ? s : "";
            std::transform(out.begin(), out.end(), out.begin(),
                [](unsigned char c) { return char(std::tolower(c)); });
            return out;
        }

        bool containsLower(const char* haystack, const std::string& lowerNeedle)
        {
            if (!haystack || lowerNeedle.empty())
                return false;

            for (const char* p = haystack; *p; ++p)
            {
                size_t k = 0;
                while (k < lowerNeedle.size() && p[k] &&
                       char(std::tolower(static_cast<unsigned char>(p[k]))) == lowerNeedle[k])
                    ++k;
                if (k == lowerNeedle.size())
                    return true;
            }
            return false;
        }

        template <typename Fn>
        int forEachMatching(const char* substr, Fn&& fn)
        {
            if (!substr || !*substr)
                return 0;

            const std::string needle = lowerOf(substr);
            int hits = 0;

            for (int i = 0; i < int(entries.size()); ++i)
            {
                TextureEntry& e = entries[i];

                bool match = false;
                if (!e.lowerPath.empty())
                    match = e.lowerPath.find(needle) != std::string::npos;
                else
                    match = containsLower(textureLabel(e), needle);

                if (match && fn(e, i))
                    ++hits;
            }
            return hits;
        }

        void refreshPreview(TextureEntry& e)
        {
            void* fresh = gen::currentSrv(e.texture);
            if (!fresh && liveTexture(e.texture))
                fresh = asTex(e.texture)->m_shaderViews[0];
            if (!srvIsDrawable2D(fresh))
                return;

            if (e.drawable)
                releaseSrv(e.srvLinear);
            addRefSrv(fresh);
            e.srvLinear = fresh;
            e.drawable = true;
        }
    }

    int countTexturesMatching(const char* substr, bool residentOnly)
    {
        return forEachMatching(substr, [residentOnly](TextureEntry& e, int)
        {
            return !residentOnly || (e.texture != nullptr && (e.resource || e.drawable));
        });
    }

    int loadTexturesMatching(const char* substr)
    {
        budgetHit = false;
        raiseBudgetIfWanted();

        return forEachMatching(substr, [](TextureEntry& e, int index)
        {
            if (e.resource || e.srvLinear)
                return false;

            fb::TextureStreamingManager* mgr = nullptr;
            uint16_t handle = 0;
            if (!streamingTarget(e.texture, mgr, handle))
                return false;

            e.handle = handle;
            handleToEntry[handle] = index;
            if (isPinned(handle))
                return false;

            queueLoad(handle);
            return true;
        });
    }

    int tintTexturesMatching(const char* substr, const float rgb[3], float brightness)
    {
        gen::Params p;
        p.colorA[0] = rgb[0];
        p.colorA[1] = rgb[1];
        p.colorA[2] = rgb[2];
        p.colorA[3] = 1.0f;
        p.brightness = brightness;

        return forEachMatching(substr, [&p](TextureEntry& e, int)
        {
            if (!e.texture)
                return false;

            std::string err;
            if (!gen::tintExisting(e.texture, p, err))
                return false;

            refreshPreview(e);
            return true;
        });
    }

    int revertTexturesMatching(const char* substr)
    {
        return forEachMatching(substr, [](TextureEntry& e, int)
        {
            if (!e.texture || !gen::revert(e.texture))
                return false;
            refreshPreview(e);
            return true;
        });
    }

    size_t loadsPending()
    {
        std::lock_guard<std::mutex> lock(loadMutex);
        return loadQueue.size();
    }

    size_t loadsFinished() { return loadsDone.load(); }
    size_t loadsRequested() { return loadsAsked.load(); }
    size_t texturesResident() { return residentTextures; }
    size_t texturesCatalogued() { return entries.size(); }

    bool looksLikeParamBlock(void* block, uint8_t& vec, uint8_t& tex, uint8_t& bol)
    {
        return block && blockIsSane(block, vec, tex, bol);
    }

    namespace detail
    {
        struct ClonedBlock
        {
            void** slot = nullptr;
            void* original = nullptr;
            void* copy = nullptr;
        };

        std::vector<ClonedBlock> g_clonedBlocks;
    }

    bool isClonedParamBlock(void* p)
    {
        for (const ClonedBlock& c : g_clonedBlocks)
            if (c.copy == p)
                return true;
        return false;
    }

    void* cloneParamBlockInto(void** slot)
    {
        if (!slot)
            return nullptr;

        void* block = *slot;

        uint8_t vec = 0, tex = 0, bol = 0;
        if (!looksLikeParamBlock(block, vec, tex, bol))
            return nullptr;

        const uint32_t size = static_cast<fb::ShaderParameterBlock*>(block)->size();
        if (size < 0x10 || size > 0x2000)
            return nullptr;

        auto* copy = static_cast<uint8_t*>(::operator new(size, std::nothrow));
        if (!copy)
            return nullptr;

        std::memcpy(copy, block, size);
        *slot = copy;

        g_clonedBlocks.push_back({ slot, block, copy });
        logger::info("[textures] cloned parameter block {} -> {} ({} bytes)",
            block, static_cast<void*>(copy), size);

        return copy;
    }

    namespace detail
    {
        struct GrownBlock
        {
            fb::SurfaceShaderInstance* instance;
            void* original;
            uint16_t originalHeader;
            void* grown;
        };
        std::vector<GrownBlock> g_grownBlocks;
        std::vector<void*> g_retiredBlocks;

        struct GrowRequest { uint64_t setKey; uint32_t material; uint32_t handle; float value[4]; };
        std::vector<GrowRequest> g_growQueue;

        void queueGrow(uint64_t setKey, uint32_t material, uint32_t handle, const float* value)
        {
            for (const GrowRequest& g : g_growQueue)
                if (g.setKey == setKey && g.material == material && g.handle == handle)
                    return;
            GrowRequest g{ setKey, material, handle, { value[0], value[1], value[2], value[3] } };
            g_growQueue.push_back(g);
        }

        void* engineAlloc(uint32_t size)
        {
#if defined(BFVE_GAME_BF4)
            return reinterpret_cast<void*(__fastcall*)(uint64_t, uint64_t)>(OFF_Malloc_allocAligned)(size, 16);
#else
            // malloc traps on hook threads, use a live block's arena
            void* reference = nullptr;
            for (const MaterialEntry& m : materials)
                if (m.block) { reference = m.block; break; }
            if (!reference)
                return nullptr;
            const uintptr_t entry = reinterpret_cast<const uintptr_t*>(OFF_g_arenaMap)[reinterpret_cast<uintptr_t>(reference) >> 16];
            if (entry == 0 || entry == ~uintptr_t(0) || entry == ~uintptr_t(1))
            {
                logger::warning("[textures] block {} is not arena memory (map entry {:#x})", reference, entry);
                return nullptr;
            }
            using Alloc = void*(__fastcall*)(void* arena, void* edx, unsigned size, unsigned align);
            return reinterpret_cast<Alloc>(OFF_MemoryArena_alloc)(reinterpret_cast<void*>(entry), nullptr, size, 16);
#endif
        }

        void engineFree(void* p)
        {
            if (!p || !OFF_operator_delete)
                return;
#if defined(BFVE_GAME_BF4)
            reinterpret_cast<void(__fastcall*)(void*)>(OFF_operator_delete)(p);
#else
            reinterpret_cast<void(__cdecl*)(void*)>(OFF_operator_delete)(p);
#endif
        }

        uint16_t& instanceHeader(fb::SurfaceShaderInstance* inst)
        {
#if defined(BFVE_GAME_BF4)
            return inst->m_blockSize;
#else
            return inst->m_blockHeader;
#endif
        }

        fb::SurfaceShaderInstance* instanceOf(const MaterialEntry& m)
        {
            if (!m.set)
                return nullptr;
            const auto* set = static_cast<const uint8_t*>(m.set);
            uint8_t* materials = setMaterials(set);
            if (!materials || m.index >= setMaterialCount(set))
                return nullptr;
            return reinterpret_cast<fb::SurfaceShaderInstance*>(materials + 1ull * m.index * layout.instanceStride);
        }

        void* growBlockWithVector(fb::SurfaceShaderInstance* inst, uint32_t handle, const float* value)
        {
            if (!inst)
                return nullptr;
            void* block = inst->m_block;
            uint8_t vec = 0, tex = 0, bol = 0;
            if (block && (!blockIsSane(block, vec, tex, bol) || vec == 255))
                return nullptr;

            const auto* old = static_cast<const fb::ShaderParameterBlock*>(block);
            for (uint32_t i = 0; i < vec; ++i)
                if (old->m_entries[i].m_handle == handle)
                    return block;

            const uint32_t nvec = vec + 1u, total = nvec + tex + bol;
#if defined(BFVE_GAME_BF4)
            constexpr uint32_t TEX_BYTES = 8;
#else
            constexpr uint32_t TEX_BYTES = 4;
#endif
            const uint32_t head = (8u * total + 15u) & ~0xFu;
            const uint32_t size = 16u + head + 16u * nvec + TEX_BYTES * tex + bol;
            const uint16_t header = uint16_t((size & 0x1FFF) | (1u << 13) | (tex ? 1u << 14 : 0u) | (bol ? 1u << 15 : 0u));

            auto* grown = static_cast<uint8_t*>(engineAlloc(size));
            if (!grown || (reinterpret_cast<uintptr_t>(grown) & 15) != 0)
            {
                logger::warning("[textures] engine allocation of {} bytes failed ({})", size, static_cast<void*>(grown));
                return nullptr;
            }
            std::memset(grown, 0, size);

            auto* nb = reinterpret_cast<fb::ShaderParameterBlock*>(grown);
#if defined(BFVE_GAME_BF4)
            nb->m_sizeFlags = header;
#else
            nb->m_header = header;
            nb->m_size = uint16_t(size);
#endif
            nb->m_vectorCount = uint8_t(nvec);
            nb->m_textureCount = tex;
            nb->m_boolCount = bol;
            {
                uint16_t off = uint16_t(head);
                uint32_t e = 0;
                for (uint32_t i = 0; i < nvec; ++i, ++e, off += 16)
                    nb->m_entries[e] = { 0, off, 1 };
                for (uint32_t i = 0; i < tex; ++i, ++e, off += uint16_t(TEX_BYTES))
                    nb->m_entries[e] = { 0, off, 1 };
                for (uint32_t i = 0; i < bol; ++i, ++e, off += 1)
                    nb->m_entries[e] = { 0, off, 1 };
            }

            const char* oldData = old ? reinterpret_cast<const char*>(old) + 0x10 : nullptr;
            char* data = nb->values();
            uint32_t e = 0;
            for (uint32_t i = 0; i < vec; ++i, ++e)
            {
                nb->m_entries[e].m_handle = old->m_entries[i].m_handle;
                std::memcpy(data + nb->m_entries[e].m_offset, oldData + old->m_entries[i].m_offset, 16);
            }
            nb->m_entries[e].m_handle = handle;
            std::memcpy(data + nb->m_entries[e].m_offset, value, 16);
            ++e;
            for (uint32_t i = 0; i < tex; ++i, ++e)
            {
                const auto& src = old->m_entries[vec + i];
                nb->m_entries[e].m_handle = src.m_handle;
                std::memcpy(data + nb->m_entries[e].m_offset, oldData + src.m_offset, TEX_BYTES);
            }
            for (uint32_t i = 0; i < bol; ++i, ++e)
            {
                const auto& src = old->m_entries[vec + tex + i];
                nb->m_entries[e].m_handle = src.m_handle;
                data[nb->m_entries[e].m_offset] = oldData[src.m_offset];
            }

            GrownBlock* rec = nullptr;
            for (GrownBlock& r : g_grownBlocks)
                if (r.instance == inst)
                    rec = &r;
            if (rec)
            {
                if (inst->m_block == rec->grown)
                    g_retiredBlocks.push_back(rec->grown);
                else
                {
                    rec->original = block;
                    rec->originalHeader = instanceHeader(inst);
                }
                rec->grown = grown;
            }
            else
            {
                g_grownBlocks.push_back({ inst, block, instanceHeader(inst), grown });
            }

            inst->m_block = grown;
            instanceHeader(inst) = header;
            logger::info("[textures] grew parameter block {} -> {} ({} bytes, {} vector slot(s))",
                block, static_cast<void*>(grown), size, nvec);
            return grown;
        }

        void restoreGrownBlock(fb::SurfaceShaderInstance* inst)
        {
            for (auto it = g_grownBlocks.begin(); it != g_grownBlocks.end(); ++it)
            {
                if (it->instance != inst)
                    continue;
                if (inst->m_block == it->grown)
                {
                    inst->m_block = it->original;
                    instanceHeader(inst) = it->originalHeader;
                    g_retiredBlocks.push_back(it->grown);
                }
                g_grownBlocks.erase(it);
                return;
            }
        }

        void freeRetiredBlocks()
        {
            for (void* p : g_retiredBlocks)
                engineFree(p);
            g_retiredBlocks.clear();
        }

        void releaseGrownBlocks()
        {
            for (const GrownBlock& r : g_grownBlocks)
            {
                if (r.instance->m_block != r.grown)
                    continue;
                r.instance->m_block = r.original;
                instanceHeader(r.instance) = r.originalHeader;
                g_retiredBlocks.push_back(r.grown);
            }
            if (!g_grownBlocks.empty())
                logger::info("[textures] released {} grown parameter block(s)", g_grownBlocks.size());
            g_grownBlocks.clear();
            g_growQueue.clear();
        }
    }

    void tickGameThread()
    {
        freeRetiredBlocks();
        if (g_growQueue.empty())
            return;

        std::vector<GrowRequest> queue;
        queue.swap(g_growQueue);
        bool grew = false;
        for (const GrowRequest& g : queue)
        {
            MaterialEntry live{};
            Resolve why = Resolve::Ok;
            liveBlock(g.setKey, g.material, live, why);
            if (!live.set)
                continue;
            fb::SurfaceShaderInstance* inst = instanceOf(live);
            if (!inst)
                continue;
            uint16_t off = 0;
            if (inst->m_block && blockIsSane(inst->m_block, live.vecCount, live.texCount, live.boolCount))
            {
                live.block = inst->m_block;
                if (slotForHandle(live, g.handle, false, off))
                    continue;
            }
            if (growBlockWithVector(inst, g.handle, g.value))
                grew = true;
        }
        if (grew)
            requestRescan(1);
    }

    bool addVectorParam(const MaterialEntry& m, const char* name, const float* value)
    {
        if (!name || !*name)
            return false;
        registerParamName(name);
        return addVectorParamHandle(m, shaderParamHandle(name), value);
    }

    bool addVectorParamHandle(const MaterialEntry& m, uint32_t handle, const float* value)
    {
        if (!handle || !m.setKey)
            return false;

        if (!instanceOf(m))
            return false;
        queueGrow(m.setKey, m.index, handle, value);

        if (ParamOverride* o = findOverride(m.setKey, m.index, handle, nullptr))
        {
            std::memcpy(o->value, value, sizeof(o->value));
            o->isTexture = false;
            o->added = true;
        }
        else
        {
            ParamOverride n;
            n.setKey = m.setKey;
            n.material = m.index;
            n.handle = handle;
            n.added = true;
            std::memcpy(n.value, value, sizeof(n.value));
            paramOverrides.push_back(n);
        }
        return true;
    }

    void removeAddedParam(const MaterialEntry& m, uint32_t handle)
    {
        dropOverride(m.setKey, m.index, handle, m.block);
        fb::SurfaceShaderInstance* inst = instanceOf(m);
        if (!inst)
            return;
        restoreGrownBlock(inst);
        for (const ParamOverride& o : paramOverrides)
            if (o.added && !o.isTexture && o.setKey == m.setKey && o.material == m.index)
                queueGrow(o.setKey, o.material, o.handle, o.value);
        requestRescan(1);
    }

    void releaseClonedParamBlocks()
    {
        for (const ClonedBlock& c : g_clonedBlocks)
        {
            if (*c.slot == c.copy)
                *c.slot = c.original;

            ::operator delete(c.copy);
        }

        if (!g_clonedBlocks.empty())
            logger::info("[textures] released {} cloned parameter block(s)",
                g_clonedBlocks.size());

        g_clonedBlocks.clear();
    }

    int privatiseBlockTextures(void* block)
    {
        uint8_t vec = 0, tex = 0, bol = 0;
        if (!looksLikeParamBlock(block, vec, tex, bol))
            return 0;

        int detached = 0;
        for (uint32_t t = 0; t < tex; ++t)
        {
            const uint32_t slot = uint32_t(vec) + t;

            uint32_t handle = 0;
            uint16_t offset = 0;
            if (!paramInfo(static_cast<const uint8_t*>(block), slot, handle, offset))
                continue;

            void* current = readTex(block, offset);
            if (!current)
                continue;

            if (gen::isClonedTexture(current))
                continue;

            void* copy = gen::cloneTexture(current);
            if (!copy)
                continue;

            if (!findTexBackup(block, slot))
                texBackups.push_back({ block, slot, current });

            writeTex(block, offset, copy);

            if (const TextureEntry* src = findTextureEntry(current))
                catalogueClone(copy, *src);

            ++detached;
        }

        if (detached)
            logger::info("[textures] block {}: {} texture slot(s) given private copies",
                block, detached);

        return detached;
    }

    uint32_t paramHandle(const char* name)
    {
        return name ? shaderParamHandle(name) : 0;
    }

    void registerParamName(const char* name)
    {
        if (!name || !*name)
            return;
        handleNames.emplace(shaderParamHandle(name), name);
    }

    bool blockHasHandle(void* block, uint32_t handle)
    {
        uint8_t vec = 0, tex = 0, bol = 0;
        if (!handle || !looksLikeParamBlock(block, vec, tex, bol))
            return false;

        const uint32_t slots = uint32_t(vec) + tex + bol;
        for (uint32_t i = 0; i < slots; ++i)
        {
            uint32_t h = 0;
            uint16_t off = 0;
            if (paramInfo(static_cast<const uint8_t*>(block), i, h, off) && h == handle)
                return true;
        }
        return false;
    }

    void setMirrorBlocks(const std::vector<void*>* blocks) { g_mirrorBlocks = blocks; }

    uint32_t searchTextures(const char* needle)
    {
        if (!needle)
            return 0;

        std::snprintf(search, sizeof(search), "%s", needle);
        selected = -1;

        std::string lower = needle;
        std::transform(lower.begin(), lower.end(), lower.begin(),
            [](unsigned char c) { return char(std::tolower(c)); });

        uint32_t hits = 0, named = 0;
        for (const TextureEntry& e : entries)
        {
            if (!e.nameCached)
                continue;
            ++named;
            if (e.lowerPath.find(lower) != std::string::npos)
                ++hits;
        }

        if (hits)
            logger::info("[textures] browser filtered to \"{}\" - {} match(es)", needle, hits);
        else
            logger::warning("[textures] browser filtered to \"{}\" - nothing matches, of {} "
                            "named texture(s) in the catalogue. Press \"Load all\" and rescan "
                            "if it has not streamed in yet.", needle, named);

        return hits;
    }
    void setEditRedirect(EditRedirect fn, void* user)
    {
        g_editRedirect = fn;
        g_editRedirectUser = user;
    }

    const char* nameForParamHandle(uint32_t handle) { return nameForHandle(handle); }

    void* registerAssetTextureName(void* textureAsset, const char* path)
    {
        if (!textureAsset || !path || !*path)
            return nullptr;

        void* tex = resolveAssetTexture(textureAsset);
        if (!tex)
            return nullptr;

        std::string lower = path;
        std::transform(lower.begin(), lower.end(), lower.begin(),
            [](unsigned char c) { return char(std::tolower(c)); });

        auto it = textureNames.find(tex);
        if (it == textureNames.end() || it->second != lower)
        {
            textureNames[tex] = lower;

            for (TextureEntry& e : entries)
            {
                if (e.texture != tex)
                    continue;

                e.nameCached = true;
                e.lowerPath = lower;
                const size_t slash = e.lowerPath.find_last_of("/\\");
                e.shortName = (slash == std::string::npos) ? e.lowerPath
                                                           : e.lowerPath.substr(slash + 1);
                ++catalogGeneration;
                break;
            }
        }

        return tex;
    }

    void* g_lampEditTex = nullptr;

    void cacheName(TextureEntry& e) { detail::ensureNameCache(e); }

    std::string lowerCopy(const std::string& s)
    {
        std::string out = s;
        std::transform(out.begin(), out.end(), out.begin(),
            [](unsigned char c) { return char(std::tolower(c)); });
        return out;
    }


    bool selectTextureNamed(const char* path)
    {
        if (!path || !*path)
            return false;

        std::string want = path;
        std::transform(want.begin(), want.end(), want.begin(),
            [](unsigned char c) { return char(std::tolower(c)); });

        for (size_t i = 0; i < entries.size(); ++i)
        {
            if (!entries[i].nameCached)
                continue;
            if (entries[i].lowerPath == want)
            {
                selectTextureIndex(int(i));
                return true;
            }
        }

        logger::warning("[textures] {} is not in the catalogue - scan, or load it first", path);
        return false;
    }



    // "lightceiling_02_flicker_Mesh" -> "lightceiling_02"
    std::string meshFamily(const std::string& meshName)
    {
        std::string base = meshName;
        if (const size_t slash = base.find_last_of("/\\"); slash != std::string::npos)
            base = base.substr(slash + 1);

        std::string family;
        size_t start = 0;
        while (start <= base.size())
        {
            const size_t at = base.find('_', start);
            const std::string token = base.substr(start,
                at == std::string::npos ? std::string::npos : at - start);

            if (!family.empty())
                family += '_';
            family += token;

            const bool numeric = !token.empty() &&
                token.find_first_not_of("0123456789") == std::string::npos;
            if (numeric || at == std::string::npos)
                break;

            start = at + 1;
        }

        std::transform(family.begin(), family.end(), family.begin(),
            [](unsigned char c) { return char(std::tolower(c)); });
        return family;
    }

    void collectMeshTextures(const std::vector<MeshVariationRef>& meshes,
                                    std::vector<std::pair<uint32_t, std::string>>& found, bool& byName)
    {
        const auto add = [&found](uint32_t handle, std::string path)
        {
            for (const auto& [h, p] : found)
                if (p == path)
                    return;
            found.push_back({ handle, std::move(path) });
        };

        for (const MeshVariationRef& ref : meshes)
        {
            const uint32_t wantMesh = uint32_t(ref.key >> 32);

            for (const auto& [key, e] : g_ebx)
                if (uint32_t(key >> 32) == wantMesh)
                    for (const EbxMaterial& em : e.materials)
                        for (const auto& [handle, path] : em.textures)
                            add(handle, path);

            for (const MaterialEntry& m : materials)
            {
                if (uint32_t(m.setKey >> 32) != wantMesh || !m.texCount || !m.block)
                    continue;

                for (uint32_t t = 0; t < m.texCount; ++t)
                {
                    const uint32_t slot = uint32_t(m.vecCount) + t;

                    uint32_t handle = 0;
                    uint16_t offset = 0;
                    if (!paramInfo(static_cast<const uint8_t*>(m.block), slot, handle, offset))
                        continue;

                    void* tex = readTex(m.block, offset);
                    if (!tex)
                        continue;

                    std::string path = texturePath(tex);
                    if (!path.empty())
                        add(handle, std::move(path));
                }
            }
        }

        byName = false;
        if (found.empty())
        {
            byName = true;
            for (const MeshVariationRef& ref : meshes)
            {
                const std::string family = meshFamily(ref.name);
                if (family.empty())
                    continue;

                for (const TextureEntry& e : entries)
                    if (found.size() < 12 && e.lowerPath.find(family) != std::string::npos)
                        add(0, e.lowerPath);
            }
        }
    }

    std::vector<std::string> meshTextureNames(const MeshVariationRef& ref, size_t max)
    {
        std::vector<std::pair<uint32_t, std::string>> found;
        bool byName = false;
        collectMeshTextures({ ref }, found, byName);
        std::vector<std::string> out;
        for (const auto& [h, p] : found)
        {
            if (out.size() >= max)
                break;
            out.push_back(p);
        }
        return out;
    }




    void* meshSetByName(const char* name)
    {
#if defined(BFVE_GAME_BF4)
        (void)name;
        return nullptr;
#else
        if (!name || !*name || !g_bf3MeshSetVtable)
            return nullptr;
        std::string key = name;
        std::transform(key.begin(), key.end(), key.begin(),
            [](unsigned char c) { return char(std::tolower(c)); });
        auto it = g_bf3ResourceByName.find(key);
        if (it == g_bf3ResourceByName.end() || *static_cast<void**>(it->second) != g_bf3MeshSetVtable)
            return nullptr;
        return it->second;
#endif
    }

    bool heldVecParam(const void* block, uint32_t handle, float (&out)[4])
    {
        if (g_heldCount.load(std::memory_order_relaxed) == 0)
            return false;

        std::lock_guard<std::mutex> lock(g_heldMutex);
        auto it = g_heldVec.find(block);
        if (it == g_heldVec.end())
            return false;
        for (const HeldVec& h : it->second)
            if (h.handle == handle)
            {
                out[0] = h.value[0]; out[1] = h.value[1]; out[2] = h.value[2]; out[3] = h.value[3];
                return true;
            }
        return false;
    }
}
