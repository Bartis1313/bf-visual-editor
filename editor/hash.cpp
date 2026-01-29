#include "editor.h"

// we have to somehow add unique hashing since not all states got ref'ed names easily
StateHash VisualEnvironmentEditor::ComputeStateHash(fb::VisualEnvironmentState* state)
{
    StateHash hash;
    if (!state)
        return hash;

    hash.priority = state->priority;
    hash.visibility = state->visibility;
    hash.componentMask = 0U;

    if (state->outdoorLight)      hash.componentMask |= Comp_OutdoorLight;
    if (state->enlighten)         hash.componentMask |= Comp_Enlighten;
    if (state->tonemap)           hash.componentMask |= Comp_Tonemap;
    if (state->colorCorrection)   hash.componentMask |= Comp_ColorCorrection;
    if (state->sky)               hash.componentMask |= Comp_Sky;
    if (state->fog)               hash.componentMask |= Comp_Fog;
    if (state->wind)              hash.componentMask |= Comp_Wind;
    if (state->sunFlare)          hash.componentMask |= Comp_Sunflare;
    if (state->dynamicAO)         hash.componentMask |= Comp_DynamicAO;
    if (state->dof)               hash.componentMask |= Comp_Dof;
    if (state->vignette)          hash.componentMask |= Comp_Vignette;
    if (state->filmGrain)         hash.componentMask |= Comp_FilmGrain;
    if (state->lensScope)         hash.componentMask |= Comp_LensScope;
    if (state->cameraParams)      hash.componentMask |= Comp_CameraParams;
    if (state->screenEffect)      hash.componentMask |= Comp_ScreenEffect;
    if (state->damageEffect)      hash.componentMask |= Comp_DamageEffect;
    if (state->planarReflection)  hash.componentMask |= Comp_PlanarReflection;
    if (state->dynamicEnvmap)     hash.componentMask |= Comp_DynamicEnvmap;
    if (state->characterLighting) hash.componentMask |= Comp_CharacterLighting;
    if (state->motionBlur)        hash.componentMask |= Comp_MotionBlur;

    return hash;
}


StateEditEntry* VisualEnvironmentEditor::FindEditEntry(const StateHash& hash)
{
    for (auto& entry : m_EditList)
    {
        if (entry.hash == hash)
            return &entry;
    }
    return nullptr;
}

void VisualEnvironmentEditor::SyncEditList()
{
    for (fb::VisualEnvironmentState* state : m_StateOrder)
    {
        if (!state)
            continue;

        auto itr = m_StateDataMap.find(state);
        if (itr == m_StateDataMap.end())
            continue;

        const StateEditData& data = itr->second;
        if (data.overrideEnabled)
            AddOrUpdateEditEntry(state, data);
    }

}

void VisualEnvironmentEditor::RestoreEditValuesFromEntry(StateEditData& dst, const StateEditData& src)
{
    dst.overrideEnabled = src.overrideEnabled;

#define RESTORE_IF_BOTH_HAVE(hasFlag, editMember, CopyFunc) \
    if (dst.hasFlag && src.hasFlag) { \
        CopyFunc(&dst.editMember, &src.editMember); \
    }

    RESTORE_IF_BOTH_HAVE(hasOutdoorLight, editOutdoorLight, CopyOutdoorLight);
    RESTORE_IF_BOTH_HAVE(hasEnlighten, editEnlighten, CopyEnlighten);
    RESTORE_IF_BOTH_HAVE(hasTonemap, editTonemap, CopyTonemap);
    RESTORE_IF_BOTH_HAVE(hasColorCorrection, editColorCorrection, CopyColorCorrection);
    RESTORE_IF_BOTH_HAVE(hasSky, editSky, CopySky);
    RESTORE_IF_BOTH_HAVE(hasFog, editFog, CopyFog);
    RESTORE_IF_BOTH_HAVE(hasWind, editWind, CopyWind);
    RESTORE_IF_BOTH_HAVE(hasSunFlare, editSunFlare, CopySunFlare);
    RESTORE_IF_BOTH_HAVE(hasDynamicAO, editDynamicAO, CopyDynamicAO);
    RESTORE_IF_BOTH_HAVE(hasDof, editDof, CopyDof);
    RESTORE_IF_BOTH_HAVE(hasVignette, editVignette, CopyVignette);
    RESTORE_IF_BOTH_HAVE(hasFilmGrain, editFilmGrain, CopyFilmGrain);
    RESTORE_IF_BOTH_HAVE(hasLensScope, editLensScope, CopyLensScope);
    RESTORE_IF_BOTH_HAVE(hasCameraParams, editCameraParams, CopyCameraParams);
    RESTORE_IF_BOTH_HAVE(hasScreenEffect, editScreenEffect, CopyScreenEffect);
    RESTORE_IF_BOTH_HAVE(hasDamageEffect, editDamageEffect, CopyDamageEffect);
    RESTORE_IF_BOTH_HAVE(hasPlanarReflection, editPlanarReflection, CopyPlanarReflection);
    RESTORE_IF_BOTH_HAVE(hasDynamicEnvmap, editDynamicEnvmap, CopyDynamicEnvmap);
    RESTORE_IF_BOTH_HAVE(hasCharacterLighting, editCharacterLighting, CopyCharacterLighting);
    RESTORE_IF_BOTH_HAVE(hasMotionBlur, editMotionBlur, CopyMotionBlur);

#undef RESTORE_IF_BOTH_HAVE
}

