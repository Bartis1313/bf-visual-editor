#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <cstdint>
#include "general.h"
#include "offsets.h"
#include "typeinfo.h"
#include <array>
#include "eastl.h"
#include "vfunc.h"

#include <string>
#include <cstring>

#include "../SDK_bf3/enums.h"
#include "../SDK_bf3/declarations.h"
#include "../SDK_bf3/structs.h"

#define _DataContainer_
#define _Asset_
#define _WorldRenderSettings_
#define _SystemSettings_
#define _EnlightenRuntimeSettings_
#define _LevelData_
#define _ClientRoundOverEntity_
#define _EntityBusPeer_
#define _Entity_
#define _SpatialEntity_
#define _LocalLightEntity_
#define _LensFlareEntity_
#define _ClientStaticModelGroupEntity_
#define _ShaderDatabase_
#define _SpotLightEntity_
#define _PointLightEntity_
#define _GameEntity_
#define _ClientGameEntity_
#define _VisualEnvironmentEntity_
#define _EmitterComponentData_
#define _ProcessorData_
#define _DxTexture_
#define _MeshSet_

namespace fb
{
	class DataContainer : public ITypedObject
	{
	public:
		unsigned __int16 m_refCnt;
		unsigned __int16 m_flags;

		fb::Guid* getInstanceGuid()
		{
			if ((m_flags & 0x100) == 0)
				return nullptr;

			// lea eax, [ecx-10h]
			fb::Guid* guid = reinterpret_cast<fb::Guid*>(this) - 1;

			if (guid == nullptr)
				return nullptr;

			return guid;
		}

		bool is(const char* name)
		{
			fb::TypeInfo* typeinf = this->GetType();
			if (!typeinf)
				return false;

			return strcmp(typeinf->m_InfoData->m_Name, name) == 0;
		}

		bool isClassId(unsigned int classId)
		{
			fb::TypeInfo* typeinf = this->GetType();
			if (!typeinf)
				return false;

			if (typeinf->GetTypeCode() != fb::BasicTypesEnum::kTypeCode_Class)
				return false;

			const fb::ClassInfo* classInfo = (fb::ClassInfo*)typeinf;
			if (!classInfo)
				return false;

			return classInfo->m_ClassId == classId;
		}
	};

