#pragma once
#include "Singleton.h"

class Logger final: public dae::Singleton<Logger>
{
public:
	Logger() = default;
	~Logger();
	Logger(const Logger& other) = delete;
	Logger(Logger&& other) = delete;
	Logger& operator=(const Logger& other) = delete;
	Logger& operator=(Logger&& other) = delete;


	void LogInfo(const std::string& msg);
	void LogError(const std::string& msg);
	void LogWarning(const std::string& msg);

private:



};

