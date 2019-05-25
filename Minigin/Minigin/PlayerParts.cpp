#include "MiniginPCH.h"
#include "PlayerParts.h"
#include "GameObject.h"
#include "Components.h"
#include "Enumerations.h"
#include "DigDugMovementComponent.h"
#include "GameEvents.h"
#include "ObservedData.h"
#include "DigDugGridComponent.h"
#include "SceneData.h"
#include "Time.h"
#include "Scene.h"

using namespace Minigin;
using namespace DigDug;

DigDug::FSMStatePlayerIdle::FSMStatePlayerIdle(const std::string& deadState, const std::string& crushedState,
	const std::string& throwState, const std::string& moveState)
	: m_DeadState(deadState)
	, m_CrushedState(crushedState)
	, m_ThrowState(throwState)
	, m_MoveState(moveState)
{
}
void DigDug::FSMStatePlayerIdle::Enter(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(data);
	UNREFERENCED_PARAMETER(sceneData);
	auto go = GetGameObject();
	if (go)
	{
		auto d = Direction::Right;
		auto* pMove = go->GetComponent<DigDugMovementComponent>();
		if (pMove)
		{
			d = pMove->GetLookDirection();
		}

		auto* pSprite = go->GetComponent<SpriteComponent>();
		if (pSprite)
		{
			switch (d)
			{
			case Direction::Right:
				pSprite->SetCurrentSprite(unsigned int(PlayerSpriteID::IdleRight));
				break;
			case Direction::Down:
				pSprite->SetCurrentSprite(unsigned int(PlayerSpriteID::IdleDown));
				break;
			case Direction::Left:
				pSprite->SetCurrentSprite(unsigned int(PlayerSpriteID::IdleLeft));
				break;
			case Direction::Up:
				pSprite->SetCurrentSprite(unsigned int(PlayerSpriteID::IdleUp));
				break;
			}
		}


	}
}
FSMState* DigDug::FSMStatePlayerIdle::OnNotify(ObservedEvent oevent, const ObservedData& odata, FSMData& data)
{
	UNREFERENCED_PARAMETER(odata);
	UNREFERENCED_PARAMETER(data);
	switch (oevent)
	{
		break;
	case GameEvent::InputPumpPressed:
		return GetState(m_ThrowState);
		break;
	case GameEvent::InputMovePressed:
		return GetState(m_MoveState);
		break;
	case GameEvent::HitByObstacle:
		return GetState(m_CrushedState);
		break;
	case GameEvent::HitByEnemy:
		return GetState(m_DeadState);
		break;
	}
	return this;
}