	class Asset : public DataContainer
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 2035;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x023C2C44;
		}
		char* m_Name; //0x0008

		// try to get shorter debugname
		std::string tryGetDebugName() const
		{
			std::string name{ m_Name };
			const size_t pos = name.find_last_of("/\\");
			if (pos != std::string::npos)
				name = name.substr(pos + 1);

			return name;
		}

	};//Size=0x000C

	class CapturedOutdoorLightComponentData
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 1592;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x02405EBC;
		}
		Vec3 m_SunColor; //0x0060
		Vec2 m_CloudShadowSpeed; //0x0070
		char _0x0078[8];
		Vec3 m_SkyColor; //0x0080
		Vec3 m_GroundColor; //0x0090
		float m_SunRotationY; //0x00A0
		float m_SunRotationX; //0x00A4
		Realm m_Realm; //0x00A8
		float m_SkyLightAngleFactor; //0x00AC
		float m_SunSpecularScale; //0x00B0
		float m_SkyEnvmapShadowScale; //0x00B4
		float m_SunShadowHeightScale; //0x00B8
		float m_CloudShadowCoverage; //0x00BC
		TextureAsset* m_CloudShadowTexture; //0x00C0
		float m_TranslucencyDistortion; //0x00C4
		float m_CloudShadowSize; //0x00C8
		float m_TranslucencyAmbient; //0x00CC
		float m_CloudShadowExponent; //0x00D0
		float m_TranslucencyScale; //0x00D4
		float m_TranslucencyPower; //0x00D8
		bool m_Enable; //0x00DC
		bool m_CloudShadowEnable; //0x00DD
		char _0x00DE[2];
	};//Size=0x00E0

	class CapturedEnlightenComponentData
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 1584;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x0240601C;
		}
		Vec3 m_SkyBoxSkyColor; //0x0060
		Vec3 m_SkyBoxBackLightColor; //0x0070
		Vec3 m_SkyBoxGroundColor; //0x0080
		Vec3 m_TerrainColor; //0x0090
		Vec3 m_SkyBoxSunLightColor; //0x00A0
		float m_BounceScale; //0x00B0
		float m_CullDistance; //0x00B4
		float m_SunScale; //0x00B8
		Realm m_Realm; //0x00BC
		float m_SkyBoxBackLightRotationY; //0x00C0
		float m_SkyBoxSunLightColorSize; //0x00C4
		float m_SkyBoxBackLightColorSize; //0x00C8
		float m_SkyBoxBackLightRotationX; //0x00CC
		bool m_SkyBoxEnable; //0x00D0
		bool m_Enable; //0x00D1
		char _0x00D2[14];
	};//Size=0x00E0

	class CapturedTonemapComponentData
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 1688;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x023D1DEC;
		}
		Vec3 m_BloomScale; //0x0060
		Realm m_Realm; //0x0070
		TonemapMethod m_TonemapMethod; //0x0074
		float m_MinExposure; //0x0078
		float m_MiddleGray; //0x007C
		float m_ExposureAdjustTime; //0x0080
		float m_MaxExposure; //0x0084
		float m_ChromostereopsisOffset; //0x0088
		float m_ChromostereopsisScale; //0x008C
		bool m_ChromostereopsisEnable; //0x0090
		char _0x0091[15];
	};//Size=0x00A0

	class CapturedColorCorrectionComponentData
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 1687;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x023D1E18;
		}
		Vec3 m_Contrast; //0x0060
		Vec3 m_Saturation; //0x0070
		Vec3 m_Brightness; //0x0080
		Realm m_Realm; //0x0090
		float m_Hue; //0x0094
		TextureAsset* m_ColorGradingTexture; //0x0098
		bool m_ColorGradingEnable; //0x009C
		bool m_Enable; //0x009D
		char _0x009E[2];
	};//Size=0x00A0

	class CapturedSkyComponentData
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 1590;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x02405F14;
		}
		Vec3 m_CloudLayerSunColor; //0x0060
		Vec3 m_CloudLayer2Color; //0x0070
		Vec3 m_CloudLayer1Color; //0x0080
		TextureAsset* m_SkyGradientTexture; //0x0090
		float m_SunSize; //0x0094
		float m_BrightnessScale; //0x0098
		float m_SunScale; //0x009C
		float m_PanoramicUVMaxX; //0x00A0
		float m_PanoramicUVMinY; //0x00A4
		float m_PanoramicUVMaxY; //0x00A8
		Realm m_Realm; //0x00AC
		float m_PanoramicUVMinX; //0x00B0
		TextureAsset* m_PanoramicTexture; //0x00B4
		TextureAsset* m_PanoramicAlphaTexture; //0x00B8
		float m_PanoramicTileFactor; //0x00BC
		TextureAsset* m_CloudLayerMaskTexture; //0x00C0
		float m_CloudLayer1Altitude; //0x00C4
		float m_CloudLayer1TileFactor; //0x00C8
		float m_CloudLayer1Rotation; //0x00CC
		float m_CloudLayer1Speed; //0x00D0
		float m_CloudLayer1SunLightIntensity; //0x00D4
		float m_PanoramicRotation; //0x00D8
		float m_CloudLayer1AmbientLightIntensity; //0x00DC
		float m_CloudLayer1SunLightPower; //0x00E0
		float m_CloudLayer1AlphaMul; //0x00E4
		TextureAsset* m_CloudLayer1Texture; //0x00E8
		float m_CloudLayer2Altitude; //0x00EC
		float m_CloudLayer2TileFactor; //0x00F0
		float m_CloudLayer2Rotation; //0x00F4
		float m_CloudLayer2Speed; //0x00F8
		float m_CloudLayer2SunLightIntensity; //0x00FC
		float m_CloudLayer2SunLightPower; //0x0100
		float m_CloudLayer2AmbientLightIntensity; //0x0104
		float m_StaticEnvmapScale; //0x0108
		float m_CloudLayer2AlphaMul; //0x010C
		TextureAsset* m_CloudLayer2Texture; //0x0110
		TextureAsset* m_StaticEnvmapTexture; //0x0114
		float m_SkyVisibilityExponent; //0x0118
		float m_SkyEnvmap8BitTexScale; //0x011C
		TextureAsset* m_CustomEnvmapTexture; //0x0120
		float m_CustomEnvmapScale; //0x0124
		float m_CustomEnvmapAmbient; //0x0128
		bool m_Enable; //0x012C
		char _0x012D[3];
	};//Size=0x0130

	class CapturedFogComponentData
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 1591;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x02405EE8;
		}
		Vec4 m_Curve; //0x0060
		Vec4 m_FogColorCurve; //0x0070
		Vec3 m_FogColor; //0x0080
		float m_FogColorStart; //0x0090
		float m_FogDistanceMultiplier; //0x0094
		Realm m_Realm; //0x0098
		float m_End; //0x009C
		float m_TransparencyFadeStart; //0x00A0
		float m_Start; //0x00A4
		float m_TransparencyFadeEnd; //0x00A8
		float m_FogColorEnd; //0x00AC
		float m_TransparencyFadeClamp; //0x00B0
		float m_HeightFogVisibilityRange; //0x00B4
		float m_HeightFogAltitude; //0x00B8
		float m_HeightFogDepth; //0x00BC
		float m_HeightFogFollowCamera; //0x00C0
		bool m_FogGradientEnable; //0x00C4
		bool m_Enable; //0x00C5
		bool m_HeightFogEnable; //0x00C6
		bool m_FogColorEnable; //0x00C7
		char _0x00C8[8];
	};//Size=0x00D0

	class CapturedWindComponentData
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 1587;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x02405F98;
		}
		Realm m_Realm; //0x0060
		float m_WindDirection; //0x0064
		float m_WindStrength; //0x0068
		char _0x006C[4];
	};//Size=0x0070

	class CapturedSunFlareComponentData
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 1586;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x02405FC4;
		}
		Vec4 m_Element5AlphaScreenPosCurve; //0x0060
		Vec4 m_Element5AlphaOccluderCurve; //0x0070
		Vec4 m_Element5SizeScreenPosCurve; //0x0080
		Vec4 m_Element2SizeOccluderCurve; //0x0090
		Vec4 m_Element5SizeOccluderCurve; //0x00A0
		Vec4 m_Element2AlphaOccluderCurve; //0x00B0
		Vec4 m_Element2AlphaScreenPosCurve; //0x00C0
		Vec2 m_Element1Size; //0x00D0
		char _0x00D8[8];
		Vec4 m_Element1SizeOccluderCurve; //0x00E0
		Vec4 m_Element1SizeScreenPosCurve; //0x00F0
		Vec4 m_Element1AlphaOccluderCurve; //0x0100
		Vec4 m_Element1AlphaScreenPosCurve; //0x0110
		Vec2 m_Element5Size; //0x0120
		char _0x0128[8];
		Vec4 m_Element4SizeOccluderCurve; //0x0130
		Vec2 m_Element3Size; //0x0140
		Vec2 m_Element2Size; //0x0148
		Vec4 m_Element4AlphaScreenPosCurve; //0x0150
		Vec4 m_Element2SizeScreenPosCurve; //0x0160
		Vec4 m_Element4SizeScreenPosCurve; //0x0170
		Vec2 m_Element4Size; //0x0180
		char _0x0188[8];
		Vec4 m_Element3AlphaScreenPosCurve; //0x0190
		Vec4 m_Element4AlphaOccluderCurve; //0x01A0
		Vec4 m_Element3SizeOccluderCurve; //0x01B0
		Vec4 m_Element3SizeScreenPosCurve; //0x01C0
		Vec4 m_Element3AlphaOccluderCurve; //0x01D0
		SurfaceShaderBaseAsset* m_Element1Shader; //0x01E0
		float m_OccluderSize; //0x01E4
		SurfaceShaderBaseAsset* m_Element2Shader; //0x01E8
		float m_Element3RayDistance; //0x01EC
		SurfaceShaderBaseAsset* m_Element4Shader; //0x01F0
		float m_Element4RayDistance; //0x01F4
		float m_Element2RayDistance; //0x01F8
		float m_Element1RayDistance; //0x01FC
		float m_Element5RayDistance; //0x0200
		SurfaceShaderBaseAsset* m_Element3Shader; //0x0204
		Realm m_Realm; //0x0208
		SurfaceShaderBaseAsset* m_Element5Shader; //0x020C
		bool m_Element5Enable; //0x0210
		bool m_Element3Enable; //0x0211
		bool m_Element2Enable; //0x0212
		bool m_Element1Enable; //0x0213
		bool m_DebugDrawOccluder; //0x0214
		bool m_Enable; //0x0215
		bool m_Element4Enable; //0x0216
		char _0x0217[9];
	};//Size=0x0220

	class CapturedDynamicAOComponentData
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 1585;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x02405FF0;
		}
		Realm m_Realm; //0x0060
		float m_SsaoMaxDistanceOuter; //0x0064
		float m_SsaoFade; //0x0068
		float m_SsaoRadius; //0x006C
		float m_SsaoMaxDistanceInner; //0x0070
		float m_HbaoRadius; //0x0074
		float m_HbaoAttenuation; //0x0078
		float m_HbaoAngleBias; //0x007C
		float m_HbaoPowerExponent; //0x0080
		float m_HbaoContrast; //0x0084
		float m_HbaoMaxFootprintRadius; //0x0088
		bool m_Enable; //0x008C
		char _0x008D[3];
	};//Size=0x0090

	class CapturedDofComponentData
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 1686;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x023D1E44;
		}
		Realm m_Realm; //0x0060
		float m_NearDistanceScale; //0x0064
		float m_FocusDistance; //0x0068
		BlurFilter m_BlurFilter; //0x006C
		float m_BlurFilterDeviation; //0x0070
		float m_FarDistanceScale; //0x0074
		float m_BlurAdd; //0x0078
		float m_Scale; //0x007C
		float m_DiffusionDofFocalLength; //0x0080
		float m_DiffusionDofAperture; //0x0084
		bool m_DiffusionDofEnable; //0x0088
		bool m_Enable; //0x0089
		char _0x008A[6];
	};//Size=0x0090

	class CapturedVignetteComponentData
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 1685;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x023D1E70;
		}
		Vec2 m_Scale; //0x0060
		char _0x0068[8];
		Vec3 m_Color; //0x0070
		Realm m_Realm; //0x0080
		float m_Exponent; //0x0084
		float m_Opacity; //0x0088
		bool m_Enable; //0x008C
		char _0x008D[3];
	};//Size=0x0090

	class CapturedFilmGrainComponentData
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 1684;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x023D1E9C;
		}
		Vec3 m_ColorScale; //0x0060
		Vec2 m_TextureScale; //0x0070
		Realm m_Realm; //0x0078
		TextureAsset* m_Texture; //0x007C
		bool m_LinearFilteringEnable; //0x0080
		bool m_RandomEnable; //0x0081
		bool m_Enable; //0x0082
		char _0x0083[13];
	};//Size=0x0090

	class CapturedLensScopeComponentData
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 1683;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x023D1EC8;
		}
		Vec3 m_ChromaticAberrationColor1; //0x0060
		Vec2 m_RadialBlendDistanceCoefficients; //0x0070
		char _0x0078[8];
		Vec3 m_ChromaticAberrationColor2; //0x0080
		Vec2 m_BlurCenter; //0x0090
		Vec2 m_ChromaticAberrationDisplacement2; //0x0098
		Vec2 m_ChromaticAberrationDisplacement1; //0x00A0
		Vec2 m_ChromaticAberrationStrengths; //0x00A8
		float m_BlurScale; //0x00B0
		Realm m_Realm; //0x00B4
		bool m_Enable; //0x00B8
		char _0x00B9[7];
	};//Size=0x00C0

	class CapturedCameraParamsComponentData
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 1582;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x02406074;
		}
		float m_ViewDistance; //0x0060
		float m_NearPlane; //0x0064
		float m_SunShadowmapViewDistance; //0x0068
		char _0x006C[4];
	};//Size=0x0070

	class CapturedScreenEffectComponentData
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 1581;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x024060B0;
		}
		Vec4 m_ScreenEffectParams; //0x0060
		ScreenEffectFrameType m_FrameType; //0x0070
		SurfaceShaderBaseAsset* m_Shader; //0x0074
		float m_FrameWidth; //0x0078
		float m_OuterFrameOpacity; //0x007C
		float m_InnerFrameOpacity; //0x0080
		float m_Angle; //0x0084
		Realm m_Realm; //0x0088
		char _0x008C[4];
	};//Size=0x0090

	class CapturedDamageEffectComponentData
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 1580;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x024060DC;
		}
		Vec4 m_RightDamage; //0x0060
		Vec4 m_TopDamage; //0x0070
		Vec4 m_LeftDamage; //0x0080
		Vec4 m_BottomDamage; //0x0090
		float m_OuterFrameOpacity; //0x00A0
		float m_InnerFrameOpacity; //0x00A4
		float m_FrameWidth; //0x00A8
		SurfaceShaderBaseAsset* m_Shader; //0x00AC
		float m_StartCriticalEffectHealthThreshold; //0x00B0
		float m_EndCriticalEffectHealthThreshold; //0x00B4
		float m_MinDamagePercentageThreshold; //0x00B8
		float m_FallofTime; //0x00BC
		float m_MaxOpacityDamagePercentage; //0x00C0
		Realm m_Realm; //0x00C4
		bool m_DebugDamage; //0x00C8
		char _0x00C9[7];
	};//Size=0x00D0

	class CapturedShaderParamsComponentData
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 1583;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x02406048;
		}
		Vec4 m_Value; //0x0060
		char* m_ParameterName; //0x0070
		char _0x0074[12];
	};//Size=0x0080

	class CapturedPlanarReflectionComponentData
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 1589;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x02405F40;
		}
		BlurFilter m_VerticalBlurFilter; //0x0060
		float m_HorizontalDeviation; //0x0064
		float m_GroundHeight; //0x0068
		float m_VerticalDeviation; //0x006C
		BlurFilter m_HorizontalBlurFilter; //0x0070
		bool m_SkyRenderEnable; //0x0074
		bool m_Enable; //0x0075
		char _0x0076[10];
	};//Size=0x0080

	class CapturedDynamicEnvmapComponentData
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 1588;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x02405F6C;
		}
		Vec3 m_KeyColorEnvmap; //0x0060
		Vec3 m_GroundColorEnvmap; //0x0070
		Vec3 m_SkyColorEnvmap; //0x0080
		bool m_Enable; //0x0090
		char _0x0091[15];
	};//Size=0x00A0

	class CapturedCharacterLightingComponentData
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 1579;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x02406118;
		}
		Vec3 m_BottomLight; //0x0060
		Vec3 m_TopLight; //0x0070
		CharacterLightingMode m_CharacterLightingMode; //0x0080
		float m_CameraUpRotation; //0x0084
		float m_TopLightDirY; //0x0088
		float m_BlendFactor; //0x008C
		float m_TopLightDirX; //0x0090
		bool m_LockToCameraDirection; //0x0094
		bool m_FirstPersonEnable; //0x0095
		bool m_CharacterLightEnable; //0x0096
		char _0x0097[9];
	};//Size=0x00A0

	class CapturedMotionBlurComponentData
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 1578;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x02406144;
		}
		Realm m_Realm; //0x0060
		float m_CutoffGradientScale; //0x0064
		float m_MotionBlurCutoffRadius; //0x0068
		bool m_MotionBlurEnable; //0x006C
		char _0x006D[3];
	};//Size=0x0070

	struct ShProbe
	{
		__m128 shR;
		__m128 shG;
		__m128 shB;
		__m128 shA;

		void ScaleProbeColor(float r, float g, float b)
		{
			auto probe = *this;
			probe.shR = _mm_mul_ps(probe.shR, _mm_set1_ps(r));
			probe.shG = _mm_mul_ps(probe.shG, _mm_set1_ps(g));
			probe.shB = _mm_mul_ps(probe.shB, _mm_set1_ps(b));
		}
	};

	class VisualEnvironment
	{
	public:
		CapturedOutdoorLightComponentData outdoorLight; //0x0000
		CapturedEnlightenComponentData enlighten; //0x0080
		CapturedTonemapComponentData tonemap; //0x0100
		CapturedColorCorrectionComponentData colorCorrection; //0x0140
		CapturedSkyComponentData sky; //0x0180
		CapturedFogComponentData fog; //0x0250
		CapturedWindComponentData wind; //0x02C0
		CapturedSunFlareComponentData sunFlare; //0x02D0
		CapturedDynamicAOComponentData dynamicAO; //0x0490
		CapturedDofComponentData dof; //0x04C0
		CapturedVignetteComponentData vignette; //0x04F0
		CapturedFilmGrainComponentData filmGrain; //0x0520
		CapturedLensScopeComponentData lensScope; //0x0550
		CapturedCameraParamsComponentData cameraParams; //0x05B0
		CapturedScreenEffectComponentData screenEffect; //0x05C0
		CapturedDamageEffectComponentData damageEffect; //0x05F0
		CapturedShaderParamsComponentData shaderparams[16]; //0x0660
		CapturedPlanarReflectionComponentData planarReflection; //0x0860
		CapturedDynamicEnvmapComponentData dynamicEnvmap; //0x0880
		CapturedCharacterLightingComponentData characterLighting; //0x08C0
		CapturedMotionBlurComponentData motionBlur; //0x0900
		int32_t debug; //0x0910
		char pad_0914[8]; //0x0914
		uint32_t shaderparamsCount; //0x091C
		int32_t resources; //0x0920
		char pad_0924[156]; //0x0924
		Vec4 activeScreenEffectPosition; //0x09C0
		uint32_t screenEffectStateId; //0x09D0
		float screenEffectVisibilty; //0x09D4
		bool skyEnvmapDirty; //0x09D8
		char pad_09D9[4];

		fb::ShProbe& getShProbe()
		{
			return *reinterpret_cast<fb::ShProbe*>(
				reinterpret_cast<std::uintptr_t>(this) + 0x980
				);
		}
	}; //Size: 0x9E0

	struct VisualEnvironmentResources
	{
		void* colorGradingTexture; //0x0000 ITexture*
		void* filmGrainTexture; //0x0004
		void* skyGradientTexture; //0x0008
		void* panoramicSkyTexture; //0x000C
		void* panoramicSkyAlphaTexture; //0x0010
		void* cloudLayer0Texture; //0x0014
		void* cloudLayer1Texture; //0x0018
		void* cloudLayerMaskTexture; //0x001C
		void* cloudShadowTexture; //0x0020
		void* staticEnvmapTexture; //0x0024
		void* customEnvmapTexture; //0x0028
		void* screenEffectShader; //0x002C SmartRef<ISurfaceShader>
		void* damageEffectShader; //0x0030
	};//Size=0x0034

	struct TextureHeader
	{
		unsigned int version; //0x0000
		unsigned int type; //0x0004
		unsigned int format; //0x0008
		unsigned int flags; //0x000C
		unsigned __int16 width; //0x0010
		unsigned __int16 height; //0x0012
		unsigned __int16 depth; //0x0014
		unsigned __int16 sliceCount; //0x0016
		unsigned __int16 unused0; //0x0018
		unsigned __int8 mipmapCount; //0x001A
		unsigned __int8 mipmapBaseIndex;//0x001B
		Guid streamingChunkId; //0x001C
		unsigned int mipmapSizes[15]; //0x002C
		unsigned int mipmapChainSize; //0x0068
		unsigned int resourceNameHash; //0x006C
		char textureGroup[16]; //0x0070
	};//Size=0x0080

	struct VisualEnvironmentState
	{
		float visibility;
		int priority;
		bool overrideVisibility;
		bool excluded;
		char pad_0x0A[0x2];
		int stateId;
		Vec4 activeEffectPosition;
		CapturedOutdoorLightComponentData* outdoorLight; //0x0000
		CapturedEnlightenComponentData* enlighten; //0x0080
		CapturedTonemapComponentData* tonemap; //0x0100
		CapturedColorCorrectionComponentData* colorCorrection; //0x0140
		CapturedSkyComponentData* sky; //0x0180
		CapturedFogComponentData* fog; //0x0250
		CapturedWindComponentData* wind; //0x02C0
		CapturedSunFlareComponentData* sunFlare; //0x02D0
		CapturedDynamicAOComponentData* dynamicAO; //0x0490
		CapturedDofComponentData* dof; //0x04C0
		CapturedVignetteComponentData* vignette; //0x04F0
		CapturedFilmGrainComponentData* filmGrain; //0x0520
		CapturedLensScopeComponentData* lensScope; //0x0550
		CapturedCameraParamsComponentData* cameraParams; //0x05B0
		CapturedScreenEffectComponentData* screenEffect; //0x05C0
		CapturedDamageEffectComponentData* damageEffect; //0x05F0
		CapturedPlanarReflectionComponentData* planarReflection; //0x0860
		CapturedDynamicEnvmapComponentData* dynamicEnvmap; //0x0880
		CapturedCharacterLightingComponentData* characterLighting; //0x08C0
		CapturedMotionBlurComponentData* motionBlur; //0x0900
		CapturedShaderParamsComponentData* shaderparams[16]; //0x0070
		unsigned int shaderParamsCount; //0x00B0
		VisualEnvironmentResources resources; //0x00B4
		char _0x00E8[8];
	};//Size=0x00F0

	class DxRenderSettings
	{
	public:
		char unknown0[32]; //0x0000
		__int32 Width; //0x0020
		__int32 Height; //0x0024
		char unknown40[48]; //0x0028
		__int32 ResX; //0x0058
		__int32 RexY; //0x005C
		char unknown96[120]; //0x0060
		ID3D11Device* m_device; //0x00D8
		ID3D11DeviceContext* m_deviceContext; //0x00DC
		char unknown224[20]; //0x00E0
		IDXGISwapChain* m_swapChain; //0x00F4
		char unknown248[4]; //0x00F8
	};

	class RenderScreenInfo
	{
	public:
		UINT m_nWidth; // 0x0
		UINT m_nHeight; // 0x4
		UINT m_nWindowWidth; // 0x8
		UINT m_nWindowHeight; // 0xC
		FLOAT fRefreshRate; // 0x10
	};

	class DxRenderer
	{
	public:
		DxRenderSettings* m_settings;
		BYTE Pad_004[0x4]; // 0x00
		UINT m_nFrameCounter; // 0x08
		BOOL m_bFrameInProgress; // 0x0C
		HWND m_hWnd; // 0x10
		BYTE Pad_014[0x4]; // 0x14
		BYTE m_bFullscreenWanted; // 0x18
		BYTE m_bFullscreenActive; // 0x19
		BYTE m_bMinimized; // 0x1A
		BYTE m_bMinimizing; // 0x1B
		BYTE m_bResizing; // 0x1C
		BYTE m_bOccluded; // 0x1D
		BYTE m_bVSync; // 0x1E
		char pad[0x1]; // 0x1F
		RenderScreenInfo m_screenInfo; // 0x20
		char pad1[0xA4]; // 0x34
		ID3D11Device* pDevice; // 0xD8
		ID3D11DeviceContext* pContext; // 0xDC
		char pad3[0x14]; // 0xE0
		IDXGISwapChain* pSwapChain; // 0xF4

	public:
		static DxRenderer* GetInstance()
		{
			return *(DxRenderer**)OFF_g_dxRenderer;
		}
	};

	class IMouse
	{
	public:
		virtual void init(); // V: 0x0
		virtual void release(); // V: 0x4
		virtual void preFrameUpdate(float); // V: 0x8
		virtual void update(float); // V: 0xC
		virtual void updateBetweenFrames(float); // V: 0x10
		virtual bool isDown(void*, bool); // V: 0x14
		virtual bool wentDown(void*); // V: 0x18
		virtual bool wentUp(void*); // V: 0x1C
		virtual float getLevel(void*, bool); // V: 0x20
		virtual float getLevel(void*); // V: 0x24
		virtual bool getBufferedAction(void*); // V: 0x28
		virtual void enableCursorMode(bool, int); // V: 0x2C
		virtual void setCursorPosition(int, int); // V: 0x30
		virtual void setCursorRect(bool, unsigned int, unsigned int, unsigned int, unsigned int); // V: 0x34
		virtual void setMouseSensitivity(float); // V: 0x38
		virtual void setUIOwnsInput(bool); // V: 0x3C
	};

	class InputCache
	{
	public:
		char _0x0000[4];
		float m_conceptCache[253]; //0x0004 indexed by InputConceptIdentifiers
	};

	class BorderInputNode
	{
	public:
		virtual void init();
		virtual void exit();
		virtual void clear();
		virtual void suppressButtons(); // V: 0x10
		virtual float getLevel(bool); // V: 0x14
		virtual float getLevel2(bool); // V: 0x18
		virtual bool isDown(bool); // V: 0x1C
		virtual bool isDown2(); // V: 0x20
		virtual bool wentDown(); // V: 0x24
		virtual bool wentDown2(); // V: 0x28
		virtual bool wentUp(); // V: 0x2C
		virtual bool wentUp2(); // V: 0x30
		virtual bool isMouseActive(); // V: 0x34
		virtual void* getPad(); // V: 0x38
		virtual class MotionController* getMotionController(); // V: 0x3C
		virtual class IMouse* getMouse(); // V: 0x40
		virtual class IKeyboard* getKeyboard(); // V: 0x44
		virtual void setUIInputTyping(bool); // V: 0x48
		virtual void setUIInputClicking(bool, int); // V: 0x4C
		virtual void setUICursorPosition(int, int); // V: 0x50
		virtual void setUICursorRect(bool, unsigned int, unsigned int, unsigned int, unsigned int); // V: 0x54
		virtual void updateBetweenFrames(float); // V: 0x58
		virtual /*class fb::InputActionData **/void* getTriggeredInputAction(); // V: 0x5C
		virtual bool getTriggeredInputAction(/*class fb::InputActionData &*/void*); // V: 0x60
		virtual void update(float); // V: 0x64
		virtual int preFrameUpdate(float fDeltaTime);
		//virtual void postFrameUpdate(float);
		virtual void postFrameUpdate(float); // V: 0x6C

		static BorderInputNode* GetInstance()
		{
			DWORD BORDERINPUTNODE = OFF_g_borderInputNode;
			return *(BorderInputNode**)BORDERINPUTNODE;
		}

		char unknown0[4];
		class InputCache* m_inputCache;
		//new
		bool m_forceReadCache; // 0x8
		char pad[0x3];
		class Pad* m_pad; // 0xC
		MotionController* m_motionController; // 0x10
		class IKeyboard* m_keyboard; // 0x14 enableTypingMode!
		IMouse* m_mouse;
		bool m_disableInput;
	};

	class GameTime
	{
	public:
		unsigned int m_ticks;
		unsigned int m_tickFrequency;
		unsigned int m_tickIndexInFrame;
		unsigned int m_lastTickIndexInFrame;
		unsigned int m_tickCountInFrame;
		float m_deltaTime;
		float m_passedDeltaTimeInFrame;
		long double m_time;
		bool m_useVariableDeltaTime;
	};

	class GameContext
	{
	public:
		char pad[8];
		class PlayerManager* m_playerManager; // 0x08
		class GameTime* m_gameTime; // 0x0C
		class ClientLevel* m_level; // 0x10
		class MaterialGridManager* m_materialGridManager; // 0x14
		DWORD m_animationManager; // 0x18 ant::AnimationManager
		DWORD m_modelAnimationManager; // 0x1C ModelAnimationManager
		DWORD m_blueprintBundleManager; // 0x20 BlueprintBundleManager
		DWORD m_dlcManager; // 0x24 DLCManager
		DWORD m_demoControl; // 0x28 DemoControl
		INT m_realm; // 0x2C
	}; // 0x30

	class WorldRenderSettings/*Base*/
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 851;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x0240627C;
		}
		char padInherit[sizeof(DataContainer)]; // 0x0000
		char SKIPME[8]; // 0x0008
		Vec3 m_DynamicEnvmapDefaultPosition; //0x0010
		Vec3 m_SubSurfaceColor; //0x0020
		float m_ViewportScale; //0x0030
		float m_ShadowMinScreenArea; //0x0034
		float m_ShadowViewportScale; //0x0038
		unsigned __int32 m_FxaaQuality; //0x003C
		float m_CullScreenAreaScale; //0x0040
		float m_PlanarReflectionCullFOV; //0x0044
		unsigned __int32 m_GenericEntityMaxVisibleEntityCount; //0x0048
		float m_ShadowmapSizeZScale; //0x004C
		unsigned __int32 m_ShadowmapResolution; //0x0050
		unsigned __int32 m_ShadowmapQuality; //0x0054
		float m_ShadowmapPoissonFilterScale; //0x0058
		unsigned __int32 m_ShadowmapSliceCount; //0x005C
		float m_ShadowmapSliceSchemeWeight; //0x0060
		float m_ShadowmapFirstSliceScale; //0x0064
		float m_ShadowmapViewDistance; //0x0068
		unsigned __int32 m_PlanarReflectionWidth; //0x006C
		float m_ShadowmapExtrusionLength; //0x0070
		float m_ShadowmapMinScreenArea; //0x0074
		float m_TempPlanarReflectionY; //0x0078
		float m_SubSurfaceRolloffKeyLight; //0x007C
		__int32 m_FxaaForceVendor; //0x0080
		unsigned __int32 m_PlanarReflectionHeight; //0x0084
		float m_LightIntensityScale; //0x0088
		float m_LightLodSpecularFadeAreaEnd; //0x008C
		float m_ShadowmapMinFov; //0x0090
		float m_LightLodSpecularFadeAreaStart; //0x0094
		float m_LightLodRadiusFactor; //0x0098
		float m_LightForceIntensity; //0x009C
		float m_LightLodFadeArea; //0x00A0
		float m_LightLodMinArea; //0x00A4
		__int32 m_DrawDebugDynamicEnvmapMipLevel; //0x00A8
		unsigned __int32 m_MaxDecalVolumeCount; //0x00AC
		float m_MotionBlurScale; //0x00B0
		float m_MotionBlurMax; //0x00B4
		float m_MotionBlurNoiseScale; //0x00B8
		unsigned __int32 m_MotionBlurQuality; //0x00BC
		unsigned __int32 m_MaxSpotLightShadowCount; //0x00C0
		MipmapFilterMode m_DynamicEnvmapFilterMode; //0x00C4
		unsigned __int32 m_MotionBlurMaxSampleCount; //0x00C8
		unsigned __int32 m_MotionBlurFrameAverageCount; //0x00CC
		float m_MotionBlurMaxFrameTime; //0x00D0
		float m_ForceMotionBlurDepthCutoff; //0x00D4
		float m_ForceMotionBlurCutoffGradientScale; //0x00D8
		unsigned __int32 m_MultisampleCount; //0x00DC
		float m_MultisampleThreshold; //0x00E0
		unsigned __int32 m_MaxPointLightCount; //0x00E4
		unsigned __int32 m_ReflectionEnvmapSize; //0x00E8
		float m_SpotLightNearPlane; //0x00EC
		float m_SpotLightShadowmapPoissonFilterScale; //0x00F0
		__int32 m_OnlyShadowmapSlice; //0x00F4
		WorldViewMode m_ViewMode; //0x00F8
		unsigned __int32 m_SpotLightShadowmapResolution; //0x00FC
		QualityLevel m_SpotLightShadomapLevel; //0x0100
		unsigned __int32 m_SpotLightShadowmapQuality; //0x0104
		__int32 m_OnlyLightTileY; //0x0108
		__int32 m_OnlyLightTileX; //0x010C
		float m_XenonHdrColorScale; //0x0110
		float m_XenonHdrColorScaleFactor; //0x0114
		unsigned __int32 m_Ps3LightTileJobMaxFbTileCount; //0x0118
		__int32 m_OnlyLightTileIndex; //0x011C
		unsigned __int32 m_Ps3LightTileJobCount; //0x0120
		unsigned __int32 m_Ps3LightTileJobPriority; //0x0124
		unsigned __int32 m_XenonLightTileMaxLineLightCount; //0x0128
		unsigned __int32 m_XenonLightTileMaxSpotLightCount; //0x012C
		unsigned __int32 m_XenonLightTileConstantBufferSize; //0x0130
		unsigned __int32 m_XenonLightTileMaxPointLightCount; //0x0134
		unsigned __int32 m_MaxDestructionVolumeCount; //0x0138
		float m_DynamicEnvmapFilterWidth; //0x013C
		float m_LightTileMinArea; //0x0140
		unsigned __int32 m_DynamicEnvmapResolution; //0x0144
		ShaderGBufferLayout m_GBufferLayout; //0x0148
		__int32 m_DrawDebugSkyEnvmapMipLevel; //0x014C
		float m_DecalVolumeScale; //0x0150
		unsigned __int32 m_LightOverdrawMaxLayerCount; //0x0154
		float m_LightRadiusScale; //0x0158
		float m_LightAttenuationThreshold; //0x015C
		unsigned __int32 m_Ps3LightTileMethod; //0x0160
		float m_Ps3LightTileJobYieldGranularity; //0x0164
		unsigned __int32 m_OutdoorLightTileBatchCount; //0x0168
		unsigned __int32 m_XenonDrawDebugLightTileGridMode; //0x016C
		float m_LightStencilMinArea; //0x0170
		unsigned __int32 m_MaxSpotLightCount; //0x0174
		MipmapFilterMode m_SkyEnvmapFilterMode; //0x0178
		float m_SkyEnvmapFilterWidth; //0x017C
		unsigned __int32 m_GBufferTestCount; //0x0180
		unsigned __int32 m_SkyEnvmapResolution; //0x0184
		unsigned __int32 m_SkyEnvmapSidesPerFrameCount; //0x0188
		float m_SubSurfaceRolloffLocalLight; //0x018C
		__int32 m_OnlyTileIndex; //0x0190
		bool m_OccluderMeshZPrepassEnable; //0x0194
		bool m_OccluderMeshZPrepassDebugEnable; //0x0195
		bool m_WireframeEnable; //0x0196
		bool m_HalfResEnable; //0x0197
		bool m_HalfResLensFlaresEnable; //0x0198
		bool m_ZPassEnable; //0x0199
		bool m_ForegroundEnable; //0x019A
		bool m_TiledHalfResCompositeEnable; //0x019B
		bool m_ForegroundZPassEnable; //0x019C
		bool m_DrawDebugVelocityBuffer; //0x019D
		bool m_ForegroundDepthClearEnable; //0x019E
		bool m_ForegroundAsMainEnable; //0x019F
		bool m_CustomEnvmapEnable; //0x01A0
		bool m_SkyEnvmapEnable; //0x01A1
		bool m_DrawDebugVisibleEntityTypes; //0x01A2
		bool m_DrawDebugHalfResEnvironment; //0x01A3
		bool m_DrawSolidBoundingBoxes; //0x01A4
		bool m_BilateralHalfResCompositeEnable; //0x01A5
		bool m_ForegroundTransparentEnable; //0x01A6
		bool m_TiledHalfResStencilOccludersEnable; //0x01A7
		bool m_SkyEnvmapUpdateEnable; //0x01A8
		bool m_SkyEnvmapForceUpdateEnable; //0x01A9
		bool m_SkyEnvmapUse8BitTexture; //0x01AA
		bool m_DynamicEnvmapEnable; //0x01AB
		bool m_ScreenEffectEnable; //0x01AC
		bool m_DrawDebugZBufferEnable; //0x01AD
		bool m_MotionBlurStencilPassEnable; //0x01AE
		bool m_FinalPostEnable; //0x01AF
		bool m_DrawDebugQuarterShadowmaps; //0x01B0
		bool m_DrawDebugSkyEnvmap; //0x01B1
		bool m_DxShadowmap16BitEnable; //0x01B2
		bool m_DrawDebugTransShadowmap; //0x01B3
		bool m_DrawDebugSpotLightShadowmaps; //0x01B4
		bool m_DrawShadowFrustums; //0x01B5
		bool m_DrawDebugShadowmaps; //0x01B6
		bool m_SkyEnable; //0x01B7
		bool m_DrawBoundingSpheres; //0x01B8
		bool m_DrawFrustums; //0x01B9
		bool m_StaticEnvmapEnable; //0x01BA
		bool m_SkyFogEnable; //0x01BB
		bool m_DrawLineBoundingBoxes; //0x01BC
		bool m_DrawDebugDestructionVolumes; //0x01BD
		bool m_LensFlaresEnable; //0x01BE
		bool m_DrawDebugLensFlareOccluders; //0x01BF
		bool m_DrawDebugLensFlares; //0x01C0
		bool m_LensFlareOcclusionEnable; //0x01C1
		bool m_DrawDebugWorldOcclusions; //0x01C2
		bool m_CloudShadowEnable; //0x01C3
		bool m_OverrideDynamicAO; //0x01C4
		bool m_DrawDebugDynamicAO; //0x01C5
		bool m_EmissiveEnable; //0x01C6
		bool m_UnlitEnable; //0x01C7
		bool m_SkyEnvmapMipmapGenEnable; //0x01C8
		bool m_DrawLightSources; //0x01C9
		bool m_GBufferClearEnable; //0x01CA
		bool m_DxGBufferLight16BitEnable; //0x01CB
		bool m_DxGBufferNormal16BitEnable; //0x01CC
		bool m_GBufferAlphaTestSimpleEnable; //0x01CD
		bool m_SpecularLightingEnable; //0x01CE
		bool m_SkinLightingEnable; //0x01CF
		bool m_TranslucencyLightingEnable; //0x01D0
		bool m_DynamicEnvmapLightingEnable; //0x01D1
		bool m_OutdoorLightEnable; //0x01D2
		bool m_PointLightsEnable; //0x01D3
		bool m_LineLightsEnable; //0x01D4
		bool m_ConeLightsEnable; //0x01D5
		bool m_SpotLightsEnable; //0x01D6
		bool m_LightProbesEnable; //0x01D7
		bool m_LightStencilMethodEnable; //0x01D8
		bool m_LightVolumeMethodEnable; //0x01D9
		bool m_LightVolumeDepthTestEnable; //0x01DA
		bool m_OutputGammaCorrectionEnable; //0x01DB
		bool m_DrawDebugMultisampleClassify; //0x01DC
		bool m_OutdoorKeyLightEnable; //0x01DD
		bool m_OutdoorSkyLightEnable; //0x01DE
		bool m_OutdoorLightSpecularEnable; //0x01DF
		bool m_OutdoorLightTilingEnable; //0x01E0
		bool m_OutdoorLightTileRenderEnable; //0x01E1
		bool m_OutdoorLightTileBlendEnable; //0x01E2
		bool m_OutdoorLightTileSimpleShaderEnable; //0x01E3
		bool m_Ps3TiledHalfTargetEnable; //0x01E4
		bool m_XenonOutdoorLightTilingEnable; //0x01E5
		bool m_XenonPointLightsTilingEnable; //0x01E6
		bool m_XenonSpotLightsTilingEnable; //0x01E7
		bool m_XenonLineLightsTilingEnable; //0x01E8
		bool m_XenonAsyncLightJobsEnable; //0x01E9
		bool m_XenonLightTileDownsampleSpecularCheckEnable; //0x01EA
		bool m_XenonLightTileCpuPointLightCullingEnable; //0x01EB
		bool m_Ps3TiledSSAOTargetsEnable; //0x01EC
		bool m_Ps3SCullEnable; //0x01ED
		bool m_Ps3ColorCompressionEnable; //0x01EE
		bool m_Enable; //0x01EF
		bool m_Ps3TilingEnable; //0x01F0
		bool m_Ps3DepthBoundsLightCullingEnable; //0x01F1
		bool m_Ps3ZCullEnable; //0x01F2
		bool m_ConsoleRenderTargetPoolSharingEnable; //0x01F3
		bool m_XenonFloatDepthBufferEnable; //0x01F4
		bool m_Ps3LightTileJobYieldEnable; //0x01F5
		bool m_Ps3HdrClearEnable; //0x01F6
		bool m_XenonFastHdrEnable; //0x01F7
		bool m_ShadowmapViewDistanceScaleEnable; //0x01F8
		bool m_Ps3RenderTargetPoolGBufferOffsetEnable; //0x01F9
		bool m_HdrEnable; //0x01FA
		bool m_SpotLightShadowmapEnable; //0x01FB
		bool m_SimpleShadowmapsEnable; //0x01FC
		bool m_DrawTransparent; //0x01FD
		bool m_DrawTransparentDecal; //0x01FE
		bool m_ShadowmapFixedDepthEnable; //0x01FF
		bool m_DrawDebugDynamicEnvmap; //0x0200
		bool m_MotionBlurGeometryPassEnable; //0x0201
		bool m_DxLinearDepth32BitFormatEnable; //0x0202
		bool m_DynamicEnvmapMipmapGenEnable; //0x0203
		bool m_MotionBlurEnable; //0x0204
		bool m_ColoredShadowmapSlicesEnable; //0x0205
		bool m_DxDeferredCsPathEnable; //0x0206
		bool m_DrawDebugGBuffer; //0x0207
		bool m_DrawDebugLightSources; //0x0208
		bool m_DrawDebugLightShadowSources; //0x0209
		bool m_DrawDebugLightStats; //0x020A
		bool m_DrawDebugLightTiles; //0x020B
		bool m_DrawDebugLightTileVolumes; //0x020C
		bool m_DrawDebugLightTileFbGrid; //0x020D
		bool m_DrawDebugLightTileGrid; //0x020E
		bool m_DrawDebugLightTileSources; //0x020F
		bool m_ApplyShadowmapsEnable; //0x0210
		bool m_GenerateShadowmapsEnable; //0x0211
		bool m_DxSpotLightShadowmap16BitEnable; //0x0212
		bool m_Ps3Shadowmap16BitEnable; //0x0213
		bool m_Ps3SpotLightShadowmap16BitEnable; //0x0214
		bool m_GenericEntityRendererEnable; //0x0215
		bool m_SetupJobEnable; //0x0216
		bool m_ShadowmapAccumBilinearEnable; //0x0217
		bool m_LightWidthEnable; //0x0218
		bool m_LightIntensityNormalizationEnable; //0x0219
		bool m_LightCullEnable; //0x021A
		bool m_LightDepthCullEnable; //0x021B
		bool m_LightNormalCullEnable; //0x021C
		bool m_LightConeCullEnable; //0x021D
		bool m_LightTileOverlayEnable; //0x021E
		bool m_Ps3ShadowmapTilingEnable; //0x021F
		bool m_LightAttenuationThresholdEnable; //0x0220
		bool m_SkyVisibilityEnvmapScalingEnable; //0x0221
		bool m_ZBufferShadowTestEnable; //0x0222
		bool m_ShadowmapsEnable; //0x0223
		bool m_ShadowmapAccumEnable; //0x0224
		bool m_ShadowmapCullVolumeEnable; //0x0225
		bool m_OpaqueSortBySolutionEnable; //0x0226
		bool m_MainOpaqueZPassEnable; //0x0227
		bool m_DrawDebugDecalVolumes; //0x0228
		bool m_PlanarReflectionEnable; //0x0229
		bool m_TransparencyShadowmapsEnable; //0x022A
		bool m_DynamicCullDrawOccludedBoxesEnable; //0x022B
		bool m_PlanarReflectionBlur; //0x022C
		bool m_DrawDebugPlanarReflectionCullFrustum; //0x022D
		bool m_FreezePlanarReflectionCullFrustum; //0x022E
		bool m_PlanarReflectionDebugCullMode; //0x022F
		bool m_ShadowmapFixedMovementEnable; //0x0230
		bool m_DecalVolumeEnable; //0x0231
		bool m_DrawDebugPlanarReflection; //0x0232
		bool m_FxaaEnable; //0x0233
		bool m_DynamicCullSpuJobEnable; //0x0234
		bool m_DynamicCullZBufferTestEnable; //0x0235
		bool m_ViewFxEnable; //0x0236
		char _0x0237[9];
	};//Size=0x0240

	class WorldRenderer
	{
	public:
		class RootView
		{
		public:
			DWORD index;
			/*WorldDrawViewInfo* */void* viewInfo;
		};
		virtual int addRef(); // V: 0x0
		virtual int release(); // V: 0x4
		virtual void createUpdateJob(); // (const struct fb::WorldRenderUpdateParams &, class EA::Jobs::JobInstanceHandle &, class EA::Jobs::JobInstanceHandle &); // V: 0x8
		virtual void draw(); // V: 0xC
		virtual void setSettings(class WorldRenderSettings*); // V: 0x10
		virtual const class WorldRenderSettings* getSettings(); // V: 0x14
		virtual void addModule();// (struct fb::IWorldRenderModule *); // V: 0x18
		virtual void removeModule(); // (struct fb::IWorldRenderModule *); // V: 0x1C
		virtual void addPrimitiveRenderer(); // (struct fb::IWorldPrimitiveRenderer *); // V: 0x20
		virtual void removePrimitiveRenderer(); // (struct fb::IWorldPrimitiveRenderer *); // V: 0x24
		virtual class ITexture* getHDRRenderTargetColorTexture(unsigned int); // V: 0x28
		virtual class ITexture* getGeometryBufferTextures(); // V: 0x2C
		virtual unsigned int getGeometryBufferTextureCount(); // V: 0x30
		virtual class PostProcessSystem* getPostProcessSystem(); // V: 0x34
		virtual DWORD getHudRenderTargetView(); // V: 0x38
		virtual class fb::ITexture* getHudRenderTargetTexture(); // V: 0x3C
		virtual DWORD getWorldOcclusionQueryModule(); // V: 0x40
		char _0x0004[32];
		DWORD m_viewWidth; //0x0024
		DWORD m_viewHeight; //0x0028
		char _0x002C[0x895C];
		WorldRenderSettings* m_worldRenderSettings; //0x8988
	};

	class WorldRenderModule
	{
	public:
		char _0x0000[24];
		class WorldRenderer* m_worldRenderer;
		bool m_fmvPlaying;
		bool m_worldEnable;
		char pad[0x2];
		class EnlightenRenderer* m_enlightenRenderer;
	};

	class EnlightenRenderer
	{
	public:
		enum EnlightenState : __int32
		{
			EnlightenState_Dynamic = 0x0,
			EnlightenState_Static = 0x1,
			EnlightenState_Disabled = 0x2,
		};

		char pad_0000[12]; //0x0000
		EnlightenState state; //0x000C
		char pad_0010[472]; //0x0010
		class EnlightenRuntimeSettings* m_runtimeSettings; //0x01E8
	};

	class SystemSettings : public DataContainer
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 2444;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x023C2760;
		}
		char* m_Name; //0x0008
	};//Size=0x000C

	class EnlightenRuntimeSettings : public SystemSettings
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 2445;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x02405A88;
		}
		char _0x000C[4];
		Vec3 m_AlbedoDefaultColor; //0x0010
		float m_LocalLightForceRadius; //0x0020
		__int32 m_DrawDebugSystemDependenciesEnable; //0x0024
		float m_TemporalCoherenceThreshold; //0x0028
		float m_SkyBoxScale; //0x002C
		unsigned __int32 m_MinSystemUpdateCount; //0x0030
		unsigned __int32 m_JobCount; //0x0034
		__int32 m_DrawDebugSystemBoundingBoxEnable; //0x0038
		unsigned __int32 m_LightProbeMaxUpdateSolveCount; //0x003C
		float m_DrawDebugLightProbeSize; //0x0040
		bool m_CompensateSunShadowHeightScale; //0x0044
		bool m_SaveRadiosityTexturesEnable; //0x0045
		bool m_ShadowsEnable; //0x0046
		bool m_LightMapsEnable; //0x0047
		bool m_LocalLightsEnable; //0x0048
		bool m_LocalLightCullingEnable; //0x0049
		bool m_LocalLightCustumFalloff; //0x004A
		bool m_LightProbeForceUpdate; //0x004B
		bool m_ForceDynamic; //0x004C
		bool m_DrawDebugSystemsEnable; //0x004D
		bool m_LightProbeEnable; //0x004E
		bool m_LightProbeJobsEnable; //0x004F
		bool m_DrawDebugLightProbes; //0x0050
		bool m_DrawDebugLightProbeOcclusion; //0x0051
		bool m_DrawDebugLightProbeStats; //0x0052
		bool m_DrawDebugLightProbeBoundingBoxes; //0x0053
		bool m_Enable; //0x0054
		bool m_DrawSolveTaskPerformance; //0x0055
		bool m_DrawDebugColoringEnable; //0x0056
		bool m_DrawDebugTextures; //0x0057
		bool m_DrawDebugBackFaces; //0x0058
		bool m_DrawDebugTargetMeshes; //0x0059
		bool m_DrawWarningsEnable; //0x005A
		bool m_AlbedoForceUpdateEnable; //0x005B
		bool m_AlbedoForceColorEnable; //0x005C
		bool m_DrawDebugEntities; //0x005D
		bool m_TerrainMapEnable; //0x005E
		bool m_EmissiveEnable; //0x005F
	};//Size=0x0060

	class ClientLevel
	{
	public:
		char pad[0x24];
		class LevelData* m_levelData;
		char pad2[0x90];
		class WorldRenderModule* m_worldRenderModule;
		void* physics;
		class GameWorld* m_gameWorld;
	};

	class LevelData
	{
	public:
		char pad[0x8];
		char* name; // fb::asset
	};

	class ClientGameContext : public GameContext
	{
	public:
		static ClientGameContext* GetInstance()
		{
			return *(ClientGameContext**)(OFF_g_clientGameContext);
		}

		class ClientPlayerManager* m_clientPlayerManager; // 0x30
	};

	class ClientPlayerManager
	{
	public:
		char pad[0xBC];
		class ClientPlayer* m_localPlayer;
	};

	class ClientPlayer
	{
	public:
		char pad[0x3D8];
		void* m_controlledControllable; // 0x3D8

		bool isAlive()
		{
			return m_controlledControllable != nullptr;
		}
	};

	class ClientRoundOverEntity
	{
	public:
		char pad[0x60];
		long double m_startTime;
		float m_roundTime;
		bool m_started;
	};

	class VisualEnvironmentManager
	{
	public:
		static VisualEnvironmentManager* GetInstance()
		{
			return *(VisualEnvironmentManager**)(OFF_g_visualEnvironmentManager);
		}

		char pad[0x20];
		eastl::vector<VisualEnvironmentState*> m_states;

		void setDirty(bool dirty)
		{
			*(bool*)((char*)this + 0x18C0) = dirty;
		}

		fb::VisualEnvironment& getEnv()
		{
			return *(VisualEnvironment*)((char*)this + 0x1A0);
		}
	};

	class EntityBusPeer : public ITypedObject
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 226;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x023C6FA0;
		}
		//char _0x0000[4];
	};//Size=0x0004

	class Entity : public EntityBusPeer
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 399;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x023C6F50;
		}
		char _0x0004[8];
	};//Size=0x000C

	template <typename T>
	class EntityWithBusAndData : public Entity
	{
	public:
		class EntityBus* m_entityBus;
		T* m_data;
	};

	class SpatialEntity : public Entity
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 611;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x023C4A70;
		}
		char _0x000C[4];
	};//Size=0x0010

	template <class T>
	class SpatialEntityWithBusAndData : public SpatialEntity // 0x00
	{
	public:
		EntityBus* m_entityBus; // 0x10
		T* m_data; // 0x14
	}; // 0x18

	class LocalLightEntity : public SpatialEntityWithBusAndData<LocalLightEntityData>
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 613;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x02404E80;
		}
		char _0x0010[144];

		LinearTransform& transform()
		{
			return *reinterpret_cast<LinearTransform*>(reinterpret_cast<char*>(this) + 0x20);
		}
		const Vec3& position() { return transform().m_trans; }

		void setDirty()
		{
			typedef __int16(__thiscall* tSetDirty)(void* _this);
			tSetDirty oSetDirty = (tSetDirty)OFF_LocalLightEntity_setDirty;

			oSetDirty(this);
		}
	};//Size=0x00A0

	class EntityCreator
	{
	public:
		enum RayCastTest
		{
			RCTDetailed = 0,
			RCTCollision
		};
		LPVOID vftable; // 0x00
		EntityCreator* m_previousCreator; // 0x04
		EntityCreator* m_nextCreator; // 0x08
		Realm m_realm; // 0x0C
		INT m_linked; // 0x10
	}; // 0x14

	class EntityCollectionSegment;

	class EntityCollection
	{
	public:
		class EntityCollectionSegment* firstSegment; // 0x0
		class EntityCreator* creator; // 0x4
	};

	class EntityCollectionSegment
	{
	public:
		eastl::vector<EntityCollection*> m_Collection;
		void* m_subLevel; // 0x10
		EntityCollectionSegment* m_prev; // 0x14
		EntityCollectionSegment* m_next; // 0x18
		DWORD m_iterableSize; // 0x1C
		DWORD m_collectionIndex; // 0x20
	};

	class EntityWorld
	{
	public:
		class EntityIterator
		{
		public:
			eastl::vector<EntityCollection>* m_collections; //offset = 0x0, length = 0x4
			EntityCollectionSegment* m_currentSegment; //offset = 0x4, length = 0x4
			unsigned int m_collectionIndexIt; //offset = 0x8, length = 0x4
			unsigned int m_collectionIndexEnd; //offset = 0xC, length = 0x4
			unsigned int m_entityIndexIt; //offset = 0x10, length = 0x4
			unsigned int m_entityIndexEnd; //offset = 0x14, length = 0x4
			bool m_onlyIncludeIterable; //offset = 0x18, length = 0x1
			char pad_19[3];
		};

		void kindOfQuery(class ClassInfo* classInfo, EntityIterator* result, bool onlyIncludeIterable = true)
		{
			typedef void(__thiscall* kindOfQuery_t)(void*, ClassInfo*, EntityIterator*, bool);
			kindOfQuery_t m_kindOfQuery = (kindOfQuery_t)OFF_EntityWorld_kindOfQuery;

			m_kindOfQuery(this, classInfo, result, onlyIncludeIterable);
		}
	};

	struct RayCastHit
	{
		Vec3 m_position; //0x0000 (16)
		Vec3 m_normal; //0x0010 (16)
		void* m_rigidBody; //0x0020 PhysicsEntityBase* (GroupPhysicsEntity for world geometry)
		void* m_material; //0x0024 MaterialContainerPair*
		unsigned __int32 m_part; //0x0028
		unsigned __int32 m_bone; //0x002C
		float m_lambda; //0x0030
		char _0x0034[0x0C];
	};//Size=0x0040

	// fixed_vector
	struct PhysicsExcludeList
	{
		void** m_begin; //0x0000
		void** m_end; //0x0004
		void** m_capacity; //0x0008
		void* m_allocator; //0x000C
		void* m_data[8]; //0x0010
	};//Size=0x0030

	constexpr intptr_t kGameWorldRayCaster = 0x840;
	// flags the soldier ground probe uses (sub_60D870, OnGroundState::update); start the ray outside the capsule
	constexpr unsigned int kRayCastWorldOnly = 0x14u;
	constexpr intptr_t kClientLevelPhysicsManager = 0xBC;
	constexpr intptr_t kHavokPhysicsRayCaster = 0x40;

	inline void* clientGameWorld()
	{
		ClientGameContext* ctx = ClientGameContext::GetInstance();
		if (ctx && ctx->m_level && ctx->m_level->m_gameWorld)
			return ctx->m_level->m_gameWorld;
		return *reinterpret_cast<void**>(OFF_g_entityWorld);
	}

	inline void* physicsRayCaster()
	{
		ClientGameContext* ctx = ClientGameContext::GetInstance();
		if (ctx && ctx->m_level)
		{
			void* manager = *reinterpret_cast<void**>(reinterpret_cast<uint8_t*>(ctx->m_level) + kClientLevelPhysicsManager);
			void* caster = manager ? *reinterpret_cast<void**>(static_cast<uint8_t*>(manager) + kHavokPhysicsRayCaster) : nullptr;
			if (caster)
				return caster;
		}
		void* gw = clientGameWorld();
		return gw ? static_cast<uint8_t*>(gw) + kGameWorldRayCaster : nullptr;
	}

	inline bool physicsRayQuery(const Vec3& from, const Vec3& to, RayCastHit& hit, unsigned int flags,
	                            void* exclude = nullptr)
	{
		void* caster = physicsRayCaster();
		if (!caster)
			return false;
		using fn_t = bool (__thiscall*)(void*, const char*, const void*, const void*, RayCastHit*, unsigned int, const void*);
		fn_t fn = (*static_cast<fn_t**>(caster))[0];
		alignas(16) float a[4] = { from.m_x, from.m_y, from.m_z, 0.0f };
		alignas(16) float b[4] = { to.m_x, to.m_y, to.m_z, 0.0f };
		PhysicsExcludeList ex{ };
		ex.m_begin = ex.m_data;
		ex.m_end = ex.m_data;
		ex.m_capacity = ex.m_data + 8;
		if (exclude)
			*ex.m_end++ = exclude;
		return fn(caster, "bfve", a, b, &hit, flags, exclude ? &ex : nullptr);
	}

	template<typename T> struct EntityList : public EntityWorld::EntityIterator
	{
		EntityList() {}
		EntityList(fb::ClassInfo* classInfo, bool onlyIncludeIterable = true)
		{
			EntityWorld* manager = static_cast<EntityWorld*>(clientGameWorld()); // dword_236B60C, see physicsRayCaster
			if (!manager)
				return;

			manager->kindOfQuery(classInfo, this, onlyIncludeIterable);
		}

		T* nextOfType()
		{
			while (m_currentSegment)
			{
				if (m_entityIndexIt < m_entityIndexEnd)
				{
					T* entity = reinterpret_cast<T*>(m_currentSegment->m_Collection.at(m_entityIndexIt));
					m_entityIndexIt++;
					if (entity)
						return entity;
				}
				else
				{
					m_currentSegment = m_currentSegment->m_next;
					if (m_currentSegment)
					{
						m_entityIndexIt = 0;
						m_entityIndexEnd = m_onlyIncludeIterable
							? m_currentSegment->m_iterableSize
							: static_cast<DWORD>(m_currentSegment->m_Collection.size());
					}
				}
			}
			return nullptr;
		}

		T* nextOfKind()
		{
			if (m_collectionIndexIt >= m_collectionIndexEnd)
				return nullptr;

			while (true)
			{
				T* result = nextOfType();
				if (result)
					return result;

				m_collectionIndexIt++;
				if (m_collectionIndexIt >= m_collectionIndexEnd)
					return nullptr;

				EntityCollectionSegment* firstSegment =
					(m_collections->begin() + m_collectionIndexIt)->firstSegment;

				if (firstSegment)
				{
					m_currentSegment = firstSegment;
					m_entityIndexIt = 0;
					m_entityIndexEnd = m_onlyIncludeIterable
						? firstSegment->m_iterableSize
						: static_cast<DWORD>(firstSegment->m_Collection.size());
				}
				else
				{
					m_currentSegment = nullptr;
					m_entityIndexIt = 0;
					m_entityIndexEnd = 0;
				}
			}
		}
	};

	struct DomainInfo
	{
		eastl::string name; // 0x00
		eastl::string root; // 0x10
		uint32_t flags; // 0x20 bitfield
		char imports[16]; // 0x24 vector<DomainImportInfo>
		char emergencyMapping[16]; // 0x34 vector<TypeRemapInfo>
	}; // 0x44
	static_assert(sizeof(DomainInfo) == 0x44, "DomainInfo");

	class InternalDatabaseDomain;

	class InternalDatabasePartition
	{
	public:
		void* vtable; // 0x00
		const char* m_name; // 0x04 partition path
		InternalDatabaseDomain* m_domain; // 0x08
		Guid m_partitionGuid; // 0x0C
		char m_instanceFastLookup[0x20]; // 0x1C hash_map<Guid const*, DataContainer*>
		eastl::vector<fb::DataContainer*> m_instances; // 0x3C
		DataContainer* m_primaryInstance; // 0x4C
		void* m_ownedMemoryBlock; // 0x50
	}; // 0xB4
	static_assert(offsetof(InternalDatabasePartition, m_instances) == 0x3C, "InternalDatabasePartition");

	class InternalDatabaseDomain
	{
	public:
		void* vtable; // 0x00
		void* m_arena; // 0x04
		char pad_0008[0x3C]; // 0x08 lazy allocator, lazy partition list, flags, manager
		char m_guidMap[0x20]; // 0x44 hash_map<Guid const*, InternalDatabasePartition*>
		char m_nameMap[0x20]; // 0x64 hash_map<char const*, InternalDatabasePartition*>
		eastl::vector<InternalDatabasePartition*> m_partitions; // 0x84
	}; // 0x128
	static_assert(offsetof(InternalDatabaseDomain, m_partitions) == 0x84, "InternalDatabaseDomain");

	class ResourceManager
	{
	public:
		class Compartment
		{
		public:
			class StringMap
			{
			public:
				void* vtable; //0x0000
				void* m_arena; //0x0004
				void* m_root; //0x0008
				uint32_t m_stringCount; //0x000C
				uint32_t m_caseSensitivity; //0x0010
				uint32_t m_locked; //0x0014
			};//Size=0x0018

			void* m_realHeap; // 0x00
			char pad_0004[0x2C]; // 0x04 lock
			StringMap m_handles; // 0x30 StringMap<ResourceHandle*>, lower-case paths
			char pad_0048[0x08]; // 0x48 thisTag
			InternalDatabaseDomain* m_domain; // 0x50
			DomainInfo m_domainInfo; // 0x54
			char m_domainName[64]; // 0x98 "Compartment:%s(%d)"
			eastl::vector<void*> m_pendingPostloads; // 0xD8 IResourceObject*
			eastl::vector<fb::ITypedObject*> m_objects; // 0xE8 m_dataContainersToUnregister: the primary instance of every loaded partition
			char pad_00F8[0x40]; // 0xF8 availableChunks, queuedReads, batchObjects, affectedObjects
			eastl::vector<fb::DataContainer*> m_dataContainers; // 0x138 emptied at the end of every fixup
		}; // 0x2A0
		static_assert(offsetof(Compartment, m_handles) == 0x30, "Compartment");
		static_assert(offsetof(Compartment, m_objects) == 0xE8, "Compartment");
		static_assert(offsetof(Compartment, m_dataContainers) == 0x138, "Compartment");

	public:
		volatile uint32_t m_bundleLoadInProgress; // 0x00
		std::array<ResourceManager::Compartment*, 205> m_compartments; // 0x04 allocCompartment bound 0xCD
		Compartment* m_currentlyLoadingCompartment; // 0x338

		static ResourceManager* GetInstance()
		{
			return *(ResourceManager**)(OFF_g_resourceManager);
		}
	};

	class SpotLightEntity : public LocalLightEntity
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 614;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x02404ED0;
		}
		char _0x00A0[16];
	};//Size=0x00B0

	class Message : public ITypedObject
	{
	public:
		virtual void* dtor(void*);
		// non retail only
		//virtual const char* getMessageName();

		DWORD m_Category;
		DWORD m_Type;
		char pad_000C[20];
		bool m_ownedByMessageManager;
		char pad_0021[7];
	};

	struct ClientEntityFactoryParams
	{
		char pad[0xB0];
		fb::GameObjectData* data;
	};

	class DataContext
	{
		/*const class fb::DataBus **/void* bus; // this+0x0
		const class fb::DataContainer* const data; // this+0x4
		const class fb::DataContainer* const exposed; // this+0x8

	}; // fb::DataContext

	// ctor sub_17B2720, shaders sub_17B0600
	class LensFlareEntity : public EntityWithBusAndData<LensFlareEntityData>
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 404;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x02404D68;
		}

		char _0x0014[12];
		LinearTransform m_transform; //0x0020
		eastl::vector<void*> m_surfaceShaders; //0x0060 SmartRef<ISurfaceShader const> per element
		bool m_enabled; //0x0070
		bool m_halfRes; //0x0071
		bool m_directionEnable; //0x0072
		char _0x0073[13];
		Vec3 m_direction; //0x0080
	};//Size=0x0090

	class StaticModelGroupMeshInstance
	{
	public:
		static constexpr uintptr_t kRigidVTable = OFF_vt_RigidMeshEntity;
		static constexpr uintptr_t kCompositeVTable = OFF_vt_CompositeMeshEntity;

		void* m_vtable; //0x0000
		char _0x0004[12];
		LinearTransform m_transform; //0x0010
	};

	// Physics part -> its mesh instance; sub_6053C0 writes m_instance, sub_605270 reads
	// m_transformIndex.
	struct StaticModelGroupPartEntry
	{
		StaticModelGroupMeshInstance* m_instance; //0x0000
		uint16_t m_transformIndex; //0x0004
		uint16_t _0x0006;
	};//Size=0x0008

	// sub_11D4850
	class StaticModelGroupPhysics
	{
	public:
		char _0x0000[0x1C];
		uint32_t m_flags; //0x001C bit 30: every part maps to entry 0
		char _0x0020[0x64];
		StaticModelGroupPartEntry* m_partTable; //0x0084 GroupPhysicsEntity::m_parts begin
		StaticModelGroupPartEntry* m_partTableEnd; //0x0088
	};

	// fb::ClientStaticModelGroupMember ctor sub_10B6250
	class ClientStaticModelGroupMember
	{
	public:
		static constexpr uintptr_t kVTable = OFF_vt_ClientStaticModelGroupMember;

		void* m_vtable; //0x0000
		char _0x0004[8];
		StaticModelGroupMemberData* m_data; //0x000C
		StaticModelGroupPhysics* m_physics; //0x0010
		char _0x0014[0x1C];
		StaticModelGroupMeshInstance** m_entitiesWithoutPhysics;//0x0030 only when m_InstanceTransforms is set
		void* m_meshSet; //0x0034
	};//Size=0x0038

	class ClientStaticModelGroupEntity : public SpatialEntityWithBusAndData<StaticModelGroupEntityData>
	{
	public:
		static __inline unsigned int ClassId() { return 654; }
		static __inline uintptr_t ClassInfoPtr() { return 0x023EB5DC; }

		char _0x0018[0x54];
		StaticModelGroupEntityData* m_groupData; //0x006C
		StaticModelGroupPhysics* m_physics; //0x0070
		ClientStaticModelGroupMember* m_membersBegin; //0x0074
		ClientStaticModelGroupMember* m_membersEnd; //0x0078
		ClientStaticModelGroupMember* m_membersCap; //0x007C
		char _0x0080[8];
	};//Size=0x0088

	struct DebugRenderVertex
	{
		float x, y, z; //0x0000
		unsigned int color; //0x000C 0xAABBGGRR
		float nx, ny, nz; //0x0010
		unsigned int pad; //0x001C
	};//Size=0x0020

	class DebugRenderer2
	{
	public:
		enum Geometry { Triangles3d = 0, Lines3d = 1, Triangles2d = 2, Lines2d = 3 };

		static DebugRenderer2* Singleton()
		{
			using fn_t = DebugRenderer2* (*)();
			return reinterpret_cast<fn_t>(OFF_DebugRenderer2_singleton)();
		}
		void setTransparent(bool v) { reinterpret_cast<void (__thiscall*)(void*, bool)>(OFF_DebugRenderer2_setTransparent)(this, v); }
		void setDoubleSided(bool v) { reinterpret_cast<void (__thiscall*)(void*, bool)>(OFF_DebugRenderer2_setDoubleSided)(this, v); }
		void setDepthTest(bool v) { reinterpret_cast<void (__thiscall*)(void*, bool)>(OFF_DebugRenderer2_setDepthTest)(this, v); }
		void setDepthWrite(bool v) { reinterpret_cast<void (__thiscall*)(void*, bool)>(OFF_DebugRenderer2_setDepthWrite)(this, v); }
		DebugRenderVertex* beginVertices(int type, unsigned int count)
		{
			using fn_t = DebugRenderVertex* (__thiscall*)(void*, int, unsigned int);
			return reinterpret_cast<fn_t>(OFF_DebugRenderer2_beginVertices)(this, type, count);
		}
		void endVertices(int type, DebugRenderVertex* end)
		{
			using fn_t = void (__thiscall*)(void*, int, DebugRenderVertex*);
			reinterpret_cast<fn_t>(OFF_DebugRenderer2_endVertices)(this, type, end);
		}
		// drawText(int x, int y, const char*, Color32, float scale): queues a Text2d command.
		void drawText(int x, int y, const char* text, unsigned int color, float scale)
		{
			using fn_t = void (__thiscall*)(void*, int, int, const char*, unsigned int, float);
			reinterpret_cast<fn_t>(OFF_DebugRenderer2_drawText)(this, x, y, text, color, scale);
		}
		void drawText2d(int x, int y, const char* text, unsigned int color, float scale) { drawText(x, y, text, color, scale); }
		void drawLine2d(const float a[2], const float b[2], unsigned int color)
		{
			using fn_t = void (__thiscall*)(void*, const float*, const float*, unsigned int);
			reinterpret_cast<fn_t>(OFF_DebugRenderer2_drawLine2d)(this, a, b, color);
		}
		void drawLineRect2d(const float min[2], const float max[2], unsigned int color)
		{
			using fn_t = void (__thiscall*)(void*, const float*, const float*, unsigned int);
			reinterpret_cast<fn_t>(OFF_DebugRenderer2_drawLineRect2d)(this, min, max, color);
		}
		void drawRect2d(const float min[2], const float max[2], unsigned int color)
		{
			using fn_t = void (__thiscall*)(void*, const float*, const float*, unsigned int);
			reinterpret_cast<fn_t>(OFF_DebugRenderer2_drawRect2d)(this, min, max, color);
		}
	};
	// BF4 splits 3D and 2D into two classes; the shared render backend uses both names.
	using DebugRenderer = DebugRenderer2;

	// dword_2356C90. vtable[4](this, arena, name) fetches a shader by name (sub_17B0600); the
	// arena is *(*(entity->m_entityBus + 4) + 0x34) (sub_545DC0).
	class ShaderDatabase
	{
	public:
		static ShaderDatabase* Singleton()
		{
			return *reinterpret_cast<ShaderDatabase**>(OFF_g_shaderSystem);
		}
		static constexpr size_t kFindSlot = 4;
	};

	class DxTexture;
	// DxTexture::create(arena, desc) OFF_DxTexture_create: +0x40 set = rebuild the texture at +0x44 in place.
	struct DxTextureCreateDesc
	{
		char _0x0000[0x40];
		bool m_rebuildInPlace; //0x0040
		char _0x0041[3];
		DxTexture* m_target; //0x0044
	};

	class DxTexture
	{
	public:
		static __inline void* VTable() { return reinterpret_cast<void*>(OFF_vt_DxTexture); }

		void* m_vtable; //0x0000
		unsigned __int32 m_refCount; //0x0004
		void* m_arena; //0x0008
		const char* m_name; //0x000C not written by the ctor - do not trust
		unsigned __int32 m_memoryType; //0x0010
		unsigned __int32 m_flags; //0x0014
		unsigned __int32 m_type; //0x0018
		unsigned __int32 m_format; //0x001C
		unsigned __int32 m_width; //0x0020
		unsigned __int32 m_height; //0x0024
		unsigned __int32 m_depth; //0x0028
		unsigned __int32 m_mipmapCount; //0x002C
		unsigned __int32 m_sliceCount; //0x0030
		bool m_srgb; //0x0034
		char _0x0035[3];
		unsigned __int32 m_bindFlags; //0x0038
		unsigned __int32 m_subResourceCount; //0x003C
		void* m_subResourceData; //0x0040
		void* m_streamingHeader; //0x0044
		void* m_subResourceCopy; //0x0048
		char _0x004C[8];
		unsigned __int16 m_handle; //0x0054 streaming handle, 0xFFFF = none
		char _0x0056[2];
		unsigned __int32 m_bindFlags2; //0x0058
		unsigned __int32 m_resFormat; //0x005C DXGI_FORMAT of m_resource
		unsigned __int32 m_shaderFormat; //0x0060 DXGI_FORMAT of the views
		void* m_streamingInfo; //0x0064 128 bytes, own allocation
		ID3D11Resource* m_resource; //0x0068
		void* m_data; //0x006C
		ID3D11ShaderResourceView* m_shaderViews[2]; //0x0070 [0] linear, [1] sRGB
		bool m_ownsResource; //0x0078
		char _0x0079[3];
	};//Size=0x007C

	struct TextureStreamingEntry
	{
		DxTexture* m_texture; //0x0000
		unsigned __int32 m_dataHandle; //0x0004
		unsigned __int32 m_preAllocatedHandle; //0x0008
		unsigned __int16 m_compartmentCount; //0x000C
		unsigned __int16 m_mipRefCount[15]; //0x000E
		unsigned __int8 m_mipCount; //0x002C sourceMipmapCount
		unsigned __int8 m_origMinMip; //0x002D
		unsigned __int8 m_minMip; //0x002E
		unsigned __int8 m_activeMipmapCount; //0x002F
		unsigned __int8 m_activeMipmapIndex; //0x0030
		unsigned __int8 m_wantedMipmapCount; //0x0031
		unsigned __int8 m_state; //0x0032 pool; 2 = on-demand
		bool m_loading; //0x0033
		bool m_forceLoaded; //0x0034
		char _0x0035[3];
		float m_diffMipmapCount; //0x0038
		float m_wantedMinMipClamp; //0x003C
		float m_activeMinMipClamp; //0x0040
	};//Size=0x0044

	// dword_23577EC, 672128 bytes (ctor sub_6D0300). m_textures is a fixed_vector<TextureInfo,
	// 8192> at 0x30 (begin/end/cap/alloc, buffer at 0x40).
	class TextureStreamingManager
	{
	public:
		void* m_arena; //0x0000
		char m_lock[0x2C]; //0x0004 sub_46FF80 / sub_46FF90
		TextureStreamingEntry* m_begin; //0x0030
		TextureStreamingEntry* m_end; //0x0034
		TextureStreamingEntry* m_capacity; //0x0038
		char _0x003C[4];
		TextureStreamingEntry m_entries[0x2000];//0x0040
		char _0x88040[0x1C0AC];
		unsigned __int32 m_onDemandBudgetUsed; //0xA40EC m_reservedPoolSize[2]

		static TextureStreamingManager* Singleton()
		{
			return *reinterpret_cast<TextureStreamingManager**>(OFF_g_textureStreamingManager);
		}

		TextureStreamingEntry* entry(unsigned __int16 handle)
		{
			return (handle & 0x7FFF) == handle ? &m_begin[handle] : nullptr;
		}

		// 0 not loaded, 1 loading, 2 loaded, 3 not on-demand
		int getOnDemandStatus(unsigned __int16 handle)
		{
			using fn_t = int(__thiscall*)(TextureStreamingManager*, unsigned __int16);
			return reinterpret_cast<fn_t>(OFF_TextureStreamingManager_getOnDemandStatus)(this, handle);
		}
		bool loadOnDemand(unsigned __int16 handle, bool)
		{
			using fn_t = bool(__thiscall*)(TextureStreamingManager*, unsigned __int16);
			return reinterpret_cast<fn_t>(OFF_TextureStreamingManager_loadOnDemand)(this, handle);
		}
		bool unloadOnDemand(unsigned __int16 handle, bool)
		{
			using fn_t = bool(__thiscall*)(TextureStreamingManager*, unsigned __int16);
			return reinterpret_cast<fn_t>(OFF_TextureStreamingManager_unloadOnDemand)(this, handle);
		}
	};

	// fb::RelocPtr<T>: pointer in the low dword, 8 bytes in the record.
	struct RelocPtr
	{
		void* m_ptr; //0x0000
		unsigned __int32 _0x0004;
		template <typename T> T* as() const { return static_cast<T*>(m_ptr); }
	};//Size=0x0008

	struct MeshSetLayout
	{
		unsigned __int32 m_meshType; //0x0000
		unsigned __int32 m_flags; //0x0004
		unsigned __int32 m_lodCount; //0x0008
		unsigned __int32 m_subsetCount; //0x000C
		Vec3 m_boundingMin; //0x0010
		Vec3 m_boundingMax; //0x0020
		RelocPtr m_lods[5]; //0x0030 MeshLayout*
		RelocPtr m_name; //0x0058
		RelocPtr m_shortName; //0x0060
		unsigned __int32 m_nameHash; //0x0068
		char _0x006C[4];
	};//Size=0x0070

	class MeshSet
	{
	public:
		void* m_vtable; //0x0000
		unsigned __int32 m_refCount; //0x0004
		void* m_arena; //0x0008
		unsigned __int16 m_handle; //0x000C
		char _0x000E[2];
		MeshSetLayout* m_layout; //0x0010
		void* m_rsxData; //0x0014
		unsigned __int32 m_cellBlockSize; //0x0018
		unsigned __int8 m_minAvailableLodIndex; //0x001C
		char _0x001D[3];
		float m_lodScale; //0x0020
		float m_cullScale; //0x0024
		void* m_geomDeclArray; //0x0028
		char _0x002C[4];
		char m_meshData[5][0x20]; //0x0030 MeshData[5]
		bool m_postLoaded; //0x00D0
		char _0x00D1[0x2F];
	};//Size=0x0100

	struct MeshData
	{
		void* m_arena; //0x0000
		void* m_vertexBuffer; //0x0004 DxRenderBuffer*
		void* m_indexBuffer; //0x0008 DxRenderBuffer*
		const void* m_vertexData; //0x000C
		const void* m_indexData; //0x0010
		char _0x0014[0x0C];
	};//Size=0x0020

