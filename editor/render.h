#pragma once

#include "../SDK/sdk.h"
#include <imgui.h>
#include <string>

namespace render
{
	// TODO: use this
	namespace Colors
	{
		inline ImColor White = ImColor{ 255, 255, 255, 255 };
		inline ImColor Black = ImColor{ 0, 0, 0, 255 };
		inline ImColor Red = ImColor{ 255, 0, 0, 255 };
		inline ImColor Green = ImColor{ 0, 255, 0, 255 };
		inline ImColor DarkBlue = ImColor{ 0, 0, 255, 255 };
		inline ImColor LightBlue = ImColor{ 0, 140, 250, 255 };
		inline ImColor Grey = ImColor{ 128, 128, 128, 128 };
		inline ImColor Yellow = ImColor{ 255, 255, 0, 255 };
		inline ImColor Purple = ImColor{ 140, 20, 252, 255 };
		inline ImColor Turquoise = ImColor{ 60, 210, 200, 255 };
		inline ImColor Palevioletred = ImColor{ 220, 110, 150, 255 };
		inline ImColor Pink = ImColor{ 255, 100, 180, 255 };
		inline ImColor Coral = ImColor{ 255, 127, 80, 255 };
		inline ImColor Cyan = ImColor{ 0, 255, 255, 255 };
		inline ImColor Blank = ImColor{ 0, 0, 0, 0 };
		inline ImColor Orange = ImColor{ 255, 125, 0, 255 };
	}


	bool worldToScreen(const fb::Vec3& world, ImVec2& out);
	void drawSphere(const fb::Vec3& pos, float radius, int segments, int rings, const ImColor& color, float thickness = 1.0f);
	void text(const ImVec2& pos, ImFont* font, const std::string& text, const ImColor& color, bool centered = true, bool dropShadow = false);
	void text(const ImVec2& pos, const std::string& text, const ImColor& color, bool centered = true, bool dropShadow = false);
	void line(const ImVec2& start, const ImVec2& end, const ImColor& color, float thickness = 1.0f);
}