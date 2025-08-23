#pragma once
#include "Window.h"
#include "stb_image.h"

Window::Window(std::string _appName) : appName(_appName) // change to make easier
{
    width = 1920;
    height = 1080;
    for (size_t i = 0; i <= 1024; i++)
    {
        keys[i] = 0;
    };
}

Window::Window(std::string _appName, GLint windowWidth, GLint windowHeight ) : appName(_appName) // change to make easier
{
    width = windowWidth;
    height = windowHeight;

    for (size_t i = 0; i <= 1024; i++)
    {
        keys[i] = 0;
    };
}

int Window::Initialise()
{

    /* Initialize the library */
    if (!glfwInit())
    {
        Logger::GetInstance().LogMsg(Logger::ERR, "Failed to init GLFW");
        return -1;
    }
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    
    if(width == 0 || height == 0)
    {
        monitor = glfwGetPrimaryMonitor();
        mode = glfwGetVideoMode(monitor);
        width = mode->width;
        height = mode->height;
    }

    std::cout << width << ":" << height << std::endl;
    
    /* Create a windowed mode window and its OpenGL context */
    mainWindow = glfwCreateWindow(width, height, appName.c_str(), NULL, NULL);
    if (!mainWindow)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(mainWindow);
    

    // Handle key + Mouse Input
    createCallbacks();

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        Logger::GetInstance().LogMsg(Logger::ERR, "GLEW failed to inti");
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    glEnable(GL_DEPTH_TEST);

    glfwSetWindowUserPointer(mainWindow, this);

    
}



GLFWwindow* Window::getWindow()
{
    return mainWindow;
}

void Window::createCallbacks()
{
    glfwSetKeyCallback(mainWindow, handleKeys);
    glfwSetCursorPosCallback(mainWindow, handleMouse);
}

GLfloat Window::getXChange()
{
    GLfloat theChange = xChange;
    xChange = 0.0f;
    return theChange;
}

GLfloat Window::getYChange()
{
    GLfloat theChange = yChange;
    yChange = 0.0f;
    return theChange;
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            theWindow->keys[key] = true;

        }
        else if (action == GLFW_RELEASE)
        {
            theWindow->keys[key] = false;

        }
    }
};

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    theWindow->mouseFirstMoved;
    if (theWindow->mouseFirstMoved)
    {
        theWindow->lastX = xPos;
        theWindow->lastY = yPos;
        theWindow->mouseFirstMoved = false;

    }
    theWindow->xChange = xPos - theWindow->lastX;
    theWindow->yChange = theWindow->lastY - yPos;

    theWindow->lastX = xPos;
    theWindow->lastY = yPos;
}

Window::~Window()
{
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}
