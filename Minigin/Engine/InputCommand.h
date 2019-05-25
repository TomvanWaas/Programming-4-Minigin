#pragma once
#include <vector>
namespace Minigin
{

	class InputCommand abstract
	{
	public:
		explicit InputCommand() = default;
		virtual ~InputCommand() = default;

		virtual void Execute() = 0;

	};

	class MultiInputCommand final : public InputCommand
	{
	public:
		explicit MultiInputCommand(const std::vector<std::shared_ptr<InputCommand>>& commands)
			: m_pCommands(commands)
		{}
		virtual ~MultiInputCommand() = default;
		virtual void Execute() override
		{
			for (std::shared_ptr<InputCommand>& pCommand : m_pCommands)
			{
				if (pCommand != nullptr)
				{
					pCommand->Execute();
				}
			}
		}
	private:
		std::vector<std::shared_ptr<InputCommand>> m_pCommands;
	};
}