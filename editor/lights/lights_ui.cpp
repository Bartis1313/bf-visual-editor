#include "lights.h"
#include "../render/render.h"
#include "../textures/textures.h"
#include "../textures/surfacepick.h"
#include "../textures/texgen.h"
#include "../emitters/emitters.h"
#include "../editor_context.h"
#include "../ui/ui_helpers.h"
#include "../enlighten/enlighten_spawn.h"
#include "../../utils/log.h"

#include <imgui.h>
#include <algorithm>
#include <array>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <unordered_map>
#include <unordered_set>

namespace editor::lights
{
    static char filterBuffer[256] = { };
    static int typeFilter = 0;

    static const char* leaf(const std::string& path)
    {
        const size_t slash = path.find_last_of("/\\");
        return slash == std::string::npos ? path.c_str() : path.c_str() + slash + 1;
    }

    // ShaderParameterEntityData values into the lamp material
    static void renderShaderDrivers(LightDataEntry& entry)
    {
        if (entry.lampShaderParams.empty())
            return;

        if (!ImGui::TreeNodeEx("Glow color##Light", ImGuiTreeNodeFlags_DefaultOpen))
            return;

        static std::unordered_map<void*, std::array<float, 4>> originals;

        for (void* data : entry.lampShaderParams)
        {
            uint32_t handle = 0;
            float value[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
            if (!readShaderDriver(data, handle, value))
                continue;

            ImGui::PushID(data);

            if (originals.find(data) == originals.end())
                originals[data] = { value[0], value[1], value[2], value[3] };

            const char* name = textures::nameForParamHandle(handle);
            char label[128];
            std::snprintf(label, sizeof(label), "%s##drv", name ? name : "color");

            float shown[4] = { value[0], value[1], value[2], value[3] };
            shaderParamOverride(handle, shown);

            ImGui::SetNextItemWidth(-90.0f);
            if (ImGui::ColorEdit4(label, shown, ImGuiColorEditFlags_Float
                    | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_AlphaBar))
                setShaderDriverFor(entry, handle, shown);

            ImGui::SameLine();
            if (ImGui::SmallButton("reset"))
            {
                const std::array<float, 4>& o = originals[data];
                const float back[4] = { o[0], o[1], o[2], o[3] };
                writeShaderDriver(data, back);
                clearShaderParamOverride(handle);
                std::erase_if(entry.shaderDrivers,
                    [handle](const LightDataEntry::ShaderDriverEdit& e) { return e.handle == handle; });
            }

            ImGui::PopID();
        }

        ImGui::TreePop();
    }

    // lamp mesh materials, cached per selected light
    static const void* g_assetFor = nullptr;
    static std::vector<MeshMaterialInfo> g_assetMaterials;

    static void ensureAssetMaterials(const LightDataEntry& entry)
    {
        if (g_assetFor == entry.dataPtr)
            return;

        g_assetFor = entry.dataPtr;
        g_assetMaterials.clear();

        for (const MeshVariationRef& ref : entry.lampMeshes)
        {
            if (!ref.asset)
                continue;

            for (MeshMaterialInfo& mi : meshAssetMaterials(ref.asset))
            {
                for (const std::string& p : mi.parameters)
                    textures::registerParamName(p.c_str());
                for (const MeshMaterialInfo::TextureBinding& tb : mi.textures)
                    if (!tb.parameter.empty())
                        textures::registerParamName(tb.parameter.c_str());

                g_assetMaterials.push_back(std::move(mi));
            }
        }
    }

    static void renderAssetMaterials()
    {
        for (const MeshMaterialInfo& mi : g_assetMaterials)
        {
            ImGui::BulletText("%s", mi.shader.empty() ? "(unnamed shader)" : leaf(mi.shader));

            for (const MeshMaterialInfo::TextureBinding& tb : mi.textures)
            {
                ImGui::Indent();
                if (!textures::renderTextureRowByPath(tb.texture.c_str(), tb.parameter.c_str()))
                    ImGui::TextDisabled("%s = %s",
                        tb.parameter.empty() ? "(slot)" : tb.parameter.c_str(),
                        tb.texture.empty() ? "(none)" : leaf(tb.texture));
                ImGui::Unindent();
            }
        }
    }

    struct HaloTint
    {
        float rgb[3] = { 1.0f, 1.0f, 1.0f };
        float brightness = 1.0f;
        bool applied = false;
    };
    static std::unordered_map<const void*, HaloTint> g_haloTints;

    static std::vector<std::string> haloTextureKeys(const LightDataEntry& entry)
    {
        std::vector<std::string> keys;
        for (void* data : entry.lampFlares)
        {
            const uint32_t count = flareElementCount(data);
            for (uint32_t i = 0; i < count; ++i)
            {
                fb::LensFlareElement* el = flareElement(data, i);
                const std::string shader = el ? flareShaderName(el) : std::string();
                if (shader.empty())
                    continue;

                const std::string key = textures::lowerCopy(flareTextureKey(shader));
                if (!key.empty() && std::find(keys.begin(), keys.end(), key) == keys.end())
                    keys.push_back(key);
            }
        }
        return keys;
    }

    static bool isFlareTexture(textures::TextureEntry& te, const std::vector<std::string>& keys)
    {
        textures::cacheName(te); // paths fill lazily
        if (te.lowerPath.find("lensflare") == std::string::npos)
            return false;
        for (const std::string& k : keys)
            if (te.lowerPath.find(k) != std::string::npos)
                return true;
        return false;
    }

    static int applyHaloTint(const LightDataEntry& entry, const HaloTint& tint)
    {
        textures::gen::Params p;
        p.colorA[0] = tint.rgb[0];
        p.colorA[1] = tint.rgb[1];
        p.colorA[2] = tint.rgb[2];
        p.colorA[3] = 1.0f;
        p.brightness = tint.brightness;

        const std::vector<std::string> keys = haloTextureKeys(entry);
        int done = 0;
        for (textures::TextureEntry& te : textures::entries)
        {
            if (!te.texture || !isFlareTexture(te, keys))
                continue;

            std::string err;
            if (textures::gen::tintExisting(te.texture, p, err))
                ++done;
            else
                logger::warning("[lights] halo tint {}: {}", te.lowerPath, err);
        }
        return done;
    }

    static void revertHaloTint(const LightDataEntry& entry)
    {
        const std::vector<std::string> keys = haloTextureKeys(entry);
        for (textures::TextureEntry& te : textures::entries)
            if (te.texture && isFlareTexture(te, keys))
                textures::gen::revert(te.texture);
    }

    static int countHaloTextures(const LightDataEntry& entry, bool residentOnly)
    {
        const std::vector<std::string> keys = haloTextureKeys(entry);
        int n = 0;
        for (textures::TextureEntry& te : textures::entries)
            if (isFlareTexture(te, keys) && (!residentOnly || te.texture))
                ++n;
        return n;
    }

    static void renderShaderCombo(const char* label, LightDataEntry& entry, uint32_t element)
    {
        const char* current = "choose...";
        for (const LightDataEntry::FlareShaderEdit& e : entry.flareShaders)
            if (e.element == element)
                current = leaf(e.shader);

        ImGui::SetNextItemWidth(260.0f);
        if (!ImGui::BeginCombo(label, current))
            return;

        for (const FlareShaderChoice& c : flareShaderPalette())
            if (ImGui::Selectable(leaf(c.name)))
                setFlareShaderFor(entry, element, c.name);

        ImGui::EndCombo();
    }

    static void renderHalo(LightDataEntry& entry)
    {
        static const void* flaresFor = nullptr;
        static uint32_t flaresGen = 0;
        if (flaresFor != entry.dataPtr || flaresGen != flareListGeneration)
        {
            flaresFor = entry.dataPtr;
            scanLensFlaresFor(entry.lampFlares);
            flaresGen = flareListGeneration;
            applyFlareShaders(entry);
        }

        if (flareShaderPalette().empty())
            harvestFlareShaders();

        // flare texture, shared by lamps using it
        {
            HaloTint& tint = g_haloTints[entry.dataPtr];

            const int total = countHaloTextures(entry, false);
            const int resident = countHaloTextures(entry, true);

            if (total == 0)
            {
                ImGui::TextDisabled("color: no flare texture catalogued - scan textures first");
            }
            else if (resident < total)
            {
                if (ImGui::SmallButton("load halo textures"))
                    for (const std::string& k : haloTextureKeys(entry))
                        textures::loadTexturesMatching(k.c_str());
                ImGui::SameLine();
                ImGui::TextDisabled("%d of %d loaded", resident, total);
            }
            else
            {
                bool changed = false;
                ImGui::SetNextItemWidth(200.0f);
                changed |= ImGui::ColorEdit3("color##halo", tint.rgb, ImGuiColorEditFlags_Float);
                ImGui::SameLine();
                ImGui::SetNextItemWidth(70.0f);
                changed |= ImGui::DragFloat("##halob", &tint.brightness, 0.01f, 0.0f, 4.0f, "x%.2f");
                if (changed)
                    tint.applied = applyHaloTint(entry, tint) > 0;

                if (tint.applied)
                {
                    ImGui::SameLine();
                    if (ImGui::SmallButton("reset##halocol"))
                    {
                        tint = HaloTint{};
                        revertHaloTint(entry);
                    }
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("shared by every lamp with this flare texture");
            }
        }

        if (!flareShaderPalette().empty())
        {
            renderShaderCombo("look##halo", entry, ALL_FLARE_ELEMENTS);
            if (!entry.flareShaders.empty())
            {
                ImGui::SameLine();
                if (ImGui::SmallButton("reset##halolook"))
                    clearFlareShadersFor(entry);
            }
        }

        for (void* data : entry.lampFlares)
        {
            const uint32_t count = flareElementCount(data);
            if (!count)
                continue;

            ImGui::PushID(data);
            for (uint32_t i = 0; i < count; ++i)
            {
                fb::LensFlareElement* el = flareElement(data, i);
                if (!el)
                    continue;

                ImGui::PushID(int(i));

                const std::string shader = flareShaderName(el);
                char label[128];
                std::snprintf(label, sizeof(label), "size %u  %s", i, shader.empty() ? "" : leaf(shader));

                float size[2] = { el->m_Size.m_x, el->m_Size.m_y };
                ImGui::SetNextItemWidth(160.0f);
                if (ImGui::DragFloat2(label, size, 0.005f))
                {
                    setFlareFieldFor(entry, i, offsetof(fb::LensFlareElement, m_Size.m_x), &size[0], 1);
                    setFlareFieldFor(entry, i, offsetof(fb::LensFlareElement, m_Size.m_y), &size[1], 1);
                }

                ImGui::PopID();
            }
            ImGui::PopID();
            break;
        }

        if (ImGui::TreeNode("advanced##halo"))
        {
            static char wanted[192] = "FX/Lensflare/Shaders/LF_Green_HaloGlow";
            ImGui::SetNextItemWidth(300.0f);
            ImGui::InputTextWithHint("##fsload", "shader path", wanted, sizeof(wanted));
            ImGui::SameLine();
            if (ImGui::SmallButton("load shader"))
                fetchFlareShader(wanted);

            for (void* data : entry.lampFlares)
            {
                const uint32_t count = flareElementCount(data);
                if (!count)
                    continue;

                ImGui::PushID(data);
                for (uint32_t i = 0; i < count; ++i)
                {
                    fb::LensFlareElement* el = flareElement(data, i);
                    if (!el)
                        continue;

                    ImGui::PushID(int(i));

                    char header[64];
                    std::snprintf(header, sizeof(header), "element %u##adv", i);
                    if (ImGui::TreeNode(header))
                    {
                        renderShaderCombo("shader", entry, i);

                        float ray = el->m_RayDistance;
                        ImGui::SetNextItemWidth(160.0f);
                        if (ImGui::DragFloat("ray distance", &ray, 0.005f))
                            setFlareFieldFor(entry, i, offsetof(fb::LensFlareElement, m_RayDistance), &ray, 1);

                        struct Curve { const char* name; size_t offset; };
                        static const Curve kCurves[] = {
                            { "size vs occluder", offsetof(fb::LensFlareElement, m_SizeOccluderCurve) },
                            { "size vs screen pos", offsetof(fb::LensFlareElement, m_SizeScreenPosCurve) },
                            { "size vs angle", offsetof(fb::LensFlareElement, m_SizeAngleCurve) },
                            { "size vs cam dist", offsetof(fb::LensFlareElement, m_SizeCamDistCurve) },
                            { "alpha vs occluder", offsetof(fb::LensFlareElement, m_AlphaOccluderCurve) },
                            { "alpha vs screen pos", offsetof(fb::LensFlareElement, m_AlphaScreenPosCurve) },
                            { "alpha vs angle", offsetof(fb::LensFlareElement, m_AlphaAngleCurve) },
                            { "alpha vs cam dist", offsetof(fb::LensFlareElement, m_AlphaCamDistCurve) },
                        };

                        for (const Curve& c : kCurves)
                        {
                            const fb::Vec4& cv = *reinterpret_cast<const fb::Vec4*>(
                                reinterpret_cast<const uint8_t*>(el) + c.offset);
                            float v[4] = { cv.m_x, cv.m_y, cv.m_z, cv.m_w };

                            ImGui::PushID(int(c.offset));
                            ImGui::SetNextItemWidth(-140.0f);
                            if (ImGui::DragFloat4(c.name, v, 0.005f))
                                setFlareFieldFor(entry, i, uint32_t(c.offset), v, 4);
                            ImGui::PopID();
                        }

                        ImGui::TreePop();
                    }

                    ImGui::PopID();
                }
                ImGui::PopID();
                break;
            }

            ImGui::TreePop();
        }
    }

    static fb::LocalLightEntityData* g_focus = nullptr;

    static bool lock = false;
    static PlacedMesh locked;

    bool currentMesh(PlacedMesh& out)
    {
        if (lock && locked.mesh)
        {
            out = locked;
            return true;
        }
        return aimedPlacedMesh(out);
    }

    static void renderMeshes()
    {
        ImGui::Checkbox("overlay", &showMeshOverlay);
        ImGui::SameLine();
        ImGui::Checkbox("occlusion", &overlayOcclusion);
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("dimmed when behind geometry");
        ImGui::SameLine();
        ImGui::Checkbox("textures", &meshOverlayTextures);
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("materials of the placements near the crosshair, with their textures");
        ImGui::SameLine();
        ImGui::Checkbox("wireframe", &meshOverlayWireframe);
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("LOD0 triangles of the aimed placement (and of find-box matches), one color per material.");
        ImGui::SameLine();
        if (ImGui::SmallButton("style"))
            ImGui::OpenPopup("mesh_overlay_style");
        if (ImGui::BeginPopup("mesh_overlay_style"))
        {
            ImGui::SetNextItemWidth(160.0f);
            ImGui::SliderFloat("line width (m)", &render::lineWidthWorld, 0.0f, 0.10f, "%.3f");
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("0 = one-pixel engine lines; above that every 3D line is a quad this wide.");
            ImGui::Checkbox("depth", &render::lineDepthTest);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("3D lines hidden by geometry in front of them.");
            ImGui::Checkbox("only the aimed placement", &meshOverlayOnlyAimed);
            ImGui::Checkbox("labels", &meshOverlayLabels);
            const ImGuiColorEditFlags cf = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf;
            ImGui::ColorEdit4("meshes", meshOverlayColor, cf);
            ImGui::ColorEdit4("wireframe", meshOverlayWireColor, cf);
            ImGui::SameLine();
            ImGui::Checkbox("palette per material", &meshOverlayWirePalette);
            ImGui::EndPopup();
        }
        ImGui::SameLine();
        ImGui::SetNextItemWidth(140.0f);
        ImGui::SliderFloat("range", &meshOverlayMaxDistance, 5.0f, 200.0f, "%.0f m");
        ImGui::SameLine();
        ImGui::Checkbox("lock", &lock);
        ImGui::SameLine();
        static bool aimRay = false;
        ImGui::Checkbox("aim ray", &aimRay);
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("physics ray from the crosshair, shows the hit mesh");
        ImGui::SameLine();
        ImGui::Checkbox("GPU pick", &textures::pick::enabled);
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("draw probe under the crosshair every %u frames over %u, the Shaders tab needs it", textures::pick::interval, textures::pick::slices);
        ImGui::SameLine();
        ImGui::TextDisabled("%zu placed", placedMeshCount());

        if (meshPickPending)
        {
            meshPickPending = false;
            lock = true;
            locked = meshPick;
        }

        PlacedMesh cur;
        bool have = false;
        bool exactRay = false;
        if (lock)
        {
            have = locked.mesh != nullptr;
            cur = locked;
        }
        else
        {
            aimRayEnabled = aimRay;
            AimRay r;
            if (aimRay && lastAimRay(r))
            {
                if (r.exactValid)
                {
                    have = true;
                    exactRay = true;
                    cur = r.exact;
                    ImGui::TextDisabled("engine ray hit at %.1f m, part %u of the group", r.t, r.part);
                }
                else if (placedMeshAt(r.hit, 0.75f, cur))
                {
                    have = true;
                    ImGui::TextDisabled("%s ray hit at %.1f m (%.1f, %.1f, %.1f) - nearest placed box", r.engine ? "engine" : "box",
                        r.t, r.hit.m_x, r.hit.m_y, r.hit.m_z);
                }
                else
                    ImGui::TextDisabled("%s ray hit at %.1f m but no placed box there", r.engine ? "engine" : "box", r.t);
            }
            if (!have)
                have = showMeshOverlay && aimedPlacedMesh(cur);
            if (have)
                locked = cur;
        }

        char* const find = meshOverlayFilter;
        ImGui::SetNextItemWidth(220.0f);
        ImGui::InputTextWithHint("##findMesh", "find placed mesh by name", find, sizeof(meshOverlayFilter));
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("only matching placements, at any range");
        if (find[0])
        {
            fb::Vec3 cam{ };
            if (!render::cameraPosition(cam))
                ImGui::TextDisabled("no camera");
            const std::vector<NearMesh> found = placedMeshesMatching(find, cam, 10);
            if (found.empty())
                ImGui::TextDisabled("no placed mesh matches");
            for (size_t i = 0; i < found.size(); ++i)
            {
                const NearMesh& n = found[i];
                const char* name = n.mesh.mesh ? static_cast<fb::MeshAsset*>(n.mesh.mesh)->m_Name : nullptr;
                ImGui::PushID(int(i) + 1000);
                if (ImGui::SmallButton("lock"))
                {
                    lock = true;
                    locked = n.mesh;
                }
                ImGui::SameLine();
                ImGui::Text("%.0f m%s", n.distance, n.hasBox ? "" : " (origin)");
                ImGui::SameLine();
                ImGui::TextDisabled("%s  @ %.0f, %.0f, %.0f", name ? leaf(name) : "?",
                    n.mesh.pos.m_x, n.mesh.pos.m_y, n.mesh.pos.m_z);
                ImGui::PopID();
            }
            if (lock)
            {
                have = locked.mesh != nullptr;
                cur = locked;
            }
        }

        textures::SurfaceInfo si;
        const bool surface = !lock && !exactRay && textures::pick::enabled && textures::surfaceUnderCrosshair(si) && !si.meshName.empty();
        if (!have && !surface)
        {
            ImGui::TextDisabled(showMeshOverlay ? "aim at a mesh" : "turn the overlay on and aim at a mesh");
            return;
        }

        MeshVariationRef ref;
        if (surface)
        {
            ref.key = si.setKey;
            ref.name = si.meshName;
            ImGui::Text("%s", leaf(ref.name));
            ImGui::SameLine();
            ImGui::TextDisabled("drawn under the crosshair: material %u, %s", si.material, leaf(si.shaderName));
        }
        else
        {
            if (!meshRefFor(cur, ref))
                return;
            ImGui::Text("%s", leaf(ref.name));
            ImGui::SameLine();
            ImGui::TextDisabled("%.1f, %.1f, %.1f", cur.pos.m_x, cur.pos.m_y, cur.pos.m_z);
        }

        const std::vector<MeshVariationRef> refs{ ref };
        ImGui::PushID(int(ref.key ^ (ref.key >> 32)));
        if (ImGui::TreeNodeEx("Textures##Mesh", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (!textures::renderMeshTextures(refs))
                ImGui::TextDisabled("none catalogued - scan textures");
            ImGui::TreePop();
        }
        if (ImGui::TreeNodeEx("Materials##Mesh"))
        {
            if (!textures::renderMeshMaterials(refs))
                ImGui::TextDisabled("not realized");
            ImGui::TreePop();
        }
        ImGui::PopID();
    }

    static void pickUnderCrosshair()
    {
        static bool wasDown = false;
        const bool down = (GetAsyncKeyState(VK_MBUTTON) & 0x8000) != 0;
        if (down && !wasDown)
        {
            if (fb::LocalLightEntity* e = closestLightToCrosshair())
                for (auto& [dataPtr, entry] : getEntries())
                    if (entry.activeEntities.count(e))
                    {
                        g_focus = dataPtr;
                        break;
                    }
        }
        wasDown = down;
    }

    static void renderLightEntitiesTab()
    {
        pickUnderCrosshair();

        ImGui::SetNextItemWidth(220.0f);
        ImGui::InputTextWithHint("##LightFilter", "filter by name...", filterBuffer, sizeof(filterBuffer));

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100.0f);
        const char* typeFilters[] = { "All", "SpotLight", "PointLight", "LocalLight" };
        ImGui::Combo("##LightType", &typeFilter, typeFilters, IM_ARRAYSIZE(typeFilters));

        ImGui::SameLine();
        if (ImGui::Button("Scan"))
            scanAndApplyOverrides();

        ImGui::SameLine();
        if (ImGui::Button("Reset all"))
            resetAll();

        ImGui::SameLine();
        if (ImGui::Button(showOverlay ? "Overlay: on" : "Overlay: off"))
            ImGui::OpenPopup("light_overlay");
        if (ImGui::BeginPopup("light_overlay"))
        {
            ImGui::Checkbox("draw the overlay", &showOverlay);
            ImGui::SameLine();
            ImGui::Checkbox("occlusion", &overlayOcclusion);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("dimmed when behind geometry");
            ImGui::BeginDisabled(!showOverlay);
            ImGui::Checkbox("only the one at the crosshair", &showOnlyClosest);
            ImGui::SetNextItemWidth(180.0f);
            ImGui::SliderFloat("max distance (m)", &overlayMaxDistance, 5.0f, 500.0f, "%.0f");
            ImGui::EndDisabled();
            ImGui::EndPopup();
        }

        const auto displayNames = buildDisplayNames();

        if (fb::LocalLightEntity* aimed = closestLightToCrosshair())
        {
            for (auto& [dataPtr, entry] : getEntries())
                if (entry.activeEntities.count(aimed))
                {
                    const auto it = displayNames.find(dataPtr);
                    ImGui::TextDisabled("aim: %s   (middle mouse opens it)",
                        it != displayNames.end() ? it->second.c_str() : entry.assetName.c_str());
                    break;
                }
        }

        auto labelOf = [&](fb::LocalLightEntityData* ptr, const LightDataEntry* e) -> const std::string&
        {
            const auto it = displayNames.find(ptr);
            return it != displayNames.end() ? it->second : e->assetName;
        };

        std::vector<std::pair<fb::LocalLightEntityData*, LightDataEntry*>> entries;
        for (auto& [ptr, entry] : getEntries())
            entries.emplace_back(ptr, &entry);

        if (entries.empty())
        {
            ImGui::TextDisabled("No lights yet - spawn in, or press Scan.");
            return;
        }

        size_t totalActive = 0;
        size_t totalWithOverride = 0;
        for (const auto& [ptr, entry] : entries)
        {
            totalActive += entry->ActiveCount();
            if (entry->hasOverride)
                totalWithOverride++;
        }

        if (totalWithOverride)
            ImGui::TextDisabled("%zu lights   %zu instances   %zu modified",
                entries.size(), totalActive, totalWithOverride);
        else
            ImGui::TextDisabled("%zu lights   %zu instances", entries.size(), totalActive);

        std::string filterStr = filterBuffer;
        std::transform(filterStr.begin(), filterStr.end(), filterStr.begin(), ::tolower);
        std::sort(entries.begin(), entries.end(),
            [&](const auto& a, const auto& b) { return labelOf(a.first, a.second) < labelOf(b.first, b.second); });

        const float reserve = 2.0f * ImGui::GetFrameHeightWithSpacing();
        ImGui::BeginChild("LightList", ImVec2(0, -reserve), true);

        for (auto& [dataPtr, entry] : entries)
        {
            if (typeFilter == 1 && !entry->isSpotLight)
                continue;
            if (typeFilter == 2 && !entry->isPointLight)
                continue;
            if (typeFilter == 3 && (entry->isSpotLight || entry->isPointLight))
                continue;

            const std::string& dispName = labelOf(dataPtr, entry);

            if (!filterStr.empty())
            {
                std::string haystack = dispName + " " + entry->assetName;
                std::transform(haystack.begin(), haystack.end(), haystack.begin(), ::tolower);
                if (haystack.find(filterStr) == std::string::npos)
                    continue;
            }

            ImVec4 textColor;
            if (entry->hasOverride)
                textColor = ImVec4{ 0.2f, 1.0f, 0.2f, 1.0f };
            else if (entry->ActiveCount() == 0)
                textColor = ImVec4{ 0.5f, 0.5f, 0.5f, 1.0f };
            else
                textColor = ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f };

            ImGui::PushID(dataPtr);

            const char* typePrefix = entry->isSpotLight ? "[S]" : (entry->isPointLight ? "[P]" : "[L]");
            ImGui::TextColored(ImVec4{ 0.6f, 0.6f, 0.8f, 1.0f }, "%s", typePrefix);
            ImGui::SameLine();

            char header[512];
            sprintf_s(header, "%s - %zu active%s###LightHeader",
                dispName.c_str(),
                entry->ActiveCount(),
                entry->hasOverride ? " [MODIFIED]" : "");

            const bool focus = dataPtr == g_focus;
            if (focus)
                ImGui::SetNextItemOpen(true);

            ImGui::PushStyleColor(ImGuiCol_Text, textColor);
            bool open = ImGui::CollapsingHeader(header);
            ImGui::PopStyleColor();

            if (focus)
            {
                ImGui::SetScrollHereY(0.0f);
                g_focus = nullptr;
            }

            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::TextUnformatted(entry->assetName.c_str());
                ImGui::TextDisabled("%s   %s", entry->lightType.c_str(), entry->containerType.c_str());
                if (entry->origCaptured)
                {
                    ImGui::Separator();
                    ImGui::TextColored(ui::originalColor, "original");
                    ImGui::Text("color  %.2f, %.2f, %.2f",
                        entry->origColor.m_x, entry->origColor.m_y, entry->origColor.m_z);
                    ImGui::Text("radius  %.2f   intensity  %.2f",
                        entry->origRadius, entry->origIntensity);
                }
                ImGui::EndTooltip();
            }

            if (open)
            {
                ImGui::Indent();
                renderEditor(*entry);
                ImGui::Unindent();
            }

            ImGui::PopID();
        }

        ImGui::EndChild();
    }

