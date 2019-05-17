#include "MiniginPCH.h"
#include "EnemyParts.h"
#include "GameObject.h"
#include "Components.h"
#include "DigDugGridComponent.h"
#include "Time.h"
#include "ObservedData.h"
#include "PlayerManager.h"
#include "Enumerations.h"
#include "GameEvents.h"
#include "ObservedEvent.h"
#include "SpriteCollisionComponent.h"


using namespace DigDug;


FSMStatePookaMoving::FSMStatePookaMoving(float speed, DigDugGridComponent* pGrid, GameObject* pObject,
	FSMState* pPumped, FSMState* pGhost, FSMState* pCrushed, float ghostDelay)
	: m_pObject(pObject)
	, m_pGrid(pGrid)
	, m_pPumpedState(pPumped)
	, m_CurrentDirection(Direction::Right)
	, m_AccuTime(0)
	, m_Speed(speed)
	, m_pGhostState(pGhost)
	, m_pCrushedState(pCrushed)
	, m_GhostTime(ghostDelay)
{
}
void FSMStatePookaMoving::Enter(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(sceneData);
	UNREFERENCED_PARAMETER(data);
	//Prepare Colliders
	m_AccuTime = 0.0f;

	if (m_pObject)
	{
		//If Directio not yet in BData
		Direction d = Direction::None;
		if (!data.GetData<Direction>("Move", d))
		{
			data.AddData("Move", Direction::Right);
			d = Direction::Right;
		}
		m_CurrentDirection = d;
		if (!(data.GetData<Direction>("Look", d)))
		{
			data.AddData("Look", Direction::Right);
			d = Direction::Right;
		}

		auto* pSprite = m_pObject->GetComponent<SpriteComponent>();
		if (pSprite && d == Direction::Right)
		{
			pSprite->SetCurrentSprite(unsigned int(EnemySpriteID::MoveRight));
		}
		else if (pSprite && d == Direction::Left)
		{
			pSprite->SetCurrentSprite(unsigned int(EnemySpriteID::MoveLeft));
		}
		


		auto* pCol = m_pObject->GetComponent<AABBCollisionComponent>();
		if (pCol)
		{
			pCol->SetTrigger(true);
		}
	}

	
}
FSMState* FSMStatePookaMoving::UpdateFirst(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(data);
	m_AccuTime += sceneData.GetTime()->GetDeltaTime();
	if (m_AccuTime >= m_GhostTime)
	{
		return m_pGhostState;
	}


	if (m_pObject && m_pGrid)
	{
		auto& t = m_pObject->GetTransform();
		auto* pMove = m_pObject->GetComponent<MovementComponent>();
		if (pMove)
		{
			Vector2 world = t.GetWorldPosition();
			Vector2 offset = m_pGrid->GetWalkableOffset()*0.5f;

			bool marked[5]{ false };
			marked[int(Direction::Left)] = m_pGrid->IsMarked(m_pGrid->ClosestPoint(world + offset * Vector2(-1, 0)));
			marked[int(Direction::Right)] = m_pGrid->IsMarked(m_pGrid->ClosestPoint(world + offset * Vector2(1, 0)));
			marked[int(Direction::Up)] = m_pGrid->IsMarked(m_pGrid->ClosestPoint(world + offset * Vector2(0, -1)));
			marked[int(Direction::Down)] = m_pGrid->IsMarked(m_pGrid->ClosestPoint(world + offset * Vector2(0, 1)));
			marked[int(Direction::None)] = true;

			//If on gridpoint => Can change dir (Or if forward is not marked
			//If on walkable => All dir possible
			if (m_pGrid->IsOnWalkablePoint(world) && (rand() % 3 == 0 || m_CurrentDirection == Direction::None))
			{
				SetDirection(GetRandomDirection(marked), data);
			}
			//Else if on normal, reverse possible
			if (!marked[int(m_CurrentDirection)] && m_pGrid->IsOnPoint(world))
			{
				switch (m_CurrentDirection)
				{
				case Direction::Left: SetDirection(Direction::Right, data); break;
				case Direction::Right: SetDirection(Direction::Left, data); break;
				case Direction::Up: SetDirection(Direction::Down, data); break;
				case Direction::Down: SetDirection(Direction::Up, data); break;
				}
			}

			//Move
			Vector2 dir = Vector2::Zero;
			switch (m_CurrentDirection)
			{
			case Direction::Left:
				dir.x -= 1;
				break;
			case Direction::Right:
				dir.x += 1;
				break;
			case Direction::Down:
				dir.y += 1;
				break;
			case Direction::Up:
				dir.y -= 1;
				break;
			}
			//Clamp
			dir *= m_Speed * sceneData.GetTime()->GetDeltaTime();
			dir = m_pGrid->ClosestGrid(dir + world) - world;

			pMove->QueueMovement(dir);
		}
	}
	return this;
}
FSMState* FSMStatePookaMoving::OnNotify(ObservedEvent oevent, const ObservedData& odata, FSMData& data)
{
	UNREFERENCED_PARAMETER(data);
	UNREFERENCED_PARAMETER(odata);
	switch (oevent)
	{
	case ObservedEvent::ColliderEntered:
	{
		AABBCollisionComponent* pCol = nullptr;
		if (odata.GetData<AABBCollisionComponent*>("Collider", pCol) && pCol && pCol->GetGameObject())
		{
			if (pCol->GetTag() == "Pump")
			{
				return m_pPumpedState;
			}
			else if (pCol->GetTag() == "Player")
			{
				auto& root = pCol->GetGameObject()->GetRoot();
				ObservedData d{};
				root.Notify(GameEvent::HitByEnemy, d);
				root.NotifyChildren(GameEvent::HitByEnemy, d);
			}
		}
		break;
	}
	case GameEvent::HitByObstacle:
	{
		return m_pCrushedState;
		break;
	}
	}
	return this;
}
void FSMStatePookaMoving::Exit(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(sceneData);
	data.SetData<Direction>("Move", m_CurrentDirection);
}

