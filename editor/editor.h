#pragma once

#include <vector>
#include <unordered_map>
#include <map>
#include <mutex>
#include <filesystem>

#include <imgui.h>
#include <nlohmann/json.hpp>
#include <magic_enum/magic_enum.hpp>

#include "types.h"

using json = nlohmann::json;

namespace fb
{
    class VisualEnvironment;
    struct VisualEnvironmentState;
    class VisualEnvironmentManager;
    class WorldRenderSettings;
    class LocalLightEntity;
    class ResourceManager;
    class SpatialPrefabBlueprint;
    class ObjectBlueprint;
    class PrefabBlueprint;
    class WorldPartData;
    class SubWorldData;
    class LogicPrefabBlueprint;
}

namespace MessageIds
{
    constexpr uint32_t RoundUnloaded_Category = 0xA23A0E7C;
    constexpr uint32_t RoundUnloaded_Type = 0xADFF435D;
    constexpr uint32_t LevelLoaded_Category = 0xA23A0E7C;
    constexpr uint32_t LevelLoaded_Type = 0xDE718F06;
}

class VisualEnvironmentEditor
{
public:
    static VisualEnvironmentEditor& Instance()
    {
        static VisualEnvironmentEditor env;
        return env;
    }

    VisualEnvironmentEditor(const VisualEnvironmentEditor&) = delete;
    VisualEnvironmentEditor& operator=(const VisualEnvironmentEditor&) = delete;

#pragma region interfaces
    void Render();
    void OnManagerUpdateBegin(fb::VisualEnvironmentManager* manager);
    void OnManagerUpdateEnd(fb::VisualEnvironmentManager* manager);
    void OnVisualEnvironmentUpdated(fb::VisualEnvironment* ve);
    void OnMessage(uint32_t category, uint32_t type);
    void OnVisualEnvironmentEntityCreated(fb::VisualEnvironmentEntity* entity, fb::VisualEnvironmentEntityData* data);
    void OnVisualEnvironmentEntityDestroyed(fb::VisualEnvironmentEntity* entity);


    void OnLightEntityCreated(fb::LocalLightEntity* entity, fb::LocalLightEntityData* data);
    void OnLightEntityDestroyed(fb::LocalLightEntity* entity);
    void ScanExistingLightEntities();
    void ScanAndApplyLightOverrides();
#pragma endregion

    bool& IsEnabled() { return m_Enabled; }
    void SetEnabled(bool enabled) { m_Enabled = enabled; }

    StateHash ComputeStateHash(fb::VisualEnvironmentState* state);
    StateEditEntry* FindEditEntry(const StateHash& hash);
    StateEditEntry* FindEditEntryByPriorityAndMask(int priority, uint32_t componentMask);
    void AddOrUpdateEditEntry(fb::VisualEnvironmentState* state, const StateEditData& data);
    void RemoveEditEntry(const StateHash& hash);

    void ScanEmitters();
    void OnEmitterCreated(fb::EmitterTemplate* emitter, fb::EmitterTemplateData* data);
private:
    VisualEnvironmentEditor();
    ~VisualEnvironmentEditor() = default;

    enum class EditorState
    {
        Idle,
        Loading,
        Ready,
        Active,
        Unloading
    };

    EditorState m_EditorState = EditorState::Idle;
    bool m_LevelUnloadingSignaled = false;
    bool m_SafeToOperate = false;
    bool m_Enabled = true;
    bool m_OverridesEnabled = true;

    void SetEditorState(EditorState newState);
    bool IsSafeToOperate() const;
    void OnLevelLoaded();
    void OnLevelUnloading();
    void TryRecoverFromUnloading(fb::VisualEnvironmentManager* manager);

    std::map<fb::VisualEnvironmentState*, StateEditData> m_StateDataMap;
    std::unordered_map<fb::VisualEnvironmentState*, StateEntityInfo> m_StateEntityMap;
    std::vector<fb::VisualEnvironmentState*> m_StateOrder;
    int m_SelectedStateIndex = 0;
    size_t m_LastKnownStateCount = 0;
    bool m_HasCapturedOriginals = false;
    std::string m_CapturedMapName;
    std::vector<StateEditEntry> m_EditList;

    void SyncStateList(fb::VisualEnvironmentManager* manager);
    void CaptureState(fb::VisualEnvironmentState* state);
    void CaptureStateData(fb::VisualEnvironmentState* state, StateEditData& data);
    void ApplyStateEdits(fb::VisualEnvironmentState* state, const StateEditData& data);
    void RestoreState(fb::VisualEnvironmentState* state, const StateEditData& data);
    void RestoreAllStates();
    void ResetStateToOriginal(StateEditData& data);
    void OnStateRemoved(fb::VisualEnvironmentState* state);
    void InvalidateCapture();