DigDug::FSMStatePlayerMoving::FSMStatePlayerMoving(const std::string& deadState, const std::string& crushedState,
	const std::string& throwState, const std::string& idleState, DigDugGridComponent* pGrid)
	: m_DeadState(deadState)
	, m_CrushedState(crushedState)
	, m_IdleState(idleState)
	, m_ThrowState(throwState)
	, m_pGrid(pGrid)
{
}
void DigDug::FSMStatePlayerMoving::Enter(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(data);
	UNREFERENCED_PARAMETER(sceneData);
	auto go = GetGameObject();
	if (go)
	{
		//Enable move comp
		auto d = Direction::Right;
		auto* pMove = go->GetComponent<DigDugMovementComponent>();
		if (pMove)
		{
			pMove->SetEnabled(true);
			d = pMove->GetLookDirection();
		}

		auto* pSprite = go->GetComponent<SpriteComponent>();
		if (pSprite)
		{
			switch (d)
			{
			case Direction::Right:
				pSprite->SetCurrentSprite(unsigned int(PlayerSpriteID::DigRight));
				break;
			case Direction::Down:
				pSprite->SetCurrentSprite(unsigned int(PlayerSpriteID::DigDown));
				break;
			case Direction::Left:
				pSprite->SetCurrentSprite(unsigned int(PlayerSpriteID::DigLeft));
				break;
			case Direction::Up:
				pSprite->SetCurrentSprite(unsigned int(PlayerSpriteID::DigUp));
				break;
			}
		}
	}
}
void DigDug::FSMStatePlayerMoving::Exit(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(sceneData);
	UNREFERENCED_PARAMETER(data);
	auto go = GetGameObject();
	if (go)
	{
		//Disable move comp
		auto pMove = go->GetComponent<DigDugMovementComponent>();
		if (pMove) pMove->SetEnabled(false);
	}
}
FSMState* DigDug::FSMStatePlayerMoving::UpdateSecond(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(data);
	UNREFERENCED_PARAMETER(sceneData);
	auto go = GetGameObject();
	if (go)
	{
		auto pMove = go->GetComponent<DigDugMovementComponent>();
		if (pMove && pMove->GetMoveDirection() == Direction::None)
		{
			return GetState(m_IdleState);
		}
		else if (pMove)
		{
			auto d = pMove->GetLookDirection();
			auto* pSprite = go->GetComponent<SpriteComponent>();
			if (pSprite)
			{
				switch (d)
				{
				case Direction::Right:
					if (m_pGrid && m_pGrid->IsMarked(m_pGrid->ClosestPoint(go->GetTransform().GetWorldPosition() + m_pGrid->GetWalkableOffset()*Vector2(1,0)*0.5f)))
					{
						pSprite->SetCurrentSprite(unsigned int(PlayerSpriteID::MoveRight));
					}
					else
					{
						pSprite->SetCurrentSprite(unsigned int(PlayerSpriteID::DigRight));
					}
					break;
				case Direction::Down:
					if (m_pGrid && m_pGrid->IsMarked(m_pGrid->ClosestPoint(go->GetTransform().GetWorldPosition() + m_pGrid->GetWalkableOffset()*Vector2(0, 1)*0.5f)))
					{
						pSprite->SetCurrentSprite(unsigned int(PlayerSpriteID::MoveDown));
					}
					else
					{
						pSprite->SetCurrentSprite(unsigned int(PlayerSpriteID::DigDown));
					}
					break;
				case Direction::Left:
					if (m_pGrid && m_pGrid->IsMarked(m_pGrid->ClosestPoint(go->GetTransform().GetWorldPosition() + m_pGrid->GetWalkableOffset()*Vector2(-1, 0)*0.5f)))
					{
						pSprite->SetCurrentSprite(unsigned int(PlayerSpriteID::MoveLeft));
					}
					else
					{
						pSprite->SetCurrentSprite(unsigned int(PlayerSpriteID::DigLeft));
					}
					break;
				case Direction::Up:
					if (m_pGrid && m_pGrid->IsMarked(m_pGrid->ClosestPoint(go->GetTransform().GetWorldPosition() + m_pGrid->GetWalkableOffset()*Vector2(0, -1)*0.5f)))
					{
						pSprite->SetCurrentSprite(unsigned int(PlayerSpriteID::MoveUp));
					}
					else
					{
						pSprite->SetCurrentSprite(unsigned int(PlayerSpriteID::DigUp));
					}
					break;
				}
			}
		}

		if (m_pGrid)
		{
			m_pGrid->Mark(go->GetTransform().GetWorldPosition(), 0.5f);
		}
	}
	return this;
}
FSMState* DigDug::FSMStatePlayerMoving::OnNotify(ObservedEvent oevent, const ObservedData& odata, FSMData& data)
{
	UNREFERENCED_PARAMETER(odata);
	UNREFERENCED_PARAMETER(data);
	switch (oevent)
	{
	case GameEvent::InputPumpPressed:
		return GetState(m_ThrowState);
		break;
	case GameEvent::HitByObstacle:
		return GetState(m_CrushedState);
		break;
	case GameEvent::HitByEnemy:
		return GetState(m_DeadState);
		break;
	}
	return this;
}