void FSMStatePookaMoving::SetDirection(Direction d, FSMData& data)
{
	if (d == Direction::Left)
	{
		SetSprite(unsigned int(EnemySpriteID::MoveLeft));
		data.SetData("Look", d);
	}
	else if (d == Direction::Right)
	{
		SetSprite(unsigned int(EnemySpriteID::MoveRight));
		data.SetData("Look", d);
	}
	else if (d == Direction::None && m_CurrentDirection == Direction::Left)
	{
		SetSprite(unsigned int(EnemySpriteID::IdleLeft));
		data.SetData("Look", m_CurrentDirection);
	}
	else if (d == Direction::None && m_CurrentDirection == Direction::Right)
	{
		SetSprite(unsigned int(EnemySpriteID::IdleRight));
		data.SetData("Look", m_CurrentDirection);
	}
	m_CurrentDirection = d;
}
Direction FSMStatePookaMoving::GetRandomDirection(bool* marks)
{
	int tries = 5;
	while (tries-- > 0)
	{
		int r = rand() % 4;
		if (marks[r] && m_CurrentDirection != Direction(r)) return Direction(r);
	}
	return Direction::None;
}
void FSMStatePookaMoving::SetSprite(unsigned i)
{
	if (m_pObject)
	{
		auto* pSprite = m_pObject->GetComponent<SpriteComponent>();
		if (pSprite) pSprite->SetCurrentSprite(i);
	}
}




