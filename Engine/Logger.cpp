#include "stdafx.h"
#include "Logger.h"
#include <iostream>


Logger::Logger()
{
	m_ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	m_Log.open("./Log.txt");

	////To Check all colors
	//for (int i = 0; i < 255; ++i)
	//{
	//	SetConsoleTextAttribute(m_ConsoleHandle, WORD(i));
	//	std::cout << std::to_string(i) << ": Color\n";
	//}
}

void Logger::LogInfo(const std::string& msg)
{
	UNREFERENCED_PARAMETER(msg);
#ifdef LOG_CONSOLE
	SetConsoleTextAttribute(m_ConsoleHandle, 15);	//Hardcoded number
	std::cout << "[Logger::Info] > " << msg << std::endl;
#endif
#ifdef LOG_FILE
	m_Log << "[Info] > " << msg << std::endl;
#endif
}

void Logger::LogError(const std::string& msg)
{
	UNREFERENCED_PARAMETER(msg);
#ifdef LOG_CONSOLE
	SetConsoleTextAttribute(m_ConsoleHandle, 207);	//Hardcoded number
	std::cout << "[Logger::Error] > " << msg << std::endl;
#endif
#ifdef LOG_FILE
	m_Log << "[Error] > " << msg << std::endl;
#endif
}

void Logger::LogWarning(const std::string& msg)
{
	UNREFERENCED_PARAMETER(msg);
#ifdef LOG_CONSOLE
	SetConsoleTextAttribute(m_ConsoleHandle, 14);	//Hardcoded number
	std::cout << "[Logger::Warning] > " << msg << std::endl;
#endif
#ifdef LOG_FILE
	m_Log << "[Warning] > " << msg << std::endl;
#endif
}

