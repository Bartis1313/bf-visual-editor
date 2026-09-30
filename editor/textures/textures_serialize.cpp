#include <format>
#include "textures.h"
#include "texgen.h"
#include "../../utils/log.h"

#include <algorithm>

namespace editor::textures
{
    namespace
    {
        constexpr uint32_t RETRY_EVERY_FRAMES = 30;
        constexpr uint32_t MAX_TRIES = 120; // ~60 s of level time
        constexpr int RESOLVES_PER_RUN = 6;

        struct TexRef
        {
            std::string path;
            int clone = -1;

            bool empty() const { return path.empty() && clone < 0; }
        };

        struct CloneSpec
        {
            std::string of;
            void* live = nullptr;
        };

        struct PendingEdit
        {
            TexRef target;
            gen::EditInfo edit;
        };

        struct PendingSky
        {
            int slot = -1;
            TexRef target;
            bool toNull = false;
        };

        struct PendingParamTexture
        {
            uint64_t setKey = 0;
            uint32_t material = 0;
            uint32_t handle = 0;
            std::string path;
        };

        std::vector<CloneSpec> g_clones;
        std::vector<PendingEdit> g_pendingEdits;
        std::vector<PendingSky> g_pendingSky;
        std::vector<PendingParamTexture> g_pendingParams;
        uint32_t g_tries = 0;
        uint32_t g_frame = 0;

        json vec4ToJson(const float (&v)[4])
        {
            return json::array({ v[0], v[1], v[2], v[3] });
        }

        bool jsonToVec4(const json& j, float (&v)[4])
        {
            if (!j.is_array() || j.size() != 4)
                return false;

            for (size_t i = 0; i < 4; ++i)
            {
                if (!j[i].is_number())
                    return false;
                v[i] = j[i].get<float>();
            }
            return true;
        }

        json refToJson(void* texture, std::vector<std::pair<void*, std::string>>& clones)
        {
            json j;

            if (!texture)
                return j;

            if (gen::isClonedTexture(texture))
            {
                void* const src = gen::cloneSource(texture);
                const std::string of = texturePath(src);
                if (of.empty())
                    return j;

                for (size_t i = 0; i < clones.size(); ++i)
                    if (clones[i].first == texture)
                    {
                        j["clone"] = int(i);
                        return j;
                    }

                clones.push_back({ texture, of });
                j["clone"] = int(clones.size() - 1);
                return j;
            }

            const std::string path = texturePath(texture);
            if (!path.empty())
                j["path"] = path;

            return j;
        }

        TexRef refFromJson(const json& j)
        {
            TexRef r;
            // old configs: bare path string
            if (j.is_string())
            {
                r.path = j.get<std::string>();
                return r;
            }

            if (!j.is_object())
                return r;

            if (j.contains("path") && j["path"].is_string())
                r.path = j["path"].get<std::string>();
            else if (j.contains("clone") && j["clone"].is_number_integer())
                r.clone = j["clone"].get<int>();

            return r;
        }

        // null = not yet
        void* resolve(const TexRef& r)
        {
            if (r.clone >= 0)
            {
                if (size_t(r.clone) >= g_clones.size())
                    return nullptr;

                CloneSpec& spec = g_clones[size_t(r.clone)];
                if (spec.live)
                    return spec.live;

                void* const src = textureByPath(spec.of);
                if (!src)
                    return nullptr;

                spec.live = gen::cloneTexture(src);
                if (spec.live)
                {
                    catalogueCloneOf(spec.live, src);
                    logger::info("[textures] config: restored a private copy of {}", spec.of);
                }
                return spec.live;
            }

            return r.path.empty() ? nullptr : textureByPath(r.path);
        }

        void clearPending()
        {
            g_clones.clear();
            g_pendingEdits.clear();
            g_pendingSky.clear();
            g_pendingParams.clear();
            g_tries = 0;
        }
    }

    void clearPendingConfig()
    {
        clearPending();
    }

