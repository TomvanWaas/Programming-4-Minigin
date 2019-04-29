#include "MiniginPCH.h"
#include "Logger.h"
#include <iostream>

void Logger::LogInfo(const std::string& msg)
{
	std::cout << "[Logger::Info] > " << msg << std::endl;
}

void Logger::LogError(const std::string& msg)
{
	std::cout << "[Logger::Error] > " << msg << std::endl;
}

void Logger::LogWarning(const std::string& msg)
{
	std::cout << "[Logger::Warning] > " << msg << std::endl;
}

