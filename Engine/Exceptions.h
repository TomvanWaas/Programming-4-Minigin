#pragma once
#include <exception>
namespace Minigin
{
	class ResourceException final : public std::exception
	{
		std::string m_ResourcePath;
	public:
		explicit ResourceException(const std::string& resourcePath)
			: m_ResourcePath(resourcePath)
		{}
		virtual ~ResourceException() = default;

		const char* what() const throw ()
		{
			std::stringstream msg{};
			msg << "Failed to load Resource: " << m_ResourcePath;
			std::string msgS = msg.str();
			return msgS.c_str();
		}
	};
}