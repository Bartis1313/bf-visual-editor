#include "lights.h"
#include "lights_internal.h"
#include "../../utils/log.h"
#include "../render/render.h"

#include <algorithm>
#include <atomic>
#include <cstring>
#include <mutex>
#include <unordered_set>
#include <vector>

namespace editor::lights
{
    using namespace detail;

    static std::vector<FlareInstance> flareList;

#if defined(BFVE_GAME_BF4)
    void collectHeads(uintptr_t classInfo, uint16_t expectedClassId, std::vector<void*>& out)
    {
        if (!classInfo)
            return;

        const uint16_t classId = reinterpret_cast<fb::ClassInfo*>(classInfo)->m_ClassId;
        if (classId != expectedClassId)
        {
            logger::warning("[lights] ClassInfo {:#x} is class {}, expected {}", classInfo, classId, expectedClassId);
            return;
        }

        for (uint32_t base : fb::ITERABLE_BASES)
        {
            for (uint32_t realm : fb::ITERABLE_REALMS)
            {
                void** const slot = fb::iterableListHead(classInfo, realm, base);
                void* const head = slot ? *slot : nullptr;
                if (head && std::find(out.begin(), out.end(), head) == out.end())
                    out.push_back(head);
            }
        }
    }
#endif

    // BF4 iterable link +0x30, BF3 EntityWorld kind query
    template <typename Fn>
    static void forEachFlareEntity(Fn&& fn)
    {
#if defined(BFVE_GAME_BF4)
        std::vector<void*> heads;
        collectHeads(fb::LensFlareEntity::ClassInfoPtr(), 896, heads);
        if (heads.empty())
        {
            logger::warning("[lights] no LensFlareEntity list on this level");
            return;
        }

        for (void* const head : heads)
        {
            uint32_t seen = 0;
            for (void* node = head; node; )
            {
                if (++seen > 65536)
                    break;

                auto* e = reinterpret_cast<fb::LensFlareEntity*>(
                    static_cast<uint8_t*>(node) - fb::LENS_FLARE_LINK_OFFSET);
                if (!fn(e))
                    return;

                void* next = *static_cast<void**>(node);
                if (!next || next == head)
                    break;
                node = next;
            }
        }
#else
        fb::ClientGameContext* ctx = fb::ClientGameContext::GetInstance();
        if (!ctx || !ctx->m_level || !ctx->m_level->m_gameWorld)
            return;

        fb::EntityList<fb::LensFlareEntity> list{ reinterpret_cast<fb::ClassInfo*>(fb::LensFlareEntity::ClassInfoPtr()) };
        fb::LensFlareEntity* e = nullptr;
        while ((e = list.nextOfKind()) != nullptr)
            if (!fn(e))
                return;
#endif
    }

    namespace
    {
        struct SunFlare { fb::LensFlareEntity* entity; fb::Vec3 direction; bool directionEnable; };
        std::vector<SunFlare> g_sunFlares;
        bool g_sunFlaresScanned = false;
    }

    // level unload, entities die with it
    void forgetSunFlares()
    {
        g_sunFlares.clear();
        g_sunFlaresScanned = false;
    }

    size_t followSunFlares(const fb::Vec3* dir)
    {
        if (!dir)
        {
            for (const SunFlare& s : g_sunFlares)
            {
                s.entity->m_direction = s.direction;
                s.entity->m_directionEnable = s.directionEnable;
            }
            forgetSunFlares();
            return 0;
        }
        if (!g_sunFlaresScanned)
        {
            g_sunFlaresScanned = true;
            fb::Vec3 cam{};
            render::cameraPosition(cam);
            forEachFlareEntity([&](fb::LensFlareEntity* e) -> bool
            {
                // sun flares sit kilometers out
                if (fb::distanceSq(e->m_transform.m_trans, cam) > 1500.0f * 1500.0f)
                    g_sunFlares.push_back({ e, e->m_direction, e->m_directionEnable });
                return true;
            });
            logger::info("[lights] {} lens flare(s) placed as a sun follow the sun rotation", g_sunFlares.size());
        }
        for (const SunFlare& s : g_sunFlares)
        {
            s.entity->m_direction = *dir;
            s.entity->m_directionEnable = true;
        }
        return g_sunFlares.size();
    }