    void renderTab()
    {
        if (ImGui::CollapsingHeader("Spawn light"))
            enlighten::spawn::renderUI();
        renderLightEntitiesTab();
    }

    void renderMeshesPanel()
    {
        renderMeshes();
    }

    static void renderNearbyMeshes(LightDataEntry& entry)
    {
        fb::Vec3 pos{ };
        if (!entryWorldPos(entry, pos) || placedMeshCount() == 0)
            return;
        if (!ImGui::TreeNodeEx("Nearby meshes##Light"))
            return;

        const std::vector<NearMesh> found = placedMeshesNear(pos, 8, 12.0f);
        if (found.empty())
            ImGui::TextDisabled("none within 12 m");
        for (size_t i = 0; i < found.size(); ++i)
        {
            const NearMesh& n = found[i];
            const char* name = n.mesh.mesh ? static_cast<fb::MeshAsset*>(n.mesh.mesh)->m_Name : nullptr;
            ImGui::PushID(int(i));
            if (ImGui::SmallButton("edit"))
            {
                meshPick = n.mesh;
                meshPickPending = true;
                textures::focusTab = true;
                textures::focusMeshTab = true;
            }
            ImGui::SameLine();
            ImGui::Text("%.1f m%s", n.distance, n.hasBox ? "" : " (origin)");
            ImGui::SameLine();
            ImGui::TextDisabled("%s", name ? leaf(name) : "?");
            ImGui::PopID();
        }
        ImGui::TreePop();
    }