FSMStateEnemyPumping::FSMStateEnemyPumping(GameObject* pObject, float targetTime, float deflateSpeed, FSMState* pMove, FSMState* pDead, FSMState* pCrushed)
	: m_pMovingState(pMove)
	, m_pDeadState(pDead)
	, m_pObject(pObject)
	, m_IsBeingPumped(true)
	, m_AccuPump(0)
	, m_TargetPump(targetTime)
	, m_DeflateSpeed(deflateSpeed)
	, m_pCrushedState(pCrushed)
{
}
void FSMStateEnemyPumping::Enter(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(sceneData);
	m_IsBeingPumped = true;

	if (m_pObject)
	{
		m_AccuPump += m_TargetPump * 0.25f;

		Direction d{};
		if (!data.GetData("Look", d)) { d = Direction::Right; }

		auto* pSprite = m_pObject->GetComponent<SpriteComponent>();
		if (pSprite && d == Direction::Right)
		{
			pSprite->SetCurrentSprite(unsigned int(EnemySpriteID::PumpedRightTier1));
		}
		else if (pSprite && d == Direction::Left)
		{
			pSprite->SetCurrentSprite(unsigned int(EnemySpriteID::PumpedLeftTier1));
		}

		auto *pCol = m_pObject->GetComponent<AABBCollisionComponent>();
		if (pCol)
		{
			pCol->SetEnabled(true);
			pCol->SetTrigger(true);
		}

	}
}
FSMState* FSMStateEnemyPumping::UpdateFirst(const SceneData& sceneData, FSMData& data)
{
	if (m_IsBeingPumped)
	{
		m_AccuPump += sceneData.GetTime()->GetDeltaTime();
		if (m_AccuPump >= m_TargetPump)
		{
			Direction look;
			if (!data.GetData("Look", look)) look = Direction::Right;
			auto* pSprite = m_pObject->GetComponent<SpriteComponent>();
			if (pSprite && look == Direction::Left) pSprite->SetCurrentSprite(unsigned int(EnemySpriteID::PumpedLeftTier4));
			else if (pSprite && look == Direction::Right) pSprite->SetCurrentSprite(unsigned int(EnemySpriteID::PumpedRightTier4));
			
			//Disable Coll
			auto col = m_pObject->GetComponent<AABBCollisionComponent>();
			if (col) col->SetEnabled(false);
			return m_pDeadState;
		}
	}
	else
	{
		m_AccuPump -= sceneData.GetTime()->GetDeltaTime() * m_DeflateSpeed;
		if (m_AccuPump <= 0)
		{
			m_AccuPump = 0;
			return m_pMovingState;
		}
	}
	//Update sprite by Accu
	if (m_pObject)
	{
		Direction look;
		if (!data.GetData("Look", look)) look = Direction::Right;

		auto* pSprite = m_pObject->GetComponent<SpriteComponent>();
		if (pSprite)
		{
			float frac = m_AccuPump / m_TargetPump;
			if (frac < 0.33f)
			{
				if (look == Direction::Left) pSprite->SetCurrentSprite(unsigned int(EnemySpriteID::PumpedLeftTier1));
				else if (look == Direction::Right) pSprite->SetCurrentSprite(unsigned int(EnemySpriteID::PumpedRightTier1));
			}
			else if (frac < 0.66f)
			{
				if (look == Direction::Left) pSprite->SetCurrentSprite(unsigned int(EnemySpriteID::PumpedLeftTier2));
				else if (look == Direction::Right) pSprite->SetCurrentSprite(unsigned int(EnemySpriteID::PumpedRightTier2));
			}
			else
			{
				if (look == Direction::Left) pSprite->SetCurrentSprite(unsigned int(EnemySpriteID::PumpedLeftTier3));
				else if (look == Direction::Right) pSprite->SetCurrentSprite(unsigned int(EnemySpriteID::PumpedRightTier3));
			}
		}
	}

	return this;
}
FSMState* FSMStateEnemyPumping::OnNotify(ObservedEvent oevent, const ObservedData& odata, FSMData& data)
{
	UNREFERENCED_PARAMETER(data);
	UNREFERENCED_PARAMETER(odata);

	switch (oevent)
	{
	case ObservedEvent::ColliderEntered:
	{
		AABBCollisionComponent* pCol = nullptr;
		if (!m_IsBeingPumped && odata.GetData<AABBCollisionComponent*>("Collider", pCol) && pCol && pCol->GetGameObject())
		{
			if (pCol->GetTag() == "Pump")
			{
				m_IsBeingPumped = true;
				m_AccuPump += m_TargetPump * 0.25f;
				Logger::GetInstance().LogInfo("FSMStateEnemyPumping::OnNotify > Pump Attached...");
			}
		}
		break;
	}
	case GameEvent::HitByObstacle:
		return m_pCrushedState;
		break;

	case  ObservedEvent::ColliderExited:
		{		

		AABBCollisionComponent* pCol = nullptr;
		if (m_IsBeingPumped && odata.GetData<AABBCollisionComponent*>("Collider", pCol) && pCol && pCol->GetGameObject())
		{
			if (pCol->GetTag() == "Pump")
			{
				m_IsBeingPumped = false;
				Logger::GetInstance().LogInfo("FSMStateEnemyPumping::OnNotify > Pump Released...");
			}
		}
		break;
		}
	}
	return this;
}