    static fb::LensFlareElement* flareElements(void* flareData, uint32_t& count)
    {
        count = 0;
        if (!flareData)
            return nullptr;

        auto& elements = static_cast<fb::LensFlareEntityData*>(flareData)->m_Elements;
        const uint32_t n = elements.size();
        if (!elements.m_firstElement || n > 64)
            return nullptr;

        count = n;
        return elements.m_firstElement;
    }

    uint32_t flareElementCount(void* flareData)
    {
        uint32_t count = 0;
        flareElements(flareData, count);
        return count;
    }

    fb::LensFlareElement* flareElement(void* flareData, uint32_t index)
    {
        uint32_t count = 0;
        fb::LensFlareElement* first = flareElements(flareData, count);
        return (first && index < count) ? &first[index] : nullptr;
    }

    std::string flareShaderName(void* element)
    {
        if (!element)
            return { };

        auto* shader = static_cast<fb::Asset*>(static_cast<fb::LensFlareElement*>(element)->m_Shader);
        if (!shader)
            return { };

        char text[192];
        if (!copyEngineString(shader->m_Name, text, sizeof(text)))
            return { };

        return text;
    }

    std::string flareTextureKey(const std::string& shaderName)
    {
        const size_t slash = shaderName.find_last_of("/\\");
        const std::string tail = slash == std::string::npos ? shaderName : shaderName.substr(slash + 1);

        std::vector<std::string> parts;
        size_t start = 0;
        for (;;)
        {
            const size_t at = tail.find('_', start);
            parts.push_back(tail.substr(start, at == std::string::npos ? std::string::npos : at - start));
            if (at == std::string::npos)
                break;
            start = at + 1;
        }
        if (parts.empty())
            return tail;

        const std::string& last = parts.back();
        const bool numeric = !last.empty() && last.find_first_not_of("0123456789") == std::string::npos;
        return (numeric && parts.size() >= 2) ? parts[parts.size() - 2] + "_" + last : last;
    }

    namespace { std::vector<FlareShaderChoice> g_flarePalette; }

    const std::vector<FlareShaderChoice>& flareShaderPalette() { return g_flarePalette; }

    // LensFlareEntity::m_elementShaders[i]
    void* flareShaderSlot(void* entity, uint32_t elementIndex)
    {
        auto* flare = static_cast<fb::LensFlareEntity*>(entity);
        if (!flare)
            return nullptr;

#if defined(BFVE_GAME_BF4)
        void** first = static_cast<void**>(flare->m_elementShaders.m_firstElement);
        if (!first || elementIndex >= flare->m_elementShaders.size())
            return nullptr;
#else
        void** first = flare->m_surfaceShaders.begin();
        if (!first || elementIndex >= flare->m_surfaceShaders.size())
            return nullptr;
#endif
        return first + elementIndex;
    }

    // vtable[0] addref, [1] release (sub_140CCCDA0, sub_140CB3650)
    static void shaderAddRef(void* obj)
    {
        if (obj)
            reinterpret_cast<void(__fastcall*)(void*)>((*static_cast<void***>(obj))[0])(obj);
    }

    static void shaderRelease(void* obj)
    {
        if (obj)
            reinterpret_cast<void(__fastcall*)(void*)>((*static_cast<void***>(obj))[1])(obj);
    }

    // addref new before releasing old, as the engine
    static bool installFlareShader(void* slot, void* shaderObject)
    {
        if (!slot)
            return false;

        void*& ref = *static_cast<void**>(slot);
        void* const old = ref;
        if (old == shaderObject)
            return true;

        shaderAddRef(shaderObject);
        ref = shaderObject;
        shaderRelease(old);
        return true;
    }

