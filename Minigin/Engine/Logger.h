#pragma once
#include "Singleton.h"
#include <fstream>

namespace Engine
{
	class Logger final : public Engine::Singleton<Logger>
	{
	public:
		Logger();
		virtual ~Logger() = default;
		Logger(const Logger& other) = delete;
		Logger(Logger&& other) = delete;
		Logger& operator=(const Logger& other) = delete;
		Logger& operator=(Logger&& other) = delete;

		void SetPath(const std::string& path);

		void LogInfo(const std::string& msg);
		void LogError(const std::string& msg);
		void LogWarning(const std::string& msg);

	private:
		HANDLE m_ConsoleHandle;
		std::ofstream m_Log;


	};

}