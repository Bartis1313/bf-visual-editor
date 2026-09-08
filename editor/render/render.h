#pragma once

#include "../../SDK/fb.h"
#include <imgui.h>
#include <string>

enum class NotifyType
{
	Info,
	Success,
	Warning,
	Error
};

namespace render
{
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

	enum class Backend { ImGui, Engine };
	inline Backend backend = Backend::Engine;
	bool engineReady();
	ImVec2 displaySize();
	void line2(const ImVec2& a, const ImVec2& b, const ImColor& c, float thickness = 1.0f);
	void rect2(const ImVec2& mn, const ImVec2& mx, const ImColor& c, bool filled = false, float thickness = 1.0f);
	void label(const ImVec2& pos, const char* text, const ImColor& c, float scale = 1.0f, bool shadow = true);
	float textWidth(const char* text, float scale = 1.0f);
	float textHeight(float scale = 1.0f);
	inline bool lineDepthTest = true;
	inline float lineWidthWorld = 0.0f;
	void line3(const fb::Vec3& a, const fb::Vec3& b, const ImColor& c, float thickness = 1.0f);

	bool worldToScreen(const fb::Vec3& world, ImVec2& out);
	bool worldToScreen(const fb::Vec3& world, ImVec2& out, float& depth);
	bool cameraPosition(fb::Vec3& out);
	bool cameraForward(fb::Vec3& out);
	void drawSphere(const fb::Vec3& pos, float radius, int segments, int rings, const ImColor& color, float thickness = 1.0f);
	void circle(const ImVec2& center, float radius, const ImColor& color, int segments = 20, float thickness = 1.5f);
	void text(const ImVec2& pos, ImFont* font, const std::string& text, const ImColor& color, bool centered = true, bool dropShadow = false);
	void text(const ImVec2& pos, const std::string& text, const ImColor& color, bool centered = true, bool dropShadow = false);
	void line(const ImVec2& start, const ImVec2& end, const ImColor& color, float thickness = 1.0f);

	struct ImNotify
	{
		ImNotify() = default;
		ImNotify(const std::string& title, const std::string& message, float maxTime, NotifyType type)
			: title{ title }, message{ message }, maxTime{ maxTime }, type{ type }
		{
			time = ImGui::GetTime();
		}

		float maxTime{ };
		std::string title{ };
		std::string message{ };
		NotifyType type{ NotifyType::Info };
		double time{ };
		float alpha{ };

		static void handle();
		static void add(NotifyType type, const std::string& title, const std::string& message, float time = 5.0f);
	};
}