    namespace
    {
        struct FlareShaderClaim
        {
            void* entity;
            void* data;
            uint32_t element;
            void* shader;
        };

        std::mutex g_flareClaimMutex;
        std::vector<FlareShaderClaim> g_flareClaims;

        // read lock-free by the game-thread hook
        std::atomic<uint32_t> g_flareClaimCount{ 0 };
    }

    bool hasFlareShaderClaims()
    {
        return g_flareClaimCount.load(std::memory_order_relaxed) != 0;
    }

    // realized-from LensFlareEntityData or null
    static void* flareIdentity(void* entity)
    {
        return entity ? static_cast<fb::LensFlareEntity*>(entity)->m_data : nullptr;
    }

    void clearFlareShaderClaim(void* entity, uint32_t elementIndex)
    {
        std::lock_guard<std::mutex> lock(g_flareClaimMutex);
        for (size_t i = 0; i < g_flareClaims.size(); ++i)
            if (g_flareClaims[i].entity == entity && g_flareClaims[i].element == elementIndex)
            {
                g_flareClaims.erase(g_flareClaims.begin() + ptrdiff_t(i));
                g_flareClaimCount.store(uint32_t(g_flareClaims.size()), std::memory_order_relaxed);
                return;
            }
    }

    // sub_140CCCDA0 hook, after the array refill
    void applyFlareShaderClaims(void* entity)
    {
        if (!g_flareClaimCount.load(std::memory_order_relaxed))
            return;

        std::vector<FlareShaderClaim> mine;
        {
            std::lock_guard<std::mutex> lock(g_flareClaimMutex);
            for (const FlareShaderClaim& c : g_flareClaims)
                if (c.entity == entity)
                    mine.push_back(c);
        }

        void* const identity = flareIdentity(entity);

        for (const FlareShaderClaim& c : mine)
        {
            if (!identity || identity != c.data || !c.shader)
                continue;

            if (void* slot = flareShaderSlot(entity, c.element))
                installFlareShader(slot, c.shader);
        }
    }

    bool setFlareShader(void* entity, uint32_t elementIndex, void* shaderObject)
    {
        void* const identity = flareIdentity(entity);
        void* slot = flareShaderSlot(entity, elementIndex);
        if (!identity || !slot || !shaderObject)
            return false;

        {
            std::lock_guard<std::mutex> lock(g_flareClaimMutex);

            bool replaced = false;
            for (FlareShaderClaim& c : g_flareClaims)
                if (c.entity == entity && c.element == elementIndex)
                {
                    c.data = identity;
                    c.shader = shaderObject;
                    replaced = true;
                    break;
                }

            if (!replaced)
                g_flareClaims.push_back({ entity, identity, elementIndex, shaderObject });

            g_flareClaimCount.store(uint32_t(g_flareClaims.size()),
                                    std::memory_order_relaxed);
        }

        return installFlareShader(slot, shaderObject);
    }

