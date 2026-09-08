#include "surfacepick.h"
#include "../../utils/log.h"

#include <Windows.h>
#include <d3d11.h>
#include <d3d11_1.h>
#include <MinHook.h>
#include <algorithm>
#include <atomic>
#include <cstring>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <unordered_map>
#include <vector>

// A probe cycle is `slices` consecutive frames. Each scene draw is re-issued once, in the frame
// its identity (pixel shader, srv0, count) hashes to, under a 1x1 scissor at the crosshair with
// an occlusion query and no color/depth writes. Constant buffers are snapshotted once per
// frame; results are read back only after an event query says the GPU is done, so the render
// thread never waits on the GPU
namespace editor::textures::pick
{
    namespace
    {
        using DrawIndexed_t = void(STDMETHODCALLTYPE*)(ID3D11DeviceContext*, UINT, UINT, INT);
        using Draw_t = void(STDMETHODCALLTYPE*)(ID3D11DeviceContext*, UINT, UINT);
        using DrawIndexedInstanced_t = void(STDMETHODCALLTYPE*)(ID3D11DeviceContext*, UINT, UINT, UINT, INT, UINT);
        using DrawInstanced_t = void(STDMETHODCALLTYPE*)(ID3D11DeviceContext*, UINT, UINT, UINT, UINT);

        DrawIndexed_t oDrawIndexed = nullptr;
        Draw_t oDraw = nullptr;
        DrawIndexedInstanced_t oDrawIndexedInstanced = nullptr;
        DrawInstanced_t oDrawInstanced = nullptr;

        ID3D11Device* g_device = nullptr;
        ID3D11DeviceContext* g_immediate = nullptr;

        struct Record
        {
            ID3D11PixelShader* ps;
            ID3D11ShaderResourceView* srvs[16];
            ID3D11Buffer* cbs[3];
            UINT cbFirst[3];
            uint32_t order; // scene draw index within its frame; hits sort by it, last = nearest
            uint32_t slice;
        };

        constexpr uint32_t kMaxRecords = 8000;
        Record g_records[kMaxRecords];
        ID3D11Query* g_queries[kMaxRecords];
        std::atomic<uint32_t> g_recordCount{ 0 }; // slots are claimed lock-free from any recording thread
        std::atomic<uint32_t> g_sceneDraws{ 0 };
        std::atomic<bool> g_armed{ false };
        std::atomic<uint32_t> g_slice{ 0 };
        int g_px = 0, g_py = 0, g_w = 0, g_h = 0;

        // Probe variants of the game's own states, keyed by the original (held with a ref so
        // the address cannot be reused under the key). Lookups take the shared lock.
        std::shared_mutex g_cacheMutex;
        std::unordered_map<void*, ID3D11RasterizerState*> g_rsProbe;
        std::unordered_map<void*, ID3D11BlendState*> g_bsProbe;
        std::unordered_map<void*, ID3D11DepthStencilState*> g_dsProbe;
        struct ViewSize { int w, h; };
        std::unordered_map<ID3D11View*, ViewSize> g_viewSize; // refs held, cleared every cycle

        // One staging copy per (frame, constant buffer), taken at that frame's present.
        struct Snapshot { ID3D11Buffer* source; ID3D11Buffer* staging; uint32_t slice; UINT width; };
        std::vector<Snapshot> g_snapshots;
        std::vector<std::pair<UINT, ID3D11Buffer*>> g_stagingPool;
        constexpr size_t kMaxSnapshotsPerFrame = 16;
        ID3D11Query* g_fence = nullptr; // D3D11_QUERY_EVENT ended after the last frame's copies
        bool g_pending = false;
        uint32_t g_pendingFrames = 0;

        std::mutex g_resultMutex;
        Surface g_result;
        uint32_t g_frame = 0;

        // Sorted; swapped whole so the recording threads never take a lock for it.
        std::atomic<std::shared_ptr<const std::vector<const void*>>> g_shaderFilter;

        bool knownShader(const void* ps)
        {
            if (!onlyKnown)
                return true;
            const std::shared_ptr<const std::vector<const void*>> f = g_shaderFilter.load(std::memory_order_acquire);
            if (!f || f->empty())
                return true;
            return std::binary_search(f->begin(), f->end(), ps);
        }