FSMStateEnemyCrushed::FSMStateEnemyCrushed(DigDugGridComponent* pGrid, GameObject* pObj, float fallspeed, FSMState* pDead)
	: m_pObject(pObj)
	, m_pGrid(pGrid)
	, m_pDeadState(pDead)
	, m_FallSpeed(fallspeed)
{
}
void FSMStateEnemyCrushed::Enter(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(sceneData);
	if (m_pObject)
	{
		Direction d = Direction::Right;
		if (!data.GetData("Look", d)) d = Direction::Right;

		auto* pSprite = m_pObject->GetComponent<SpriteComponent>();
		if (pSprite && d == Direction::Right)
		{
			pSprite->SetCurrentSprite(unsigned int(EnemySpriteID::IdleRight));
		}
		else if (pSprite && d == Direction::Left)
		{
			pSprite->SetCurrentSprite(unsigned int(EnemySpriteID::IdleLeft));
		}

		auto* pCol = m_pObject->GetComponent<AABBCollisionComponent>();
		if (pCol) pCol->SetEnabled(false);
	}
}
FSMState* FSMStateEnemyCrushed::UpdateFirst(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(data);
	if (m_pObject && m_pGrid)
	{
		auto& t = m_pObject->GetTransform();

		//If on gp && below !marked => return deadstate 
		Vector2 o = m_pGrid->GetWalkableOffset()*0.5f;
		if (!m_pGrid->IsMarked(m_pGrid->ClosestPoint(t.GetWorldPosition() + Vector2(0, o.y))) &&	//Below marked
			(m_pGrid->ClosestPoint(t.GetWorldPosition()).y - t.GetWorldPosition().y) < m_FallSpeed*sceneData.GetTime()->GetDeltaTime()*0.5f) //Is on point (Y)
		{
			return m_pDeadState;
		}

		//Else move down
		Vector2 next = t.GetWorldPosition() + Vector2(0, m_FallSpeed * sceneData.GetTime()->GetDeltaTime());
		next = m_pGrid->ClosestGrid(next) - t.GetWorldPosition();
		t.SetLocalPosition(t.GetLocalPosition() + next);
	}
	return this;
}
void FSMStateEnemyCrushed::Exit(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(sceneData);

	if (m_pObject)
	{
		Direction look = Direction::Right;
		if (!data.GetData("Look", look)) look = Direction::Right;

		auto* pSprite = m_pObject->GetComponent<SpriteComponent>();
		if (pSprite && look == Direction::Right)
		{
			pSprite->SetCurrentSprite(unsigned int(EnemySpriteID::CrushedRight));
		}
		else if (pSprite && look == Direction::Left)
		{
			pSprite->SetCurrentSprite(unsigned int(EnemySpriteID::CrushedLeft));
		}
	}
}








