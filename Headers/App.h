#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS

#include <string>
#include "Window.h"
#include "Database.h"
#include "Logger.h"
#include "FileDialog.h"
#include "QueryConsole.h"
#include "LoggerUI.h"
#include "SchemaEditorUI.h"
#include "LayoutDimensions.h"
#include "json.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


class App
{

public:
	struct AppState // manages all app specific details
	{               // create a theme class with an internal struct to control app theme that has presets that is saved to init json   
		bool dbOpen = false;
		bool dbFirstOpen = false;

		bool openWindow = true;

		std::string appName; // window name
		std::string path; // db name
		std::string logFileName; // log dump file

		// App dimensions
		// Layout
		float maxWindowWidth = 0;
		float maxWindowHeight = 0;

		// updates all ui windows dynamically
		void update(float w, float h)
		{
			maxWindowWidth = w;
			maxWindowHeight = h;
		}
	} state;

private:

	std::shared_ptr<Database> dbPtr = std::make_shared<Database>(); // ensures there is only one instance of the db 

	// composite class
	FileDialog fileDialog;
	QueryConsole queryConsole;
	LoggerUI logUI;
	SchemaEditorUI schemaEditor;


	Window mainWindow; // modify to accept window name
	GLFWwindow* theWindow = nullptr;


	void Init(); // initalize all classes
	void Shutdown(); // clean up all classes
	void RenderGUI(); // main render loop

	

	

public:
	App(std::string& _appName, std::string& _logFileName);
	App(std::string& _appName, std::string& _logFileName, GLint windowX, GLint windowY);
	void Run(bool &running); // brings it all together into a single command
		
};