    void* fetchFlareShader(const char* name)
    {
        if (!name || !*name)
            return nullptr;

        fb::ShaderDatabase* mgr = fb::ShaderDatabase::Singleton();
        if (!mgr)
        {
            logger::warning("[lights] shader database not available");
            return nullptr;
        }

        void* fn = vfunc::getVFunc(mgr, fb::ShaderDatabase::FIND_SLOT);
        if (!fn)
            return nullptr;

#if defined(BFVE_GAME_BF4)
        using fetch_t = void* (__fastcall*)(void*, void*, const char*);
        void* const arena = fb::ShaderDatabase::Allocator();
#else
        // thiscall (db, arena, name), arena from a realized flare's bus
        // *(*(m_entityBus + 4) + 0x34) - sub_545DC0
        using fetch_t = void* (__thiscall*)(void*, void*, const char*);
        void* arena = nullptr;
        for (const FlareInstance& fi : flareList)
        {
            auto* flare = static_cast<fb::LensFlareEntity*>(fi.entity);
            if (!flare || !flare->m_entityBus)
                continue;
            uint8_t* inner = *reinterpret_cast<uint8_t**>(reinterpret_cast<uint8_t*>(flare->m_entityBus) + 4);
            arena = inner ? *reinterpret_cast<void**>(inner + 0x34) : nullptr;
            if (arena)
                break;
        }
        if (!arena)
        {
            logger::warning("[lights] no realized flare to borrow an arena from - scan first");
            return nullptr;
        }
#endif
        void* shader = reinterpret_cast<fetch_t>(fn)(mgr, arena, name);

        if (!shader)
        {
            logger::warning("[lights] no shader called \"{}\"", name);
            return nullptr;
        }

        for (const FlareShaderChoice& c : g_flarePalette)
            if (c.object == shader)
                return shader;

        shaderAddRef(shader);
        g_flarePalette.push_back({ name, shader });
        logger::info("[lights] loaded flare shader {}", name);
        return shader;
    }

    static const char* const kFlareShaders[] = {
        "FX/Lensflare/Shaders/Glow_Circle_Blue_LensDirt",
        "FX/Lensflare/Shaders/Glow_Circle_Orange_LensDirt",
        "FX/Lensflare/Shaders/Glow_Circle_Orange",
        "FX/Lensflare/Shaders/Glow_Circle_Red_LensDirt",
        "FX/Lensflare/Shaders/Glow_Circle_Red",
        "FX/Lensflare/Shaders/LF_Blue_CoreGlow",
        "FX/Lensflare/Shaders/LF_Blue_Flare_Box",
        "FX/Lensflare/Shaders/LF_Blue_Flare_Circle",
        "FX/Lensflare/Shaders/LF_Blue_HaloGlow",
        "FX/Lensflare/Shaders/LF_ComputerScreen_Cyan_HaloGlow",
        "FX/Lensflare/Shaders/LF_ComputerScreen_Flare_Circle",
        "FX/Lensflare/Shaders/LF_ComputerScreen_HaloGlow",
        "FX/Lensflare/Shaders/LF_CoolWhite_Flare_Box",
        "FX/Lensflare/Shaders/LF_CoolWhite_Flare_Circle",
        "FX/Lensflare/Shaders/LF_CoolWhite_HaloGlow",
        "FX/Lensflare/Shaders/LF_Orange_CoreGlow_SP_Shanghai",
        "FX/Lensflare/Shaders/LF_Orange_CoreGlow",
        "FX/Lensflare/Shaders/LF_Orange_Flare_Box",
        "FX/Lensflare/Shaders/LF_Orange_Flare_Circle",
        "FX/Lensflare/Shaders/LF_Orange_HaloGlow",
        "FX/Lensflare/Shaders/LF_Red_CoreGlow",
        "FX/Lensflare/Shaders/LF_Red_Flare_Box",
        "FX/Lensflare/Shaders/LF_Red_Flare_Circle",
        "FX/Lensflare/Shaders/LF_Red_HaloGlow",
        "FX/Lensflare/Shaders/LF_White_CoreGlow",
        "FX/Lensflare/Shaders/LF_White_Flare_Box",
        "FX/Lensflare/Shaders/LF_White_Flare_Circle",
        "FX/Lensflare/Shaders/LF_White_HaloGlow",
        "FX/Lensflare/Shaders/LF_Yellow_CoreGlow_SP_Suez",
        "FX/Lensflare/Shaders/LF_Yellow_CoreGlow",
        "FX/Lensflare/Shaders/LF_Yellow_Flare_Box_Shanghai",
        "FX/Lensflare/Shaders/LF_Yellow_Flare_Box",
        "FX/Lensflare/Shaders/LF_Yellow_Flare_Circle",
        "FX/Lensflare/Shaders/LF_Yellow_HaloGlow_SP_Suez",
        "FX/Lensflare/Shaders/LF_Yellow_HaloGlow",
        "FX/Lensflare/Shaders/LensFlare_CarAlarm_Orange_FlareRoundBlur",
        "FX/Lensflare/Shaders/LensFlare_CarAlarm_Orange_FlareStreaked",
        "FX/Lensflare/Shaders/LensFlare_FlareRoundBlur",
        "FX/Lensflare/Shaders/LensFlare_FlareStreaked_Red",
        "FX/Lensflare/Shaders/LensFlare_FlareStreaked",
        "FX/Lensflare/Shaders/LensFlare_Lamp_01",
        "FX/Lensflare/Shaders/LensFlare_Lamp_03_Blue_BokehDirt_EQ1",
        "FX/Lensflare/Shaders/Reflection_02_Orange_Circle",
    };

