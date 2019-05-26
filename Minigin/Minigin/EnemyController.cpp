#include "MiniginPCH.h"
#include "EnemyController.h"
#include "GameObject.h"
#include "GameEvents.h"
#include "DigDugGridComponent.h"
#include "Time.h"
#include "SceneData.h"
#include "ObservedData.h"
#include "Enumerations.h"
#include "PlayerManager.h"
#include "FiniteStateMachineComponent.h"

using namespace DigDug;
using namespace Engine;



EnemyController::EnemyController(DigDugGridComponent* pGrid, float fireInterval, float ghostInterval)
	: m_pGrid(pGrid)
	, m_Current(Direction::None)
	, m_FireAccu(0)
	, m_GhostAccu(0)
	, m_FireInterval(fireInterval)
	, m_GhostInterval(ghostInterval)
{
}

void EnemyController::UpdateFirstOverride(const SceneData& sceneData)
{
	auto pObject = GetGameObject();
	if (!pObject) return;
	auto pFsm = pObject->GetComponent<FiniteStateMachineComponent>();
	if (!pFsm) return;

	std::string name = pFsm->GetStateName();
	if (name == "MoveState")
	{
		m_FireAccu += sceneData.GetTime()->GetDeltaTime();
		if (m_FireAccu >= m_FireInterval)
		{
			m_FireAccu -= m_FireInterval;
			pObject->Notify(GameEvent::InputFirePressed, ObservedData{});
			return;
		}

		m_GhostAccu += sceneData.GetTime()->GetDeltaTime();
		if (m_GhostAccu >= m_GhostInterval)
		{
			m_GhostAccu -= m_GhostInterval;
			pObject->Notify(GameEvent::InputGhostPressed, ObservedData{});
			return;
		}
		UpdateAsMove(sceneData);
	}
	else if (name == "GhostState")
	{
		m_Current = Direction::None;
		UpdateAsGhost(sceneData);
	}	
}

void EnemyController::UpdateAsMove(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);

	auto pObject = GetGameObject();
	if (!pObject || !m_pGrid) return;

	const auto& world = pObject->GetTransform().GetWorldPosition();
	const auto& offset = m_pGrid->GetOffset();

	bool marked[5]{ false };
	marked[int(Direction::Left)] = m_pGrid->IsMarked(m_pGrid->ClosestPoint(world + offset * Vector2(-1, 0)));
	marked[int(Direction::Right)] = m_pGrid->IsMarked(m_pGrid->ClosestPoint(world + offset * Vector2(1, 0)));
	marked[int(Direction::Up)] = m_pGrid->IsMarked(m_pGrid->ClosestPoint(world + offset * Vector2(0, -1)));
	marked[int(Direction::Down)] = m_pGrid->IsMarked(m_pGrid->ClosestPoint(world + offset * Vector2(0, 1)));
	marked[int(Direction::None)] = true;


	//If on walkable => All dir possible (depending on marked)
	int chance = rand() % 3;
	if ( (m_pGrid->IsOnWalkablePoint(world) && (chance == 0)) || m_Current == Direction::None)
	{
		for (int i = 0; i < 5; ++i)
		{
			int d = rand() % 4 + 1;
			if (marked[d])
			{
				m_Current = Direction(d);
				break;
			}
		}

	}


	//Reverse if forward not marked
	if (!marked[int(m_Current)])
	{
		switch (m_Current)
		{
		case Direction::Left:
		{
			m_Current = Direction::Right;
		} break;
		case Direction::Right:
		{
			m_Current = Direction::Left;
		} break;
		case Direction::Up:
		{
			m_Current = Direction::Down;
		} break;
		case Direction::Down:
		{
			m_Current = Direction::Up;
		} break;
		}
	}

	//If marked, notify object
	if (marked[int(m_Current)])
	{
		ObservedData d{};
		switch (m_Current)
		{
		case Direction::Left:
		{
			pObject->Notify(GameEvent::InputLeftPressed, d);
		} break;
		case Direction::Right:
		{
			pObject->Notify(GameEvent::InputRightPressed, d);
		} break;
		case Direction::Up:
		{
			pObject->Notify(GameEvent::InputUpPressed, d);
		} break;
		case Direction::Down:
		{
			pObject->Notify(GameEvent::InputDownPressed, d);
		} break;
		}
	}
}


void EnemyController::UpdateAsGhost(const SceneData& sceneData)
{
	auto pO = GetGameObject();

	//Get Closest player
	auto pm = sceneData.GetManager<PlayerManager>();
	if (pm && pO)
	{
		auto closest = pm->GetClosestPlayer(pO->GetTransform().GetWorldPosition());
		if (closest)
		{
			ObservedData d{};
			Vector2 dir = closest->GetTransform().GetWorldPosition() - pO->GetTransform().GetWorldPosition();
			dir.Normalize();
			if (dir.x > 0.5f)
			{
				pO->Notify(GameEvent::InputRightPressed, d);
			}
			else if (dir.x < -0.5f)
			{
				pO->Notify(GameEvent::InputLeftPressed, d);
			}

			if (dir.y > 0.5f)
			{
				pO->Notify(GameEvent::InputDownPressed, d);
			}
			else if (dir.y < -0.5f)
			{
				pO->Notify(GameEvent::InputUpPressed, d);
			}
		}		
	}
}

