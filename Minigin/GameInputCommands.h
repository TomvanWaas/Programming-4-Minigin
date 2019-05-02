#pragma once
#include "InputCommand.h"


namespace DigDug
{
	enum class Direction;
	class DigDugMovementComponent;
}

class PlayerInput final : public InputCommand
{
	DigDug::Direction m_Direction;
	DigDug::DigDugMovementComponent* m_pMovement;
public:
	explicit PlayerInput(DigDug::Direction d, DigDug::DigDugMovementComponent* pMove);
	virtual void Execute() override;
};

template <class T>
class InputSetData final : public InputCommand
{
	T* m_pData;
	T m_Target;
public:
	explicit InputSetData(T& ref, const T& val)
		: m_Target(val)
		, m_pData(&ref)
	{}
	virtual void Execute() override
	{
		if (m_pData != nullptr) *m_pData = m_Target;
	}
};