    static void loadFlareShaderPalette()
    {
        for (const char* name : kFlareShaders)
            fetchFlareShader(name);

        logger::info("[lights] flare palette: {} shader(s) loaded", g_flarePalette.size());
    }

    // database fetch when not realized
    static void* flareShaderByName(const std::string& name)
    {
        for (const FlareShaderChoice& c : g_flarePalette)
            if (c.name == name)
                return c.object;

        return fetchFlareShader(name.c_str());
    }

    uint32_t applyFlareShaders(LightDataEntry& entry)
    {
        if (entry.flareShaders.empty() || entry.lampFlares.empty())
            return 0;

        uint32_t written = 0;

        for (const LightDataEntry::FlareShaderEdit& edit : entry.flareShaders)
        {
            void* const object = flareShaderByName(edit.shader);
            if (!object)
                continue;

            for (const FlareInstance& fi : flareList)
            {
                if (!fi.data)
                    continue;

                if (std::find(entry.lampFlares.begin(), entry.lampFlares.end(), fi.data) ==
                    entry.lampFlares.end())
                    continue;

                if (edit.element == ALL_FLARE_ELEMENTS)
                {
                    const uint32_t count = flareElementCount(fi.data);
                    for (uint32_t e = 0; e < count; ++e)
                        written += setFlareShader(fi.entity, e, object) ? 1 : 0;
                }
                else
                {
                    written += setFlareShader(fi.entity, edit.element, object) ? 1 : 0;
                }
            }
        }

        return written;
    }

    // sub_140CB3650 re-reads element data every frame
    uint32_t applyFlareFields(LightDataEntry& entry)
    {
        if (entry.flareFields.empty() || entry.lampFlares.empty())
            return 0;

        uint32_t written = 0;

        for (const LightDataEntry::FlareFieldEdit& f : entry.flareFields)
        {
            for (void* data : entry.lampFlares)
            {
                void* const el = flareElement(data, f.element);
                if (!el)
                    continue;

                float* dst = reinterpret_cast<float*>(static_cast<uint8_t*>(el) + f.offset);
                for (uint8_t i = 0; i < f.count; ++i)
                    dst[i] = f.value[i];
                ++written;
            }
        }

        return written;
    }

    void setFlareFieldFor(LightDataEntry& entry, uint32_t element, uint32_t offset,
                          const float* value, uint8_t count)
    {
        if (!value || (count != 1 && count != 4))
            return;

        LightDataEntry::FlareFieldEdit edit;
        edit.element = element;
        edit.offset = offset;
        edit.count = count;
        for (uint8_t i = 0; i < count; ++i)
            edit.value[i] = value[i];

        auto it = std::find_if(entry.flareFields.begin(), entry.flareFields.end(),
            [element, offset](const LightDataEntry::FlareFieldEdit& e)
            { return e.element == element && e.offset == offset; });

        if (it != entry.flareFields.end())
            *it = edit;
        else
            entry.flareFields.push_back(edit);

        entry.flareFieldsApplied = false;
        applyFlareFields(entry);
        entry.flareFieldsApplied = true;
    }