    json serialize()
    {
        json root;
        std::vector<std::pair<void*, std::string>> clones;

        json params = json::array();
        uint32_t skippedBlock = 0, skippedTexture = 0;

        for (const ParamOverride& o : paramOverrides)
        {
            if (o.setKey == 0)
            {
                ++skippedBlock;
                continue;
            }

            json e;
            e["setKey"] = o.setKey;
            e["material"] = o.material;
            e["handle"] = o.handle;

            if (const char* n = paramName(o.handle))
                e["name"] = n;
            if (o.added)
                e["added"] = true;

            if (o.isTexture)
            {
                const std::string path = texturePath(o.texture);
                if (path.empty())
                {
                    ++skippedTexture;
                    continue;
                }
                e["texture"] = path;
            }
            else
            {
                e["value"] = vec4ToJson(o.value);
            }

            params.push_back(std::move(e));
        }
        root["params"] = std::move(params);

        json edits = json::array();
        uint32_t skippedEdit = 0;

        for (const gen::EditInfo& info : gen::listEdits())
        {
            json target = refToJson(info.dxTexture, clones);
            if (target.empty())
            {
                ++skippedEdit;
                continue;
            }

            json e;
            e["target"] = std::move(target);

            if (!info.sourceFile.empty())
                e["source"] = info.sourceFile;

            if (info.tinted)
            {
                json t;
                t["color"] = vec4ToJson(info.params.colorA);
                t["brightness"] = info.params.brightness;
                t["upscale"] = info.params.upscale;
                t["sharpness"] = info.params.sharpness;
                t["detail"] = info.params.detail;
                e["tint"] = std::move(t);
            }

            edits.push_back(std::move(e));
        }
        root["edits"] = std::move(edits);

        json sky = json::array();
        for (int i = 0; i < skySlotCount(); ++i)
        {
            if (!skyOverride[i].enabled)
                continue;

            json e;
            e["slot"] = skySlotLabel(i);

            // null = cloud layer off
            if (!skyOverride[i].asset)
            {
                e["texture"] = nullptr;
            }
            else
            {
                json target = refToJson(skyOverride[i].asset, clones);
                if (target.empty())
                    continue;
                e["texture"] = std::move(target);
            }

            sky.push_back(std::move(e));
        }
        root["sky"] = std::move(sky);

        json shaderEdits = json::array();
        for (const ShaderEdit& e : listShaderEdits())
        {
            json j;
            j["graph"] = e.graph;
            j["original"] = vec4ToJson(e.original);
            j["value"] = vec4ToJson(e.value);
            shaderEdits.push_back(std::move(j));
        }
        root["shaderEdits"] = std::move(shaderEdits);

        json cloneList = json::array();
        for (const auto& [ptr, of] : clones)
        {
            json c;
            c["of"] = of;
            cloneList.push_back(std::move(c));
        }
        root["clones"] = std::move(cloneList);

        if (skippedBlock || skippedTexture || skippedEdit)
            logger::info("[textures] config: {} block-keyed override(s), {} unnameable "
                         "texture(s) and {} unnameable edit(s) not saved - they have no "
                         "identity outside this session",
                skippedBlock, skippedTexture, skippedEdit);

        return root;
    }

    void deserialize(const json& root)
    {
        if (!root.is_object())
            return;

        clearPending();

        if (root.contains("clones") && root["clones"].is_array())
        {
            for (const json& c : root["clones"])
            {
                CloneSpec spec;
                if (c.is_object() && c.contains("of") && c["of"].is_string())
                    spec.of = c["of"].get<std::string>();
                g_clones.push_back(std::move(spec));
            }
        }

        uint32_t loaded = 0;

        if (root.contains("params") && root["params"].is_array())
        {
            for (const json& e : root["params"])
            {
                if (!e.is_object() || !e.contains("setKey") || !e.contains("handle"))
                    continue;

                ParamOverride o;
                o.setKey = e["setKey"].get<uint64_t>();
                o.material = e.value("material", 0u);
                o.handle = e["handle"].get<uint32_t>();
                o.added = e.value("added", false);

                if (e.contains("texture") && e["texture"].is_string())
                {
                    const std::string path = e["texture"].get<std::string>();
                    o.isTexture = true;
                    o.texture = textureByPath(path);

                    if (!o.texture)
                        g_pendingParams.push_back({ o.setKey, o.material, o.handle, path });
                }
                else if (!e.contains("value") || !jsonToVec4(e["value"], o.value))
                {
                    continue;
                }

                auto it = std::find_if(paramOverrides.begin(), paramOverrides.end(),
                    [&o](const ParamOverride& x)
                    {
                        return x.setKey == o.setKey && x.material == o.material &&
                               x.handle == o.handle && x.isTexture == o.isTexture;
                    });

                if (it != paramOverrides.end())
                    *it = o;
                else
                    paramOverrides.push_back(o);

                ++loaded;
            }
        }

        if (root.contains("edits") && root["edits"].is_array())
        {
            for (const json& e : root["edits"])
            {
                if (!e.is_object() || !e.contains("target"))
                    continue;

                PendingEdit pe;
                pe.target = refFromJson(e["target"]);
                if (pe.target.empty())
                    continue;

                if (e.contains("source") && e["source"].is_string())
                    pe.edit.sourceFile = e["source"].get<std::string>();

                if (e.contains("tint") && e["tint"].is_object())
                {
                    pe.edit.tinted = true;
                    if (e["tint"].contains("color"))
                        jsonToVec4(e["tint"]["color"], pe.edit.params.colorA);
                    pe.edit.params.brightness = e["tint"].value("brightness", 1.0f);
                    pe.edit.params.upscale = e["tint"].value("upscale", 1);
                    pe.edit.params.sharpness = e["tint"].value("sharpness", 0.0f);
                    pe.edit.params.detail = e["tint"].value("detail", 0.0f);
                }

                if (!pe.edit.sourceFile.empty() || pe.edit.tinted)
                    g_pendingEdits.push_back(std::move(pe));
            }
        }

        if (root.contains("sky") && root["sky"].is_array())
        {
            for (const json& e : root["sky"])
            {
                if (!e.is_object() || !e.contains("slot") || !e["slot"].is_string())
                    continue;

                PendingSky ps;
                ps.slot = skySlotByLabel(e["slot"].get<std::string>().c_str());
                if (ps.slot < 0)
                    continue;

                if (e.contains("texture") && e["texture"].is_null())
                    ps.toNull = true;
                else if (e.contains("texture"))
                    ps.target = refFromJson(e["texture"]);
                else
                    continue;

                g_pendingSky.push_back(std::move(ps));
            }
        }

        if (root.contains("shaderEdits") && root["shaderEdits"].is_array())
            for (const json& j : root["shaderEdits"])
            {
                if (!j.is_object() || !j.contains("graph") || !j["graph"].is_string())
                    continue;
                ShaderEdit e;
                e.graph = j["graph"].get<std::string>();
                if (j.contains("original")) jsonToVec4(j["original"], e.original);
                if (j.contains("value")) jsonToVec4(j["value"], e.value);
                queueShaderEdit(e);
            }

        logger::info("[textures] config: {} material override(s); {} edit(s), {} sky slot(s), "
                     "{} texture reference(s) and {} shader edit(s) waiting",
            loaded, g_pendingEdits.size(), g_pendingSky.size(), g_pendingParams.size(), pendingShaderWork());

        g_frame = RETRY_EVERY_FRAMES;
        applyPendingConfig();
    }

