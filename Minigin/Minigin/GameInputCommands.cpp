#include "MiniginPCH.h"
#include "GameInputCommands.h"
#include "Enumerations.h"
#include "DigDugMovementComponent.h"
#include "Button.h"
#include "ObservedData.h"
#include "GameEvents.h"
#include "SceneManager.h"
#include "SceneData.h"
#include "PlayerManager.h"
#include "Scene.h"
#include "EnemyManager.h"

using namespace Engine;
using namespace DigDug;

PlayerInput::PlayerInput(DigDug::Direction d, int playerId, Scene* pScene)
	: m_pScene(pScene)
	, m_Direction(d)
	, m_PlayerId(playerId)
{
}

void PlayerInput::Execute()
{
	if (m_pScene)
	{
		auto pm = m_pScene->GetSceneData().GetManager<PlayerManager>();
		if (pm)
		{
			auto pl = pm->GetPlayer(m_PlayerId);
			if (pl)
			{
				auto pMove = pl->GetComponent<DigDugMovementComponent>();
				if (pMove)
				{
					pMove->Move(m_Direction);
					if (pMove->GetGameObject() && m_Direction != Direction::None)
					{
						ObservedData d{};
						pMove->GetGameObject()->GetRoot().Notify(GameEvent::InputMovePressed, d);
						pMove->GetGameObject()->GetRoot().NotifyChildren(GameEvent::InputMovePressed, d);
					}
				}
			}
		}
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

void PlayerNotifier::Execute()
{
	if (m_pScene)
	{
		auto pm = m_pScene->GetSceneData().GetManager<PlayerManager>();
		if (pm)
		{
			auto pO = pm->GetPlayer(m_PlayerId);
			if (pO)
			{
				ObservedData d{};
				pO->Notify(m_Event, d);
				pO->NotifyChildren(m_Event, d);
			}
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

void SceneSetInput::Execute()
{
	if (m_pSceneManager)
	{
		m_pSceneManager->SetActiveScene(m_SceneName);
	}
}

void EnemyNotifier::Execute()
{
	if (m_pScene)
	{
		auto pm = m_pScene->GetSceneData().GetManager<EnemyManager>();
		if (pm)
		{
			auto pO = pm->GetEnemy(m_EnemyId);
			if (pO)
			{
				ObservedData d{};
				pO->Notify(m_Event, d);
				pO->NotifyChildren(m_Event, d);
			}
		}
	}
}

