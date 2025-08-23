#pragma once
#include <filesystem>
#include <vector>
#include <algorithm>
#include <iostream>
#include "LayoutDimensions.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

class FileDialog
{
private:
	bool fileChosen = false;
	void NavigateTo(const std::filesystem::path &path);
	void RefreshDir();

	std::vector<std::filesystem::directory_entry> entries;
	std::filesystem::path chosenPath;
	std::filesystem::path currentDir = std::filesystem::current_path();

	int maxWindowWidth = 0;
	int maxWindowHeight = 0;

	fileDialogDims fileDialogDims;

public:
	void Init(float& _maxWindowWidth, float& _maxWindowHeight);
	void Render(float &_maxWindowWidth, float &_maxWindowHeight);
	bool isFileChosen() { return fileChosen; };
	std::string getChosenPath() { return  chosenPath.string(); }

};