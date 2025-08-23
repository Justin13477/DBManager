#pragma once
#include "Database.h"
#include "Window.h"
#include <vector>
#include <deque>
#include <algorithm>
#include "LayoutDimensions.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

class QueryConsole
{


public:
	void Init(float _windowWidth, float _windowHeight);
	void Render(std::shared_ptr<Database> db, float &_maxWindowWidth,float &_maxWindowHeight);

private:
	float maxWindowWidth;
	float maxWindowHeight;
	std::vector<std::string> colNames;
	char buffer[1024] = { 0 }; // buffer for typing commands
	queryConsoleDims queryConsoleDims;

	std::vector<std::vector<std::string>> lastQueryResults; // caches last query 
	std::vector<std::vector<std::string>> GetQueryResults() { return lastQueryResults; } // gets query results

	
	

};