#define _DxRenderBuffer_
	class DxRenderBuffer
	{
	public:
		void* m_vtable; //0x0000
		unsigned __int32 m_refCount; //0x0004
		ID3D11Buffer* m_buffer; //0x0008
		char m_desc[0xF4]; //0x000C RenderBufferCreateDesc
	};//Size=0x0100

	struct MeshSubset
	{
		RelocPtr m_geometryDecls; //0x0000
		RelocPtr m_materialName; //0x0008
		unsigned __int32 m_materialIndex; //0x0010
		unsigned __int32 m_primitiveCount; //0x0014
		unsigned __int32 m_startIndex; //0x0018
		unsigned __int32 m_vertexOffset; //0x001C
		unsigned __int32 m_vertexCount; //0x0020
		unsigned __int8 m_vertexStride; //0x0024
		unsigned __int8 m_primitiveType; //0x0025
		unsigned __int8 m_bonesPerVertex; //0x0026
		unsigned __int8 m_boneCount; //0x0027
		RelocPtr m_boneIndices; //0x0028
		unsigned __int8 m_elements[16][4]; //0x0030 usage, format, offset, stream
		unsigned __int8 m_streams[4][2]; //0x0070 stride, classification
		unsigned __int8 m_elementCount; //0x0078
		unsigned __int8 m_streamCount; //0x0079
		char _0x007A[2];
		float m_texCoordRatios[6]; //0x007C
	};//Size=0x0094

	struct MeshLayout
	{
		unsigned __int32 m_type; //0x0000
		unsigned __int32 m_subsetCount; //0x0004
		RelocPtr m_subsets; //0x0008 MeshSubset*
		char m_categorySubsetIndices[0x30]; //0x0010 RelocArray<u8>[4]
		unsigned __int32 m_flags; //0x0040
		unsigned __int32 m_indexBufferFormat; //0x0044 0 = 16-bit, 1 = 32-bit
		unsigned __int32 m_indexDataSize; //0x0048
		unsigned __int32 m_vertexDataSize; //0x004C
		unsigned __int32 m_edgePartitionBufferSize; //0x0050
		char m_dataChunkId[16]; //0x0054
		unsigned __int32 m_auxVertexIndexDataOffset; //0x0064
		RelocPtr m_embeddedEdgeData; //0x0068
		RelocPtr m_shaderDebugName; //0x0070
		RelocPtr m_name; //0x0078
		RelocPtr m_shortName; //0x0080
		unsigned __int32 m_nameHash; //0x0088
		RelocPtr m_data; //0x008C MeshData*
		char _0x0094[0x14];
		RelocPtr m_subsetPartIndices; //0x00A8
	};//Size=0x00B0

	// eastl::hash_node of MeshVariationManager::m_variations (sub_17398C0 = getMeshVariation).
	struct MeshVariationNode
	{
		unsigned __int64 m_key; //0x0000 variationHash | (MeshAsset::m_NameHash << 32)
		void* m_set; //0x0008 MeshVariationSet*
		int m_refCount; //0x000C
		MeshVariationNode* m_next; //0x0010
	};//Size=0x0018

	// dword_23B3E88, 176 bytes (ctor sub_1739850). Lock sub_46FF80(this) at +0.
	class MeshVariationManager
	{
	public:
		char m_lock[0x20]; //0x0000
		void* m_heap; //0x0020
		char _0x0024[4];
		MeshVariationNode** m_buckets; //0x0028 m_bucketCount + 1 entries, last is a sentinel
		unsigned __int32 m_bucketCount; //0x002C
		unsigned __int32 m_elementCount; //0x0030

		static MeshVariationManager* Singleton()
		{
			return *reinterpret_cast<MeshVariationManager**>(OFF_g_meshVariationManager);
		}
	};

	class MeshVariationSet
	{
	public:
		void* m_vtable; //0x0000
		unsigned __int32 m_refCount; //0x0004
		void* m_materials; //0x0008
		unsigned __int32 m_materialCount; //0x000C
	};//Size=0x0010

	class SurfaceShaderInstance
	{
	public:
		void* m_shader; //0x0000 SurfaceShader*, refcounted
		void* m_block; //0x0004 ShaderParameterBlock*
		unsigned __int16 m_blockHeader; //0x0008
		char _0x000A[2];
	};//Size=0x000C

	class PixelShaderPermutation
	{
	public:
		Guid m_guid; //0x0000
		void* m_constantFunctionBlock; //0x0010
		void* m_textureFunctionBlock; //0x0014
		void* m_constants; //0x0018 DxShaderConstants*
		ID3D11PixelShader* m_shader; //0x001C
		unsigned __int32 m_metrics; //0x0020
		const unsigned char* m_data; //0x0024 DXBC
		unsigned __int32 m_dataSize; //0x0028
	};//Size=0x002C

	class DxShaderSolution
	{
	public:
		unsigned __int64 m_stateHash; //0x0000
		unsigned __int8 m_flags; //0x0008
		unsigned __int8 m_surfaceType; //0x0009
		unsigned __int8 m_blendMode; //0x000A
		unsigned __int8 _0x000B;
		const void* m_state; //0x000C ShaderSolutionState*
		unsigned __int32 _0x0010[2];
		void* m_vertexPermutation; //0x0018
		unsigned __int32 _0x001C;
		PixelShaderPermutation* m_pixelPermutation; //0x0020
		unsigned __int32 _0x0024;
		void* m_geometryPermutation; //0x0028
		unsigned __int32 _0x002C[5];
	};//Size=0x0040

	// eastl::pair<unsigned __int64, ShaderSolution const*>, 0x10.
	struct ShaderSolutionPair
	{
		unsigned __int64 m_key; //0x0000
		DxShaderSolution* m_solution; //0x0008
		unsigned __int32 _0x000C;
	};//Size=0x0010

	class SurfaceShader
	{
	public:
		void* m_vtable; //0x0000
		unsigned __int32 m_refCount; //0x0004
		unsigned __int32 m_nameHash; //0x0008
		bool m_initialized; //0x000C
		char _0x000D[3];
		unsigned __int32 m_shaderType; //0x0010
		unsigned __int8 m_flags; //0x0014
		unsigned __int8 m_boolParameterCount; //0x0015
		unsigned __int8 m_boolParameterDefaultMask; //0x0016
		unsigned __int8 m_boolParameterRequiredMask; //0x0017
		unsigned __int32 m_boolParameterIds[8]; //0x0018
		char m_streamableTextureSet[0x0C]; //0x0038
		ShaderSolutionPair* m_solutionPairs; //0x0044
		unsigned __int32 m_solutionPairCount; //0x0048
		unsigned __int32 _0x004C; //0x004C retail: no debug name, object is 0x50 (sub_6A46B0 allocs 80)
	};//Size=0x0050

	class ShaderParameterBlock
	{
	public:
		struct Entry
		{
			unsigned __int32 m_handle; //0x0000
			unsigned __int16 m_offset; //0x0004 from m_entries
			unsigned __int16 m_arraySize; //0x0006
		};//Size=0x0008

		unsigned __int16 m_header; //0x0000
		unsigned __int16 m_size; //0x0002
		unsigned __int8 m_vectorCount; //0x0004
		unsigned __int8 m_textureCount; //0x0005
		unsigned __int8 m_boolCount; //0x0006
		char _0x0007[9];
		Entry m_entries[1]; //0x0010

		unsigned __int16 size() const { return m_size; }
		char* values() { return reinterpret_cast<char*>(this) + 0x10; }
	};

	class PointLightEntity : public LocalLightEntity
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 615;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x02404EA8;
		}
	};//Size=0x00A0

	class IGameRenderer
	{
	public:
		virtual VOID Function0(); // V: 0x0
		virtual VOID Function1(); // V: 0x4
		virtual VOID init(); // V: 0x8
		virtual VOID postLoadInit(); // V: 0xC
		virtual VOID createUpdateJob(FLOAT, FLOAT, LPVOID, LPVOID, LPVOID); // V: 0x10
		// valid Draw Job
		virtual VOID joinUpdateJob(); // V: 0x14
		virtual VOID setJobEnable(BOOL); // V: 0x18
		virtual BOOL getJobEnable(); // V: 0x1C
		virtual VOID onUnloadLevel(); // V: 0x20
		virtual INT getAffinity(); // V: 0x24
		virtual void* getResolution(); // V: 0x28
		virtual VOID addModule(INT, LPVOID); // V: 0x2C
		virtual VOID removeModule(INT); // V: 0x30
		// valid Draw Job
		virtual const class GameRenderSettings* getSettings(); // V: 0x34
		//this
		virtual class ScreenRenderer* getScreenRenderer(); // V: 0x38
		virtual VOID updatePerfOverlay(BOOL, FLOAT); // V: 0x3C
		// valid Draw Job
		virtual VOID resetPerfOverlay(); // V: 0x40
		virtual BOOL isFadeInAllowed(); // V: 0x44
		virtual VOID setVsyncEnable(BOOL); // V: 0x48
		// valid Draw Job
		virtual BOOL getVsyncEnable(); // V: 0x4C
		virtual VOID setPresentEnable(BOOL); // V: 0x50
		virtual VOID getAdapterInformation(LPVOID, UINT&, UINT&); // V: 0x54
		virtual const void*& getTempDeprecatedView(); // V: 0x58

		INT m_refCount; // 0x04
	}; // 0x08

	class RenderViewDesc
	{
	public:
		//D3DXMATRIX transform;
		LinearTransform transform; // 0x00
		INT type; // 0x40
		char pad[0x4]; // 0x44
		FLOAT fovY; // 0x48
		FLOAT defaultFovY; // 0x4C
		FLOAT nearPlane; // 0x50
		FLOAT farPlane; // 0x54
		FLOAT aspect; // 0x58
		FLOAT orthoWidth; // 0x5C
		FLOAT orthoHeight; // 0x60
		FLOAT stereoSeparation; // 0x64
		FLOAT stereoConvergence; // 0x68
		Vec2 viewportOffset; // 0x6C
		Vec2 viewportScale; // 0x74
	};

	class RenderView
	{
	public:
		RenderViewDesc m_desc; // 0x00
		char pad[0x4]; // 0x7C
		INT m_dirtyFlags; // 0x80
		char pad2[0x16C]; // 0x84
		FLOAT m_fovX; // 0x1F0
		FLOAT m_depthToWidthRatio; // 0x1F4
		FLOAT m_fovScale; // 0x1F8
		FLOAT m_fovScaleSqr; // 0x1FC
		LinearTransform m_viewMatrix; // 0x200
		LinearTransform m_viewMatrixTranspose; // 0x240
		LinearTransform m_viewMatrixInverse; // 0x280
		//D3DXMATRIX m_viewMatrixInverse; // 0x280
		LinearTransform m_projectionMatrix; // 0x2C0
		LinearTransform m_viewMatrixAtOrigin; // 0x300
		LinearTransform m_projectionMatrixTranspose; // 0x340
		LinearTransform m_projectionMatrixInverse; // 0x380
		LinearTransform m_viewProjectionMatrix; // 0x3C0
		LinearTransform m_viewProjectionMatrixTranspose;// 0x400
		LinearTransform m_viewProjectionMatrixInverse; // 0x440

		BOOL Update()
		{
			DxRenderer* dxRenderer = DxRenderer::GetInstance();
			if (dxRenderer == NULL)
				return FALSE;
			DWORD UPDATEMATRICES = OFF_RenderView_updateMatrices;
			FLOAT screenX = static_cast<FLOAT>(dxRenderer->m_screenInfo.m_nWindowWidth);
			FLOAT screenY = static_cast<FLOAT>(dxRenderer->m_screenInfo.m_nWindowHeight);
			this->m_desc.aspect = screenX / screenY;

			((VOID(__fastcall*)(RenderView*, LPVOID))UPDATEMATRICES)(this, NULL);
			return TRUE;
		}
	};

	class GameRenderViewParams
	{
	public:
		RenderView view; // 0x00
		RenderView prevView; // 0x480
		RenderView secondaryStreamingView; // 0x900
		INT secondaryStreamingViewEnable; // 0xD80
		char paad[0xC]; // 0xD84
		LinearTransform firstPersonTransform; // 0xD90
	};

	class GameRenderer : public IGameRenderer
	{
	public:
		char pad[0x48]; // 0x08
		GameRenderViewParams m_viewParams; // 0x50
	public:
		static GameRenderer* Singleton()
		{
			DWORD GAMERENDERER = OFF_g_gameRenderer;
			return *(GameRenderer**)(GAMERENDERER);
		}
	};

	class GameEntity : public SpatialEntity
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 621;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x023EBF54;
		}
		char _0x0010[16];
	};//Size=0x0020

	class ClientGameEntity : public GameEntity
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 622;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x023E97F0;
		}
	};//Size=0x0020

	class VisualEnvironmentEntity : public ClientGameEntity
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 665;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x023E94E4;
		}
		VisualEnvironmentReferenceObjectData* m_referenceObjectData;
		char pad[0xC];
		fb::VisualEnvironmentState m_state;
	};//Size=0x0130

	class EmitterComponentData : public DataContainer
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 805;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x02407480;
		}
	};//Size=0x0008

	class ProcessorData : public EmitterComponentData
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 818;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x024074AC;
		}
		char _0x0008[8];
		Vec4 m_DefaultValues; //0x0010
		ProcessorData* m_NextProcessor; //0x0020
		class EmitterComponentData* m_Pre; //0x0024
		EmittableField m_EvaluatorInput; //0x0028
		bool m_Enable; //0x002C
		char _0x002D[3];
	};//Size=0x0030

	class SpawnColorRandomData : public ProcessorData
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 0xFFFFFFFF; // sentinel: no BF3 processor uses this id
		}
		Vec3 m_Color0; //0x0030
		Vec3 m_Color1; //0x0040
	};//Size=0x0050

	class EmitterAsset;
	class EmitterEntityData;
	class EmitterDocument; // fwd: only used as Array<EmitterDocument*> element

	class EmitterTemplate
	{
	public:
		char pad[0x14];
		eastl::vector<fb::ProcessorData*> m_processors;
		unsigned __int8 m_processorIndices[ProcessorTypeCount];
		char pad3[0x2];
		fb::Vec2 m_uvPos;
		fb::Vec2 m_uvSize;
		fb::Vec2 m_uvPosNm;
		fb::Vec2 m_uvSizeNm;

		template<typename T, typename E>
		requires std::is_enum_v<E>
		T* getProcessor(E e)
		{
			unsigned __int8 v1{ };
			v1 = m_processorIndices[e];
			if (v1 == 0xFF)
				return nullptr;

			return (T*)m_processors[v1];
		}
	};

	class IKeyboard
	{
	public:
		virtual void init(); // V: 0x0
		virtual void release(); // V: 0x4
		virtual void preFrameUpdate(float); // V: 0x8
		virtual void update(float); // V: 0xC
		virtual bool isDown(int); // V: 0x10
		virtual bool wentDown(int); // V: 0x14
		virtual bool wentUp(int); // V: 0x18
		virtual void enableTypingMode(bool); // V: 0x1C
	};

	struct EffectParams
	{
		union Value
		{
			__m128 v;
			float f;
			int i;
			bool b;
		};

		enum ParameterType : __int32
		{
			PTInvalid = 0x0,
			PTFloat = 0x1,
			PTInt = 0x2,
			PTBool = 0x3,
			PTVector = 0x4,
		};

		Value m_parameterValue[4];
		const char* m_parameterName[4];
		ParameterType m_parameterType[4];
		unsigned int m_paramCount;
		char pad[8];
	};

	class EffectManager
	{
	public:
		static EffectManager* GetInstance()
		{
			return *(EffectManager**)OFF_g_effectManager;
		}

		uint32_t stopEffect(uint32_t handle)
		{
			typedef uint32_t(__thiscall* tStopEffect)(void* _this, uint32_t handleId);
			tStopEffect stopEffect = (tStopEffect)OFF_EffectManager_stopEffect;

			return stopEffect(this, handle);
		}

		[[nodiscard]] uint32_t playEffect(fb::Asset* asset, fb::LinearTransform* tr, fb::EffectParams* params, bool isFirstPerson)
		{
			typedef uint32_t(__thiscall* tplayEffect)(
				fb::EffectManager* _this,
				fb::Asset* asset,
				fb::LinearTransform* linear,
				void* level,
				fb::EffectParams* a5,
				bool isFirstPerson,
				char unkFlagZero);
			tplayEffect playEffect = (tplayEffect)OFF_EffectManager_playEffect;

			return playEffect(this, asset, tr, fb::ClientGameContext::GetInstance()->m_level, params, isFirstPerson, 0);
		}
	};

	struct EnlightenMaterial
	{
		fb::Vec3 color;
		fb::Guid guid;
		unsigned __int64 shaderId;
		bool emissive;
	};

	inline RenderView* getActiveRenderView(GameRenderer* gr)
	{
		return gr ? &gr->m_viewParams.view : nullptr;
	}
	inline LinearTransform* getViewMatrix(RenderView* v) { return v ? &v->m_viewMatrix : nullptr; }
	inline LinearTransform* getViewMatrixInverse(RenderView* v) { return v ? &v->m_viewMatrixInverse : nullptr; }
	inline LinearTransform* getProjectionMatrix(RenderView* v) { return v ? &v->m_projectionMatrix : nullptr; }
	inline LinearTransform* getViewProjectionMatrix(RenderView* v) { return v ? &v->m_viewProjectionMatrix : nullptr; }
	inline void updateRenderView(RenderView* v) { if (v) v->Update(); }

	inline ClientPlayer* getLocalPlayer(ClientGameContext* ctx)
	{
		if (!ctx || !ctx->m_clientPlayerManager) return nullptr;
		return ctx->m_clientPlayerManager->m_localPlayer;
	}

	inline const char* getCurrentLevelName(ClientGameContext* ctx)
	{
		if (!ctx || !ctx->m_level || !ctx->m_level->m_levelData) return nullptr;
		return ctx->m_level->m_levelData->name;
	}

	inline WorldRenderSettings* getLiveWorldRenderSettings(ClientGameContext* ctx)
	{
		if (!ctx || !ctx->m_level || !ctx->m_level->m_worldRenderModule || !ctx->m_level->m_worldRenderModule->m_worldRenderer)
			return nullptr;
		return ctx->m_level->m_worldRenderModule->m_worldRenderer->m_worldRenderSettings;
	}
}