    bool HasCapturedState(fb::VisualEnvironmentState* state) const;
    void SortStatesByPriority();
    void ApplyStateOverride(fb::VisualEnvironmentState* state, StateEditData& data);
    void SyncEditList();
    void RestoreEditValuesFromEntry(StateEditData& dst, const StateEditData& src);
    void CopyEditDataDirectly(StateEditData& dst, const StateEditData& src);

    VisualEnvironmentGlobalData m_GlobalVEData;

    void CaptureGlobalVEData(fb::VisualEnvironment* ve);
    void ApplyGlobalVEEdits(fb::VisualEnvironment* ve);
    void ResetGlobalVEToOriginal();
    void ResetGlobalVEComponent(int componentIndex);

    fb::WorldRenderSettings m_OriginalWorldRenderSettings{};
    fb::WorldRenderSettings m_EditWorldRenderSettings{};
    bool m_HasCapturedWorldRenderSettings = false;
    bool m_WorldRenderOverrideEnabled = false;

    void CaptureWorldRenderSettings();
    void RestoreWorldRenderSettings();
    void ApplyWorldRenderSettings();

    std::mutex m_LightDataMutex;
    std::unordered_map<fb::LocalLightEntityData*, LightDataEntry> m_LightDataEntries;
    bool m_LightDataScanned = false;
    char m_LightFilterBuffer[256] = "";
    int m_LightTypeFilter = 0;

    void ScanAllLightData();
    void RegisterLightData(fb::LocalLightEntityData* data, const std::string& assetName, const std::string& containerType, void* container);

    template<typename ContainerType>
    void ProcessLightContainer(ContainerType* container, const char* containerTypeName);
    void ProcessObjectBlueprint(fb::ObjectBlueprint* objBp);
    void CaptureOriginalLightData(LightDataEntry& entry);
    void ApplyLightOverride(LightDataEntry& entry);
    void ApplyLightOverrideToData(fb::LocalLightEntityData* data, const LightDataEntry& entry);
    void ClearLightData();

    std::unordered_map<fb::VisualEnvironmentEntityData*, std::string> m_VEDataNameMap;
    void ScanVEDataFromResourceManager();
    bool m_VEDataScanned = false;

    bool IsPlayerAlive() const;
    std::string GetCurrentMapName() const;
    fb::WorldRenderSettings* GetWorldRenderSettings() const;
    fb::VisualEnvironmentManager* GetManager() const;
    std::string GetConfigDir() const;
    std::string GetConfigPath(const std::string& mapName) const;
    std::string SanitizeMapName(const std::string& mapName) const;
    std::vector<std::string> GetAvailableConfigs() const;
    std::string GetStateName(fb::VisualEnvironmentState* state, int index) const;

#define DECLARE_COPY_FUNC(name) \
        void Copy##name(fb::name##ComponentData* dst, const fb::name##ComponentData* src);

    DECLARE_COPY_FUNC(OutdoorLight);
    DECLARE_COPY_FUNC(Enlighten);
    DECLARE_COPY_FUNC(Tonemap);
    DECLARE_COPY_FUNC(ColorCorrection);
    DECLARE_COPY_FUNC(Sky);
    DECLARE_COPY_FUNC(Fog);
    DECLARE_COPY_FUNC(Wind);
    DECLARE_COPY_FUNC(SunFlare);
    DECLARE_COPY_FUNC(DynamicAO);
    DECLARE_COPY_FUNC(Dof);
    DECLARE_COPY_FUNC(Vignette);
    DECLARE_COPY_FUNC(FilmGrain);
    DECLARE_COPY_FUNC(LensScope);
    DECLARE_COPY_FUNC(CameraParams);
    DECLARE_COPY_FUNC(ScreenEffect);
    DECLARE_COPY_FUNC(DamageEffect);
    DECLARE_COPY_FUNC(PlanarReflection);
    DECLARE_COPY_FUNC(DynamicEnvmap);
    DECLARE_COPY_FUNC(CharacterLighting);
    DECLARE_COPY_FUNC(MotionBlur);

#undef DECLARE_COPY_FUNC

    void CopyWorldRenderSettings(fb::WorldRenderSettings* dst, const fb::WorldRenderSettings* src);

    bool m_AutoLoadConfigs = true;
    char m_ConfigNameBuffer[256] = {};
    const char* m_ConfigDirectory = "VisEnvEditor";

    bool SaveConfig(const std::filesystem::path& filePath);
    bool LoadConfig(const std::filesystem::path& filePath);
    bool SaveConfigForCurrentMap();
    bool LoadConfigForCurrentMap();

    json SerializeVec2(const fb::Vec2& v) const;
    json SerializeVec3(const fb::Vec3& v) const;
    json SerializeVec4(const fb::Vec4& v) const;
    void DeserializeVec2(const json& j, fb::Vec2& v) const;
    void DeserializeVec3(const json& j, fb::Vec3& v) const;
    void DeserializeVec4(const json& j, fb::Vec4& v) const;