FSMStateEnemyGhost::FSMStateEnemyGhost(GameObject* pObj, float speed, float duration, DigDugGridComponent* pGrid, FSMState* pMove, FSMState* pCrushed,
	FSMState* pPumped)
	: m_pObject(pObj)
	, m_pCrushedState(pCrushed)
	, m_pMoveState(pMove)
	, m_pPumpedState(pPumped)
	, m_pGrid(pGrid)
	, m_Speed(speed)
	, m_MinDuration(duration)
{
}
void FSMStateEnemyGhost::Enter(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(data);

	m_Accu = 0;
	
	auto* pm = sceneData.GetManager<PlayerManager>();
	if (pm && m_pObject)
	{
		m_pClosestPlayer = pm->GetClosestPlayer(m_pObject->GetTransform().GetWorldPosition());
	}

	if (m_pObject)
	{
		auto* pSprite = m_pObject->GetComponent<SpriteComponent>();
		if (pSprite)
		{
			pSprite->SetCurrentSprite(unsigned int(EnemySpriteID::Ghost));
		}
	}

}
FSMState* FSMStateEnemyGhost::UpdateFirst(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(data);
	if (m_pObject && m_pGrid && m_pClosestPlayer)
	{
		auto& t = m_pObject->GetTransform();

		m_Accu += sceneData.GetTime()->GetDeltaTime();
		if (m_Accu >= m_MinDuration)
		{
			//Close to marked point & duration ran out => Move to closest point
			Vector2 closest = m_pGrid->ClosestWalkablePoint(t.GetWorldPosition());
			if (m_pGrid->IsMarked(closest))
			{
				if (t.GetWorldPosition().Equals(closest, m_Speed*sceneData.GetTime()->GetDeltaTime()*0.5f))
				{
					t.SetWorldPosition(closest);
					return m_pMoveState;
				}
				else
				{
					t.SetLocalPosition((closest - t.GetWorldPosition()).Normalized() * m_Speed * sceneData.GetTime()->GetDeltaTime() + t.GetLocalPosition());
				}
			}
		}
		else
		{
			Vector2 dir = m_pClosestPlayer->GetTransform().GetWorldPosition();
			dir -= t.GetWorldPosition();
			dir.Normalize();
			t.SetLocalPosition(t.GetLocalPosition() + dir * m_Speed * sceneData.GetTime()->GetDeltaTime());
		}
	}
	return this;
}
FSMState* FSMStateEnemyGhost::OnNotify(ObservedEvent oevent, const ObservedData& odata, FSMData& data)
{
	UNREFERENCED_PARAMETER(data);

	switch (oevent)
	{
	case ObservedEvent::ColliderEntered:
	{
		AABBCollisionComponent* pCol = nullptr;
		if (odata.GetData<AABBCollisionComponent*>("Collider", pCol) && pCol && pCol->GetGameObject())
		{
			if (pCol->GetTag() == "Pump")
			{
				return m_pPumpedState;
			}
			else if (pCol->GetTag() == "Player")
			{
				auto& root = pCol->GetGameObject()->GetRoot();
				ObservedData d{};
				root.Notify(GameEvent::HitByEnemy, d);
				root.NotifyChildren(GameEvent::HitByEnemy, d);
			}
		}
		break;
	}
	case GameEvent::HitByObstacle:
	{
		return m_pCrushedState;
		break;
	}
	}
	return this;
}









