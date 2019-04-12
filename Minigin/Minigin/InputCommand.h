#pragma once
class InputCommand abstract
{
public:
	InputCommand() = default;
	virtual ~InputCommand() = default;

	virtual void Execute() = 0;

};