DigDug::FSMStatePlayerThrowing::FSMStatePlayerThrowing(const std::string& deadState, const std::string& crushedState,
	const std::string& idleState, const std::string& pumpState)
	: m_DeadState(deadState)
	, m_CrushedState(crushedState)
	, m_IdleState(idleState)
	, m_PumpState(pumpState)
{
}
void DigDug::FSMStatePlayerThrowing::Enter(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(data);
	UNREFERENCED_PARAMETER(sceneData);

	auto go = GetGameObject();
	if (go)
	{
		auto d = Direction::Right;
		auto* pMove = go->GetComponent<DigDugMovementComponent>();
		if (pMove)
		{
			d = pMove->GetLookDirection();
		}

		auto* pSprite = go->GetComponent<SpriteComponent>();
		if (pSprite)
		{
			switch (d)
			{
			case Direction::Right:
				pSprite->SetCurrentSprite(unsigned int(PlayerSpriteID::ThrowRight));
				break;
			case Direction::Down:
				pSprite->SetCurrentSprite(unsigned int(PlayerSpriteID::ThrowDown));
				break;
			case Direction::Left:
				pSprite->SetCurrentSprite(unsigned int(PlayerSpriteID::ThrowLeft));
				break;
			case Direction::Up:
				pSprite->SetCurrentSprite(unsigned int(PlayerSpriteID::ThrowUp));
				break;
			}
		}
	}
}
FSMState* DigDug::FSMStatePlayerThrowing::OnNotify(ObservedEvent oevent, const ObservedData& odata, FSMData& data)
{
	UNREFERENCED_PARAMETER(odata);
	UNREFERENCED_PARAMETER(data);
	switch (oevent)
	{
	case GameEvent::PumpMissed:
		return GetState(m_IdleState);
		break;
	case GameEvent::PumpHit:
		return GetState(m_PumpState);
		break;
	case GameEvent::HitByObstacle:
		return GetState(m_CrushedState);
		break;
	case GameEvent::HitByEnemy:
		return GetState(m_DeadState);
		break;
	}
	return this;
}







DigDug::FSMStatePlayerPumping::FSMStatePlayerPumping(const std::string& deadState, const std::string& crushedState,
	const std::string& idleState, const std::string& moveState)
	: m_IdleState(idleState)
	, m_DeadState(deadState)
	, m_CrushedState(crushedState)
	, m_MoveState(moveState)
{
}
void DigDug::FSMStatePlayerPumping::Enter(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(data);
	UNREFERENCED_PARAMETER(sceneData);

	auto go = GetGameObject();
	if (go)
	{
		auto d = Direction::Right;
		auto* pMove = go->GetComponent<DigDugMovementComponent>();
		if (pMove)
		{
			d = pMove->GetLookDirection();
		}

		auto* pSprite = go->GetComponent<SpriteComponent>();
		if (pSprite)
		{
			switch (d)
			{
			case Direction::Right:
				pSprite->SetCurrentSprite(unsigned int(PlayerSpriteID::PumpRight));
				break;
			case Direction::Down:
				pSprite->SetCurrentSprite(unsigned int(PlayerSpriteID::PumpDown));
				break;
			case Direction::Left:
				pSprite->SetCurrentSprite(unsigned int(PlayerSpriteID::PumpLeft));
				break;
			case Direction::Up:
				pSprite->SetCurrentSprite(unsigned int(PlayerSpriteID::PumpUp));
				break;
			}
		}
	}
}
FSMState* DigDug::FSMStatePlayerPumping::OnNotify(ObservedEvent oevent, const ObservedData& odata, FSMData& data)
{
	UNREFERENCED_PARAMETER(odata);
	UNREFERENCED_PARAMETER(data);
	switch (oevent)
	{
	case GameEvent::InputPumpReleased:
		return GetState(m_IdleState);
		break;
	case GameEvent::InputMovePressed:
		return GetState(m_MoveState);
		break;
	case GameEvent::HitByObstacle:
		return GetState(m_CrushedState);
		break;
	case GameEvent::HitByEnemy:
		return GetState(m_DeadState);
		break;
	case GameEvent::PumpMissed:
		return GetState(m_IdleState);
		break;
	}
	return this;
}