    static void renderLampSection(LightDataEntry& entry)
    {
        if (!entry.dataPtr)
            return;

        if (entry.isSpotLight)
        {
            auto* spot = static_cast<fb::SpotLightEntityData*>(entry.dataPtr);
            if (ImGui::TreeNodeEx("Projected texture##Light"))
            {
                if (!spot->m_Texture)
                    ImGui::TextDisabled("none");
                else if (!textures::renderAssetTextureRow(spot->m_Texture))
                {
                    const char* name = spot->m_Texture->m_Name;
                    ImGui::TextWrapped("%s", name ? name : "(unnamed texture asset)");
                }

                static std::unordered_map<void*, void*> originals;
                void** slot = reinterpret_cast<void**>(&spot->m_Texture);
                if (originals.find(slot) == originals.end())
                    originals[slot] = spot->m_Texture;

                textures::renderAssetTexturePicker(slot, "gobo");

                if (originals[slot] != spot->m_Texture)
                {
                    ImGui::SameLine();
                    if (ImGui::SmallButton("reset##gobo"))
                        spot->m_Texture = static_cast<fb::TextureAsset*>(originals[slot]);
                }

                ImGui::TreePop();
            }
        }

        renderNearbyMeshes(entry);

        if (entry.lampMeshes.empty() && entry.lampFlares.empty() &&
            entry.lampEffects.empty() && entry.lampShaderParams.empty())
            return;

        if (!entry.lampEffects.empty() && ImGui::TreeNodeEx("Effect##Light"))
        {
            for (size_t i = 0; i < entry.lampEffects.size(); ++i)
            {
                const std::string& fx = entry.lampEffects[i];

                ImGui::PushID(int(i));
                if (ImGui::SmallButton("open"))
                {
                    std::snprintf(emitters::searchFilter, sizeof(emitters::searchFilter), "%s", leaf(fx));
                    emitters::focusTab = true;
                }
                ImGui::SameLine();
                ImGui::TextDisabled("%s", leaf(fx));
                ImGui::PopID();
            }
            ImGui::TreePop();
        }

        if (!entry.lampFlares.empty() &&
            ImGui::TreeNodeEx("Halo##Light", ImGuiTreeNodeFlags_DefaultOpen))
        {
            renderHalo(entry);
            ImGui::TreePop();
        }

        if (!entry.lampMeshes.empty() &&
            ImGui::TreeNodeEx("Lamp textures##Light", ImGuiTreeNodeFlags_DefaultOpen))
        {
            textures::renderMeshTextures(entry.lampMeshes);
            ImGui::TreePop();
        }

        if (!entry.lampMeshes.empty() && ImGui::TreeNodeEx("Lamp materials##Light"))
        {
            ensureAssetMaterials(entry);
            renderAssetMaterials();
            textures::renderMeshMaterials(entry.lampMeshes);
            ImGui::TreePop();
        }

        renderShaderDrivers(entry);
    }

