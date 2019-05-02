#include "MiniginPCH.h"
#include "GameInputCommands.h"
#include "Enumerations.h"
#include "DigDugMovementComponent.h"


PlayerInput::PlayerInput(DigDug::Direction d, DigDug::DigDugMovementComponent* pMove)
	: m_pMovement(pMove)
	, m_Direction(d)
{
}

void PlayerInput::Execute()
{
	if (m_pMovement != nullptr)
	{
		m_pMovement->Move(m_Direction);
	}
}

