#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <cstdint>
#include "general.h"
#include "typeinfo.h"
#include <array>
#include "eastl.h"
#include "vfunc.h"

namespace fb
{
	struct Vec4
	{
		float m_x; //0x0000
		float m_y; //0x0004
		float m_z; //0x0008
		float m_w; //0x000C
	};//Size=0x0010

	struct Vec3
	{
		float m_x; //0x0000
		float m_y; //0x0004
		float m_z; //0x0008
		char _0x000C[4];
	};//Size=0x0010

	struct Vec2
	{
		float m_x; //0x0000
		float m_y; //0x0004
	};//Size=0x0008

	enum Realm : __int32
	{
		Realm_Client = 0x0,
		Realm_Server = 0x1,
		Realm_ClientAndServer = 0x2,
		Realm_None = 0x3,
		Realm_Pipeline = 0x4,
	};

	class DataContainer : public ITypedObject
	{
	public:
		unsigned __int16 m_refCnt;
		unsigned __int16 m_flags;

		fb::Guid* getInstanceGuid()
		{
			fb::DataContainer* result; // eax

			if ((this->m_flags & 0x100) == 0)
				return 0;
			result = this - 2;
			if (this == (fb::DataContainer*)0x10)
				return 0;
			return (fb::Guid*)result;
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
	};//Size=0x000C