    json SerializeStateData(const StateEditData& data) const;
    void DeserializeStateData(const json& j, StateEditData& data);
    json SerializeLightData(const LightDataEntry& entry) const;
    void DeserializeLightData(const json& j, LightDataEntry& entry);

#define DECLARE_SERIAL_FUNCS(name, type) \
        json Serialize##name(const type& c) const; \
        void Deserialize##name(const json& j, type& c);

    DECLARE_SERIAL_FUNCS(OutdoorLight, fb::OutdoorLightComponentData);
    DECLARE_SERIAL_FUNCS(Enlighten, fb::EnlightenComponentData);
    DECLARE_SERIAL_FUNCS(Tonemap, fb::TonemapComponentData);
    DECLARE_SERIAL_FUNCS(ColorCorrection, fb::ColorCorrectionComponentData);
    DECLARE_SERIAL_FUNCS(Sky, fb::SkyComponentData);
    DECLARE_SERIAL_FUNCS(Fog, fb::FogComponentData);
    DECLARE_SERIAL_FUNCS(Wind, fb::WindComponentData);
    DECLARE_SERIAL_FUNCS(Sunflare, fb::SunFlareComponentData);
    DECLARE_SERIAL_FUNCS(DynamicAO, fb::DynamicAOComponentData);
    DECLARE_SERIAL_FUNCS(Dof, fb::DofComponentData);
    DECLARE_SERIAL_FUNCS(Vignette, fb::VignetteComponentData);
    DECLARE_SERIAL_FUNCS(FilmGrain, fb::FilmGrainComponentData);
    DECLARE_SERIAL_FUNCS(LensScope, fb::LensScopeComponentData);
    DECLARE_SERIAL_FUNCS(CameraParams, fb::CameraParamsComponentData);
    DECLARE_SERIAL_FUNCS(ScreenEffect, fb::ScreenEffectComponentData);
    DECLARE_SERIAL_FUNCS(DamageEffect, fb::DamageEffectComponentData);
    DECLARE_SERIAL_FUNCS(PlanarReflection, fb::PlanarReflectionComponentData);
    DECLARE_SERIAL_FUNCS(DynamicEnvmap, fb::DynamicEnvmapComponentData);
    DECLARE_SERIAL_FUNCS(CharacterLighting, fb::CharacterLightingComponentData);
    DECLARE_SERIAL_FUNCS(MotionBlur, fb::MotionBlurComponentData);
    DECLARE_SERIAL_FUNCS(WorldRenderSettings, fb::WorldRenderSettings);

#undef DECLARE_SERIAL_FUNCS

    bool m_ShowOriginalValues = true;
    bool m_HighlightModified = true;
    bool m_ShowConsole = false;
    ImVec4 m_ModifiedColor = ImVec4{ 1.0f, 0.8f, 0.2f, 1.0f };
    ImVec4 m_OriginalColor = ImVec4{ 0.5f, 0.8f, 1.0f, 1.0f };

    std::unordered_map<fb::EmitterTemplateData*, EmitterEditData> m_EmitterMap;
    fb::EmitterTemplateData* m_SelectedEmitter = nullptr;
    EmitterTreeNode m_EmitterTree;
    char m_EmitterSearchBuf[64] = {};
    bool m_EmittersScanned = false;
    void InsertIntoTree(fb::EmitterTemplateData* data, const std::string& fullPath);

    void RenderEmitterTab();
    void RenderEmitterTreeNode(EmitterTreeNode& node);
    void RenderEmitterSelectable(fb::EmitterTemplateData* data, EmitterEditData& edit);
    void RenderEmitterProperties();
    std::pair<size_t, size_t> CountEmittersInNode(const EmitterTreeNode& node);
    void RenderColorProcessor(EmitterEditData& edit);
    bool NodeHasMatch(const EmitterTreeNode& node, const char* search);

    void RenderMenuBar();
    void RenderStatusBar();
    void RenderStateSelector();
    void RenderStateEditor(fb::VisualEnvironmentState* state, StateEditData& data);
    void RenderComponentEditors(StateEditData& data);
    void RenderConfigSection();
    void RenderGlobalVEEditor();
    void RenderLightsTab();
    void RenderLightDataEditor(LightDataEntry& entry);

#define DECLARE_RENDER_FUNC(name, type) \
        void Render##name##Component(type* edit, const type* orig);