FSMStateFygarMoving::FSMStateFygarMoving(float speed, float fireDelay, float ghostDelay, DigDugGridComponent* pGrid,
	const std::string& pumpedState, const std::string& ghostState, const std::string& crushedState,
	const std::string& fireState)
	: m_CurrentDirection(Direction::Right)
	, m_GhostAccu(0)
	, m_pGrid(pGrid)
	, m_Speed(speed)
	, m_CrushedState(crushedState)
	, m_GhostTime(ghostDelay)
	, m_PumpedState(pumpedState)
	, m_GhostState(ghostState)
	, m_FireState(fireState)
	, m_FireTime(fireDelay)
	, m_FireAccu(0)
{
}
void FSMStateFygarMoving::Enter(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(sceneData);
	UNREFERENCED_PARAMETER(data);
	//Prepare Colliders


	auto obj = GetGameObject();
	if (obj)
	{
		//If Directio not yet in BData
		Direction d = Direction::None;
		if (!data.GetData<Direction>("Move", d))
		{
			data.AddData("Move", Direction::Right);
			d = Direction::Right;
		}
		m_CurrentDirection = d;
		if (!(data.GetData<Direction>("Look", d)))
		{
			data.AddData("Look", Direction::Right);
			d = Direction::Right;
		}

		auto* pSprite = obj->GetComponent<SpriteComponent>();
		if (pSprite && d == Direction::Right)
		{
			pSprite->SetCurrentSprite(unsigned int(EnemySpriteID::MoveRight));
		}
		else if (pSprite && d == Direction::Left)
		{
			pSprite->SetCurrentSprite(unsigned int(EnemySpriteID::MoveLeft));
		}
		auto* pCol = obj->GetComponent<AABBCollisionComponent>();
		if (pCol)
		{
			pCol->SetTrigger(true);
		}
	}
}
FSMState* FSMStateFygarMoving::UpdateFirst(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(data);
	m_GhostAccu += sceneData.GetTime()->GetDeltaTime();
	m_FireAccu += sceneData.GetTime()->GetDeltaTime();

	if (m_GhostAccu >= m_GhostTime)
	{
		m_GhostAccu = 0;
		return GetState(m_GhostState);
	}
	else if (m_FireAccu >= m_FireTime)
	{
		m_FireAccu = 0;
		return GetState(m_FireState);
	}

	auto obj = GetGameObject();
	if (obj && m_pGrid)
	{
		auto& t = obj->GetTransform();
		auto* pMove = obj->GetComponent<MovementComponent>();
		if (pMove)
		{
			Vector2 world = t.GetWorldPosition();
			Vector2 offset = m_pGrid->GetWalkableOffset()*0.5f;

			bool marked[5]{ false };
			marked[int(Direction::Left)] = m_pGrid->IsMarked(m_pGrid->ClosestPoint(world + offset * Vector2(-1, 0)));
			marked[int(Direction::Right)] = m_pGrid->IsMarked(m_pGrid->ClosestPoint(world + offset * Vector2(1, 0)));
			marked[int(Direction::Up)] = m_pGrid->IsMarked(m_pGrid->ClosestPoint(world + offset * Vector2(0, -1)));
			marked[int(Direction::Down)] = m_pGrid->IsMarked(m_pGrid->ClosestPoint(world + offset * Vector2(0, 1)));
			marked[int(Direction::None)] = true;

			//If on gridpoint => Can change dir (Or if forward is not marked
			//If on walkable => All dir possible
			if (m_pGrid->IsOnWalkablePoint(world) && (rand() % 3 == 0 || m_CurrentDirection == Direction::None))
			{
				SetDirection(GetRandomDirection(marked), data);
			}
			//Else if on normal, reverse possible
			if (!marked[int(m_CurrentDirection)] && m_pGrid->IsOnPoint(world))
			{
				switch (m_CurrentDirection)
				{
				case Direction::Left: SetDirection(Direction::Right, data); break;
				case Direction::Right: SetDirection(Direction::Left, data); break;
				case Direction::Up: SetDirection(Direction::Down, data); break;
				case Direction::Down: SetDirection(Direction::Up, data); break;
				}
			}

			//Move
			Vector2 dir = Vector2::Zero;
			switch (m_CurrentDirection)
			{
			case Direction::Left:
				dir.x -= 1;
				break;
			case Direction::Right:
				dir.x += 1;
				break;
			case Direction::Down:
				dir.y += 1;
				break;
			case Direction::Up:
				dir.y -= 1;
				break;
			}
			//Clamp
			dir *= m_Speed * sceneData.GetTime()->GetDeltaTime();
			dir = m_pGrid->ClosestGrid(dir + world) - world;

			pMove->QueueMovement(dir);
		}
	}
	return this;
}
FSMState* FSMStateFygarMoving::OnNotify(ObservedEvent oevent, const ObservedData& odata, FSMData& data)
{
	UNREFERENCED_PARAMETER(data);
	UNREFERENCED_PARAMETER(odata);
	switch (oevent)
	{
	case ObservedEvent::ColliderEntered:
	{
		AABBCollisionComponent* pCol = nullptr;
		if (odata.GetData<AABBCollisionComponent*>("Collider", pCol) && pCol && pCol->GetGameObject())
		{
			if (pCol->GetTag() == "Pump")
			{
				return GetState(m_PumpedState);
			}
			else if (pCol->GetTag() == "Player")
			{
				auto& root = pCol->GetGameObject()->GetRoot();
				ObservedData d{};
				root.Notify(GameEvent::HitByEnemy, d);
				root.NotifyChildren(GameEvent::HitByEnemy, d);
			}
		}
		break;
	}
	case GameEvent::HitByObstacle:
	{
		return GetState(m_CrushedState);
		break;
	}
	}
	return this;
}
void FSMStateFygarMoving::Exit(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(sceneData);
	data.SetData<Direction>("Move", m_CurrentDirection);
}
void FSMStateFygarMoving::SetDirection(Direction d, FSMData& data)
{
	if (d == Direction::Left)
	{
		SetSprite(unsigned int(EnemySpriteID::MoveLeft));
		data.SetData("Look", d);
	}
	else if (d == Direction::Right)
	{
		SetSprite(unsigned int(EnemySpriteID::MoveRight));
		data.SetData("Look", d);
	}
	else if (d == Direction::None && m_CurrentDirection == Direction::Left)
	{
		SetSprite(unsigned int(EnemySpriteID::IdleLeft));
		data.SetData("Look", m_CurrentDirection);
	}
	else if (d == Direction::None && m_CurrentDirection == Direction::Right)
	{
		SetSprite(unsigned int(EnemySpriteID::IdleRight));
		data.SetData("Look", m_CurrentDirection);
	}
	m_CurrentDirection = d;
}
Direction FSMStateFygarMoving::GetRandomDirection(bool* marks)
{
	int tries = 5;
	while (tries-- > 0)
	{
		int r = rand() % 4;
		if (marks[r] && m_CurrentDirection != Direction(r)) return Direction(r);
	}
	return Direction::None;
}
void FSMStateFygarMoving::SetSprite(unsigned i)
{
	auto go = GetGameObject();
	if (go)
	{
		auto* pSprite = go->GetComponent<SpriteComponent>();
		if (pSprite) pSprite->SetCurrentSprite(i);
	}
}