FSMStatePlayerCrushed::FSMStatePlayerCrushed(const std::string& deadState, DigDugGridComponent* pGrid, float fallspeed)
	: m_pGrid(pGrid)
	, m_Speed(fallspeed)
	, m_DeadState(deadState)
{
}
void DigDug::FSMStatePlayerCrushed::Enter(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(sceneData);
	UNREFERENCED_PARAMETER(data);
	auto go = GetGameObject();
	if (go)
	{
		auto d = Direction::Right;
		auto* pMove = go->GetComponent<DigDugMovementComponent>();
		if (pMove)
		{
			d = pMove->GetLookDirection();
		}

		auto* pSprite = go->GetComponent<SpriteComponent>();
		if (pSprite)
		{
			switch (d)
			{
			case Direction::Right:
				pSprite->SetCurrentSprite(unsigned int(PlayerSpriteID::IdleRight));
				break;
			case Direction::Down:
				pSprite->SetCurrentSprite(unsigned int(PlayerSpriteID::IdleDown));
				break;
			case Direction::Left:
				pSprite->SetCurrentSprite(unsigned int(PlayerSpriteID::IdleLeft));
				break;
			case Direction::Up:
				pSprite->SetCurrentSprite(unsigned int(PlayerSpriteID::IdleUp));
				break;
			}
		}
	}
}
void DigDug::FSMStatePlayerCrushed::Exit(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(data);
	UNREFERENCED_PARAMETER(sceneData);
	auto go = GetGameObject();
	if (go)
	{
		auto d = Direction::Right;
		auto* pMove = go->GetComponent<DigDugMovementComponent>();
		if (pMove)
		{
			d = pMove->GetLookDirection();
		}

		auto* pSprite = go->GetComponent<SpriteComponent>();
		if (pSprite)
		{
			switch (d)
			{
			case Direction::Right:
				pSprite->SetCurrentSprite(unsigned int(PlayerSpriteID::CrushedRight));
				break;
			case Direction::Down:
				pSprite->SetCurrentSprite(unsigned int(PlayerSpriteID::CrushedDown));
				break;
			case Direction::Left:
				pSprite->SetCurrentSprite(unsigned int(PlayerSpriteID::CrushedLeft));
				break;
			case Direction::Up:
				pSprite->SetCurrentSprite(unsigned int(PlayerSpriteID::CrushedUp));
				break;
			}
		}
	}
}
FSMState* DigDug::FSMStatePlayerCrushed::UpdateFirst(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(data);
	auto go = GetGameObject();
	if (go && m_pGrid)
	{
		auto& t = go->GetTransform();

		//If on gp && below !marked => return deadstate 
		Vector2 o = m_pGrid->GetWalkableOffset()*0.5f;
		if (!m_pGrid->IsMarked(m_pGrid->ClosestPoint(t.GetWorldPosition() + Vector2(0, o.y))) &&
			(m_pGrid->ClosestPoint(t.GetWorldPosition()).y - t.GetWorldPosition().y) < m_Speed*sceneData.GetTime()->GetDeltaTime()*0.5f) 
		{
			return GetState(m_DeadState);
		}

		//Else move down
		Vector2 next = t.GetWorldPosition() + Vector2(0, m_Speed * sceneData.GetTime()->GetDeltaTime());
		next = m_pGrid->ClosestGrid(next) - t.GetWorldPosition();
		t.SetLocalPosition(t.GetLocalPosition() + next);
	}
	return this;
}









void FSMStatePlayerDead::Enter(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(data);
	UNREFERENCED_PARAMETER(sceneData);
	auto go = GetGameObject();
	if (go)
	{
		m_Accu = 0;

		//Disable Pump
		auto children = go->GetAllChildren();
		for (auto* child : children)
		{
			if (child) child->SetEnabled(false);
		}


		//Sprite
		auto d = Direction::Right;
		auto* pMove = go->GetComponent<DigDugMovementComponent>();
		if (pMove)
		{
			d = pMove->GetLookDirection();
		}

		auto* pSprite = go->GetComponent<SpriteComponent>();
		if (pSprite)
		{
			switch (d)
			{
			case Direction::Right:
				pSprite->SetCurrentSprite(unsigned int(PlayerSpriteID::DiedRight));
				break;
			case Direction::Down:
				pSprite->SetCurrentSprite(unsigned int(PlayerSpriteID::DiedDown));
				break;
			case Direction::Left:
				pSprite->SetCurrentSprite(unsigned int(PlayerSpriteID::DiedLeft));
				break;
			case Direction::Up:
				pSprite->SetCurrentSprite(unsigned int(PlayerSpriteID::DiedUp));
				break;
			}
		}
	}
}
FSMState* FSMStatePlayerDead::UpdateFirst(const SceneData& sceneData, FSMData& data)
{
	m_Accu += sceneData.GetTime()->GetDeltaTime();
	if (m_Accu >= m_Delay)
	{
		auto* pObject = GetGameObject();
		GameObject::DeleteObject(pObject);
		return nullptr;
	}
	UNREFERENCED_PARAMETER(data);
	return this;
}

























