#include "MiniginPCH.h"
#include "GameInputCommands.h"
#include "Enumerations.h"
#include "DigDugMovementComponent.h"
#include "Button.h"

using namespace DigDug;

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

InputButtonSet::InputButtonSet(ButtonManager* pManager, Action action)
	: m_pManager(pManager)
	, m_Action(action)
{
}

void InputButtonSet::Execute()
{
	if (m_pManager)
	{
		switch (m_Action)
		{
		case Action::Next:
			m_pManager->Next();
			break;
		case Action::Previous:
			m_pManager->Previous();
			break;
		case Action::Select:
			m_pManager->Select();
			break;
		}
	}
}

void InputNotifier::Execute()
{
	if (m_pObject)
	{
		ObservedData d{};
		m_pObject->Notify(m_Event, d);
		m_pObject->NotifyChildren(m_Event, d);
	}
}

