#pragma once

#include "eastl.h"

#define WORLD_RENDER_SETTINGS			0x1426724A0
#define VISUAL_ENVIRONMENT_MANAGER		0x1426724A0 // 48 8B 0D ? ? ? ? 48 85 C9 74 ? 48 8B D3 E8 ? ? ? ? 4C 8B C3

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

	struct VisualEnvironmentState
	{
		float visibility;
		int priority;
		bool overrideVisibility;
		bool excluded;
		char pad_0x0A[0x2];
		int stateId;
		Vec4 activeEffectPosition;
	};

	class VisualEnvironment
	{
	public:

	};

	class VisualEnvironmentManager
	{
	public:
		static VisualEnvironmentManager* GetInstance()
		{
			return *(VisualEnvironmentManager**)VISUAL_ENVIRONMENT_MANAGER;
		}

		char pad[0x30];
		eastl::vector<VisualEnvironmentState*> m_states;

		void setDirty(bool dirty)
		{
			*(bool*)((char*)this + 0x1FC8) = dirty;
		}

		fb::VisualEnvironment& getEnv()
		{
			// in fb3 ida fails to recognize it as seperate struct
			return *(VisualEnvironment*)((char*)this + 0x1010);
		}
	};
}