FSMStateFygarFire::FSMStateFygarFire(float time, const std::string& pumpedState, const std::string& crushedState,
	const std::string& moveState)
	: m_Accu(0)
	, m_CrushedState(crushedState)
	, m_Time(time)
	, m_MoveState(moveState)
	, m_PumpedState(pumpedState)
{
}
void FSMStateFygarFire::Enter(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(sceneData);
	m_Accu = 0;

	auto go = GetGameObject();
	if (go)
	{
		Direction d;
		if (!data.GetData<Direction>("Look", d))
		{
			d = Direction::Right;
		}
		//Sprite
		auto pSprite = go->GetComponent<SpriteComponent>();
		if (pSprite)
		{
			switch(d)
			{
			case Direction::Left:
				pSprite->SetCurrentSprite(unsigned int(EnemySpriteID::FireLeft));
				break;
			case Direction::Right:
				pSprite->SetCurrentSprite(unsigned int(EnemySpriteID::FireRight));
				break;
			}
		}

		//Notify child (Fire)
		ObservedData out{};
		out.AddData<Direction>("Direction", d);
		go->NotifyChildren(GameEvent::FygarFire, out);

	}
}
FSMState* FSMStateFygarFire::UpdateFirst(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(data);
	m_Accu += sceneData.GetTime()->GetDeltaTime();
	if (m_Accu >= m_Time)
	{
		return GetState(m_MoveState);
	}
	return this;
}
FSMState* FSMStateFygarFire::OnNotify(ObservedEvent oevent, const ObservedData& odata, FSMData& data)
{
	UNREFERENCED_PARAMETER(data);

	switch (oevent)
	{
	case ObservedEvent::ColliderEntered:
	{
		AABBCollisionComponent* pCol = nullptr;
		if (odata.GetData<AABBCollisionComponent*>("Collider", pCol) && pCol && pCol->GetGameObject())
		{
			if (pCol->GetTag() == "Pump")
			{
				return GetState(m_PumpedState);
			}
			else if (pCol->GetTag() == "Player")
			{
				auto& root = pCol->GetGameObject()->GetRoot();
				ObservedData d{};
				root.Notify(GameEvent::HitByEnemy, d);
				root.NotifyChildren(GameEvent::HitByEnemy, d);
			}
		}
		break;
	}
	case GameEvent::HitByObstacle:
	{
		return GetState(m_CrushedState);
		break;
	}
	}
	return this;
}






