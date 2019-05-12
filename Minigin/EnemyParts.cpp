#include "MiniginPCH.h"
#include "EnemyParts.h"
#include "GameObject.h"
#include "Components.h"
#include "DigDugGridComponent.h"
#include "Time.h"
#include "ObservedData.h"
#include "PlayerManager.h"
#include "Enumerations.h"

using namespace DigDug;


FSMStateEnemyMoving::FSMStateEnemyMoving(float speed, DigDugGridComponent* pGrid, GameObject* pObject,
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
void FSMStateEnemyMoving::Enter(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(sceneData);
	UNREFERENCED_PARAMETER(data);
	//Prepare Colliders
	m_AccuTime = 0.0f;

	if (m_pObject)
	{
		//If Directio not yet in BData
		Direction d = Direction::None;
		if (!(data.GetData<Direction>("Look", d)))
		{
			data.AddData("Look", Direction::Right);
			d = Direction::Right;
		}

		auto* pSprite = m_pObject->GetComponent<SpriteComponent>();
		if (pSprite && d == Direction::Right)
		{
			pSprite->SetCurrentSource(unsigned int(EnemySprite::MoveRight));
		}
		else if (pSprite && d == Direction::Left)
		{
			pSprite->SetCurrentSource(unsigned int(EnemySprite::MoveLeft));
		}
		m_CurrentDirection = d;


		auto* pCol = m_pObject->GetComponent<AABBCollisionComponent>();
		if (pCol)
		{
			pCol->SetTrigger(true);
		}
	}

	
}
FSMState* FSMStateEnemyMoving::UpdateFirst(const SceneData& sceneData, FSMData& data)
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
			Vector2 offset = m_pGrid->GetOffset()*0.5f;

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
FSMState* FSMStateEnemyMoving::OnNotify(ObservedEvent oevent, const ObservedData& odata, FSMData& data)
{
	UNREFERENCED_PARAMETER(data);
	UNREFERENCED_PARAMETER(odata);
	switch (oevent)
	{
	case ObservedEvent::HitByObstacle:
		return m_pCrushedState;
		break;
	case ObservedEvent::PumpHit:
		return m_pPumpedState;
		break;
	case ObservedEvent::ColliderEntered:
		AABBCollisionComponent* pCol = nullptr;
		if (odata.GetData<AABBCollisionComponent*>("Collider", pCol) && pCol && pCol->GetGameObject())
		{
			if (pCol->GetTag() == "Player")
			{
				ObservedData d{};
				pCol->GetGameObject()->GetRoot().Notify(ObservedEvent::Killed, d);
				pCol->GetGameObject()->GetRoot().NotifyChildren(ObservedEvent::Killed, d);
			}
		}
		break;
	}
	return this;
}
void FSMStateEnemyMoving::SetDirection(Direction d, FSMData& data)
{
	if (d == Direction::Left)
	{
		SetSprite(unsigned int(EnemySprite::MoveLeft));
		data.SetData("Look", d);
	}
	else if (d == Direction::Right)
	{
		SetSprite(unsigned int(EnemySprite::MoveRight));
		data.SetData("Look", d);
	}
	else if (d == Direction::None && m_CurrentDirection == Direction::Left)
	{
		SetSprite(unsigned int(EnemySprite::IdleLeft));
		data.SetData("Look", m_CurrentDirection);
	}
	else if (d == Direction::None && m_CurrentDirection == Direction::Right)
	{
		SetSprite(unsigned int(EnemySprite::IdleRight));
		data.SetData("Look", m_CurrentDirection);
	}
	m_CurrentDirection = d;
}
Direction FSMStateEnemyMoving::GetRandomDirection(bool* marks)
{
	int tries = 5;
	while (tries-- > 0)
	{
		int r = rand() % 4;
		if (marks[r] && m_CurrentDirection != Direction(r)) return Direction(r);
	}
	return Direction::None;
}
void FSMStateEnemyMoving::SetSprite(unsigned i)
{
	if (m_pObject)
	{
		auto* pSprite = m_pObject->GetComponent<SpriteComponent>();
		if (pSprite) pSprite->SetCurrentSource(i);
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
	m_AccuPump = 0;
	m_IsBeingPumped = true;

	if (m_pObject)
	{
		Direction d{};
		if (!data.GetData("Look", d)) { d = Direction::Right; }

		auto* pSprite = m_pObject->GetComponent<SpriteComponent>();
		if (pSprite && d == Direction::Right)
		{
			pSprite->SetCurrentSource(unsigned int(EnemySprite::PumpedRightTier1));
		}
		else if (pSprite && d == Direction::Left)
		{
			pSprite->SetCurrentSource(unsigned int(EnemySprite::PumpedLeftTier1));
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
			return m_pDeadState;
		}
	}
	else
	{
		m_AccuPump -= sceneData.GetTime()->GetDeltaTime() * m_DeflateSpeed;
		if (m_AccuPump <= 0)
		{
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
			if (frac < 0.25f)
			{
				if (look == Direction::Left) pSprite->SetCurrentSource(unsigned int(EnemySprite::PumpedLeftTier1));
				else if (look == Direction::Right) pSprite->SetCurrentSource(unsigned int(EnemySprite::PumpedRightTier1));
			}
			else if (frac < 0.5f)
			{
				if (look == Direction::Left) pSprite->SetCurrentSource(unsigned int(EnemySprite::PumpedLeftTier2));
				else if (look == Direction::Right) pSprite->SetCurrentSource(unsigned int(EnemySprite::PumpedRightTier2));
			}
			else if (frac < 0.75f)
			{
				if (look == Direction::Left) pSprite->SetCurrentSource(unsigned int(EnemySprite::PumpedLeftTier3));
				else if (look == Direction::Right) pSprite->SetCurrentSource(unsigned int(EnemySprite::PumpedRightTier3));
			}
			else
			{
				if (look == Direction::Left) pSprite->SetCurrentSource(unsigned int(EnemySprite::PumpedLeftTier4));
				else if (look == Direction::Right) pSprite->SetCurrentSource(unsigned int(EnemySprite::PumpedRightTier4));
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
	case ObservedEvent::PumpReleased:
		m_IsBeingPumped = false;
		break;
	case ObservedEvent::PumpHit:
		m_IsBeingPumped = true;
		break;
	case ObservedEvent::HitByObstacle:
		return m_pCrushedState;
		break;
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
			pSprite->SetCurrentSource(unsigned int(EnemySprite::IdleRight));
		}
		else if (pSprite && d == Direction::Left)
		{
			pSprite->SetCurrentSource(unsigned int(EnemySprite::IdleLeft));
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
		Vector2 o = m_pGrid->GetOffset()*0.5f;
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
			pSprite->SetCurrentSource(unsigned int(EnemySprite::CrushedRight));
		}
		else if (pSprite && look == Direction::Left)
		{
			pSprite->SetCurrentSource(unsigned int(EnemySprite::CrushedLeft));
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
			pSprite->SetCurrentSource(unsigned int(EnemySprite::Ghost));
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
	case ObservedEvent::PumpHit:
		return m_pPumpedState;
		break;

	case ObservedEvent::HitByObstacle:
		return m_pCrushedState;
		break;

	case ObservedEvent::ColliderEntered:
		AABBCollisionComponent* pCol = nullptr;
		if (odata.GetData<AABBCollisionComponent*>("Collider", pCol) && pCol && pCol->GetGameObject())
		{
			if (pCol->GetTag() == "Player")
			{
				ObservedData d{};
				pCol->GetGameObject()->GetRoot().Notify(ObservedEvent::Killed, d);
				pCol->GetGameObject()->GetRoot().NotifyChildren(ObservedEvent::Killed, d);
			}
		}
		break;
	}
	return this;
}












