#include "LoggerUI.h"

LoggerUI::LoggerUI()
{
	RecentLogs = Logger::GetInstance().GetRecentMsgs();

}

void LoggerUI::Init(GLint _windowWidth, GLint _windowHeight)
{
	maxWindowWidth = _windowWidth;
	maxWindowHeight = _windowHeight;

	
	
}
void LoggerUI::Render(float &_maxWindowWidth, float &_maxWindowHeight)
{
	maxWindowWidth = _maxWindowWidth;
	maxWindowHeight = _maxWindowHeight;
	loggerUIDims.update(maxWindowWidth, maxWindowHeight); // update dimensions
	bool openLoggerChild = ImGui::BeginChild("Logger UI", loggerUIDims.LoggerUIVec, true);
	if(openLoggerChild)
	{
		ImVec4 color;
		for (auto& msg : *RecentLogs) // for readablity with colors
		{
			switch (msg.level)
			{
			case Logger::INFO:
				color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
				break;

			case Logger::DEBUG:
				color = ImVec4(0, 1.0f, 0, 1.0f);
				break;

			case Logger::WARNING:
				color = ImVec4(1.0f, 1.0f, 0, 1.0f);
				break;

			case Logger::ERR:
				color = ImVec4(1.0f, 0, 0, 1.0f);
				break;

			default:
				color = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
				break;
			}
			ImGui::TextColored(color, "%s", msg.msg.c_str());
		}
		ImGui::EndChild();
	}
	else
	{
		ImGui::EndChild();
		Logger::GetInstance().LogMsg(Logger::ERR, "Logger failed to open");
	}
}