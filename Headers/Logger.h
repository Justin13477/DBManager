#pragma once
#include <ctime>
#include <fstream>
#include <mutex>
#include <deque>
//#include <print>
#include <iostream>

class Logger
{
public:
	enum LogLevel
	{
		INFO, DEBUG, WARNING, ERR
	};

	struct MsgData
	{
		std::string msg;
		LogLevel level;
	};

	std::deque<MsgData> RecentMsgs;
	std::deque<MsgData>* RecentMsgsPtr = &RecentMsgs;

	static Logger& GetInstance()
	{
		static Logger instance("log.txt");
		return instance;
	}
private:
	const char* fileName;
	int numOfLogsHeld = 200;

	std::string LoglevelToString(LogLevel level)
	{
		switch (level)
		{
		case INFO: return "[INFO]";
		case DEBUG: return "[DEBUG]";
		case WARNING: return "[WARNING]";
		case ERR: return "[ERROR]";
		default: return "[UNKNOWN]";
		}
	}

	std::string GetTimeStamp()
	{
		std::time_t now = std::time(nullptr);
		char buf[32];
		std::tm timeInfo;
		localtime_s(&timeInfo, &now);
		std::strftime(buf, sizeof(buf), "[%Y-%m-%d %H:%M:%S] ", &timeInfo);
		return std::string(buf);
	}

public: 
	Logger(const char* _fileName)
	{
		fileName = _fileName;
	}
	
	void AddRecentMsg(std::string msg, LogLevel level) // stores recent message for viewing only what is caught when running
	{
		if (RecentMsgs.size() >= numOfLogsHeld)
		{
			RecentMsgs.pop_front();
			
		}
		MsgData data = { msg, level };
		RecentMsgs.push_back(data);
	}
	std::mutex mtx; // creates mutex
	int LogMsg(LogLevel level, const char* _msg)
	{
		
		std::lock_guard<std::mutex> lock(mtx); // locks thread and auto unlocks with RAII

		std::ofstream logFile(fileName, std::ios::app); // append mode
		if (!logFile.is_open()) 
		{
			//std::println("Log file failed to open");
			return -1;
		}
		
		std::string formatted = GetTimeStamp() + LoglevelToString(level) + " " + _msg; // formatted string
		
		logFile << formatted << "\n"; // logs to file
		std::cout << formatted << std::endl;
		AddRecentMsg(formatted, level); // adds to deque for display in gui

		logFile.close();
		return 1;
	}

	std::deque<MsgData>* GetRecentMsgs() { return RecentMsgsPtr; }
};