    DECLARE_RENDER_FUNC(OutdoorLight, fb::OutdoorLightComponentData);
    DECLARE_RENDER_FUNC(Enlighten, fb::EnlightenComponentData);
    DECLARE_RENDER_FUNC(Tonemap, fb::TonemapComponentData);
    DECLARE_RENDER_FUNC(ColorCorrection, fb::ColorCorrectionComponentData);
    DECLARE_RENDER_FUNC(Sky, fb::SkyComponentData);
    DECLARE_RENDER_FUNC(Fog, fb::FogComponentData);
    DECLARE_RENDER_FUNC(Wind, fb::WindComponentData);
    DECLARE_RENDER_FUNC(SunFlare, fb::SunFlareComponentData);
    DECLARE_RENDER_FUNC(DynamicAO, fb::DynamicAOComponentData);
    DECLARE_RENDER_FUNC(Dof, fb::DofComponentData);
    DECLARE_RENDER_FUNC(Vignette, fb::VignetteComponentData);
    DECLARE_RENDER_FUNC(FilmGrain, fb::FilmGrainComponentData);
    DECLARE_RENDER_FUNC(LensScope, fb::LensScopeComponentData);
    DECLARE_RENDER_FUNC(CameraParams, fb::CameraParamsComponentData);
    DECLARE_RENDER_FUNC(ScreenEffect, fb::ScreenEffectComponentData);
    DECLARE_RENDER_FUNC(DamageEffect, fb::DamageEffectComponentData);
    DECLARE_RENDER_FUNC(PlanarReflection, fb::PlanarReflectionComponentData);
    DECLARE_RENDER_FUNC(DynamicEnvmap, fb::DynamicEnvmapComponentData);
    DECLARE_RENDER_FUNC(CharacterLighting, fb::CharacterLightingComponentData);
    DECLARE_RENDER_FUNC(MotionBlur, fb::MotionBlurComponentData);
    DECLARE_RENDER_FUNC(WorldRenderSettings, fb::WorldRenderSettings);

#undef DECLARE_RENDER_FUNC

    bool Vec2Edit(const char* label, fb::Vec2* current, const fb::Vec2* original);
    bool Vec3Edit(const char* label, fb::Vec3* current, const fb::Vec3* original, bool isColor = false);
    bool Vec4Edit(const char* label, fb::Vec4* current, const fb::Vec4* original, bool isColor = false);
    bool FloatEdit(const char* label, float* current, const float* original, float min = 0.f, float max = 0.f, const char* format = "%.3f");
    bool BoolEdit(const char* label, bool* current, const bool* original);
    bool IntEdit(const char* label, int* current, const int* original, int min = 0, int max = 0);
    bool UIntEdit(const char* label, uint32_t* current, const uint32_t* original, uint32_t min = 0, uint32_t max = 0);

    template<enumable E>
    bool EnumCombo(const char* label, int* current, const int* original);
    template<enumable E>
    bool EnumCombo(const char* label, int* current);
};

template<enumable E>
bool VisualEnvironmentEditor::EnumCombo(const char* label, int* c, const int* o)
{
    using NamesType = std::decay_t<decltype(magic_enum::enum_names<E>())>;
    const auto names = magic_enum::enum_names<E>();
    constexpr auto count = magic_enum::enum_count<E>();

    bool mod = *c != *o;
    if (mod && m_HighlightModified)
        ImGui::PushStyleColor(ImGuiCol_Text, m_ModifiedColor);

    auto getter = [](void* data, int idx, const char** out_text) -> bool
        {
            const auto& arr = *static_cast<const NamesType*>(data);
            if (idx < 0 || idx >= static_cast<int>(arr.size()))
                return false;
            if (out_text)
                *out_text = arr[idx].data();
            return true;
        };

    bool changed = ImGui::Combo(label, c, getter, (void*)&names, count);

    if (mod && m_HighlightModified)
        ImGui::PopStyleColor();

    if (m_ShowOriginalValues && ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::TextColored(m_OriginalColor, "Original: %s", (*o >= 0 && *o < count) ? names[*o].data() : "Unknown");
        ImGui::EndTooltip();
    }

    if (mod)
    {
        ImGui::SameLine();
        ImGui::PushID(label);
        if (ImGui::SmallButton("R"))
        {
            *c = *o;
            changed = true;
        }
        ImGui::PopID();
    }

    return changed;
}

template<enumable E>
bool VisualEnvironmentEditor::EnumCombo(const char* label, int* c)
{
    using NamesType = std::decay_t<decltype(magic_enum::enum_names<E>())>;
    const auto names = magic_enum::enum_names<E>();
    constexpr auto count = magic_enum::enum_count<E>();

    auto getter = [](void* data, int idx, const char** out_text) -> bool
        {
            const auto& arr = *static_cast<const NamesType*>(data);
            if (idx < 0 || idx >= static_cast<int>(arr.size()))
                return false;
            if (out_text)
                *out_text = arr[idx].data();
            return true;
        };

    return ImGui::Combo(label, c, getter, (void*)&names, count);
}