    void renderEditor(LightDataEntry& entry)
    {
        if (!entry.origCaptured)
        {
            ImGui::TextDisabled("not spawned yet");
            return;
        }

        bool changed = false;
        if (ImGui::Checkbox("override", &entry.hasOverride))
        {
            if (entry.hasOverride)
                changed = true;
        }

        ImGui::SameLine();
        if (ImGui::Button("Reset"))
        {
            entry.ResetToOriginal();
            if (entry.hasOverride)
                changed = true;
        }

        {
            fb::Vec3 pos{ };
            const bool hasPos = entryWorldPos(entry, pos);

            ImGui::SameLine();
            ImGui::BeginDisabled(!hasPos);
            ImGui::Checkbox("save with position", &entry.saveWithPosition);
            ImGui::EndDisabled();

            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                ImGui::SetTooltip("Key by world position (static lights only).%s%s",
                    hasPos ? "\n" : "", hasPos ? "" : "No entity yet.");

            if (entry.saveWithPosition && entry.ActiveCount() > 1)
                ImGui::TextDisabled("%zu instances - the first position is used", entry.ActiveCount());
        }

        renderLampSection(entry);

        if (!entry.hasOverride)
            return;

        ImGui::Separator();

        if (ImGui::TreeNodeEx("Light##Light", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ui::HdrColor3Edit("Color", &entry.color, &entry.origColor))
                changed = true;

            if (ui::FloatEdit("Radius", &entry.radius, &entry.origRadius))
                changed = true;
            if (ui::FloatEdit("Intensity", &entry.intensity, &entry.origIntensity))
                changed = true;
            if (ImGui::DragFloat("Attenuation Offset", &entry.attenuationOffset, 0.01f))
                changed = true;

            if (ImGui::Checkbox("Visible", &entry.visible)) changed = true;
            ImGui::SameLine();
            if (ImGui::Checkbox("Specular", &entry.specularEnable)) changed = true;
            ImGui::SameLine();
            if (ImGui::Checkbox("Enlighten", &entry.enlightenEnable)) changed = true;

            if (ui::EnumCombo<fb::EnlightenColorMode>("Enlighten Color Mode", &entry.enlightenColorMode))
                changed = true;

            ImGui::TreePop();
        }

        if (entry.isSpotLight && ImGui::TreeNode("Spot##Light"))
        {
            const char* shapes[] = { "Cone", "Frustum", "Ortho" };
            if (ImGui::Combo("Shape", &entry.spotShape, shapes, 3)) changed = true;
            if (ImGui::SliderFloat("Cone Inner Angle", &entry.coneInnerAngle, 0.0f, 90.0f)) changed = true;
            if (ImGui::SliderFloat("Cone Outer Angle", &entry.coneOuterAngle, 0.0f, 90.0f)) changed = true;
            if (ImGui::DragFloat("Frustum FOV", &entry.frustumFov, 0.1f)) changed = true;
            if (ImGui::DragFloat("Frustum Aspect", &entry.frustumAspect, 0.01f)) changed = true;
            if (ImGui::DragFloat("Ortho Width", &entry.orthoWidth, 0.1f)) changed = true;
            if (ImGui::DragFloat("Ortho Height", &entry.orthoHeight, 0.1f)) changed = true;
            if (ImGui::Checkbox("Cast Shadows", &entry.castShadowsEnable)) changed = true;
            if (ui::EnumCombo<fb::QualityLevel>("Shadow Min Level", &entry.castShadowsMinLevel)) changed = true;

            ImGui::TreePop();
        }

        if (entry.isPointLight && ImGui::TreeNode("Point##Light"))
        {
            if (ImGui::DragFloat("Width", &entry.pointWidth, 0.1f)) changed = true;
            if (ImGui::DragFloat("Translucency Ambient", &entry.translucencyAmbient, 0.01f)) changed = true;
            if (ImGui::DragFloat("Translucency Scale", &entry.translucencyScale, 0.01f)) changed = true;
            if (ImGui::DragScalar("Translucency Power", ImGuiDataType_U32, &entry.translucencyPower)) changed = true;
            if (ImGui::DragFloat("Translucency Distortion", &entry.translucencyDistortion, 0.01f)) changed = true;

            ImGui::TreePop();
        }

        if (changed && entry.hasOverride)
            applyOverride(entry);
    }
}