    void setFlareShaderFor(LightDataEntry& entry, uint32_t element, const std::string& shaderName)
    {
        if (shaderName.empty())
            return;

        if (element == ALL_FLARE_ELEMENTS)
            entry.flareShaders.clear();
        else
            std::erase_if(entry.flareShaders,
                [](const LightDataEntry::FlareShaderEdit& e)
                { return e.element == ALL_FLARE_ELEMENTS; });

        auto it = std::find_if(entry.flareShaders.begin(), entry.flareShaders.end(),
            [element](const LightDataEntry::FlareShaderEdit& e) { return e.element == element; });

        if (it != entry.flareShaders.end())
            it->shader = shaderName;
        else
            entry.flareShaders.push_back({ element, shaderName });

        applyFlareShaders(entry);
    }

    void clearFlareShadersFor(LightDataEntry& entry)
    {
        // sub_140CCCDA0 restores the data's shader on rebuild
        for (const FlareInstance& fi : flareList)
        {
            if (!fi.data ||
                std::find(entry.lampFlares.begin(), entry.lampFlares.end(), fi.data) ==
                    entry.lampFlares.end())
                continue;

            const uint32_t count = flareElementCount(fi.data);
            for (uint32_t e = 0; e < count; ++e)
                clearFlareShaderClaim(fi.entity, e);
        }

        entry.flareShaders.clear();
    }

    static void releaseFlarePalette()
    {
        for (const FlareShaderChoice& c : g_flarePalette)
            shaderRelease(c.object);

        g_flarePalette.clear();
    }

    void harvestFlareShaders()
    {
        releaseFlarePalette();

        forEachFlareEntity([&](fb::LensFlareEntity* e) -> bool
        {
            if (g_flarePalette.size() >= 128)
                return false;

            void* const data = e->m_data;
            if (data)
            {
                const uint32_t count = flareElementCount(data);
                for (uint32_t i = 0; i < count && i < 32; ++i)
                {
                    void* el = flareElement(data, i);
                    const std::string name = el ? flareShaderName(el) : std::string();
                    if (name.empty())
                        continue;

                    void* slot = flareShaderSlot(e, i);
                    void* object = slot ? *static_cast<void**>(slot) : nullptr;
                    if (!object)
                        continue;

                    bool have = false;
                    for (const FlareShaderChoice& c : g_flarePalette)
                        if (c.name == name) { have = true; break; }

                    if (!have)
                    {
                        shaderAddRef(object);
                        g_flarePalette.push_back({ name, object });
                    }
                }
            }
            return true;
        });

        logger::info("[lights] {} distinct flare shader(s) realized on this level",
            g_flarePalette.size());

        loadFlareShaderPalette();

        std::sort(g_flarePalette.begin(), g_flarePalette.end(),
            [](const FlareShaderChoice& a, const FlareShaderChoice& b)
            {
                const size_t ca = a.name.find_last_of('/');
                const size_t cb = b.name.find_last_of('/');
                return a.name.substr(ca == std::string::npos ? 0 : ca + 1) <
                       b.name.substr(cb == std::string::npos ? 0 : cb + 1);
            });
    }

    void scanLensFlares() { scanLensFlaresFor({}); }

    void scanLensFlaresFor(const std::vector<void*>& wantedData)
    {
        flareList.clear();

        std::unordered_set<void*> wanted(wantedData.begin(), wantedData.end());
        wanted.erase(nullptr);

        forEachFlareEntity([&](fb::LensFlareEntity* e) -> bool
        {
            if (flareList.size() >= 1024)
                return false;

            void* const data = e->m_data;
            if (wanted.empty() || (data && wanted.count(data)))
                flareList.push_back({ e, data });
            return true;
        });

        ++flareListGeneration;
        logger::info("[lights] lens flares: {}{}", flareList.size(), wanted.empty() ? "" : " for this light");
    }