#include "../SDK_bf3/classes.h"

#if !defined(_WIN64)
static_assert(sizeof(fb::StaticModelGroupMemberData) == 0x38, "StaticModelGroupMemberData retail size");
static_assert(offsetof(fb::StaticModelGroupMemberData, m_PhysicsPartRange) == 0x20, "StaticModelGroupMemberData::m_PhysicsPartRange");
static_assert(sizeof(fb::MeshSetLayout) == 0x70 && offsetof(fb::MeshSetLayout, m_nameHash) == 0x68, "MeshSetLayout");
static_assert(sizeof(fb::MeshSet) == 0x100 && sizeof(fb::MeshData) == 0x20, "MeshSet");
static_assert(sizeof(fb::MeshSubset) == 0x94 && offsetof(fb::MeshSubset, m_elements) == 0x30, "MeshSubset");
static_assert(sizeof(fb::MeshLayout) == 0xB0 && offsetof(fb::MeshLayout, m_data) == 0x8C, "MeshLayout");
static_assert(sizeof(fb::RayCastHit) == 0x40, "RayCastHit");
static_assert(sizeof(fb::PixelShaderPermutation) == 0x2C && offsetof(fb::PixelShaderPermutation, m_shader) == 0x1C && offsetof(fb::PixelShaderPermutation, m_data) == 0x24, "PixelShaderPermutation");
static_assert(sizeof(fb::DxShaderSolution) == 0x40 && offsetof(fb::DxShaderSolution, m_pixelPermutation) == 0x20, "DxShaderSolution");
static_assert(sizeof(fb::SurfaceShader) == 0x50 && offsetof(fb::SurfaceShader, m_solutionPairs) == 0x44, "SurfaceShader");
static_assert(offsetof(fb::StaticModelGroupPhysics, m_partTableEnd) == 0x88, "StaticModelGroupPhysics");
static_assert(offsetof(fb::StaticModelGroupMeshInstance, m_transform) == 0x10, "StaticModelGroupMeshInstance::m_transform");
static_assert(offsetof(fb::StaticModelGroupPhysics, m_partTable) == 0x84, "StaticModelGroupPhysics::m_partTable");
static_assert(sizeof(fb::ClientStaticModelGroupMember) == 0x38, "ClientStaticModelGroupMember size");
static_assert(sizeof(fb::ClientStaticModelGroupEntity) == 0x88, "ClientStaticModelGroupEntity size");
static_assert(offsetof(fb::ClientStaticModelGroupEntity, m_membersBegin) == 0x74, "ClientStaticModelGroupEntity::m_membersBegin");
static_assert(sizeof(fb::DxTexture) == 0x7C, "DxTexture size");
static_assert(offsetof(fb::DxTexture, m_width) == 0x20, "DxTexture::m_width");
static_assert(offsetof(fb::DxTexture, m_handle) == 0x54, "DxTexture::m_handle");
static_assert(offsetof(fb::DxTexture, m_resource) == 0x68, "DxTexture::m_resource");
static_assert(offsetof(fb::DxTexture, m_shaderViews) == 0x70, "DxTexture::m_shaderViews");
static_assert(sizeof(fb::TextureStreamingEntry) == 0x44, "TextureStreamingEntry size");
static_assert(offsetof(fb::TextureStreamingManager, m_begin) == 0x30, "TextureStreamingManager::m_begin");
static_assert(offsetof(fb::TextureStreamingManager, m_entries) == 0x40, "TextureStreamingManager::m_entries");
static_assert(offsetof(fb::TextureStreamingManager, m_onDemandBudgetUsed) == 0xA40EC, "reservedPoolSize[2]");
static_assert(offsetof(fb::MeshVariationManager, m_buckets) == 0x28, "MeshVariationManager::m_buckets");
static_assert(sizeof(fb::MeshVariationNode) == 0x18, "MeshVariationNode size");
static_assert(offsetof(fb::ShaderParameterBlock, m_entries) == 0x10, "ShaderParameterBlock::m_entries");
static_assert(sizeof(fb::LensFlareEntity) == 0x90, "LensFlareEntity size");
static_assert(offsetof(fb::LensFlareEntity, m_surfaceShaders) == 0x60, "LensFlareEntity::m_surfaceShaders");
#endif
#if !defined(_WIN64)
static_assert(sizeof(fb::VisualEnvironmentState) == 0xF0, "VisualEnvironmentState size");
static_assert(offsetof(fb::VisualEnvironmentState, resources) == 0xB4, "VisualEnvironmentState::resources");
static_assert(sizeof(fb::TextureHeader) == 0x80, "TextureHeader size");
#endif