        bool sceneSized(const ViewSize& size)
        {
            return size.w * 2 >= g_w && size.h * 2 >= g_h && size.w <= g_w && size.h <= g_h;
        }

        bool sceneView(ID3D11View* view, ViewSize& size)
        {
            {
                std::shared_lock<std::shared_mutex> lock(g_cacheMutex);
                auto it = g_viewSize.find(view);
                if (it != g_viewSize.end())
                {
                    size = it->second;
                    return sceneSized(size);
                }
            }
            ViewSize vs{ 0, 0 };
            ID3D11Resource* res = nullptr;
            view->GetResource(&res);
            if (res)
            {
                ID3D11Texture2D* tex = nullptr;
                if (SUCCEEDED(res->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&tex))) && tex)
                {
                    D3D11_TEXTURE2D_DESC d{ };
                    tex->GetDesc(&d);
                    vs = { int(d.Width), int(d.Height) };
                    tex->Release();
                }
                res->Release();
            }
            std::unique_lock<std::shared_mutex> lock(g_cacheMutex);
            if (g_viewSize.emplace(view, vs).second)
                view->AddRef();
            size = vs;
            return sceneSized(size);
        }

        template <typename T>
        bool cached(std::unordered_map<void*, T*>& map, void* key, T*& out)
        {
            std::shared_lock<std::shared_mutex> lock(g_cacheMutex);
            auto it = map.find(key);
            if (it == map.end())
                return false;
            out = it->second;
            return true;
        }

        template <typename T>
        T* store(std::unordered_map<void*, T*>& map, IUnknown* key, T* made)
        {
            std::unique_lock<std::shared_mutex> lock(g_cacheMutex);
            auto it = map.find(key);
            if (it != map.end())
            {
                if (made) made->Release();
                return it->second;
            }
            if (key) key->AddRef();
            map[key] = made;
            return made;
        }

        ID3D11RasterizerState* probeRs(ID3D11RasterizerState* rs)
        {
            ID3D11RasterizerState* out = nullptr;
            if (cached(g_rsProbe, rs, out))
                return out;
            D3D11_RASTERIZER_DESC d{ };
            if (rs)
                rs->GetDesc(&d);
            else
            {
                d.FillMode = D3D11_FILL_SOLID;
                d.CullMode = D3D11_CULL_BACK;
                d.DepthClipEnable = TRUE;
            }
            d.ScissorEnable = TRUE;
            g_device->CreateRasterizerState(&d, &out);
            return store(g_rsProbe, rs, out);
        }

        ID3D11BlendState* probeBs(ID3D11BlendState* bs)
        {
            ID3D11BlendState* out = nullptr;
            if (cached(g_bsProbe, bs, out))
                return out;
            D3D11_BLEND_DESC d{ };
            if (bs)
                bs->GetDesc(&d);
            else
            {
                d.RenderTarget[0].BlendEnable = FALSE;
                d.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
                d.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
                d.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
                d.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
                d.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
                d.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
            }
            for (auto& rt : d.RenderTarget)
                rt.RenderTargetWriteMask = 0;
            g_device->CreateBlendState(&d, &out);
            return store(g_bsProbe, bs, out);
        }

        // null when the draw does not depth test: it cannot tell what is under the crosshair
        ID3D11DepthStencilState* probeDs(ID3D11DepthStencilState* ds)
        {
            ID3D11DepthStencilState* out = nullptr;
            if (cached(g_dsProbe, ds, out))
                return out;
            D3D11_DEPTH_STENCIL_DESC d{ };
            if (ds)
                ds->GetDesc(&d);
            else
            {
                d.DepthEnable = TRUE;
                d.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
                d.DepthFunc = D3D11_COMPARISON_LESS;
            }
            if (d.DepthEnable)
            {
                d.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
                d.StencilWriteMask = 0;
                if (d.DepthFunc == D3D11_COMPARISON_LESS) d.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
                else if (d.DepthFunc == D3D11_COMPARISON_GREATER) d.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
                g_device->CreateDepthStencilState(&d, &out);
            }
            return store(g_dsProbe, ds, out);
        }

        uint32_t sliceOf(const void* ps, const void* srv0, UINT count)
        {
            const uint32_t n = slices ? slices : 1;
            uint64_t h = (uint64_t(ps) >> 4) * 0x9E3779B97F4A7C15ull;
            h ^= (uint64_t(srv0) >> 4) * 0xC2B2AE3D27D4EB4Full;
            h ^= uint64_t(count) * 0x165667B19E3779F9ull;
            h ^= h >> 29;
            return uint32_t((h >> 32) % n);
        }

        template <typename Draw>
        void probe(ID3D11DeviceContext* ctx, UINT count, Draw&& draw)
        {
            draw();
            if (!g_armed.load(std::memory_order_acquire))
                return;

            ID3D11PixelShader* ps = nullptr;
            ctx->PSGetShader(&ps, nullptr, nullptr);
            if (!ps)
                return; // depth-only: nothing to identify
            if (!knownShader(ps))
            {
                ps->Release();
                return;
            }

            ID3D11DepthStencilView* dsv = nullptr;
            ctx->OMGetRenderTargets(0, nullptr, &dsv);
            if (!dsv)
            {
                ps->Release();
                return;
            }
            ViewSize size{ };
            const bool scene = sceneView(dsv, size);
            dsv->Release();
            if (!scene)
            {
                ps->Release();
                return;
            }
            const uint32_t order = g_sceneDraws.fetch_add(1, std::memory_order_relaxed);

            ID3D11ShaderResourceView* srv0 = nullptr;
            ctx->PSGetShaderResources(0, 1, &srv0);
            const uint32_t slice = sliceOf(ps, srv0, count);
            if (srv0) srv0->Release();
            if (slice != g_slice.load(std::memory_order_relaxed))
            {
                ps->Release();
                return;
            }

            ID3D11DepthStencilState* ds = nullptr; UINT stencilRef = 0;
            ctx->OMGetDepthStencilState(&ds, &stencilRef);
            ID3D11DepthStencilState* pds = probeDs(ds);
            const uint32_t slot = pds ? g_recordCount.fetch_add(1, std::memory_order_relaxed) : kMaxRecords;
            if (slot >= kMaxRecords)
            {
                if (ds) ds->Release();
                ps->Release();
                return;
            }
            if (!g_queries[slot])
            {
                D3D11_QUERY_DESC qd{ };
                qd.Query = D3D11_QUERY_OCCLUSION;
                g_device->CreateQuery(&qd, &g_queries[slot]);
            }

            ID3D11RasterizerState* rs = nullptr;
            ctx->RSGetState(&rs);
            ID3D11BlendState* bs = nullptr; float factor[4] = { }; UINT sampleMask = 0;
            ctx->OMGetBlendState(&bs, factor, &sampleMask);
            UINT rectCount = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
            D3D11_RECT rects[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE] = { };
            ctx->RSGetScissorRects(&rectCount, rects);

            const int px = g_px * size.w / g_w, py = g_py * size.h / g_h;
            const D3D11_RECT one{ px, py, px + 1, py + 1 };
            ctx->RSSetState(probeRs(rs));
            ctx->RSSetScissorRects(1, &one);
            ctx->OMSetBlendState(probeBs(bs), factor, sampleMask);
            ctx->OMSetDepthStencilState(pds, stencilRef);

            Record& r = g_records[slot];
            r = Record{ };
            r.ps = ps;
            r.order = order;
            r.slice = slice;
            ctx->Begin(g_queries[slot]);
            draw();
            ctx->End(g_queries[slot]);
            ctx->PSGetShaderResources(0, 16, r.srvs);
            ID3D11DeviceContext1* ctx1 = nullptr;
            if (SUCCEEDED(ctx->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&ctx1))) && ctx1)
            {
                UINT counts[3] = { };
                ctx1->PSGetConstantBuffers1(0, 3, r.cbs, r.cbFirst, counts);
                ctx1->Release();
            }
            else
                ctx->PSGetConstantBuffers(0, 3, r.cbs);

            ctx->RSSetState(rs);
            ctx->RSSetScissorRects(rectCount, rectCount ? rects : nullptr);
            ctx->OMSetBlendState(bs, factor, sampleMask);
            ctx->OMSetDepthStencilState(ds, stencilRef);
            if (rs) rs->Release();
            if (bs) bs->Release();
            if (ds) ds->Release();
        }

        void STDMETHODCALLTYPE hkDrawIndexed(ID3D11DeviceContext* ctx, UINT count, UINT start, INT base)
        {
            probe(ctx, count, [&] { oDrawIndexed(ctx, count, start, base); });
        }
        void STDMETHODCALLTYPE hkDraw(ID3D11DeviceContext* ctx, UINT count, UINT start)
        {
            probe(ctx, count, [&] { oDraw(ctx, count, start); });
        }
        void STDMETHODCALLTYPE hkDrawIndexedInstanced(ID3D11DeviceContext* ctx, UINT idx, UINT inst, UINT startIdx, INT base, UINT startInst)
        {
            probe(ctx, idx, [&] { oDrawIndexedInstanced(ctx, idx, inst, startIdx, base, startInst); });
        }
        void STDMETHODCALLTYPE hkDrawInstanced(ID3D11DeviceContext* ctx, UINT vtx, UINT inst, UINT startVtx, UINT startInst)
        {
            probe(ctx, vtx, [&] { oDrawInstanced(ctx, vtx, inst, startVtx, startInst); });
        }

        uint32_t recordCount()
        {
            const uint32_t n = g_recordCount.load(std::memory_order_acquire);
            return n < kMaxRecords ? n : kMaxRecords;
        }

        ID3D11Buffer* takeStaging(UINT width)
        {
            for (auto it = g_stagingPool.begin(); it != g_stagingPool.end(); ++it)
                if (it->first == width)
                {
                    ID3D11Buffer* b = it->second;
                    g_stagingPool.erase(it);
                    return b;
                }
            D3D11_BUFFER_DESC sd{ };
            sd.ByteWidth = width;
            sd.Usage = D3D11_USAGE_STAGING;
            sd.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
            ID3D11Buffer* b = nullptr;
            g_device->CreateBuffer(&sd, nullptr, &b);
            return b;
        }

        // Copies every constant buffer this frame's records reference, on the immediate context
        // before the present, so the values the draws were issued with survive until the readback.
        void snapshotConstants(uint32_t slice)
        {
            size_t taken = 0;
            const uint32_t count = recordCount();
            for (uint32_t i = 0; i < count && taken < kMaxSnapshotsPerFrame; ++i)
            {
                const Record& r = g_records[i];
                if (r.slice != slice)
                    continue;
                for (ID3D11Buffer* b : r.cbs)
                {
                    if (!b)
                        continue;
                    bool have = false;
                    for (const Snapshot& s : g_snapshots)
                        if (s.source == b && s.slice == slice)
                        {
                            have = true;
                            break;
                        }
                    if (have)
                        continue;
                    D3D11_BUFFER_DESC d{ };
                    b->GetDesc(&d);
                    if (!d.ByteWidth)
                        continue;
                    ID3D11Buffer* staging = takeStaging(d.ByteWidth);
                    if (!staging)
                        continue;
                    g_immediate->CopyResource(staging, b);
                    g_snapshots.push_back({ b, staging, slice, d.ByteWidth });
                    ++taken;
                }
            }
        }

        void resolve()
        {
            const uint32_t count = recordCount();
            struct Mapped { const Snapshot* snap; const uint8_t* data; };
            std::vector<Mapped> mapped;
            mapped.reserve(g_snapshots.size());
            for (const Snapshot& s : g_snapshots)
            {
                D3D11_MAPPED_SUBRESOURCE map{ };
                const bool ok = SUCCEEDED(g_immediate->Map(s.staging, 0, D3D11_MAP_READ, D3D11_MAP_FLAG_DO_NOT_WAIT, &map));
                mapped.push_back({ &s, ok ? static_cast<const uint8_t*>(map.pData) : nullptr });
            }

            struct HitRef { uint32_t order; uint32_t record; };
            std::vector<HitRef> hits;
            for (uint32_t i = 0; i < count; ++i)
            {
                UINT64 samples = 0;
                if (g_immediate->GetData(g_queries[i], &samples, sizeof(samples), D3D11_ASYNC_GETDATA_DONOTFLUSH) == S_OK && samples)
                    hits.push_back({ g_records[i].order, i });
            }
            std::sort(hits.begin(), hits.end(), [](const HitRef& a, const HitRef& b) { return a.order < b.order; });
            if (hits.size() > 32)
                hits.erase(hits.begin(), hits.end() - 32);

            Surface s{ };
            s.frame = ++g_frame;
            s.draws = count;
            s.valid = !hits.empty();
            for (const HitRef& hr : hits)
            {
                const Record& r = g_records[hr.record];
                Hit& h = s.hits[s.hitCount++];
                h.pixelShader = r.ps;
                for (int i = 0; i < 16; ++i)
                    h.srvs[i] = r.srvs[i];
                for (int c = 0; c < 3; ++c)
                {
                    if (!r.cbs[c])
                        continue;
                    for (const Mapped& m : mapped)
                    {
                        if (!m.data || m.snap->source != r.cbs[c] || m.snap->slice != r.slice)
                            continue;
                        const size_t start = size_t(r.cbFirst[c]) * 16;
                        if (start < m.snap->width)
                        {
                            const size_t n = m.snap->width - start < 256 ? m.snap->width - start : 256;
                            std::memcpy(h.cb[c], m.data + start, n);
                            h.cbValid[c] = true;
                        }
                        break;
                    }
                }
            }

            for (const Mapped& m : mapped)
                if (m.data)
                    g_immediate->Unmap(m.snap->staging, 0);
            for (const Snapshot& sn : g_snapshots)
                g_stagingPool.emplace_back(sn.width, sn.staging);
            g_snapshots.clear();
            for (uint32_t i = 0; i < count; ++i)
            {
                Record& r = g_records[i];
                if (r.ps) r.ps->Release();
                for (auto* v : r.srvs)
                    if (v) v->Release();
                for (auto* b : r.cbs)
                    if (b) b->Release();
            }
            g_recordCount.store(0, std::memory_order_release);

            std::lock_guard<std::mutex> lock(g_resultMutex);
            g_result = s;
        }

        void startCycle(int x, int y, int w, int h)
        {
            {
                std::unique_lock<std::shared_mutex> lock(g_cacheMutex);
                for (auto& [view, size] : g_viewSize)
                    view->Release();
                g_viewSize.clear();
            }
            g_px = x;
            g_py = y;
            g_w = w;
            g_h = h;
            g_sceneDraws.store(0, std::memory_order_relaxed);
            g_recordCount.store(0, std::memory_order_relaxed);
            g_slice.store(0, std::memory_order_relaxed);
            g_armed.store(true, std::memory_order_release);
        }
    }

    namespace
    {
        DrawIndexed_t oDrawIndexedD = nullptr;
        Draw_t oDrawD = nullptr;
        DrawIndexedInstanced_t oDrawIndexedInstancedD = nullptr;
        DrawInstanced_t oDrawInstancedD = nullptr;

        void STDMETHODCALLTYPE hkDrawIndexedD(ID3D11DeviceContext* ctx, UINT count, UINT start, INT base)
        {
            probe(ctx, count, [&] { oDrawIndexedD(ctx, count, start, base); });
        }
        void STDMETHODCALLTYPE hkDrawD(ID3D11DeviceContext* ctx, UINT count, UINT start)
        {
            probe(ctx, count, [&] { oDrawD(ctx, count, start); });
        }
        void STDMETHODCALLTYPE hkDrawIndexedInstancedD(ID3D11DeviceContext* ctx, UINT idx, UINT inst, UINT startIdx, INT base, UINT startInst)
        {
            probe(ctx, idx, [&] { oDrawIndexedInstancedD(ctx, idx, inst, startIdx, base, startInst); });
        }
        void STDMETHODCALLTYPE hkDrawInstancedD(ID3D11DeviceContext* ctx, UINT vtx, UINT inst, UINT startVtx, UINT startInst)
        {
            probe(ctx, vtx, [&] { oDrawInstancedD(ctx, vtx, inst, startVtx, startInst); });
        }

        void hookVtable(void** vt, DrawIndexed_t* oi, Draw_t* od, DrawIndexedInstanced_t* oii, DrawInstanced_t* odi,
                        void* hi, void* hd, void* hii, void* hdi)
        {
            MH_CreateHook(vt[12], hi, reinterpret_cast<LPVOID*>(oi));
            MH_CreateHook(vt[13], hd, reinterpret_cast<LPVOID*>(od));
            MH_CreateHook(vt[20], hii, reinterpret_cast<LPVOID*>(oii));
            MH_CreateHook(vt[21], hdi, reinterpret_cast<LPVOID*>(odi));
            MH_EnableHook(vt[12]);
            MH_EnableHook(vt[13]);
            MH_EnableHook(vt[20]);
            MH_EnableHook(vt[21]);
        }
    }

    void init(ID3D11Device* device, ID3D11DeviceContext* context)
    {
        if (!device || !context || g_device)
            return;
        g_device = device;
        g_immediate = context;
        D3D11_QUERY_DESC fd{ };
        fd.Query = D3D11_QUERY_EVENT;
        device->CreateQuery(&fd, &g_fence);

        void** vt = *reinterpret_cast<void***>(context);
        hookVtable(vt, &oDrawIndexed, &oDraw, &oDrawIndexedInstanced, &oDrawInstanced,
                   hkDrawIndexed, hkDraw, hkDrawIndexedInstanced, hkDrawInstanced);

        // Frostbite records most scene draws on deferred contexts, a different class with
        // its own vtable: one of our own gives the table every deferred context shares.
        ID3D11DeviceContext* deferred = nullptr;
        if (SUCCEEDED(device->CreateDeferredContext(0, &deferred)) && deferred)
        {
            void** dvt = *reinterpret_cast<void***>(deferred);
            if (dvt[12] != vt[12])
                hookVtable(dvt, &oDrawIndexedD, &oDrawD, &oDrawIndexedInstancedD, &oDrawInstancedD,
                           hkDrawIndexedD, hkDrawD, hkDrawIndexedInstancedD, hkDrawInstancedD);
            deferred->Release();
        }
    }

    void onPresent(int crosshairX, int crosshairY, int width, int height)
    {
        if (!g_device || !g_fence)
            return;
        static uint32_t frames = 0;
        ++frames;

        if (g_armed.load(std::memory_order_acquire))
        {
            const uint32_t slice = g_slice.load(std::memory_order_relaxed);
            snapshotConstants(slice);
            if (slice + 1 < (slices ? slices : 1))
                g_slice.store(slice + 1, std::memory_order_relaxed);
            else
            {
                g_armed.store(false, std::memory_order_release);
                g_immediate->End(g_fence);
                g_pending = true;
                g_pendingFrames = 0;
            }
            return;
        }
        if (g_pending)
        {
            ++g_pendingFrames;
            if (g_immediate->GetData(g_fence, nullptr, 0, D3D11_ASYNC_GETDATA_DONOTFLUSH) != S_OK && g_pendingFrames < 120)
                return;
            g_pending = false;
            resolve();
        }
        if (!enabled || width <= 0 || height <= 0)
            return;
        if (frames % (interval ? interval : 1) != 0)
            return;
        startCycle(crosshairX, crosshairY, width, height);
    }

    bool current(Surface& out)
    {
        std::lock_guard<std::mutex> lock(g_resultMutex);
        out = g_result;
        return out.valid;
    }

    void setShaderFilter(const void* const* shaders, size_t count)
    {
        auto f = std::make_shared<std::vector<const void*>>(shaders, shaders + count);
        std::sort(f->begin(), f->end());
        f->erase(std::unique(f->begin(), f->end()), f->end());
        g_shaderFilter.store(std::shared_ptr<const std::vector<const void*>>(std::move(f)), std::memory_order_release);
    }
}
