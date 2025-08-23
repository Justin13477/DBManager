#include "App.h"

App::App(std::string& _appName, std::string& _logFileName)
    : mainWindow(_appName)
{
	state.appName = _appName;
    state.logFileName = _logFileName;
    state.maxWindowWidth = 1920;
    state.maxWindowHeight = 1080;
   

}
App::App(std::string& _appName, std::string& _logFileName, GLint windowX, GLint windowY)
    : mainWindow(_appName, windowX, windowY)

{
    state.appName = _appName;
    state.logFileName = _logFileName;
    state.maxWindowWidth = windowX;
    state.maxWindowHeight = windowY;
    
}
void App::Init()
{
    
    // init all windows with max dimensions
	mainWindow.Initialise();
    theWindow = mainWindow.getWindow();

    queryConsole.Init(state.maxWindowWidth, state.maxWindowHeight);
    fileDialog.Init(state.maxWindowWidth, state.maxWindowHeight);
    logUI.Init(state.maxWindowWidth, state.maxWindowHeight);
    schemaEditor.Init(dbPtr, state.maxWindowWidth, state.maxWindowHeight);

    // init imgui
    ImGui::CreateContext();
    IMGUI_CHECKVERSION();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(mainWindow.getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");

}

void App::Shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

}

void App::RenderGUI()
{
    // these need to take a ref to dim structs
    // make each class a panel for modularity and update positions in layout dimensions. Hardcoded with relative dimensions
    queryConsole.Render(dbPtr, state.maxWindowWidth, state.maxWindowHeight); // db access directly bad design ik
    ImGui::SameLine();
    schemaEditor.Render(state.maxWindowWidth, state.maxWindowHeight);
    logUI.Render(state.maxWindowWidth, state.maxWindowHeight);

}
void App::Run(bool &running) // for some restart functionality to change dbs without restarting
{
    running = !mainWindow.getShouldClose();
    Init();
    
    while (running = !mainWindow.getShouldClose())
    {
        state.maxWindowWidth = mainWindow.getWindowWidth(); // refresh applications dimensions each frame not sure if this is the correct way
        state.maxWindowHeight = mainWindow.getWindowHeight();

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // gui goes here       

        if (fileDialog.isFileChosen() && !state.dbFirstOpen) 
        { 
            state.path = fileDialog.getChosenPath(); // gets path
            dbPtr->OpenDB(state.path); // opens that path
            state.dbOpen = true; // maybe wrap this into a function to make opening and closing dbs more uniform 
            state.dbFirstOpen = true;
        }
        else if (!fileDialog.isFileChosen() && !state.dbOpen)
        {
            fileDialog.Render(state.maxWindowWidth, state.maxWindowHeight); // renders file dialog button and updates it internal state 
            
        }
        
        else if (fileDialog.isFileChosen() && state.dbOpen) // when file is chosen then finally opened
        {
            // render here 
            ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
            ImGui::SetNextWindowSize(ImVec2(state.maxWindowWidth, state.maxWindowHeight));
            bool openMainWindow = ImGui::Begin("Database Manager", &state.openWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
            if(!openMainWindow)
                Logger::GetInstance().LogMsg(Logger::ERR, "Main window failed to open");
            if(openMainWindow)
            {
                RenderGUI(); // all wrapped into one function
            }
            ImGui::End();
        }

       

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwPollEvents();

        glUseProgram(0);

        /* Swap front and back buffers */
        mainWindow.swapBuffers();

        /* Poll for and process events */
        glfwPollEvents();
    }
    Shutdown();
}

