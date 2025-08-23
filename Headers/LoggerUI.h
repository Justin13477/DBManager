#pragma once
#include "Logger.h"
#include "Window.h"
#include <deque>
#include <string>
#include <algorithm>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "LayoutDimensions.h"

class LoggerUI
{
private:
	std::deque<Logger::MsgData>* RecentLogs;
	float maxWindowWidth;
	float maxWindowHeight;

	loggerUIDims loggerUIDims;

public:
	LoggerUI(); // for displaying log messages in the app such as queries
	void Init(GLint _windowWidth, GLint _windowHeight);
	void Render(float& _maxWindowWidth, float& _maxWindowHeight);
};