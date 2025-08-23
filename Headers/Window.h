#pragma once
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

// glad and glfw
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <iostream>

// Imgui 
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


#include "Logger.h"

class Window
{
public:
	Window(std::string _appName);
	Window(std::string _appName, GLint windowWidth, GLint windowHeight);

	int Initialise();

	float getBufferWidth() { return bufferWidth; }
	float getBufferHeight() { return bufferHeight; }

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

	bool* getKeys() { return keys; }
	float getXChange();
	float getYChange();

	void swapBuffers() { return glfwSwapBuffers(mainWindow); }

	GLFWwindow* getWindow();

	int getWindowWidth() { return width; }
	int getWindowHeight() { return height; }


	~Window();

private:
	GLFWwindow* mainWindow;
	GLFWmonitor* monitor;
	const GLFWvidmode* mode;

	std::string appName;
	int width, height;
	int bufferWidth, bufferHeight;

	bool keys[1024];

	float lastX;
	float lastY;
	float xChange = 0.0f;
	float yChange = 0.0f;
	bool mouseFirstMoved;

	void createCallbacks();
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};