FSMStateFireIdle::FSMStateFireIdle(const std::string& activeState)
	: m_ActiveState(activeState)
{
}
void FSMStateFireIdle::Enter(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(sceneData);
	UNREFERENCED_PARAMETER(data);
	auto go = GetGameObject();
	if (go)
	{
		auto pSpr = go->GetComponent<SpriteComponent>();
		if (pSpr)
		{
			pSpr->SetCurrentSprite(unsigned int(FireSpriteID::None));
		}
		auto pCol = go->GetComponent<SpriteCollisionComponent>();
		if (pCol)
		{
			pCol->SetEnabled(false);
		}
	}
}
FSMState* FSMStateFireIdle::OnNotify(ObservedEvent oevent, const ObservedData& odata, FSMData& data)
{
	UNREFERENCED_PARAMETER(odata);
	switch (oevent)
	{
	case GameEvent::FygarFire:
	{
		Direction d;
		if (odata.GetData<Direction>("Direction", d))
		{
			if (!data.SetData<Direction>("Direction", d))
			{
				data.AddData<Direction>("Direction", d);
			}
		}
		return GetState(m_ActiveState);
	}
	break;
	}
	return this;
}







FSMStateFireActive::FSMStateFireActive(const std::string& inactiveState, float duration)
	: m_InactiveState(inactiveState)
	, m_Duration(duration)
	, m_Direction(Direction::None)
	, m_Accu(0)
{
}
void FSMStateFireActive::Enter(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(sceneData);
	m_Accu = 0;
	auto go = GetGameObject();
	if (go)
	{
		if (!data.GetData<Direction>("Direction", m_Direction))
		{
			m_Direction = Direction::None;
		}

		//Sprite
		auto pSpr = go->GetComponent<SpriteComponent>();
		if (pSpr)
		{
			switch (m_Direction)
			{
			case Direction::Left:
				pSpr->SetCurrentSprite(unsigned int(FireSpriteID::Left));
				break;
			case Direction::Right:
				pSpr->SetCurrentSprite(unsigned int(FireSpriteID::Right));
				break;
			}
		}

		//Col
		auto pCol = go->GetComponent<SpriteCollisionComponent>();
		if (pCol)
		{
			pCol->SetEnabled(true);
			pCol->SetTrigger(true);
		}
	}
}
FSMState* FSMStateFireActive::OnNotify(ObservedEvent oevent, const ObservedData& odata, FSMData& data)
{
	UNREFERENCED_PARAMETER(data);
	switch (oevent)
	{
	case ObservedEvent::ColliderEntered:
	{
		AABBCollisionComponent* pCol = nullptr;
		if (odata.GetData<AABBCollisionComponent*>("Collider", pCol) && pCol && pCol->GetTag() == "Player" && pCol->GetGameObject())
		{
			auto& root = pCol->GetGameObject()->GetRoot();
			ObservedData d{};
			root.Notify(GameEvent::HitByEnemy, d);
			root.NotifyChildren(GameEvent::HitByEnemy, d);
		}
	}
	break;
	}
	return this;
}
FSMState* FSMStateFireActive::UpdateFirst(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(data);
	UNREFERENCED_PARAMETER(sceneData);
	auto go = GetGameObject();
	if (go)
	{
		auto* pSprite = go->GetComponent<SpriteComponent>();
		if (pSprite)
		{
			//Used for offset
			auto r = pSprite->GetCurrentSource();
			auto& t = go->GetTransform();
			const Vector2& scale = go->GetTransform().GetWorldScale();

			float offset = 0;
			if (go->GetParent())
			{
				AABBCollisionComponent* pC = go->GetParent()->GetComponent<AABBCollisionComponent>();
				if (pC)
				{
					offset = pC->GetCollider().width * 0.5f;
				}
			}
			switch (m_Direction)
			{
			case Direction::Right:
				t.SetLocalPosition(r.width*0.5f * scale.x + offset, 0);
				break;
			case Direction::Left:
				t.SetLocalPosition(-(r.width*0.5f * scale.x + offset), 0);
				break;
			}
		}

		m_Accu += sceneData.GetTime()->GetDeltaTime();
		if (m_Accu >= m_Duration)
		{
			return GetState(m_InactiveState);
		}
	}
	return this;




}