void VisualEnvironmentEditor::CopyEditDataDirectly(StateEditData& dst, const StateEditData& src)
{
    dst.overrideEnabled = src.overrideEnabled;

    if (dst.hasOutdoorLight)
        CopyOutdoorLight(&dst.editOutdoorLight, &src.editOutdoorLight);

    if (dst.hasEnlighten)
        CopyEnlighten(&dst.editEnlighten, &src.editEnlighten);

    if (dst.hasTonemap)
        CopyTonemap(&dst.editTonemap, &src.editTonemap);

    if (dst.hasColorCorrection)
        CopyColorCorrection(&dst.editColorCorrection, &src.editColorCorrection);

    if (dst.hasSky)
        CopySky(&dst.editSky, &src.editSky);

    if (dst.hasFog)
        CopyFog(&dst.editFog, &src.editFog);

    if (dst.hasWind)
        CopyWind(&dst.editWind, &src.editWind);

    if (dst.hasSunFlare)
        CopySunFlare(&dst.editSunFlare, &src.editSunFlare);

    if (dst.hasDynamicAO)
        CopyDynamicAO(&dst.editDynamicAO, &src.editDynamicAO);

    if (dst.hasDof)
        CopyDof(&dst.editDof, &src.editDof);

    if (dst.hasVignette)
        CopyVignette(&dst.editVignette, &src.editVignette);

    if (dst.hasFilmGrain)
        CopyFilmGrain(&dst.editFilmGrain, &src.editFilmGrain);

    if (dst.hasLensScope)
        CopyLensScope(&dst.editLensScope, &src.editLensScope);

    if (dst.hasCameraParams)
        CopyCameraParams(&dst.editCameraParams, &src.editCameraParams);

    if (dst.hasScreenEffect)
        CopyScreenEffect(&dst.editScreenEffect, &src.editScreenEffect);

    if (dst.hasDamageEffect)
        CopyDamageEffect(&dst.editDamageEffect, &src.editDamageEffect);

    if (dst.hasPlanarReflection)
        CopyPlanarReflection(&dst.editPlanarReflection, &src.editPlanarReflection);

    if (dst.hasDynamicEnvmap)
        CopyDynamicEnvmap(&dst.editDynamicEnvmap, &src.editDynamicEnvmap);

    if (dst.hasCharacterLighting)
        CopyCharacterLighting(&dst.editCharacterLighting, &src.editCharacterLighting);

    if (dst.hasMotionBlur)
        CopyMotionBlur(&dst.editMotionBlur, &src.editMotionBlur);
}

StateEditEntry* VisualEnvironmentEditor::FindEditEntryByPriorityAndMask(int priority, uint32_t componentMask)
{
    for (auto& entry : m_EditList)
    {
        if (entry.hash.priority == priority &&
            entry.hash.componentMask == componentMask)
        {
            return &entry;
        }
    }
    return nullptr;
}

void VisualEnvironmentEditor::AddOrUpdateEditEntry(fb::VisualEnvironmentState* state, const StateEditData& data)
{
    StateHash hash = ComputeStateHash(state);

    if (StateEditEntry* entry = FindEditEntry(hash))
    {
        entry->editData = data;
    }
    else
    {
        StateEditEntry newEntry;
        newEntry.hash = hash;
        newEntry.editData = data;
        m_EditList.push_back(std::move(newEntry));
    }
}

void VisualEnvironmentEditor::RemoveEditEntry(const StateHash& hash)
{
    m_EditList.erase(
        std::remove_if(m_EditList.begin(), m_EditList.end(),
            [&hash](const StateEditEntry& e) { return e.hash == hash; }),
        m_EditList.end());
}