    const std::vector<FlareInstance>& lensFlares() { return flareList; }

    static void applyPendingFlareShaders()
    {
        static uint32_t frame = 0;
        static uint32_t tries = 0;

        std::vector<LightDataEntry*> todo;
        std::vector<void*> wanted;

        for (auto& [dataPtr, entry] : getEntries())
        {
            if (!entry.flareFields.empty() && !entry.flareFieldsApplied &&
                applyFlareFields(entry) > 0)
            {
                entry.flareFieldsApplied = true;
                logger::info("[lights] restored {} halo element value(s) for {}",
                    entry.flareFields.size(), entry.assetName);
            }

            if (!entry.shaderDrivers.empty() && !entry.shaderDriversApplied &&
                applyShaderDrivers(entry) > 0)
            {
                entry.shaderDriversApplied = true;
                logger::info("[lights] restored {} driven shader parameter(s) for {}",
                    entry.shaderDrivers.size(), entry.assetName);
            }
        }

        for (auto& [dataPtr, entry] : getEntries())
        {
            if (entry.flareShaders.empty() || entry.flareShadersApplied)
                continue;

            todo.push_back(&entry);
            wanted.insert(wanted.end(), entry.lampFlares.begin(), entry.lampFlares.end());
        }

        if (todo.empty())
        {
            tries = 0;
            return;
        }

        if (++frame < 30)
            return;
        frame = 0;

        if (++tries > 120) // ~60 s of level time
        {
            logger::warning("[lights] gave up re-applying the saved halo shader for {} "
                            "light(s) - their flares never turned up", todo.size());
            for (LightDataEntry* e : todo)
                e->flareShadersApplied = true;
            tries = 0;
            return;
        }

        if (wanted.empty())
            return; // flares not linked yet

        scanLensFlaresFor(wanted);

        uint32_t applied = 0;
        for (LightDataEntry* e : todo)
        {
            if (applyFlareShaders(*e) > 0)
            {
                e->flareShadersApplied = true;
                ++applied;
            }
        }

        if (applied)
        {
            logger::info("[lights] re-applied the saved halo shader to {} light(s)", applied);
            tries = 0;
        }
    }

    void linkFlaresByProximity()
    {
        scanLensFlaresFor({});
        if (flareList.empty())
            return;

        uint32_t linked = 0;
        for (auto& [dataPtr, entry] : getEntries())
        {
            for (fb::LocalLightEntity* light : entry.activeEntities)
            {
                fb::Vec3 lp{ };
                if (!entityPosOf(light, lp))
                    continue;

                for (const FlareInstance& fi : flareList)
                {
                    if (!fi.data)
                        continue;

                    const fb::Vec3& fp = static_cast<fb::LensFlareEntity*>(fi.entity)->m_transform.m_trans;
                    if (distanceSqr(lp, fp) > 2.25f)
                        continue;

                    if (std::find(entry.lampFlares.begin(), entry.lampFlares.end(), fi.data) ==
                        entry.lampFlares.end())
                    {
                        entry.lampFlares.push_back(fi.data);
                        ++linked;
                    }
                }
            }
        }

        if (linked)
            logger::info("[lights] linked {} flare(s) to lights by distance", linked);
    }

    void clearFlarePointers()
    {
        {
            std::lock_guard<std::mutex> lock(g_flareClaimMutex);
            g_flareClaims.clear();
            g_flareClaimCount.store(0, std::memory_order_relaxed);
        }

        releaseFlarePalette();
        flareList.clear();
    }

    void tick()
    {
        applyPendingFlareShaders();
        static uint32_t frame = 0;
        if ((++frame % 120) == 0)
        {
#if defined(BFVE_GAME_BF3)
            linkFlaresByProximity();
#endif
            nameDynamicLightsByEmitters();
        }
    }
}