    void applyPendingConfig()
    {
        if (g_pendingEdits.empty() && g_pendingSky.empty() && g_pendingParams.empty())
            return;

        if (++g_frame < RETRY_EVERY_FRAMES)
            return;
        g_frame = 0;

        if (++g_tries > MAX_TRIES)
        {
            logger::warning("[textures] config: giving up on {} edit(s), {} sky slot(s) and "
                            "{} texture reference(s) - their textures never turned up",
                g_pendingEdits.size(), g_pendingSky.size(), g_pendingParams.size());
            clearPending();
            return;
        }

        int budget = RESOLVES_PER_RUN;

        for (size_t i = 0; i < g_pendingSky.size() && budget > 0; )
        {
            PendingSky& ps = g_pendingSky[i];

            if (ps.toNull)
            {
                setSkySlot(ps.slot, nullptr);
                g_pendingSky.erase(g_pendingSky.begin() + ptrdiff_t(i));
                continue;
            }

            void* const tex = resolve(ps.target);
            if (!tex)
            {
                ++i;
                continue;
            }

            --budget;
            setSkySlot(ps.slot, tex);
            logger::info("[textures] config: sky slot {} restored", skySlotLabel(ps.slot));
            g_pendingSky.erase(g_pendingSky.begin() + ptrdiff_t(i));
        }

        for (size_t i = 0; i < g_pendingEdits.size() && budget > 0; )
        {
            PendingEdit& pe = g_pendingEdits[i];

            void* const tex = resolve(pe.target);
            if (!tex)
            {
                ++i;
                continue;
            }

            --budget;
            pe.edit.dxTexture = tex;

            std::string err;
            if (gen::replayEdit(pe.edit, err))
            {
                logger::info("[textures] config: re-applied an edit to {}",
                    pe.target.path.empty() ? "a private copy" : pe.target.path.c_str());
            }
            else
            {
                logger::warning("[textures] config: could not re-apply an edit to {}: {}",
                    pe.target.path.empty() ? "a private copy" : pe.target.path.c_str(), err);
            }

            g_pendingEdits.erase(g_pendingEdits.begin() + ptrdiff_t(i));
        }

        for (size_t i = 0; i < g_pendingParams.size() && budget > 0; )
        {
            PendingParamTexture& pp = g_pendingParams[i];

            void* const tex = textureByPath(pp.path);
            if (!tex)
            {
                ++i;
                continue;
            }

            --budget;
            for (ParamOverride& o : paramOverrides)
                if (o.isTexture && o.setKey == pp.setKey && o.material == pp.material &&
                    o.handle == pp.handle)
                {
                    o.texture = tex;
                    break;
                }

            g_pendingParams.erase(g_pendingParams.begin() + ptrdiff_t(i));
        }
    }

}