	class TextureBaseAsset : public Asset
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 2366;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x02BF1CC4;
		}
	};//Size=0x000C

	class TextureAsset : public TextureBaseAsset
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 2369;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x02BF1CF0;
		}
	};//Size=0x000C


	class OutdoorLightComponentData
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

	class EnlightenComponentData
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

	enum TonemapMethod
	{
		TonemapMethod_None, //0x0000
		TonemapMethod_Linear, //0x0001
		TonemapMethod_Filmic, //0x0002
		TonemapMethod_FilmicNeutral, //0x0003
		TonemapMethodCount //0x0004
	};

	class TonemapComponentData
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

	class ColorCorrectionComponentData
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

	class SkyComponentData
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

	class FogComponentData
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

	class WindComponentData
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

	class SurfaceShaderBaseAsset : public Asset
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 2345;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x02BF26FC;
		}
	};//Size=0x000C

	class SunFlareComponentData
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

	class DynamicAOComponentData
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

	enum BlurFilter
	{
		BfNone, //0x0000
		BfGaussian3Pixels, //0x0001
		BfGaussian5Pixels, //0x0002
		BfGaussian7Pixels, //0x0003
		BfGaussian9Pixels, //0x0004
		BfGaussian15Pixels, //0x0005
		BfGaussian31Pixels //0x0006
	};

	class DofComponentData
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

	class VignetteComponentData
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

	class FilmGrainComponentData
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

	class LensScopeComponentData
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

	class CameraParamsComponentData
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

	enum ScreenEffectFrameType
	{
		ScreenEffectFrameType_FullFrame, //0x0000
		ScreenEffectFrameType_SingleFramePart, //0x0001
		ScreenEffectFrameType_SingleSquareFramePart //0x0002
	};

	class ScreenEffectComponentData
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

	class DamageEffectComponentData
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

	class ShaderParamsComponentData
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

	class PlanarReflectionComponentData
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

	class DynamicEnvmapComponentData
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

	enum CharacterLightingMode
	{
		CharacterLightingMode_Add, //0x0000
		CharacterLightingMode_Blend //0x0001
	};

	class CharacterLightingComponentData2
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 1579;
		}
		char pad[0x60];
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
	};

	class CharacterLightingComponentData
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

	class MotionBlurComponentData
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
		OutdoorLightComponentData outdoorLight; //0x0000
		EnlightenComponentData enlighten; //0x0080
		TonemapComponentData tonemap; //0x0100
		ColorCorrectionComponentData colorCorrection; //0x0140
		SkyComponentData sky; //0x0180
		FogComponentData fog; //0x0250
		WindComponentData wind; //0x02C0
		SunFlareComponentData sunFlare; //0x02D0
		DynamicAOComponentData dynamicAO; //0x0490
		DofComponentData dof; //0x04C0
		VignetteComponentData vignette; //0x04F0
		FilmGrainComponentData filmGrain; //0x0520
		LensScopeComponentData lensScope; //0x0550
		CameraParamsComponentData cameraParams; //0x05B0
		ScreenEffectComponentData screenEffect; //0x05C0
		DamageEffectComponentData damageEffect; //0x05F0
		ShaderParamsComponentData shaderparams[16]; //0x0660
		PlanarReflectionComponentData planarReflection; //0x0860
		DynamicEnvmapComponentData dynamicEnvmap; //0x0880
		CharacterLightingComponentData characterLighting; //0x08C0
		MotionBlurComponentData motionBlur; //0x0900
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

	struct VisualEnvironmentState
	{
		float visibility;
		int priority;
		bool overrideVisibility;
		bool excluded;
		char pad_0x0A[0x2];
		int stateId;
		Vec4 activeEffectPosition;
		OutdoorLightComponentData* outdoorLight; //0x0000
		EnlightenComponentData* enlighten; //0x0080
		TonemapComponentData* tonemap; //0x0100
		ColorCorrectionComponentData* colorCorrection; //0x0140
		SkyComponentData* sky; //0x0180
		FogComponentData* fog; //0x0250
		WindComponentData* wind; //0x02C0
		SunFlareComponentData* sunFlare; //0x02D0
		DynamicAOComponentData* dynamicAO; //0x0490
		DofComponentData* dof; //0x04C0
		VignetteComponentData* vignette; //0x04F0
		FilmGrainComponentData* filmGrain; //0x0520
		LensScopeComponentData* lensScope; //0x0550
		CameraParamsComponentData* cameraParams; //0x05B0
		ScreenEffectComponentData* screenEffect; //0x05C0
		DamageEffectComponentData* damageEffect; //0x05F0
		PlanarReflectionComponentData* planarReflection; //0x0860
		DynamicEnvmapComponentData* dynamicEnvmap; //0x0880
		CharacterLightingComponentData* characterLighting; //0x08C0
		MotionBlurComponentData* motionBlur; //0x0900
		ShaderParamsComponentData* shaderparams[16]; //0x0660
		// debug
	};

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
		UINT m_nWidth;					// 0x0
		UINT m_nHeight;					// 0x4
		UINT m_nWindowWidth;			// 0x8
		UINT m_nWindowHeight;			// 0xC
		FLOAT fRefreshRate;				// 0x10
	};

	class DxRenderer
	{
	public:
		DxRenderSettings* m_settings;
		BYTE Pad_004[0x4];				// 0x00
		UINT m_nFrameCounter;			// 0x08
		BOOL m_bFrameInProgress;		// 0x0C
		HWND m_hWnd;					// 0x10
		BYTE Pad_014[0x4];				// 0x14
		BYTE m_bFullscreenWanted;		// 0x18
		BYTE m_bFullscreenActive;		// 0x19
		BYTE m_bMinimized;				// 0x1A
		BYTE m_bMinimizing;				// 0x1B
		BYTE m_bResizing;				// 0x1C
		BYTE m_bOccluded;				// 0x1D
		BYTE m_bVSync;					// 0x1E
		char pad[0x1];						// 0x1F
		RenderScreenInfo m_screenInfo;	// 0x20
		char pad1[0xA4];						// 0x34
		ID3D11Device* pDevice;			// 0xD8
		ID3D11DeviceContext* pContext;  // 0xDC
		char pad3[0x14];					// 0xE0
		IDXGISwapChain* pSwapChain;		// 0xF4

	public:
		static DxRenderer* GetInstance()
		{
			return *(DxRenderer**)0x023577D4;
		}
	};

	class IMouse
	{
	public:
		virtual void init();	// V: 0x0
		virtual void release();	// V: 0x4
		virtual void preFrameUpdate(float);	// V: 0x8
		virtual void update(float);	// V: 0xC
		virtual void updateBetweenFrames(float);	// V: 0x10
		virtual bool isDown(void*, bool);	// V: 0x14
		virtual bool wentDown(void*);	// V: 0x18
		virtual bool wentUp(void*);	// V: 0x1C
		virtual float getLevel(void*, bool);	// V: 0x20
		virtual float getLevel(void*);	// V: 0x24
		virtual bool getBufferedAction(void*);	// V: 0x28
		virtual void enableCursorMode(bool, int);	// V: 0x2C
		virtual void setCursorPosition(int, int);	// V: 0x30
		virtual void setCursorRect(bool, unsigned int, unsigned int, unsigned int, unsigned int);	// V: 0x34
		virtual void setMouseSensitivity(float);	// V: 0x38
		virtual void setUIOwnsInput(bool);	// V: 0x3C
	};

	class BorderInputNode
	{
	public:
		virtual void init();
		virtual void exit();
		virtual void clear();
		virtual void suppressButtons(); // V: 0x10
		virtual float getLevel(bool);        // V: 0x14
		virtual float getLevel2(bool); // V: 0x18
		virtual bool isDown(bool);   // V: 0x1C
		virtual bool isDown2();    // V: 0x20
		virtual bool wentDown();       // V: 0x24
		virtual bool wentDown2();        // V: 0x28
		virtual bool wentUp(); // V: 0x2C
		virtual bool wentUp2();  // V: 0x30
		virtual bool isMouseActive();   // V: 0x34
		virtual void* getPad();      // V: 0x38
		virtual class MotionController* getMotionController();    // V: 0x3C
		virtual class IMouse* getMouse(); // V: 0x40
		virtual class IKeyboard* getKeyboard();   // V: 0x44
		virtual void setUIInputTyping(bool);    // V: 0x48
		virtual void setUIInputClicking(bool, int);     // V: 0x4C
		virtual void setUICursorPosition(int, int);     // V: 0x50
		virtual void setUICursorRect(bool, unsigned int, unsigned int, unsigned int, unsigned int);     // V: 0x54
		virtual void updateBetweenFrames(float);        // V: 0x58
		virtual /*class fb::InputActionData **/void* getTriggeredInputAction(); // V: 0x5C
		virtual bool getTriggeredInputAction(/*class fb::InputActionData &*/void*);     // V: 0x60
		virtual void update(float);     // V: 0x64
		virtual int preFrameUpdate(float fDeltaTime);
		//virtual void postFrameUpdate(float);	
		virtual void postFrameUpdate(float);	// V: 0x6C

		static BorderInputNode* GetInstance()
		{
			DWORD BORDERINPUTNODE = 0x02384EB8;
			return *(BorderInputNode**)BORDERINPUTNODE;
		}

		char unknown0[4];
		class InputCache* m_inputCache;
		//new
		bool m_forceReadCache;                     // 0x8
		char pad[0x3];
		class Pad* m_pad;								 // 0xC
		MotionController* m_motionController;      // 0x10
		class IKeyboard* m_keyboard;                     // 0x14 enableTypingMode!
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
		class PlayerManager* m_playerManager;					// 0x08
		class GameTime* m_gameTime;								// 0x0C
		class ClientLevel* m_level;								// 0x10
		class MaterialGridManager* m_materialGridManager;		// 0x14
		DWORD m_animationManager;								// 0x18 ant::AnimationManager
		DWORD m_modelAnimationManager;							// 0x1C ModelAnimationManager
		DWORD m_blueprintBundleManager;							// 0x20 BlueprintBundleManager
		DWORD m_dlcManager;										// 0x24 DLCManager
		DWORD m_demoControl;									// 0x28 DemoControl
		INT m_realm;											// 0x2C
	}; // 0x30

	enum MipmapFilterMode
	{
		MipmapFilterMode_Box, //0x0000
		MipmapFilterMode_Renormalize, //0x0001
		MipmapFilterMode_Poisson13, //0x0002
		MipmapFilterMode_Poisson13Clamped //0x0003
	};

	enum WorldViewMode
	{
		WorldViewMode_Default, //0x0000
		WorldViewMode_RawLinear, //0x0001
		WorldViewMode_RawLinearAlpha, //0x0002
		WorldViewMode_Diffuse, //0x0003
		WorldViewMode_Specular, //0x0004
		WorldViewMode_Emissive, //0x0005
		WorldViewMode_Normal, //0x0006
		WorldViewMode_Smoothness, //0x0007
		WorldViewMode_Material, //0x0008
		WorldViewMode_Light, //0x0009
		WorldViewMode_LightDiffuse, //0x000A
		WorldViewMode_LightSpecular, //0x000B
		WorldViewMode_LightIndirect, //0x000C
		WorldViewMode_LightTranslucency, //0x000D
		WorldViewMode_LightOverdraw, //0x000E
		WorldViewMode_SkyVisibility, //0x000F
		WorldViewMode_SkyVisibilityRaw, //0x0010
		WorldViewMode_Overdraw, //0x0011
		WorldViewMode_DynamicAO, //0x0012
		WorldViewMode_Occluders, //0x0013
		WorldViewMode_RadiosityLightMaps, //0x0014
		WorldViewMode_RadiosityDiffuseColor, //0x0015
		WorldViewMode_RadiosityTargetUV, //0x0016
		WorldViewMode_VelocityVector, //0x0017
		WorldViewMode_DistortionVector //0x0018
	};

	enum ShaderGBufferLayout
	{
		ShaderGBufferLayout_Default, //0x0000
		ShaderGBufferLayout_Test //0x0001
	};

	enum QualityLevel
	{
		QualityLevel_Low, //0x0000
		QualityLevel_Medium, //0x0001
		QualityLevel_High, //0x0002
		QualityLevel_Ultra //0x0003
	};

	class WorldRenderSettings
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
		char padInherit[sizeof(DataContainer)];
		char SKIPME[8];
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
			/*WorldDrawViewInfo*   */void* viewInfo;
		};
		virtual int addRef();	// V: 0x0
		virtual int release();	// V: 0x4
		virtual void createUpdateJob(); // (const struct fb::WorldRenderUpdateParams &, class EA::Jobs::JobInstanceHandle &, class EA::Jobs::JobInstanceHandle &);	// V: 0x8
		virtual void draw();	// V: 0xC
		virtual void setSettings(class WorldRenderSettings*);	// V: 0x10
		virtual const class WorldRenderSettings* getSettings();	// V: 0x14
		virtual void addModule();// (struct fb::IWorldRenderModule *);	// V: 0x18
		virtual void removeModule(); // (struct fb::IWorldRenderModule *);	// V: 0x1C
		virtual void addPrimitiveRenderer(); // (struct fb::IWorldPrimitiveRenderer *);	// V: 0x20
		virtual void removePrimitiveRenderer(); // (struct fb::IWorldPrimitiveRenderer *);	// V: 0x24
		virtual struct ITexture* getHDRRenderTargetColorTexture(unsigned int);	// V: 0x28
		virtual struct ITexture* getGeometryBufferTextures();	// V: 0x2C
		virtual unsigned int getGeometryBufferTextureCount();	// V: 0x30
		virtual class PostProcessSystem* getPostProcessSystem();	// V: 0x34
		virtual DWORD getHudRenderTargetView();	// V: 0x38
		virtual struct fb::ITexture* getHudRenderTargetTexture();	// V: 0x3C
		virtual DWORD getWorldOcclusionQueryModule();	// V: 0x40
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
			return *(ClientGameContext**)(0x02380B58);
		}

		class ClientPlayerManager* m_clientPlayerManager;		// 0x30
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
		void* m_controlledControllable;		// 0x3D8

		bool isAlive()
		{
			return m_controlledControllable != nullptr;
		}
	};

	struct LinearTransform
	{
		Vec3 m_right; //0x0000
		Vec3 m_up; //0x0010
		Vec3 m_forward; //0x0020
		Vec3 m_trans; //0x0030
	};//Size=0x0040

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
			return *(VisualEnvironmentManager**)(0x023B6844);
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

	class GameDataContainerAsset : public Asset
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 2408;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x023C50B0;
		}
		class GameDataContainer* m_Data; //0x000C
	};//Size=0x0010

	class GameDataContainer : public DataContainer
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 1534;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x023C2D44;
		}
	};//Size=0x0008

	class GameObjectData : public GameDataContainer
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 1540;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x023C4F88;
		}
		unsigned __int16 m_IndexInBlueprint; //0x0008
		__int8 m_IsEventConnectionTarget; //0x000A
		__int8 m_IsPropertyConnectionTarget; //0x000B
	};//Size=0x000C

	class EntityData : public GameObjectData
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 1695;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x023C4FD4;
		}
	};//Size=0x000C

	class SpatialEntityData : public EntityData
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 1856;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x023C5000;
		}
		char _0x000C[4];
		LinearTransform m_Transform; //0x0010
	};//Size=0x0050

	struct LensFlareElement
	{
		Vec4 m_SizeCamDistCurve; //0x0000
		Vec4 m_SizeAngleCurve; //0x0010
		Vec2 m_Size; //0x0020
		char _0x0028[8];
		Vec4 m_SizeOccluderCurve; //0x0030
		Vec4 m_SizeScreenPosCurve; //0x0040
		Vec4 m_AlphaCamDistCurve; //0x0050
		Vec4 m_AlphaOccluderCurve; //0x0060
		Vec4 m_AlphaAngleCurve; //0x0070
		Vec4 m_AlphaScreenPosCurve; //0x0080
		float m_SizeCamDistMax; //0x0090
		float m_RayDistance; //0x0094
		class SurfaceShaderBaseAsset* m_Shader; //0x0098
		float m_AlphaCamDistMax; //0x009C
	};//Size=0x00A0

	class LensFlareEntityData : public SpatialEntityData
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 1865;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x02405CF4;
		}
		Array<LensFlareElement> m_Elements; //0x0050
		float m_OccluderSize; //0x0054
		bool m_Visible; //0x0058
		bool m_HalfRes; //0x0059
		bool m_DebugDrawOccluder; //0x005A
		char _0x005B[5];
	};//Size=0x0060

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
		char _0x000C[132];
	};//Size=0x0090

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
	class SpatialEntityWithBusAndData : public SpatialEntity				// 0x00
	{
	public:
		EntityBus* m_entityBus;				// 0x10
		T* m_data;							// 0x14
	}; // 0x18

	enum EnlightenColorMode
	{
		EnlightenColorMode_Multiply, //0x0000
		EnlightenColorMode_Override //0x0001
	};

	class LocalLightEntityData : public SpatialEntityData
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 1862;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x02405D30;
		}
		Vec3 m_Color; //0x0050
		Vec3 m_ParticleColorScale; //0x0060
		Vec3 m_EnlightenColorScale; //0x0070
		float m_Radius; //0x0080
		float m_Intensity; //0x0084
		float m_AttenuationOffset; //0x0088
		EnlightenColorMode m_EnlightenColorMode; //0x008C
		bool m_EnlightenEnable; //0x0090
		bool m_Visible; //0x0091
		bool m_SpecularEnable; //0x0092
		char _0x0093[13];
	};//Size=0x00A0

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

		void setDirty()
		{
			typedef __int16(__thiscall* tSetDirty)(void* _this);
			tSetDirty oSetDirty = (tSetDirty)0x17A8260;

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
		LPVOID vftable;						// 0x00
		EntityCreator* m_previousCreator;	// 0x04
		EntityCreator* m_nextCreator;		// 0x08
		Realm m_realm;						// 0x0C
		INT m_linked;						// 0x10
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
			eastl::vector<EntityCollection>* m_collections;    //offset = 0x0, length = 0x4
			EntityCollectionSegment* m_currentSegment;    //offset = 0x4, length = 0x4
			unsigned int m_collectionIndexIt;    //offset = 0x8, length = 0x4
			unsigned int m_collectionIndexEnd;    //offset = 0xC, length = 0x4
			unsigned int m_entityIndexIt;    //offset = 0x10, length = 0x4
			unsigned int m_entityIndexEnd;    //offset = 0x14, length = 0x4
			bool m_onlyIncludeIterable;    //offset = 0x18, length = 0x1
			char pad_19[3];
		};

		void kindOfQuery(class ClassInfo* classInfo, EntityIterator* result, bool onlyIncludeIterable = true)
		{
			typedef void(__thiscall* kindOfQuery_t)(void*, ClassInfo*, EntityIterator*, bool);
			kindOfQuery_t m_kindOfQuery = (kindOfQuery_t)0x00549B00;

			m_kindOfQuery(this, classInfo, result, onlyIncludeIterable);
		}
	};

	template<typename T> struct EntityList : public EntityWorld::EntityIterator
	{
		EntityList() {}
		EntityList(fb::ClassInfo* classInfo, bool onlyIncludeIterable = true)
		{
			EntityWorld* manager = (EntityWorld*)fb::ClientGameContext::GetInstance()->m_level->m_gameWorld;
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
		eastl::string name;
		eastl::string root;
		char pad[0x22];
	};

	class ResourceManager
	{
	public:
		class Compartment
		{
		public:
			char pad_0000[84];
			char* m_domain;
			DomainInfo m_domainInfo;
			char pad_0058[144 - 68];
			eastl::vector<fb::ITypedObject*> m_objects;
		};

	public:
		volatile uint32_t m_bundleLoadInProgress;
		std::array<ResourceManager::Compartment*, 204> m_compartments;

		static ResourceManager* GetInstance()
		{
			return *(ResourceManager**)(0x230D220);
		}
	};

	enum SpotLightShape
	{
		SpotLightShape_Cone, //0x0000
		SpotLightShape_Frustum, //0x0001
		SpotLightShape_OrthoFrustum //0x0002
	};

	class SpotLightEntityData : public LocalLightEntityData
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 1863;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x02405D88;
		}
		SpotLightShape m_Shape; //0x00A0
		float m_ConeInnerAngle; //0x00A4
		float m_ConeOuterAngle; //0x00A8
		float m_FrustumFov; //0x00AC
		float m_FrustumAspect; //0x00B0
		float m_OrthoWidth; //0x00B4
		float m_OrthoHeight; //0x00B8
		TextureAsset* m_Texture; //0x00BC
		QualityLevel m_CastShadowsMinLevel; //0x00C0
		bool m_CastShadowsEnable; //0x00C4
		char _0x00C5[11];
	};//Size=0x00D0

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

	struct PropertyConnection
	{
		DataContainer* m_Source; //0x0000
		DataContainer* m_Target; //0x0004
		__int32 m_SourceFieldId; //0x0008
		__int32 m_TargetFieldId; //0x000C
	};//Size=0x0010

	struct LinkConnection
	{
		DataContainer* m_Source; //0x0000
		DataContainer* m_Target; //0x0004
		__int32 m_SourceFieldId; //0x0008
		__int32 m_TargetFieldId; //0x000C
	};//Size=0x0010

	struct EventSpec
	{
		__int32 m_Id; //0x0000
	};//Size=0x0004

	enum EventConnectionTargetType
	{
		EventConnectionTargetType_Invalid, //0x0000
		EventConnectionTargetType_ClientAndServer, //0x0001
		EventConnectionTargetType_Client, //0x0002
		EventConnectionTargetType_Server, //0x0003
		EventConnectionTargetType_NetworkedClient, //0x0004
		EventConnectionTargetType_NetworkedClientAndServer //0x0005
	};

	struct EventConnection
	{
		DataContainer* m_Source; //0x0000
		DataContainer* m_Target; //0x0004
		EventSpec m_SourceEvent; //0x0008
		EventSpec m_TargetEvent; //0x000C
		EventConnectionTargetType m_TargetType; //0x0010
	};//Size=0x0014

	class DataBusData : public Asset
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 2416;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x023C2D18;
		}
		Array<PropertyConnection> m_PropertyConnections; //0x000C
		Array<LinkConnection> m_LinkConnections; //0x0010
	};//Size=0x0014

	class EntityBusData : public DataBusData
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 2417;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x023C4D38;
		}
		Array<EventConnection> m_EventConnections; //0x0014
		class InterfaceDescriptorData* m_Descriptor; //0x0018
		bool m_NeedNetworkId; //0x001C
		bool m_InterfaceHasConnections; //0x001D
		bool m_AlwaysCreateEntityBusClient; //0x001E
		bool m_AlwaysCreateEntityBusServer; //0x001F
	};//Size=0x0020

	class Blueprint : public EntityBusData
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 2418;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x023C4D90;
		}
	};//Size=0x0020

	class PrefabBlueprint : public Blueprint
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 2419;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x023C4E14;
		}
		Array<GameObjectData*> m_Objects; //0x0020
	};//Size=0x0024

	class SpatialPrefabBlueprint : public PrefabBlueprint
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 2421;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x023C4E40;
		}
	};//Size=0x0024

	class ObjectBlueprint : public Blueprint
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 2426;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x023C4DE8;
		}
		GameObjectData* m_Object; //0x0020
	};//Size=0x0024

	class WorldPartData : public SpatialPrefabBlueprint
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 2425;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x023C6A80;
		}
		Guid m_HackToSolveRealTimeTweakingIssue; //0x0024
		char _0x0024[16];
		bool m_UseDeferredEntityCreation; //0x0034
		bool m_Enabled; //0x0035
		char _0x0036[2];
	};//Size=0x0038

	class SubWorldData : public SpatialPrefabBlueprint
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 2422;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x023C6AAC;
		}
		class RegistryContainer* m_RegistryContainer; //0x0024
		bool m_IsWin32SubLevel; //0x0028
		bool m_IsXenonSubLevel; //0x0029
		bool m_IsPs3SubLevel; //0x002A
		bool m_RememberStateOnStreamOut; //0x002B
	};//Size=0x002C

	class LogicPrefabBlueprint : public PrefabBlueprint
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 2420;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x023C4E6C;
		}
	};//Size=0x0024

	class InternalDatabasePartition
	{
	public:
		char* m_name;
		char pad[0x38];
		eastl::vector<fb::DataContainer*> m_instances;
		DataContainer* m_primaryInstance;
	};

	class Message : public ITypedObject
	{
	public:
		virtual void* dtor(void*);
		virtual const char* getMessageName();

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
		/*const class fb::DataBus **/void* bus;                     // this+0x0
		const class fb::DataContainer* const data;                     // this+0x4
		const class fb::DataContainer* const exposed;                     // this+0x8

	}; // fb::DataContext

	class PointLightEntityData : public LocalLightEntityData
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 1864;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x02405D5C;
		}
		float m_Width; //0x00A0
		float m_TranslucencyAmbient; //0x00A4
		float m_TranslucencyScale; //0x00A8
		unsigned __int32 m_TranslucencyPower; //0x00AC
		float m_TranslucencyDistortion; //0x00B0
		char _0x00B4[12];
	};//Size=0x00C0

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
		virtual VOID Function0();												// V: 0x0
		virtual VOID Function1();												// V: 0x4
		virtual VOID init();													// V: 0x8
		virtual VOID postLoadInit();											// V: 0xC
		virtual VOID createUpdateJob(FLOAT, FLOAT, LPVOID, LPVOID, LPVOID);		// V: 0x10
		// valid Draw Job
		virtual VOID joinUpdateJob();											// V: 0x14
		virtual VOID setJobEnable(BOOL);										// V: 0x18
		virtual BOOL getJobEnable();											// V: 0x1C
		virtual VOID onUnloadLevel();											// V: 0x20
		virtual INT getAffinity();												// V: 0x24
		virtual void* getResolution();									// V: 0x28
		virtual VOID addModule(INT, LPVOID);									// V: 0x2C
		virtual VOID removeModule(INT);											// V: 0x30
		//  valid Draw Job
		virtual const class GameRenderSettings* getSettings();					// V: 0x34
		//this
		virtual class ScreenRenderer* getScreenRenderer();						// V: 0x38
		virtual VOID updatePerfOverlay(BOOL, FLOAT);							// V: 0x3C
		// valid Draw Job
		virtual VOID resetPerfOverlay();										// V: 0x40
		virtual BOOL isFadeInAllowed();											// V: 0x44
		virtual VOID setVsyncEnable(BOOL);										// V: 0x48
		// valid Draw Job
		virtual BOOL getVsyncEnable();											// V: 0x4C
		virtual VOID setPresentEnable(BOOL);									// V: 0x50
		virtual VOID getAdapterInformation(LPVOID, UINT&, UINT&);				// V: 0x54
		virtual const void*& getTempDeprecatedView();						// V: 0x58

		INT m_refCount;															// 0x04
	}; // 0x08

	class RenderViewDesc
	{
	public:
		//D3DXMATRIX transform;
		LinearTransform transform;				// 0x00
		INT type;								// 0x40
		char pad[0x4];								// 0x44
		FLOAT fovY;								// 0x48
		FLOAT defaultFovY;						// 0x4C
		FLOAT nearPlane;						// 0x50
		FLOAT farPlane;							// 0x54
		FLOAT aspect;							// 0x58
		FLOAT orthoWidth;						// 0x5C
		FLOAT orthoHeight;						// 0x60
		FLOAT stereoSeparation;					// 0x64
		FLOAT stereoConvergence;				// 0x68
		Vec2 viewportOffset;					// 0x6C
		Vec2 viewportScale;						// 0x74
	};

	class RenderView
	{
	public:
		RenderViewDesc m_desc;							// 0x00
		char pad[0x4];										// 0x7C
		INT m_dirtyFlags;								// 0x80
		char pad2[0x16C];										// 0x84
		FLOAT m_fovX;									// 0x1F0
		FLOAT m_depthToWidthRatio;						// 0x1F4
		FLOAT m_fovScale;								// 0x1F8
		FLOAT m_fovScaleSqr;							// 0x1FC
		LinearTransform m_viewMatrix;					// 0x200
		LinearTransform m_viewMatrixTranspose;			// 0x240
		LinearTransform m_viewMatrixInverse;			// 0x280
		//D3DXMATRIX m_viewMatrixInverse;				// 0x280
		LinearTransform m_projectionMatrix;				// 0x2C0
		LinearTransform m_viewMatrixAtOrigin;			// 0x300
		LinearTransform m_projectionMatrixTranspose;	// 0x340
		LinearTransform m_projectionMatrixInverse;		// 0x380
		LinearTransform m_viewProjectionMatrix;			// 0x3C0
		LinearTransform m_viewProjectionMatrixTranspose;// 0x400
		LinearTransform m_viewProjectionMatrixInverse;	// 0x440
	};

	class GameRenderViewParams
	{
	public:
		RenderView view;						// 0x00
		RenderView prevView;					// 0x480
		RenderView secondaryStreamingView;		// 0x900
		INT secondaryStreamingViewEnable;		// 0xD80
		char paad[0xC];								// 0xD84
		LinearTransform firstPersonTransform;		// 0xD90
	};

	class GameRenderer : public IGameRenderer
	{
	public:
		char pad[0x48];							// 0x08
		GameRenderViewParams m_viewParams;	// 0x50

	public:





		static GameRenderer* Singleton()
		{
			DWORD GAMERENDERER = 0x02384D78;
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

	class ReferenceObjectData : public GameObjectData
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 2006;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x023C4ED4;
		}
		char _0x000C[4];
		LinearTransform m_BlueprintTransform; //0x0010
		Blueprint* m_Blueprint; //0x0050
		class ObjectVariation* m_ObjectVariation; //0x0054
		int /*StreamRealm*/ m_StreamRealm; //0x0058
		bool m_CastSunShadowEnable; //0x005C
		bool m_Excluded; //0x005D
		char _0x005E[2];
	};//Size=0x0060

	class VisualEnvironmentReferenceObjectData : public ReferenceObjectData
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 2007;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x023FCE18;
		}
		__int32 m_Priority; //0x0060
		bool m_OverrideVisibility; //0x0064
		char _0x0065[11];
	};//Size=0x0070

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

	class GameEntityData : public SpatialEntityData
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 1884;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x023C5058;
		}
		Array<GameObjectData*> m_Components; //0x0050
		bool m_Enabled; //0x0054
		__int8 m_RuntimeComponentCount; //0x0055
		char _0x0056[10];
	};//Size=0x0060

	class VisualEnvironmentEntityData : public GameEntityData
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 1921;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x023FCD94;
		}
		float m_Visibility; //0x0060
		__int32 m_Priority; //0x0064
		char _0x0068[8];
	};//Size=0x0070

	class VisualEnvironmentBlueprint : public ObjectBlueprint
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 2427;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x023FCDC0;
		}
	};//Size=0x0024

	enum EmittableType
	{
		Point, //0x0000
		Quad, //0x0001
		ScreenAlignedQuad, //0x0002
		DirectionAlignedQuad, //0x0003
		WorldAlignedQuad, //0x0004
		Trail, //0x0005
		ParticleMesh, //0x0006
		EmittableTypeCount //0x0007
	};

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

	enum EmittableField
	{
		EfZero, //0x0000
		EfOne, //0x0001
		EfNormTime, //0x0002
		EfEmitterNormTime, //0x0003
		EfSpawnAnimationSpeed, //0x0004
		EfSpawnAnimationFrameIndex, //0x0005
		EfVelocity, //0x0006
		EfParameters, //0x0007
		EfRotation, //0x0008
		EfSpeed, //0x0009
		EfUserDefined, //0x000A
		EfConstantFloat, //0x000B
		EfConstantVec, //0x000C
		EfNone //0x000D
	};

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

	class EmitterTemplateData : public DataContainer
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 804;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x02407504;
		}
		char _0x0008[8];
		Vec4 m_PointLightIntensity; //0x0010
		Vec3 m_PointLightPivot; //0x0020
		Vec3 m_PointLightColor; //0x0030
		unsigned __int32 m_MaxCount; //0x0040
		char* m_Name; //0x0044
		float m_TimeScale; //0x0048
		unsigned __int32 m_LifetimeFrameCount; //0x004C
		float m_Lifetime; //0x0050
		class ProcessorData* m_RootProcessor; //0x0054
		float m_VisibleAfterDistance; //0x0058
		Array<float> m_ZOcclusionLookup; //0x005C
		EmittableType m_EmittableType; //0x0060
		class MeshAsset* m_Mesh; //0x0064
		float m_DistanceScaleNearValue; //0x0068
		float m_PointLightRadius; //0x006C
		float m_VertexPixelLightingBlendFactor; //0x0070
		float m_GlobalLocalNormalBlendFactor; //0x0074
		float m_SoftParticlesFadeDistanceMultiplier; //0x0078
		float m_LightWrapAroundFactor; //0x007C
		float m_LightMultiplier; //0x0080
		float m_DistanceScaleFarValue; //0x0084
		float m_PointLightRandomIntensityMin; //0x0088
		float m_MeshCullingDistance; //0x008C
		float m_PointLightRandomIntensityMax; //0x0090
		float m_MaxSpawnDistance; //0x0094
		float m_MinScreenArea; //0x0098
		float m_DistanceScaleLength; //0x009C
		float m_PointLightMaxClamp; //0x00A0
		float m_ParticleCullingFactor; //0x00A4
		float m_PointLightMinClamp; //0x00A8
		bool m_FollowSpawnSource; //0x00AC
		bool m_RepeatParticleSpawning; //0x00AD
		bool m_Emissive; //0x00AE
		bool m_ExclusionVolumeCullEnable; //0x00AF
		bool m_TransparencySunShadowEnable; //0x00B0
		bool m_ForceFullRes; //0x00B1
		bool m_LocalSpace; //0x00B2
		bool m_Opaque; //0x00B3
		bool m_ActAsPointLight; //0x00B4
		bool m_KillParticlesWithEmitter; //0x00B5
		bool m_ForceNiceSorting; //0x00B6
		char _0x00B7[9];
	};//Size=0x00C0

	class UpdateColorData : public ProcessorData
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 827;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x02407210;
		}
		Vec3 m_Color; //0x0030
	};//Size=0x0040

	enum EmitterParameter
	{
		EmitterParameterNone, //0x0000
		EmitterParameter1, //0x0001
		EmitterParameter2, //0x0002
		EmitterParameter3, //0x0003
		EmitterParameterVec, //0x0004
		EmitterParameterVecAverage, //0x0005
		EmitterParameterDistance, //0x0006
		EmitterParameter4 //0x0007
	};

	class EvaluatorData : public EmitterComponentData
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 806;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x024074D8;
		}
		EmitterParameter m_EmitterParameter; //0x0008
	};//Size=0x000C

	class PolynomialColorInterpData : public EvaluatorData
	{
	public:
		static __inline unsigned int ClassId()
		{
			return 807;
		}
		static __inline uintptr_t ClassInfoPtr()
		{
			return 0x0240778C;
		}
		char _0x000C[4];
		Vec3 m_Color0; //0x0010
		Vec3 m_Color1; //0x0020
		Vec4 m_Coefficients; //0x0030
	};//Size=0x0040

	enum ProcessorType
	{
		PtBaseEmitter, //0x0000
		PtSpawnRate, //0x0001
		PtSpawnSpeed, //0x0002
		PtSpawnPosition, //0x0003
		PtSpawnDirection, //0x0004
		PtSpawnSize, //0x0005
		PtSpawnAnimation, //0x0006
		PtSpawnAnimationFrame, //0x0007
		PtSpawnRotation, //0x0008
		PtSpawnOrientation, //0x0009
		PtSpawnRotationSpeed, //0x000A
		PtUpdatePosition, //0x000B
		PtUpdateAge, //0x000C
		PtTurbulance, //0x000D
		PtGravity, //0x000E
		PtLocalForce, //0x000F
		PtAirResistance, //0x0010
		PtUpdateLinearVelocity, //0x0011
		PtUpdateOrientation, //0x0012
		PtEmitter, //0x0013
		PtUpdateColor, //0x0014
		PtUpdateColorLeaf, //0x0015
		PtUpdateTransparency, //0x0016
		PtUpdateTextureCoords, //0x0017
		PtUpdateRotation, //0x0018
		PtUpdateSizeX, //0x0019
		PtUpdateSizeY, //0x001A
		PtUpdateSizeZ, //0x001B
		PtUpdateSize, //0x001C
		PtUpdateAlphaLevelMin, //0x001D
		PtUpdateAlphaLevelMax, //0x001E
		PtUpdateAlphaLevelScale, //0x001F
		PtUpdateClipScale, //0x0020
		PtUpdateCameraProximity, //0x0021
		ProcessorTypeCount //0x0022
	};

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

			return (T*)m_processors.begin()[v1];
		}
	};

	class IKeyboard
	{
	public:
		virtual void init();	// V: 0x0
		virtual void release();	// V: 0x4
		virtual void preFrameUpdate(float);	// V: 0x8
		virtual void update(float);	// V: 0xC
		virtual bool isDown(int);	// V: 0x10
		virtual bool wentDown(int);	// V: 0x14
		virtual bool wentUp(int);	// V: 0x18
		virtual void enableTypingMode(bool);	// V: 0x1C
	};
}