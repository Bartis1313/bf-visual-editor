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
        void renderStreamingControls(TextureEntry& e)
        {
            if (!e.texture)
                return;

            fb::TextureStreamingManager* mgr = nullptr;
            uint16_t handle = 0;
            if (!streamingTarget(e.texture, mgr, handle))
                return;

            const bool pinned = isPinned(handle);
            if (e.drawable && !pinned)
                return;

            const int st = onDemandStatus(e.texture);
            static const char* kNames[] = { "not loaded", "loading", "loaded",
                                            "not an on-demand texture" };
            ImGui::Text("streaming handle 0x%04X   status: %s", handle,
                (st >= 0 && st <= 3) ? kNames[st] : "query failed");

            if (st == 3)
            {
                ImGui::TextDisabled("not in the on-demand pool - the engine cannot be asked "
                                    "to stream this one in");
                return;
            }

            if (st == 0 || st == 1)
            {
                if (ImGui::Button("Load texture"))
                {
                    budgetHit = false;
                    queueLoad(handle);
                }
                ImGui::SameLine();
                ImGui::TextDisabled(st == 1 ? "already streaming in" : "pull it in to preview");
            }

            if (pinned)
            {
                if (ImGui::Button("Unload"))
                {
                    if (e.drawable)
                        releaseSrv(e.srvLinear);
                    e.srvLinear = nullptr;
                    e.drawable = false;
                    e.viewChecked = false;
                    releaseHandle(handle);
                }
                ImGui::SameLine();
                ImGui::TextDisabled("we are holding this in memory");
            }
        }

        void renderSkyResourcesSection()
        {
            ImGui::TextDisabled("live sky textures, applied immediately");

            if (ImGui::Button("Refresh asset list"))
                harvestTextureAssets();
            ImGui::SameLine();
            ImGui::Text("%zu TextureAssets", textureAssets.size());
            ImGui::SameLine();
            if (ImGui::Button("Revert all"))
                for (int i = 0; i < SKY_SLOT_COUNT; ++i)
                    skyRevertSlot(i);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("map's own textures back in every slot");
            ImGui::SameLine();
            ImGui::Checkbox("sky-relevant only", &skyRelevantOnly);

            ImGui::Separator();

            uint8_t* firstState = nullptr;
            forEachVeState([&](uint8_t* st) { if (!firstState) firstState = st; });
            if (!firstState)
            {
                ImGui::TextDisabled("no VE states");
                return;
            }

            for (int i = 0; i < SKY_SLOT_COUNT; ++i)
            {
                ImGui::PushID(3000 + i);

                void* cur = skyCurrent(i);

                void* shown = skyOverride[i].enabled ? skyOverride[i].asset : cur;
                if (shown)
                    if (TextureEntry* me = const_cast<TextureEntry*>(findTextureEntry(shown)))
                        ensureDrawable(*me);
                const TextureEntry* pe = shown ? findTextureEntry(shown) : nullptr;
                if (pe && pe->drawable)
                    ImGui::Image(reinterpret_cast<ImTextureID>(pe->srvLinear), ImVec2(40.0f, 40.0f));
                else
                    ImGui::Dummy(ImVec2(40.0f, 40.0f));
                ImGui::SameLine();

                ImGui::Text("%-22s", SKY_SLOTS[i].label);
                ImGui::SameLine(240.0f);

                if (skyOverride[i].enabled)
                {
                    const TextureEntry* e = findTextureEntry(skyOverride[i].asset);
                    ImGui::TextColored(ImVec4(0.4f, 0.9f, 0.4f, 1.0f), "%s",
                        e ? shortLabel(*e).c_str() : "(override)");
                }
                else if (cur)
                {
                    const TextureEntry* e = findTextureEntry(cur);
                    if (e)
                        ImGui::TextDisabled("%s", shortLabel(*e).c_str());
                    else
                    {
                        noteUncatalogued(cur);
                        ImGui::TextDisabled("ITexture %p", cur);
                    }
                }
                else
                {
                    ImGui::TextColored(ImVec4(0.8f, 0.6f, 0.3f, 1.0f), "(null - layer inactive)");
                }

                ImGui::SameLine();
                if (ImGui::SmallButton("set"))
                {
                    skyResPicker = i;
                    skyPickSearch[0] = 0;
                    if (textureAssets.empty())
                        harvestTextureAssets();
                }
                if (shown)
                {
                    ImGui::SameLine();
                    if (ImGui::SmallButton("edit"))
                        skyEditSlot = i;
                }

                ImGui::SameLine();
                if (ImGui::SmallButton("null"))
                {
                    skyCaptureOriginal(i);
                    skyOverride[i].enabled = true;
                    skyOverride[i].asset = nullptr;
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("empty the slot (inactive layer)");

                if (shown)
                {
                    ImGui::SameLine();
                    if (gen::isClonedTexture(shown))
                    {
                        ImGui::TextColored(ImVec4(0.5f, 0.8f, 1.0f, 1.0f), "copy");
                    }
                    else if (ImGui::SmallButton("copy"))
                    {
                        if (void* copy = gen::cloneTexture(shown))
                        {
                            if (const TextureEntry* src = findTextureEntry(shown))
                                catalogueClone(copy, *src);

                            skyCaptureOriginal(i);
                            skyOverride[i].enabled = true;
                            skyOverride[i].asset = copy;
                            logger::info("[textures] sky slot {} detached: {} -> {}",
                                SKY_SLOTS[i].label, shown, copy);
                        }
                    }
                    if (ImGui::IsItemHovered())
                        ImGui::SetTooltip("private copy for this slot");
                }

                if (skyOverride[i].enabled || skyOriginalCaptured[i])
                {
                    ImGui::SameLine();
                    if (ImGui::SmallButton("revert"))
                        skyRevertSlot(i);
                    if (ImGui::IsItemHovered())
                        ImGui::SetTooltip("put the map's own texture back");
                }

                ImGui::PopID();
            }

            if (skyEditSlot >= 0)
            {
                if (!ImGui::IsPopupOpen("sky_texture_editor"))
                {
                    ImGui::OpenPopup("sky_texture_editor");
                    ImGui::SetNextWindowSize(ImVec2(620.0f, 720.0f), ImGuiCond_Appearing);
                }

                if (ImGui::BeginPopupModal("sky_texture_editor", nullptr, 0))
                {
                    void* live = skyCurrent(skyEditSlot);
                    void* target = skyOverride[skyEditSlot].enabled
                        ? skyOverride[skyEditSlot].asset : live;

                    ImGui::Text("%s", SKY_SLOTS[skyEditSlot].label);
                    if (const TextureEntry* te = findTextureEntry(target))
                    {
                        if (const char* n = textureLabel(*te))
                            ImGui::TextDisabled("%s", n);
                        ImGui::Text("%ux%u  %u mips", te->width, te->height, te->mips);
                        if (te->drawable)
                            ImGui::Image(reinterpret_cast<ImTextureID>(te->srvLinear),
                                ImVec2(192.0f, 192.0f));
                    }
                    ImGui::Separator();

                    if (target)
                    {
                        const TextureEntry* te = findTextureEntry(target);
                        gen::renderUI(target, te ? exportNameFor(*te).c_str() : nullptr);
                    }
                    else
                    {
                        ImGui::TextDisabled("slot is empty - assign a texture first");
                    }

                    ImGui::Separator();
                    if (ImGui::Button("Close"))
                    {
                        skyEditSlot = -1;
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }
            }

            if (skyResPicker >= 0)
            {
                if (!ImGui::IsPopupOpen("pick_sky_resource"))
                {
                    ImGui::OpenPopup("pick_sky_resource");
                    ImGui::SetNextWindowSize(ImVec2(640.0f, 560.0f), ImGuiCond_Appearing);
                }

                if (ImGui::BeginPopupModal("pick_sky_resource", nullptr, 0))
                {
                    ImGui::Text("Assign to %s", SKY_SLOTS[skyResPicker].label);
                    ImGui::TextDisabled("any loaded TextureAsset - resolved via +0x18");
                    ImGui::PushItemWidth(520.0f);
                    ImGui::InputTextWithHint("##srsearch", "filter by name...",
                        skyPickSearch, sizeof(skyPickSearch));
                    ImGui::PopItemWidth();

                    ImGui::BeginChild("srpick", ImVec2(0.0f, -32.0f), true);
                    int shown = 0;
                    bool truncated = false;
                    for (const auto& [name, ptr] : textureAssets)
                    {
                        if (skyPickSearch[0] && !matchesSearch(name, skyPickSearch))
                            continue;
                        if (skyRelevantOnly && !skyPickSearch[0] && !looksSkyRelevant(name))
                            continue;

                        void* tex = resolveAssetTexture(ptr);
                        if (!tex)
                            continue;

                        if (++shown > 400)
                        {
                            truncated = true;
                            break;
                        }

                        if (TextureEntry* me = const_cast<TextureEntry*>(findTextureEntry(tex)))
                            ensureDrawable(*me);
                        if (const TextureEntry* te = findTextureEntry(tex); te && te->drawable)
                        {
                            ImGui::Image(reinterpret_cast<ImTextureID>(te->srvLinear), ImVec2(32.0f, 32.0f));
                            ImGui::SameLine();
                        }

                        if (ImGui::Selectable(name.c_str()))
                        {
                            skyCaptureOriginal(skyResPicker);
                            skyOverride[skyResPicker].enabled = true;
                            skyOverride[skyResPicker].asset = tex;
                            logger::info("[textures] sky {} -> {} (ITexture {})",
                                SKY_SLOTS[skyResPicker].label, name, tex);
                            skyResPicker = -1;
                            ImGui::CloseCurrentPopup();
                            break;
                        }
                    }
                    ImGui::EndChild();
                    if (truncated)
                        ImGui::TextDisabled("list truncated - type to filter");

                    if (ImGui::Button("Cancel"))
                    {
                        skyResPicker = -1;
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }
            }
        }

        void renderSkySection()
        {
            renderSkyResourcesSection();
        }

        void renderGallery(float paneWidth)
        {
            refreshVisible();
            const std::vector<int>& visible = g_visible;

            ImGui::TextDisabled("%zu shown / %zu catalogued / %u loaded now",
                visible.size(), entries.size(), g_liveCount);

            if (!galleryThumbs)
            {
                ImGui::BeginChild("gallerylist", ImVec2(0.0f, 0.0f), true);
                ImGuiListClipper lc;
                lc.Begin(int(visible.size()));
                while (lc.Step())
                {
                    for (int row = lc.DisplayStart; row < lc.DisplayEnd; ++row)
                    {
                        const int i = visible[row];
                        const TextureEntry& e = entries[i];
                        char label[320];
                        std::snprintf(label, sizeof(label), "%s  (%ux%u)##L%d",
                            e.shortName.c_str(), e.width, e.height, i);
                        if (ImGui::Selectable(label, selected == i))
                        {
                            selected = i;
                            zoom = 1.0f;
                            panX = panY = 0.0f;
                            ensureDrawable(entries[i]);
                        }
                    }
                }
                ImGui::EndChild();
                return;
            }

            const float cell = thumbSize + 20.0f;
            const int perRow = (std::max)(1, int((paneWidth - 24.0f) / cell));
            const int rows = (int(visible.size()) + perRow - 1) / perRow;

            int drawnThumbs = 0;
            ImGui::BeginChild("gallery", ImVec2(0.0f, 0.0f), true);
            ImGuiListClipper clipper;
            clipper.Begin(rows, cell + ImGui::GetTextLineHeightWithSpacing());
            while (clipper.Step())
            {
                for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; ++row)
                {
                    for (int col = 0; col < perRow; ++col)
                    {
                        const int idx = row * perRow + col;
                        if (idx >= int(visible.size()))
                            break;
                        if (col)
                            ImGui::SameLine();

                        const int i = visible[idx];
                        if (drawnThumbs < maxThumbs)
                            ensureDrawable(entries[i]);
                        const TextureEntry& e = entries[i];

                        ImGui::PushID(i);
                        ImGui::BeginGroup();

                        const bool isSel = (selected == i);
                        if (isSel)
                            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.26f, 0.59f, 0.98f, 0.85f));

                        if (!e.loaded || !e.texture)
                            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.45f);

                        bool clicked;
                        if (e.drawable && drawnThumbs < maxThumbs)
                        {
                            ++drawnThumbs;
                            clicked = ImGui::ImageButton("t", reinterpret_cast<ImTextureID>(e.srvLinear),
                                ImVec2(thumbSize, thumbSize));
                        }
                        else
                            clicked = ImGui::Button(e.texture ? typeName(e.type) : "...",
                                ImVec2(thumbSize + 8.0f, thumbSize + 8.0f));

                        if (isSel)
                            ImGui::PopStyleColor();

                        if (clicked)
                        {
                            selected = i;
                            zoom = 1.0f;
                            panX = 0.0f;
                            panY = 0.0f;
                        }

                        if (!e.loaded || !e.texture)
                            ImGui::PopStyleVar();

                        ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + thumbSize);
                        ImGui::TextDisabled("%s", e.shortName.c_str());
                        ImGui::PopTextWrapPos();

                        ImGui::EndGroup();

                        if (ImGui::IsItemHovered())
                        {
                            const char* full = textureLabel(e);
                            ImGui::SetTooltip("%s\n%ux%u  %u mips  %s%s\n%u material slot(s)",
                                full ? full : "(unnamed)", e.width, e.height, e.mips,
                                typeName(e.type), e.srgb ? "  sRGB" : "", e.refs);
                        }
                        ImGui::PopID();
                    }
                }
            }
            ImGui::EndChild();
        }

        void renderPreview(const TextureEntry& e, float side)
        {
            void* srv = (previewGamma && e.srvGamma) ? e.srvGamma : e.srvLinear;
            if (!e.drawable || !srv)
            {
                uint32_t fmt = 0;
                const char* dim = srvDimensionName(e.srvLinear, fmt);
                ImGui::TextColored(ImVec4(0.9f, 0.7f, 0.3f, 1.0f),
                    "no preview: view is %s (DXGI %u)", dim, fmt);
                ImGui::TextDisabled("resource %p   shaderFormat %u   resFormat %u",
                    e.resource, e.shaderFormat, e.resourceFormat);
                if (!e.texture)
                    ImGui::TextDisabled("asset %p is loaded but its texture is not resident yet - "
                                        "it will appear once the game streams it in", e.asset);
                else if (!e.resource)
                    ImGui::TextDisabled("no D3D resource either - texture is not resident");
                return;
            }

            const float half = 0.5f / zoom;
            const float limit = 0.5f - half;
            panX = (std::min)((std::max)(panX, -limit), limit);
            panY = (std::min)((std::max)(panY, -limit), limit);

            const ImVec2 uv0(0.5f + panX - half, 0.5f + panY - half);
            const ImVec2 uv1(0.5f + panX + half, 0.5f + panY + half);

            const ImVec2 p0 = ImGui::GetCursorScreenPos();
            const ImVec2 p1(p0.x + side, p0.y + side);

            ImGui::InvisibleButton("##canvas", ImVec2(side, side),
                ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
            const bool active = ImGui::IsItemActive();
            const bool hovered = ImGui::IsItemHovered();

            ImDrawList* dl = ImGui::GetWindowDrawList();
            dl->AddImage(reinterpret_cast<ImTextureID>(srv), p0, p1, uv0, uv1);
            static bool previewCompare = false;
            void* const original = previewCompare ? gen::originalView(e.texture) : nullptr;
            if (original)
            {
                const float midX = (p0.x + p1.x) * 0.5f;
                const float midU = (uv0.x + uv1.x) * 0.5f;
                dl->AddImage(reinterpret_cast<ImTextureID>(original), p0, ImVec2(midX, p1.y), uv0, ImVec2(midU, uv1.y));
                dl->AddLine(ImVec2(midX, p0.y), ImVec2(midX, p1.y), IM_COL32(255, 255, 0, 200), 1.0f);
                dl->AddText(ImVec2(p0.x + 4.0f, p0.y + 4.0f), IM_COL32(255, 255, 0, 220), "original");
                dl->AddText(ImVec2(midX + 4.0f, p0.y + 4.0f), IM_COL32(255, 255, 0, 220), "edited");
            }
            dl->AddRect(p0, p1, IM_COL32(255, 255, 255, 60));

            if (hovered)
            {
                const float wheel = ImGui::GetIO().MouseWheel;
                if (wheel != 0.0f)
                {
                    const ImVec2 m = ImGui::GetIO().MousePos;
                    const float fx = (m.x - p0.x) / side - 0.5f;
                    const float fy = (m.y - p0.y) / side - 0.5f;
                    const float before = 1.0f / zoom;
                    zoom = (std::min)((std::max)(zoom * (1.0f + wheel * 0.15f), 1.0f), 64.0f);
                    const float after = 1.0f / zoom;
                    panX += fx * (before - after);
                    panY += fy * (before - after);
                }
            }

            if (active && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
            {
                const ImVec2 d = ImGui::GetIO().MouseDelta;
                panX -= d.x / (side * zoom);
                panY -= d.y / (side * zoom);
            }

            ImGui::Text("zoom %.1fx", zoom);
            ImGui::SameLine();
            if (ImGui::SmallButton("reset view"))
            {
                zoom = 1.0f;
                panX = 0.0f;
                panY = 0.0f;
            }
            if (e.srvGamma)
            {
                ImGui::SameLine();
                ImGui::Checkbox("sRGB view", &previewGamma);
            }
            if (gen::originalView(e.texture))
            {
                ImGui::SameLine();
                ImGui::Checkbox("compare", &previewCompare);
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("left: engine texture, right: your edit");
            }
            ImGui::TextDisabled("wheel to zoom at cursor, drag to pan");
        }

        void renderReplacePicker()
        {
            if (!ImGui::BeginPopupModal("replace_texture", nullptr, 0))
                return;

            if (replaceSlot < 0)
                ImGui::Text("Replace in all %zu slot(s)", usages.size());
            else
                ImGui::Text("Replace in one slot");
            ImGui::Separator();

            static char pickSearch[128] = {};
            ImGui::PushItemWidth(560.0f);
            ImGui::InputTextWithHint("##psearch", "filter by name...", pickSearch, sizeof(pickSearch));
            ImGui::PopItemWidth();

            ImGui::BeginChild("pickgrid", ImVec2(0.0f, -32.0f), true);
            int drawn = 0;
            for (int i = 0; i < int(entries.size()); ++i)
            {
                const TextureEntry& e = entries[i];
                if (!e.drawable)
                    continue;
                if (pickSearch[0])
                {
                    const char* full = textureLabel(e);
                    if (!full || !matchesSearch(full, pickSearch))
                        continue;
                }

                if (drawn % 5 != 0)
                    ImGui::SameLine();
                ++drawn;

                ImGui::PushID(i);
                if (ImGui::ImageButton("p", reinterpret_cast<ImTextureID>(e.srvLinear),
                        ImVec2(96.0f, 96.0f)))
                {
                    const size_t n = (replaceSlot >= 0 && replaceSlot < int(usages.size())) ? 1 : usages.size();
                    if (replaceSlot >= 0 && replaceSlot < int(usages.size()))
                        writeTextureSlot(usages[replaceSlot], e.texture);
                    else
                        for (const Usage& u : usages)
                            writeTextureSlot(u, e.texture);

                    logger::info("[textures] replaced {} slot(s) with {}", n, shortLabel(e));
                    ImGui::CloseCurrentPopup();
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("%s", shortLabel(e).c_str());
                ImGui::PopID();
            }
            ImGui::EndChild();

            if (ImGui::Button("Cancel"))
                ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }

        void renderEbxTextures(const MaterialEntry& m)
        {
            if (m.ebxTextures.empty())
                return;

            ImGui::TextDisabled("textures (from the mesh variation database)");

            for (const auto& [handle, path] : m.ebxTextures)
            {
                ImGui::PushID(int(handle));

                void* const tex = textureByPath(path);
                const TextureEntry* te = tex ? findTextureEntry(tex) : nullptr;

                if (te && te->drawable && te->srvLinear)
                    ImGui::Image(reinterpret_cast<ImTextureID>(te->srvLinear), ImVec2(28.0f, 28.0f));
                else
                    ImGui::Dummy(ImVec2(28.0f, 28.0f));
                ImGui::SameLine();

                ImGui::BeginGroup();
                const char* pname = nameForHandle(handle);
                ImGui::TextUnformatted(pname ? pname : "(unnamed parameter)");
                ImGui::TextDisabled("%s", path.c_str());
                if (te)
                {
                    ImGui::SameLine();
                    if (ImGui::SmallButton("edit"))
                        selectTextureIndex(int(te - entries.data()));
                }
                else
                {
                    ImGui::SameLine();
                    ImGui::TextDisabled("(not streamed in)");
                }
                ImGui::EndGroup();

                ImGui::PopID();
            }
        }

        void renderMaterialTextureSlots(const MaterialEntry& m)
        {
            renderEbxTextures(m);

            if (!m.texCount)
                return;

            ImGui::TextDisabled("textures");
            for (uint32_t t = 0; t < m.texCount; ++t)
            {
                const uint32_t slot = uint32_t(m.vecCount) + t;

                uint32_t handle = 0;
                uint16_t offset = 0;
                if (!paramInfo(static_cast<const uint8_t*>(m.block), slot, handle, offset))
                    continue;

                void* tex = readTex(m.block, offset);

                const char* pname = nameForHandle(handle);
                const TextureEntry* te = tex ? findTextureEntry(tex) : nullptr;

                ImGui::PushID(int(slot));
                if (te && te->drawable && te->srvLinear)
                    ImGui::Image(reinterpret_cast<ImTextureID>(te->srvLinear), ImVec2(28.0f, 28.0f));
                else
                    ImGui::Dummy(ImVec2(28.0f, 28.0f));
                ImGui::SameLine();

                ImGui::BeginGroup();
                ImGui::TextUnformatted(pname ? pname : "(unnamed parameter)");
                if (te)
                {
                    const char* full = textureLabel(*te);
                    ImGui::TextDisabled("%s", full ? full : "(no path)");
                    ImGui::SameLine();
                    if (ImGui::SmallButton("edit"))
                        selectTextureIndex(int(te - entries.data()));

                    ImGui::SameLine();
                    if (gen::isClonedTexture(tex))
                    {
                        ImGui::TextColored(ImVec4(0.5f, 0.8f, 1.0f, 1.0f), "private copy");
                    }
                    else if (ImGui::SmallButton("make private"))
                    {
                        if (void* copy = gen::cloneTexture(tex))
                        {
                            assignSlot(m.block, slot, offset, handle, m.setKey, m.index,
                                       tex, copy);

                            catalogueClone(copy, *te);
                        }
                    }
                    if (ImGui::IsItemHovered())
                        ImGui::SetTooltip("private copy for this slot");
                }
                else if (tex)
                {
                    noteUncatalogued(tex);
                    ImGui::TextDisabled("(cataloguing...)");
                }
                else
                {
                    ImGui::TextDisabled("(empty slot)");
                }

                if (ImGui::SmallButton("replace"))
                {
                    slotPick = { m.block, slot, offset, handle, m.setKey, m.index, tex, true };
                    slotPickSearch[0] = '\0';
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("any catalogued texture, private copies included");

                ImGui::SameLine();
                ImGui::BeginDisabled(tex == nullptr);
                if (ImGui::SmallButton("null"))
                    assignSlot(m.block, slot, offset, handle, m.setKey, m.index, tex, nullptr);
                ImGui::EndDisabled();
                if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                    ImGui::SetTooltip("empty the slot: the shader samples nothing");

                if (findTexBackup(m.block, slot))
                {
                    ImGui::SameLine();
                    if (ImGui::SmallButton("revert"))
                        revertSlot(m.block, slot, offset, handle, m.setKey, m.index);
                    if (ImGui::IsItemHovered())
                        ImGui::SetTooltip("restore the original and drop the edit");
                }

                ImGui::EndGroup();
                ImGui::PopID();
            }

            renderSlotPicker();
        }

        void renderSlotPicker()
        {
            if (!slotPick.open)
                return;

            static uint32_t lastFrame = 0xFFFFFFFF;
            if (lastFrame == frameCounter)
                return;
            lastFrame = frameCounter;

            ImGui::SetNextWindowSize(ImVec2(460.0f, 520.0f), ImGuiCond_Appearing);
            bool keepOpen = true;
            if (!ImGui::Begin("Assign texture to slot", &keepOpen))
            {
                ImGui::End();
                if (!keepOpen)
                    slotPick.open = false;
                return;
            }

            ImGui::TextDisabled("assign to this slot");
            ImGui::SetNextItemWidth(-1.0f);
            ImGui::InputTextWithHint("##slotsearch", "search", slotPickSearch,
                                     sizeof(slotPickSearch));

            if (ImGui::SmallButton("empty the slot"))
            {
                assignSlot(slotPick.block, slotPick.slot, slotPick.offset, slotPick.handle,
                           slotPick.setKey, slotPick.material, slotPick.original, nullptr);
                slotPick.open = false;
            }
            ImGui::Separator();

            ImGui::BeginChild("slotlist", ImVec2(0.0f, 0.0f));
            int shown = 0;
            for (int i = 0; i < int(entries.size()) && shown < 400; ++i)
            {
                TextureEntry& e = entries[i];
                if (!e.texture)
                    continue;

                const std::string label = shortLabel(e);
                if (!matchesSearch(e.lowerPath.empty() ? label : e.lowerPath, slotPickSearch))
                    continue;

                ++shown;
                ImGui::PushID(i);

                ensureDrawable(e);
                if (e.drawable && e.srvLinear)
                    ImGui::Image(reinterpret_cast<ImTextureID>(e.srvLinear), ImVec2(24.0f, 24.0f));
                else
                    ImGui::Dummy(ImVec2(24.0f, 24.0f));
                ImGui::SameLine();

                if (ImGui::Selectable(label.c_str(), e.texture == slotPick.original))
                {
                    assignSlot(slotPick.block, slotPick.slot, slotPick.offset, slotPick.handle,
                               slotPick.setKey, slotPick.material, slotPick.original, e.texture);
                    slotPick.open = false;
                }
                if (ImGui::IsItemHovered())
                    if (const char* full = textureLabel(e))
                        ImGui::SetTooltip("%s", full);

                ImGui::PopID();
            }
            if (!shown)
                ImGui::TextDisabled("nothing matches");
            ImGui::EndChild();

            ImGui::End();

            if (!keepOpen)
                slotPick.open = false;
        }

        void renderDetail()
        {
            if (selected < 0 || selected >= int(entries.size()))
            {
                ImGui::TextDisabled("Select a texture on the left.");
                return;
            }

            const TextureEntry& e = entries[selected];
            if (usagesFor != selected)
                rebuildUsages();

            if (const char* full = textureLabel(e))
                ImGui::TextWrapped("%s", full);
            else
                ImGui::TextDisabled("(no asset path resolved)");

            if (e.texture)
                ImGui::Text("%ux%ux%u   %u mips   %s%s", e.width, e.height, e.depth,
                    e.mips, typeName(e.type), e.srgb ? "   sRGB" : "");
            else
                ImGui::TextColored(ImVec4(0.8f, 0.6f, 0.3f, 1.0f), "not resident yet");
            ImGui::TextDisabled("%s (%u)   res DXGI %u   shader DXGI %u   DxTexture %p",
                e.texture ? gen::formatName(e.texture) : "?", e.format, e.resourceFormat, e.shaderFormat, e.texture);

            ImGui::Separator();

            float side = ImGui::GetContentRegionAvail().x;
            const float budget = ImGui::GetContentRegionAvail().y * 0.55f;
            if (budget > 128.0f && budget < side)
                side = budget;
            renderPreview(e, (std::max)(side, 128.0f));

            ImGui::Separator();

            if (!e.loaded)
                ImGui::TextColored(ImVec4(0.9f, 0.7f, 0.3f, 1.0f),
                    "not currently loaded - preview is cached, editing needs it streamed in");

            if (ImGui::CollapsingHeader("Generate a new image"))
            {
                if (gen::renderUI(e.texture, exportNameFor(e).c_str()))
                {
                    if (void* s = gen::currentSrv(e.texture))
                    {
                        if (entries[selected].drawable)
                            releaseSrv(entries[selected].srvLinear);
                        addRefSrv(s);
                        entries[selected].srvLinear = s;
                        entries[selected].drawable = true;
                    }
                    else
                    {
                        void* orig = liveTexture(e.texture) ? asTex(e.texture)->m_shaderViews[0] : nullptr;
                        if (srvIsDrawable2D(orig))
                        {
                            if (entries[selected].drawable)
                                releaseSrv(entries[selected].srvLinear);
                            addRefSrv(orig);
                            entries[selected].srvLinear = orig;
                            entries[selected].drawable = true;
                        }
                    }
                }
            }

            if (e.texture && !gen::isClonedTexture(e.texture))
            {
                if (ImGui::Button("Make a copy"))
                {
                    if (void* copy = gen::cloneTexture(e.texture))
                    {
                        const int index = catalogueClone(copy, e);
                        if (index >= 0)
                            selectTextureIndex(index);
                    }
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("private duplicate; only slots pointed at it change");
                ImGui::SameLine();
                ImGui::TextDisabled("for editing one user of a shared texture");
            }
            else if (e.texture && gen::isClonedTexture(e.texture))
            {
                ImGui::TextColored(ImVec4(0.5f, 0.8f, 1.0f, 1.0f),
                    "private copy - edits here affect only the slots pointed at it");
            }

            renderStreamingControls(entries[selected]);

            ImGui::BeginDisabled(usages.empty());
            if (ImGui::Button("Replace everywhere"))
            {
                replaceSlot = -1;
                ImGui::OpenPopup("replace_texture");
            }
            ImGui::EndDisabled();
            ImGui::SameLine();
            ImGui::Text("used by %zu material slot(s)", usages.size());

            if (ImGui::CollapsingHeader("Materials using this texture", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::BeginChild("usages", ImVec2(0.0f, 0.0f), true);
                for (int u = 0; u < int(usages.size()); ++u)
                {
                    const Usage& use = usages[u];
                    const MaterialEntry& m = materials[use.material];

                    ImGui::PushID(u);
                    const char* pname = nameForHandle(use.handle);
                    char label[352];
                    std::snprintf(label, sizeof(label), "%s  [%s]##u%d",
                        m.meshName.c_str(), pname ? pname : "?", u);

                    if (ImGui::TreeNode(label))
                    {
                        if (ImGui::SmallButton("replace this slot"))
                            replaceSlot = u; // opened outside PushID
                        if (TexBackup* b = findTexBackup(m.block, use.slot))
                        {
                            ImGui::SameLine();
                            if (ImGui::SmallButton("revert slot"))
                            {
                                uint32_t h = 0;
                                uint16_t off = 0;
                                if (paramInfo(m.block, use.slot, h, off))
                                    writeTex(m.block, off, b->texture);
                                texBackups.erase(texBackups.begin() + (b - texBackups.data()));
                            }
                        }

                        if (m.vecCount)
                        {
                            ImGui::TextDisabled("vector parameters");
                            renderVectorParams(m, false, nullptr);
                        }
                        ImGui::TreePop();
                    }
                    ImGui::PopID();
                }
                ImGui::EndChild();
            }

            if (replaceSlot >= 0 && !ImGui::IsPopupOpen("replace_texture"))
                ImGui::OpenPopup("replace_texture");
            renderReplacePicker();
        }

        void renderBrowserOptions()
        {
            ImGui::SeparatorText("List");
            ImGui::Checkbox("thumbnails", &galleryThumbs);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("off = plain list, much cheaper with thousands catalogued");

            ImGui::PushItemWidth(150.0f);
            ImGui::SliderFloat("thumbnail size", &thumbSize, 48.0f, 192.0f, "%.0f");
            ImGui::SliderInt("max drawn per frame", &maxThumbs, 16, 256);
            ImGui::SliderInt("min edge", &minSize, 0, 2048);
            ImGui::PopItemWidth();

            ImGui::Checkbox("2D only", &only2D);
            ImGui::SameLine();
            ImGui::Checkbox("show unloaded", &showUnloaded);

            ImGui::Checkbox("all loaded assets", &includeAllAssets);
            ImGui::SameLine();
            ImGui::Checkbox("keep across scans", &keepUnloaded);

            ImGui::SeparatorText("Loading");
            ImGui::Checkbox("scan automatically", &autoScan);
            ImGui::Checkbox("auto-load the catalogue after a scan", &autoLoadAfterScan);

            ImGui::Checkbox("load on scroll", &autoLoadMissing);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("streams every unresolved texture you scroll past");

            ImGui::Checkbox("auto-resolve views", &autoResolveViews);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("resolve shader views in the background, a slice per frame");

            ImGui::Checkbox("keep everything resident", &keepAllLoaded);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("never evict; the on-demand budget is the only limit");
        }

        void renderStreamingOptions()
        {
            if (fb::TextureStreamingSettings* ts = streamingSettings())
            {
                const float mb = 1024.0f * 1024.0f;
                ImGui::SeparatorText("TextureStreamingSettings");
                ImGui::TextDisabled("pool %.1f MB   on-demand %.1f MB   headroom %.1f MB",
                    ts->m_PoolSize / mb, ts->m_OnDemandPoolSize / mb,
                    ts->m_PoolHeadroomSize / mb);

                int pool = int(ts->m_PoolSize / (1024u * 1024u));
                int onDemand = int(ts->m_OnDemandPoolSize / (1024u * 1024u));

                ImGui::PushItemWidth(150.0f);
                if (ImGui::SliderInt("pool MB", &pool, 16, 8192))
                    ts->m_PoolSize = uint32_t(pool) * 1024u * 1024u;
                if (ImGui::SliderInt("on-demand MB", &onDemand, 16, 4096))
                    ts->m_OnDemandPoolSize = uint32_t(onDemand) * 1024u * 1024u;
                ImGui::SliderFloat("mipmap bias", &ts->m_MipmapBias, -4.0f, 4.0f);
                ImGui::PopItemWidth();

                ImGui::Checkbox("override pool size", &ts->m_OverridePoolSize);
                ImGui::SameLine();
                ImGui::Checkbox("pool enable", &ts->m_PoolEnable);

                ImGui::Checkbox("dynamic loading", &ts->m_DynamicLoadingEnable);
                ImGui::SameLine();
                ImGui::Checkbox("force wanted", &ts->m_ForceWantedEnable);

                ImGui::Checkbox("only wanted in pool", &ts->m_OnlyWantedInPool);
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("off keeps textures resident after use");
                ImGui::SameLine();
                ImGui::Checkbox("instant unload", &ts->m_InstantUnloadingEnable);

                if (ImGui::Button("Apply settings"))
                    applyStreamingSettings();
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("SettingsManager::applySettings");
            }

            uint32_t used = 0, cap = 0;
            if (readBudget(used, cap))
            {
                const float mb = 1024.0f * 1024.0f;
                ImGui::SeparatorText("Live on-demand pool");
                ImGui::TextDisabled("%.1f / %.1f MB", used / mb, cap / mb);
                if (originalBudgetCap)
                {
                    ImGui::SameLine();
                    ImGui::TextDisabled("(stock %u MB)", originalBudgetCap / (1024u * 1024u));
                }

                ImGui::PushItemWidth(150.0f);
                ImGui::SliderInt("cap MB", &budgetTargetMb, 16, 8192);
                ImGui::PopItemWidth();
                ImGui::SameLine();
                if (ImGui::SmallButton("apply"))
                    setBudgetCap(uint32_t(budgetTargetMb) * 1024u * 1024u);

                ImGui::Checkbox("re-apply on every level load", &autoRaiseBudget);
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("re-raise the pool after each level load");

                if (originalBudgetCap)
                {
                    ImGui::SameLine();
                    if (ImGui::SmallButton("restore stock"))
                    {
                        autoRaiseBudget = false;
                        restoreBudgetCap();
                    }
                }
                ImGui::TextDisabled("this is real video memory - raising it keeps more textures\n"
                                    "resident at once, so do not go wild");
            }
        }

        void renderTextureBrowser()
        {
            const float paneWidth = (std::max)(ImGui::GetContentRegionAvail().x * 0.55f, 260.0f);

            ImGui::BeginChild("leftpane", ImVec2(paneWidth, 0.0f), false);

            ImGui::PushItemWidth(-1.0f);
            ImGui::InputTextWithHint("##search", "search textures by name...", search, sizeof(search));
            ImGui::PopItemWidth();

            if (ImGui::Button("Load all"))
                loadAllMissing();
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("queue every texture without GPU data, 16 per frame");

            ImGui::SameLine();
            if (ImGui::Button("Options"))
                ImGui::OpenPopup("tex_browser_opts");
            if (ImGui::BeginPopup("tex_browser_opts"))
            {
                renderBrowserOptions();
                ImGui::EndPopup();
            }

            ImGui::SameLine();
            if (ImGui::Button("Streaming"))
                ImGui::OpenPopup("tex_streaming_opts");
            if (ImGui::BeginPopup("tex_streaming_opts"))
            {
                renderStreamingOptions();
                ImGui::EndPopup();
            }

            size_t pinned = 0;
            {
                std::lock_guard<std::mutex> lock(pinMutex);
                pinned = pinnedHandles.size();
            }

            if (pinned)
            {
                ImGui::SameLine();
                if (ImGui::Button("Unload all"))
                    releaseAllPins();
            }

            const size_t asked = loadsRequested();
            const size_t done = loadsFinished();

            if (asked && done < asked)
            {
                ImGui::ProgressBar(float(done) / float(asked),
                    ImVec2(ImGui::GetContentRegionAvail().x - 60.0f, 0.0f));
                ImGui::SameLine();
                if (ImGui::SmallButton("stop"))
                    cancelLoads();
            }

            {
                size_t previewable = 0;
                for (const TextureEntry& e : entries)
                    if (e.drawable)
                        ++previewable;

                ImGui::TextDisabled("%zu resident   %zu previewable   of %zu",
                    residentTextures, previewable, entries.size());
            }

            if (budgetHit.load())
            {
                ImGui::TextColored(ImVec4(0.9f, 0.7f, 0.3f, 1.0f), "on-demand pool full");
                ImGui::SameLine();
                if (ImGui::SmallButton("retry"))
                    budgetHit = false;

                if (fb::TextureStreamingSettings* ts = streamingSettings())
                {
                    const uint32_t mb = ts->m_OnDemandPoolSize / (1024u * 1024u);
                    const uint32_t want = (mb < 512u ? 1024u : mb * 2u);

                    char raise[96];
                    std::snprintf(raise, sizeof(raise), "raise to %u MB##bump", want);

                    ImGui::SameLine();
                    if (ImGui::SmallButton(raise))
                    {
                        ts->m_OnDemandPoolSize = want * 1024u * 1024u;
                        applyStreamingSettings();
                        budgetHit = false;
                        logger::info("[textures] on-demand pool {} -> {} MB", mb, want);
                    }
                    if (ImGui::IsItemHovered())
                        ImGui::SetTooltip("raise the on-demand pool and continue the queued loads");
                }
            }

            renderGallery(paneWidth);
            ImGui::EndChild();

            ImGui::SameLine();
            ImGui::BeginChild("rightpane", ImVec2(0.0f, 0.0f), false);
            renderDetail();
            ImGui::EndChild();
        }

        void renderVectorParams(const MaterialEntry& m, bool colorsOnly,
                                const std::vector<uint32_t>* only)
        {
            for (uint32_t i = 0; i < m.vecCount; ++i)
            {
                uint32_t handle = 0;
                uint16_t offset = 0;
                if (!paramInfo(static_cast<const uint8_t*>(m.block), i, handle, offset))
                    continue;
                if (only && std::find(only->begin(), only->end(), handle) == only->end())
                    continue;
                if (colorsOnly && !paramIsColor(handle, nameForHandle(handle)))
                    continue;

                float value[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
                readVec(m.block, offset, value);

                const char* name = nameForHandle(handle);
                const int mode = effectiveMode(handle, name, value);

                char label[160];
                if (name)
                    std::snprintf(label, sizeof(label), "%s##v%u", name, i);
                else
                    std::snprintf(label, sizeof(label), "0x%08X##v%u", handle, i);

                ImGui::PushID(int(i));

                const char* tag = (mode == ParamColor) ? "rgba" : "xyzw";
                if (ImGui::SmallButton(tag))
                {
                    int& cur = paramMode[handle];
                    cur = (mode == ParamColor) ? ParamNumeric : ParamColor;
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("showing as %s - click to switch",
                        mode == ParamColor ? "a color" : "four floats");
                ImGui::SameLine();

                ImGui::PushItemWidth(-90.0f);
                const bool changed = (mode == ParamColor)
                    ? ImGui::ColorEdit4(label, value, ImGuiColorEditFlags_Float
                        | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_AlphaBar)
                    : ImGui::DragFloat4(label, value, 0.005f);
                ImGui::PopItemWidth();

                if (changed)
                {
                    MaterialEntry target = m;
                    target.block = redirectEdit(m.block);

                    if (!findVecBackup(target.block, i))
                    {
                        float original[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
                        readVec(target.block, offset, original);
                        vecBackups.push_back({ target.block, i,
                            { original[0], original[1], original[2], original[3] } });
                    }
                    writeVec(target.block, offset, value);
                    holdVecOverride(target, handle, value);
                    mirrorWrite(target.block, handle, false, value, nullptr);
                }

                if (mode == ParamColor)
                    ImGui::TextDisabled("      %.3f %.3f %.3f %.3f",
                        value[0], value[1], value[2], value[3]);

                if (const ParamOverride* o = findOverride(m.setKey, m.index, handle, m.block); o && o->added)
                {
                    ImGui::SameLine();
                    if (ImGui::SmallButton("remove"))
                        removeAddedParam(m, handle);
                    ImGui::SameLine();
                    ImGui::TextDisabled("added");
                }
                else if (VecBackup* b = findVecBackup(m.block, i))
                {
                    ImGui::SameLine();
                    if (ImGui::SmallButton("revert"))
                    {
                        dropOverride(m.setKey, m.index, handle, m.block);
                        writeVec(m.block, offset, b->value);
                        vecBackups.erase(vecBackups.begin() + (b - vecBackups.data()));
                    }
                    if (holdEdits)
                    {
                        ImGui::SameLine();
                        ImGui::TextColored(ImVec4(0.5f, 0.8f, 1.0f, 1.0f), "held");
                        if (ImGui::IsItemHovered())
                            ImGui::SetTooltip("re-written every frame: the game drives this parameter");
                    }
                }
                ImGui::PopID();
            }

            if (!colorsOnly && !only && m.setKey && m.set)
            {
                static char addName[64] = "";
                static float addValue[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
                ImGui::PushID("addparam");

                bool anyMissing = false;
                const std::vector<ShaderParam>& reads = shaderParamNames(m);
                if (reads.empty() && !m.block)
                    ImGui::TextDisabled("shader programs not resolved yet - it has to be drawn once");
                for (const ShaderParam& p : reads)
                {
                    uint16_t off = 0;
                    if (slotForHandle(m, p.handle, false, off))
                        continue;
                    if (!anyMissing)
                    {
                        ImGui::TextDisabled("shader reads:");
                        anyMissing = true;
                    }
                    ImGui::SameLine();
                    char btn[96];
                    std::snprintf(btn, sizeof(btn), "+ %s##%s", p.name.c_str(), p.name.c_str());
                    float drawValue[4] = { 1.0f, p.bytes > 4 ? 1.0f : 0.0f, p.bytes > 4 ? 1.0f : 0.0f, p.bytes > 4 ? 1.0f : 0.0f };
                    const bool fromDraw = pickedConstant(m, p, drawValue);
                    if (ImGui::SmallButton(btn))
                        addVectorParamHandle(m, p.handle, drawValue);
                    if (ImGui::IsItemHovered())
                    {
                        if (fromDraw)
                            ImGui::SetTooltip("%s constant, %u bytes\ncurrent draw value %.3f %.3f %.3f %.3f",
                                p.vertexOnly ? "vertex shader" : "pixel shader", p.bytes,
                                drawValue[0], drawValue[1], drawValue[2], drawValue[3]);
                        else
                            ImGui::SetTooltip("%s constant, %u bytes%s", p.vertexOnly ? "vertex shader" : "pixel shader",
                                p.bytes, p.vertexOnly ? " (vertex only)" : "");
                    }
                }
                ImGui::SetNextItemWidth(160.0f);
                ImGui::InputTextWithHint("##name", "parameter name", addName, sizeof(addName));
                ImGui::SameLine();
                ImGui::SetNextItemWidth(220.0f);
                ImGui::DragFloat4("##value", addValue, 0.005f);
                ImGui::SameLine();
                if (ImGui::SmallButton("add") && addName[0])
                {
                    if (!addVectorParam(m, addName, addValue))
                        logger::warning("[textures] could not add {} to {}", addName, m.label);
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("adds a slot the material never declared (e.g. Lasercolor on the red laser)");
                ImGui::PopID();
            }
        }

    }

    void renderMaterialBrowser(MaterialFilter& filter)
    {
        ImGui::PushID(&filter);
        struct IdScope { ~IdScope() { ImGui::PopID(); } } idScope;

        ImGui::PushItemWidth(-1.0f);
        ImGui::InputTextWithHint("##matsearch", "search by mesh or variation name...",
                                 filter.search, sizeof(filter.search));
        ImGui::PopItemWidth();

        ImGui::Checkbox("color parameters only", &filter.colorOnly);
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("when browsing without a search: only materials with a color parameter");
        ImGui::SameLine();
        ImGui::PushItemWidth(110.0f);
        ImGui::SliderInt("max shown", &filter.limit, 20, 1000);
        ImGui::PopItemWidth();
        ImGui::SameLine();
        ImGui::TextDisabled("%zu catalogued", materials.size());

        std::string needle = filter.search;
        std::transform(needle.begin(), needle.end(), needle.begin(),
            [](unsigned char c) { return char(std::tolower(c)); });

        ImGui::BeginChild("matlist", ImVec2(0.0f, 0.0f), true);

        if (needle != filter.cachedNeedle || filter.colorOnly != filter.cachedColorOnly ||
            catalogGeneration != filter.cachedGeneration)
        {
            filter.cachedNeedle = needle;
            filter.cachedColorOnly = filter.colorOnly;
            filter.cachedGeneration = catalogGeneration;
            filter.matches.clear();
            for (int i = 0; i < int(materials.size()); ++i)
            {
                const MaterialEntry& m = materials[i];
                if (!needle.empty())
                {
                    if (m.searchKey.find(needle) == std::string::npos)
                        continue;
                }
                else if (filter.colorOnly && !m.hasColor)
                    continue;
                filter.matches.push_back(i);
            }
        }

        const int matched = int(filter.matches.size());
        int shown = 0;
        for (int i : filter.matches)
        {
            if (shown >= filter.limit)
                break;
            ++shown;
            const MaterialEntry& m = materials[i];
            ImGui::PushID(i);
            if (ImGui::TreeNode(m.label.c_str()))
            {
                if (!m.block)
                    ImGui::TextDisabled("no parameters declared - no block");
                renderVectorParams(m, false, nullptr);
                renderMaterialTextureSlots(m);
                ImGui::TreePop();
            }
            ImGui::PopID();
        }

        if (!matched)
            ImGui::TextDisabled(materials.empty()
                ? "no materials catalogued yet - open the Textures tab and press Scan"
                : "nothing matches the current filter");
        else if (matched > shown)
            ImGui::TextDisabled("%d more match - narrow the search or raise the limit",
                matched - shown);

        ImGui::EndChild();
    }

    void renderSkyTextureSlots()
    {
        renderSkyResourcesSection();
    }

    void renderTab()
    {
        if (ImGui::Button("Scan"))
        {
            scan();
            g_lastScanElements = stats.elementCount;
            usagesFor = -1;
        }
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Re-catalogue every material and texture the level has realized.");

        ImGui::SameLine();
        ImGui::BeginDisabled(vecBackups.empty() && texBackups.empty() &&
                             gen::overrideCount() == 0);
        if (ImGui::Button("Revert all"))
            revertAll();
        ImGui::EndDisabled();

        ImGui::SameLine();
        if (ImGui::Button("Clear"))
            clear();

        ImGui::SameLine();
        if (ImGui::Button("Tools"))
            ImGui::OpenPopup("tex_tools");
        if (ImGui::BeginPopup("tex_tools"))
        {
            ImGui::BeginDisabled(gen::batchActive());
            if (ImGui::MenuItem("Export all as DDS"))
                exportAllLoaded(true);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("every resident texture, engine format and mips");
            if (ImGui::MenuItem("Export all as PNG"))
                exportAllLoaded(false);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Same, decoded to RGBA8 - slower and larger, opens anywhere.");
            ImGui::EndDisabled();
            ImGui::TextDisabled("to %s", gen::batchFolder().c_str());

            ImGui::EndPopup();
        }

        ImGui::SameLine();
        ImGui::Checkbox("hold edits", &holdEdits);
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("re-write edits every frame; off shows the game's own value");

        if (gen::batchActive())
        {
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(0.5f, 0.8f, 1.0f, 1.0f), "exporting %zu/%zu",
                gen::batchDone(), gen::batchTotal());
            ImGui::SameLine();
            if (ImGui::SmallButton("cancel"))
                gen::cancelBatch();
        }

        {
            std::string status;
            char buf[160];

            if (stats.sets)
            {
                std::snprintf(buf, sizeof(buf), "%u textures, %u materials",
                    stats.texUnique, uint32_t(materials.size()));
                status = buf;
            }
            if (const size_t edits = vecBackups.size() + texBackups.size())
            {
                std::snprintf(buf, sizeof(buf), "%zu edits live", edits);
                if (!status.empty()) status += "   ";
                status += buf;
            }
            if (!paramOverrides.empty())
            {
                std::snprintf(buf, sizeof(buf), "%zu held", paramOverrides.size());
                if (!status.empty()) status += "   ";
                status += buf;
            }
            if (!gen::batchActive() && (gen::batchDone() || gen::batchFailed()))
            {
                std::snprintf(buf, sizeof(buf), "last export: %zu written, %zu failed",
                    gen::batchDone(), gen::batchFailed());
                if (!status.empty()) status += "   ";
                status += buf;
            }

            if (!status.empty())
                ImGui::TextDisabled("%s", status.c_str());
        }

        ImGui::Separator();

        if (ImGui::BeginTabBar("texture_views"))
        {
            const bool toMeshes = focusMeshTab;
            const ImGuiTabItemFlags focus = (focusTab && !toMeshes) ? ImGuiTabItemFlags_SetSelected : 0;
            focusTab = false;
            focusMeshTab = false;

            if (ImGui::BeginTabItem("Textures", nullptr, focus))
            {
                renderTextureBrowser();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Materials"))
            {
                renderMaterialBrowser(materialFilter);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Meshes", nullptr, toMeshes ? ImGuiTabItemFlags_SetSelected : 0))
            {
                editor::lights::renderMeshesPanel();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Shaders"))
            {
                renderShadersTab();
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
    }

    int renderMeshMaterials(const std::vector<MeshVariationRef>& meshes)
    {
        if (meshes.empty())
        {
            ImGui::TextDisabled("no mesh found beside this light in its prefab");
            return 0;
        }

        if (materials.empty())
        {
            ImGui::TextDisabled("no materials catalogued yet - press Scan materials");
            return 0;
        }

        struct MatchCache
        {
            const void* meshes = nullptr;
            size_t meshCount = 0;
            size_t materialCount = 0;
            uint32_t generation = 0;
            std::vector<int> exact;
            std::vector<int> other;

            uint32_t byKey = 0, byMesh = 0, byName = 0;
        };
        static MatchCache cache;

        const bool cacheValid =
            cache.meshes == static_cast<const void*>(meshes.data()) &&
            cache.meshCount == meshes.size() &&
            cache.materialCount == materials.size() &&
            cache.generation == catalogGeneration;

        std::vector<int>& exact = cache.exact;
        std::vector<int>& other = cache.other;

        if (!cacheValid)
        {
            cache.meshes = meshes.data();
            cache.meshCount = meshes.size();
            cache.materialCount = materials.size();
            cache.generation = catalogGeneration;
            exact.clear();
            other.clear();
            cache.byKey = cache.byMesh = cache.byName = 0;

            for (const MeshVariationRef& want : meshes)
            {
                std::string lower = want.name;
                std::transform(lower.begin(), lower.end(), lower.begin(),
                    [](unsigned char c) { return char(std::tolower(c)); });

                for (int i = 0; i < int(materials.size()); ++i)
                {
                    const MaterialEntry& m = materials[i];

                    const bool isExact = want.key != 0 && m.setKey == want.key;
                    const bool isSameMesh = !isExact && (want.key >> 32) != 0 &&
                                            (m.setKey >> 32) == (want.key >> 32);
                    const bool isNamed = !isExact && !isSameMesh &&
                                         !lower.empty() && m.lowerName == lower;
                    if (!isExact && !isSameMesh && !isNamed)
                        continue;

                    if (isExact) ++cache.byKey;
                    else if (isSameMesh) ++cache.byMesh;
                    else ++cache.byName;

                    std::vector<int>& bucket = isExact ? exact : other;
                    if (std::find(bucket.begin(), bucket.end(), i) != bucket.end())
                        continue;

                    bucket.push_back(i);
                }
            }
        }

        const bool haveExact = !exact.empty();

        bool exactIsRealVariation = false;
        for (const MeshVariationRef& want : meshes)
            if (uint32_t(want.key & 0xFFFFFFFFull) != 0) { exactIsRealVariation = true; break; }

        const bool exactOnly = lampThisVariationOnly && haveExact && exactIsRealVariation;

        std::vector<int> combined;
        if (!exactOnly)
        {
            combined = exact;
            combined.insert(combined.end(), other.begin(), other.end());
        }
        const std::vector<int>& matched = exactOnly ? exact : combined;

        std::vector<int> filtered;
        filtered.reserve(matched.size());
        for (int i : matched)
        {
            const MaterialEntry& m = materials[i];
            if (lampOverridesOnly && !m.variationOverride)
                continue;
            if (lampColorOnly && !materialHasColor(m))
                continue;
            filtered.push_back(i);
        }

        const bool relaxed = filtered.empty() && !matched.empty();
        const std::vector<int>& list = relaxed ? matched : filtered;

        if (relaxed)
            ImGui::TextColored(ImVec4(0.9f, 0.7f, 0.3f, 1.0f),
                "the filters below match nothing here - showing all %zu material(s)",
                matched.size());

        ImGui::Checkbox("what the variation changes", &lampOverridesOnly);
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("only the variation's overrides, the lit parts");
        ImGui::SameLine();
        ImGui::Checkbox("colors only", &lampColorOnly);
        ImGui::SameLine();
        ImGui::Checkbox("this variation only", &lampThisVariationOnly);
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("off: every variation of the mesh");
        ImGui::SameLine();
        ImGui::TextDisabled("%zu shown", list.size());

        if (list.size() < matched.size())
        {
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(0.8f, 0.7f, 0.3f, 1.0f),
                "(%zu more matched, hidden by the filters below)",
                matched.size() - list.size());
        }
        if (exactOnly && !other.empty())
        {
            ImGui::SameLine();
            ImGui::TextDisabled("(%zu hidden in other variations)", other.size());
        }
        if (!haveExact && !other.empty())
            ImGui::TextColored(ImVec4(0.8f, 0.7f, 0.3f, 1.0f),
                "this light's variation is not realized - showing the mesh's other variations");

        ImGui::Separator();

        for (size_t n = 0; n < list.size(); ++n)
        {
            const MaterialEntry& m = materials[list[n]];
            const bool isExact = std::find(exact.begin(), exact.end(), list[n]) != exact.end();

            char variation[128];
            if (!m.variationName.empty())
                std::snprintf(variation, sizeof(variation), "%s", m.variationName.c_str());
            else if (uint32_t(m.setKey & 0xFFFFFFFFull) == 0)
                std::snprintf(variation, sizeof(variation), "(base material - no variation)");
            else
                std::snprintf(variation, sizeof(variation), "variation %08X",
                    uint32_t(m.setKey & 0xFFFFFFFFull));

            const char* shaderLeaf = "";
            if (!m.shaderName.empty())
            {
                const size_t slash = m.shaderName.find_last_of('/');
                shaderLeaf = slash == std::string::npos ? m.shaderName.c_str() : m.shaderName.c_str() + slash + 1;
            }

            char label[512];
            std::snprintf(label, sizeof(label), "%s   %s%s%s%s##m%d",
                m.meshName.c_str(), variation,
                isExact ? "" : "   (other variation)",
                shaderLeaf[0] ? "   shader: " : "", shaderLeaf, list[n]);

            ImGui::PushID(list[n]);
            if (ImGui::TreeNodeEx(label, n == 0 ? ImGuiTreeNodeFlags_DefaultOpen : 0))
            {
                const bool narrowed = lampOverridesOnly && !m.variationHandles.empty();
                renderVectorParams(m, lampColorOnly,
                                   narrowed ? &m.variationHandles : nullptr);

                if ((narrowed || lampColorOnly) && ImGui::TreeNode("everything else"))
                {
                    renderVectorParams(m, false, nullptr);
                    renderMaterialTextureSlots(m);
                    ImGui::TreePop();
                }
                else if (!narrowed && !lampColorOnly)
                {
                    renderMaterialTextureSlots(m);
                }
                renderShaderNamedTextures(m);
                ImGui::TreePop();
            }
            ImGui::PopID();
        }

        if (list.empty() && lampOverridesOnly)
            ImGui::TextDisabled("this light's variation overrides nothing on these meshes - "
                                "untick \"what the variation changes\" to see the rest");
        else if (list.empty())
            ImGui::TextDisabled(lampColorOnly
                ? "no material here declares a color - untick \"colors only\" for the rest"
                : "mesh found, but none of its materials are realized - it may not be "
                  "streamed in, or the catalogue is stale");

        return int(list.size());
    }

    bool renderAssetTextureRow(void* textureAsset, float thumb)
    {
        if (!textureAsset)
            return false;

        int index = -1;
        for (int i = 0; i < int(entries.size()); ++i)
        {
            if (entries[i].asset == textureAsset)
            {
                index = i;
                break;
            }
        }
        if (index < 0)
            return false;

        const TextureEntry& e = entries[index];

        if (e.drawable && e.srvLinear)
        {
            ImGui::Image(reinterpret_cast<ImTextureID>(e.srvLinear), ImVec2(thumb, thumb));
            ImGui::SameLine();
        }

        ImGui::BeginGroup();
        if (const char* full = textureLabel(e))
            ImGui::TextWrapped("%s", full);
        else
            ImGui::TextDisabled("(no asset path resolved)");

        if (e.texture)
            ImGui::TextDisabled("%ux%u  %u mips%s", e.width, e.height, e.mips,
                e.srgb ? "  sRGB" : "");
        else
            ImGui::TextColored(ImVec4(0.8f, 0.6f, 0.3f, 1.0f), "not resident yet");

        if (ImGui::SmallButton("edit in Textures tab"))
            selectTextureIndex(index);
        ImGui::EndGroup();
        return true;
    }

    void renderBlockParams(void* block, bool colorsOnly)
    {
        uint8_t vec = 0, tex = 0, bol = 0;
        if (!looksLikeParamBlock(block, vec, tex, bol))
        {
            ImGui::TextDisabled("not a parameter block any more - rescan");
            return;
        }

        MaterialEntry m{};
        m.block = block;
        m.vecCount = vec;
        m.texCount = tex;
        m.boolCount = bol;

        renderVectorParams(m, colorsOnly, nullptr);

        renderMaterialTextureSlots(m);
    }

    void renderLampTextureRow(TextureEntry& te, const char* label, const std::string& path)
    {
        ImGui::PushID(&te);

        ensureDrawable(te);

        if (te.drawable && te.srvLinear)
            ImGui::Image(reinterpret_cast<ImTextureID>(te.srvLinear), ImVec2(48.0f, 48.0f));
        else
            ImGui::Dummy(ImVec2(48.0f, 48.0f));

        ImGui::SameLine();
        ImGui::BeginGroup();

        ImGui::TextUnformatted(label && label[0] ? label : "texture");
        ImGui::TextDisabled("%s", path.c_str());

        const bool open = (g_lampEditTex == te.texture);
        if (ImGui::SmallButton(open ? "close" : "edit"))
            g_lampEditTex = open ? nullptr : te.texture;

        if (gen::hasOverride(te.texture))
        {
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(0.4f, 0.9f, 0.4f, 1.0f), "edited");
            ImGui::SameLine();
            if (ImGui::SmallButton("revert"))
                gen::revert(te.texture);
        }

        ImGui::EndGroup();

        if (open)
        {
            ImGui::Indent();
            if (gen::renderUI(te.texture, exportNameFor(te).c_str()))
            {
                if (void* s = gen::currentSrv(te.texture))
                {
                    releaseSrv(te.srvLinear);
                    addRefSrv(s);
                    te.srvLinear = s;
                    te.drawable = true;
                    te.ownsSrv = false;
                }
            }
            ImGui::Unindent();
        }

        ImGui::Separator();
        ImGui::PopID();
    }

    bool renderTextureRowByPath(const char* path, const char* label)
    {
        if (!path || !*path)
            return false;

        void* const tex = textureByPath(path);
        TextureEntry* te = tex ? const_cast<TextureEntry*>(findTextureEntry(tex)) : nullptr;
        if (!te)
            return false;

        renderLampTextureRow(*te, label, path);
        return true;
    }

    bool renderAssetTexturePicker(void** slot, const char* id)
    {
        if (!slot)
            return false;

        static char search[128] = {};
        static void** openFor = nullptr;

        char label[64];
        std::snprintf(label, sizeof(label), "replace##%s", id ? id : "gobo");
        if (ImGui::SmallButton(label))
        {
            openFor = slot;
            search[0] = '\0';
            if (textureAssets.empty())
                harvestTextureAssets();
        }
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("pointer on the light's own data; edited textures work too");

        if (openFor != slot)
            return false;

        bool changed = false;

        ImGui::SetNextWindowSize(ImVec2(460.0f, 480.0f), ImGuiCond_Appearing);
        bool keepOpen = true;
        if (ImGui::Begin("Projected texture", &keepOpen))
        {
            ImGui::SetNextItemWidth(-1.0f);
            ImGui::InputTextWithHint("##gobosearch", "search", search, sizeof(search));

            if (ImGui::SmallButton("none (no projection)"))
            {
                *slot = nullptr;
                changed = true;
                openFor = nullptr;
            }

            ImGui::Separator();
            ImGui::BeginChild("gobolist", ImVec2(0.0f, 0.0f));

            int shown = 0;
            for (const auto& [name, asset] : textureAssets)
            {
                if (shown >= 400)
                    break;
                if (!matchesSearch(name, search))
                    continue;

                ++shown;
                ImGui::PushID(asset);
                if (ImGui::Selectable(name.c_str(), *slot == asset))
                {
                    *slot = asset;
                    changed = true;
                    openFor = nullptr;
                }
                ImGui::PopID();
            }

            if (!shown)
                ImGui::TextDisabled("nothing matches");

            ImGui::EndChild();
        }
        ImGui::End();

        if (!keepOpen)
            openFor = nullptr;

        return changed;
    }

}
