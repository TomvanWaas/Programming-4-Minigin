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
#include "AABBCollisionComponent.h"
#include "SceneData.h"
#include "Scene.h"
#include "SpriteComponent.h"
#include "FSMData.h"
#include "MovementComponent.h"

using namespace DigDug;
using namespace Minigin;

DigDug::FSMStateEnemyMove::FSMStateEnemyMove(float speed, DigDugGridComponent* pGrid, float ghostCooldown, float fireCooldown, const std::string& ghostState, const std::string& fireState,
	const std::string& pumpState, const std::string& crushState)
	: m_PumpState(pumpState)
	, m_FireState(fireState)
	, m_GhostState(ghostState)
	, m_CrushState(crushState)
	, m_pGrid(pGrid)
	, m_Speed(speed)
	, m_Direction(Direction::None)
	, m_Desired(Direction::None)
	, m_NonZero(Direction::Right)
	, m_AccuFire(0)
	, m_AccuGhost(0)
	, m_GhostCooldown(ghostCooldown)
	, m_FireCooldown(fireCooldown)
{
}
void DigDug::FSMStateEnemyMove::Enter(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(sceneData);
	UNREFERENCED_PARAMETER(data);

	//
	if (!data.GetData<Direction>("Look", m_Direction))
	{
		data.AddData<Direction>("Look", Direction::Right);
		m_Direction = Direction::Right;
	}

	auto pObject = GetGameObject();
	if (pObject)
	{
		auto pSprite = pObject->GetComponent<SpriteComponent>();
		if (pSprite)
		{
			if (m_NonZero == Direction::Right)  pSprite->SetCurrentSprite(unsigned int(EnemySpriteID::MoveRight));
			else  pSprite->SetCurrentSprite(unsigned int(EnemySpriteID::MoveLeft));
		}
	}
}
FSMState* DigDug::FSMStateEnemyMove::OnNotify(ObservedEvent oevent, const ObservedData& odata, FSMData& data)
{
	UNREFERENCED_PARAMETER(data);
	switch (oevent)
	{
		//MOVEMENT INPUT
	case GameEvent::InputDownPressed:
	{
		HandleDesired(Direction::Down);
	}
	break;
	case GameEvent::InputLeftPressed:
	{
		HandleDesired(Direction::Left);
	}
	break;
	case GameEvent::InputRightPressed:
	{
		HandleDesired(Direction::Right);
	}
	break;
	case GameEvent::InputUpPressed:
	{
		HandleDesired(Direction::Up);
	}
	break;

	//GHOST INPUT
	case GameEvent::InputGhostPressed:
	{
		if (m_AccuGhost >= m_GhostCooldown)
		{
			m_AccuGhost = 0;
			return GetState(m_GhostState);
		}
	}
	break;
	//FIRE INPUT
	case GameEvent::InputFirePressed:
	{
		if (m_AccuFire >= m_FireCooldown && GetState(m_FireState) != nullptr)
		{
			m_AccuFire = 0;
			return GetState(m_FireState);
		}
	}
	break;
	//Collider (Pump | player)
	case ObservedEvent::ColliderEntered:
	{
		AABBCollisionComponent* pCol = nullptr;
		if (odata.GetData<AABBCollisionComponent*>("Collider", pCol) && pCol && pCol->GetGameObject())
		{
			if (pCol->GetTag() == "Pump")
			{
				return GetState(m_PumpState);
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
	//Obstacle
	case GameEvent::HitByObstacle:
	{
		return GetState(m_CrushState);
	}
	break;
	}
	return this;
}
FSMState* DigDug::FSMStateEnemyMove::UpdateSecond(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(data);
	m_AccuFire += sceneData.GetTime()->GetDeltaTime();
	m_AccuGhost += sceneData.GetTime()->GetDeltaTime();

	//Move
	auto pObject = GetGameObject();
	if (pObject)
	{
		//If worldpos is not marked (is stuck in wall?) => Ghost to get out of it
		if (m_pGrid && !m_pGrid->IsMarked(m_pGrid->ClosestPoint(pObject->GetTransform().GetWorldPosition())))
		{
			return GetState(m_GhostState);
		}

		//Else Move
		auto pMove = pObject->GetComponent<MovementComponent>();
		if (pMove && m_pGrid != nullptr && pObject != nullptr && m_Desired != Direction::None)
		{
			m_Direction = Direction::None;

			//Process Wanted
			//Same line
			if (m_Desired == m_NonZero
				|| Opposite(m_Desired, m_NonZero))
			{
				m_Direction = m_Desired;
			}
			//Cross
			else if (m_pGrid->IsOnWalkablePoint(pObject->GetTransform().GetWorldPosition(), 1 + m_Speed * sceneData.GetTime()->GetDeltaTime()))
			{
				m_Direction = m_Desired;
				pObject->GetTransform().SetWorldPosition(m_pGrid->ClosestPoint(GetGameObject()->GetTransform().GetWorldPosition()));
			}
			else
			{
				m_Direction = m_NonZero;
			}

			//Write To LOOK
			if (m_Desired == Direction::Left || m_Desired == Direction::Right)
			{
				data.SetData("Look", m_Desired);
			}
			else
			{
				data.GetData("Look", m_Desired);
			}
			auto* pSprite = pObject->GetComponent<SpriteComponent>();
			if (pSprite)
			{
				if (m_Desired == Direction::Right) pSprite->SetCurrentSprite(unsigned int(EnemySpriteID::MoveRight));
				else pSprite->SetCurrentSprite(unsigned int(EnemySpriteID::MoveLeft));
			}

			//Process Direction
			Vector2 move = Vector2::Zero;
			switch (m_Direction)
			{
			case Direction::Right:
				move = Vector2(1, 0);
				break;
			case Direction::Down:
				move = Vector2(0, 1);
				break;
			case Direction::Left:
				move = Vector2(-1, 0);
				break;
			case Direction::Up:
				move = Vector2(0, -1);
				break;
			}

			//Check if marked on Grid
			const auto wp = pObject->GetTransform().GetWorldPosition();
			const auto& offset = m_pGrid->GetOffset();

			//If forward marked
			if (!m_pGrid->IsMarked(m_pGrid->ClosestPoint(wp + offset * 0.5f * move)))
			{
				m_Direction = m_NonZero;
				//Recalc
				switch (m_Direction)
				{
				case Direction::Right:
					move = Vector2(1, 0);
					break;
				case Direction::Down:
					move = Vector2(0, 1);
					break;
				case Direction::Left:
					move = Vector2(-1, 0);
					break;
				case Direction::Up:
					move = Vector2(0, -1);
					break;
				}
			}
			if (m_pGrid->IsMarked(m_pGrid->ClosestPoint(wp + offset * 0.5f * move)))
			{
				m_NonZero = m_Direction;

				move *= m_Speed * sceneData.GetTime()->GetDeltaTime();

				//Clamp to grid
				move = m_pGrid->ClosestWalkableGrid(m_pGrid->ClosestWalkableLine(move + wp)) - wp;
				pMove->QueueMovement(move);
			}
		}
		else //Idle sprite
		{
			//Sprite
			auto* pSprite = pObject->GetComponent<SpriteComponent>();
			if (pSprite)
			{
				//Look
				Direction d = Direction::Right;
				data.GetData<Direction>("Look", d);
				switch (d)
				{
				case Direction::Right:
				{
					pSprite->SetCurrentSprite(unsigned int(EnemySpriteID::IdleRight));
				}
				break;
				case Direction::Left:
				{
					pSprite->SetCurrentSprite(unsigned int(EnemySpriteID::IdleLeft));
				}
				break;
				}
			}
		}

		m_Desired = Direction::None;
	}
	return this;
}


void DigDug::FSMStateEnemyMove::HandleDesired(Direction d)
{
	if (d != Direction::None)
	{
		if (m_NonZero == d) m_Desired = d;
		else if (Opposite(m_NonZero, d) && m_Desired != m_NonZero) m_Desired = d;
		else if (m_Desired == Direction::None) m_Desired = d;
	}
}
bool DigDug::FSMStateEnemyMove::Opposite(Direction a, Direction b) const
{
	return((a == Direction::Right && b == Direction::Left)
		|| (a == Direction::Left && b == Direction::Right)
		|| (a == Direction::Down && b == Direction::Up)
		|| (a == Direction::Up && b == Direction::Down));
}











DigDug::FSMStateEnemyGhost::FSMStateEnemyGhost(float speed, float dur, DigDugGridComponent* pGrid,
	const std::string& move, const std::string& pump, const std::string& crush)
	: m_pGrid(pGrid)
	, m_Speed(speed)
	, m_MoveState(move)
	, m_PumpState(pump)
	, m_CrushState(crush)
	, m_Duration(dur)
	, m_Accu(0)
	, m_Direction(Vector2::Zero)
{
}
void DigDug::FSMStateEnemyGhost::Enter(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(data);
	UNREFERENCED_PARAMETER(sceneData);
	/*m_Accu = 0;
	m_RequestedMoveState = false;
	m_Direction = Vector2::Zero;*/

	//Sprite
	auto pObject = GetGameObject();
	if (pObject)
	{
		auto pSprite = pObject->GetComponent<SpriteComponent>();
		if (pSprite)
		{
			pSprite->SetCurrentSprite(unsigned int(EnemySpriteID::Ghost));
		}
	}
}
FSMState* DigDug::FSMStateEnemyGhost::OnNotify(ObservedEvent oevent, const ObservedData& odata, FSMData& data)
{
	UNREFERENCED_PARAMETER(data);
	switch (oevent)
	{
		//MOVEMENT INPUT
	case GameEvent::InputDownPressed:
		if (!m_RequestedMoveState) m_Direction.y += 1;
		break;
	case GameEvent::InputLeftPressed:
		if (!m_RequestedMoveState) m_Direction.x -= 1;
		break;
	case GameEvent::InputRightPressed:
		if (!m_RequestedMoveState) m_Direction.x += 1;
		break;
	case GameEvent::InputUpPressed:
		if (!m_RequestedMoveState) m_Direction.y -= 1;
		break;

		//GHOST INPUT
	case GameEvent::InputGhostPressed:
	{
		if (GetGameObject() && GetGameObject()->GetScene() && m_RequestedMoveState == false)
		{
			m_RequestedMoveState = true;
			auto pm = GetGameObject()->GetScene()->GetSceneData().GetManager<PlayerManager>();
			if (pm)
			{
				auto cp = pm->GetClosestPlayer(GetGameObject()->GetTransform().GetWorldPosition());
				if (cp) m_Direction = cp->GetTransform().GetWorldPosition();
				else m_Direction = Vector2::Zero;
				break;
			}
		}
		m_RequestedMoveState = true;
		m_Direction = Vector2::Zero;
	}
	break;
	//Collider (Pump | player)
	case ObservedEvent::ColliderEntered:
	{
		AABBCollisionComponent* pCol = nullptr;
		if (odata.GetData<AABBCollisionComponent*>("Collider", pCol) && pCol && pCol->GetGameObject())
		{
			if (pCol->GetTag() == "Pump")
			{
				return GetState(m_PumpState);
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
	//Obstacle
	case GameEvent::HitByObstacle:
	{
		return GetState(m_CrushState);
	}
	break;
	}
	return this;
}
FSMState* DigDug::FSMStateEnemyGhost::UpdateSecond(const SceneData& sceneData, FSMData& data)
{
	auto pObject = GetGameObject();

	//Requested to return to move state
	if (m_RequestedMoveState && m_pGrid && pObject)
	{
		const auto& world = pObject->GetTransform().GetWorldPosition();

		//Check if current close pos valid
		auto closest = m_pGrid->ClosestWalkablePoint(world);
		if (m_pGrid->IsMarked(closest))
		{
			m_Direction = closest;			
		}

		//Check if invalid target pos
		else if (m_Direction.Equals(Vector2::Zero, 1))
		{
			//Request new
			m_RequestedMoveState = false;
			OnNotify(GameEvent::InputGhostPressed, ObservedData{}, data);
		}
		//Valid Target pos
		if (!m_Direction.Equals(Vector2::Zero, 1))
		{
			Vector2 dir = m_Direction - world;
			float dist = dir.Normalize();

			//If Dist < m_Speed * time => set & movestate
			if (dist <= m_Speed * sceneData.GetTime()->GetDeltaTime())
			{
				pObject->GetTransform().SetWorldPosition(m_Direction);
				m_Accu = 0;
				m_RequestedMoveState = false;
				return GetState(m_MoveState);
			}
			//Move towards
			else
			{
				dir *= m_Speed * sceneData.GetTime()->GetDeltaTime();

				//Clamp to grid 
				dir = m_pGrid->ClosestWalkableGrid(dir + world) - world;

				//
				pObject->GetTransform().SetLocalPosition(pObject->GetTransform().GetLocalPosition() + dir);
			}
		}

	}

	//Else: Move by input
	else
	{
		//Movement
		if (pObject && m_pGrid && m_Direction != Vector2::Zero)
		{
			const auto& world = pObject->GetTransform().GetWorldPosition();

			m_Direction.Normalize();
			m_Direction *= m_Speed * sceneData.GetTime()->GetDeltaTime();

			//Clamp to grid
			m_Direction = m_pGrid->ClosestWalkableGrid(m_Direction + world) - world;

			pObject->GetTransform().SetLocalPosition(pObject->GetTransform().GetLocalPosition() + m_Direction);
		}
		m_Direction = Vector2::Zero;


		//Timer
		m_Accu += sceneData.GetTime()->GetDeltaTime();
		if (m_Accu >= m_Duration)
		{
			m_Accu = 0;
			pObject->Notify(GameEvent::InputGhostPressed, ObservedData{});
		}
	}
	return this;
}








FSMStateEnemyPumping::FSMStateEnemyPumping(GameObject* pObject, DigDugGridComponent* pGrid, float targetTime, float deflateSpeed, FSMState* pMove, FSMState* pDead, FSMState* pCrushed)
	: m_pMovingState(pMove)
	, m_pDeadState(pDead)
	, m_pObject(pObject)
	, m_IsBeingPumped(true)
	, m_AccuPump(0)
	, m_TargetPump(targetTime)
	, m_DeflateSpeed(deflateSpeed)
	, m_pCrushedState(pCrushed)
	, m_pGrid(pGrid)
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
void FSMStateEnemyPumping::Exit(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(sceneData);
	UNREFERENCED_PARAMETER(data);
	//If exits while alive
	if (m_AccuPump < m_TargetPump && m_pGrid && m_pObject)
	{
		m_pObject->GetTransform().SetWorldPosition(m_pGrid->ClosestWalkableLine(m_pObject->GetTransform().GetWorldPosition()));
	}
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
				pSprite->SetCurrentSprite(unsigned int(EnemySpriteID::IdleLeft));
				break;
			case Direction::Right:
				pSprite->SetCurrentSprite(unsigned int(EnemySpriteID::IdleRight));
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







FSMStateFireActive::FSMStateFireActive(DigDugGridComponent* pGrid, const std::string& inactiveState, float duration)
	: m_InactiveState(inactiveState)
	, m_Duration(duration)
	, m_Direction(Direction::None)
	, m_Accu(0)
	, m_pGrid(pGrid)
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
			Vector2 extent = t.GetWorldPosition();

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
				extent.x += r.width * 0.5f * scale.x;
				break;
			case Direction::Left:
				t.SetLocalPosition(-(r.width*0.5f * scale.x + offset), 0);
				extent.x -= r.width * 0.5f * scale.x;
				break;
			}

			//Check if Collider hits ground => Set Accu to duration to end active state
			if (m_pGrid && !m_pGrid->IsMarked(m_pGrid->ClosestPoint(extent)))
			{
				//make sure it returns to inactive state
				m_Accu = m_Duration;
			}
		}

		//Timer
		m_Accu += sceneData.GetTime()->GetDeltaTime();
		if (m_Accu >= m_Duration)
		{
			return GetState(m_InactiveState);
		}
	}
	return this;




}






FSMStateFygarCharge::FSMStateFygarCharge(float dur, const std::string& pumpState, const std::string& crushState,
	const std::string& fireState)
	: m_Accu(0)
	, m_CrushedState(crushState)
	, m_Duration(dur)
	, m_PumpState(pumpState)
	, m_FireState(fireState)
{
}
void FSMStateFygarCharge::Enter(const SceneData& sceneData, FSMData& data)
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
			switch (d)
			{
			case Direction::Left:
				pSprite->SetCurrentSprite(unsigned int(EnemySpriteID::FireLeft));
				break;
			case Direction::Right:
				pSprite->SetCurrentSprite(unsigned int(EnemySpriteID::FireRight));
				break;
			}
		}
	}
}
FSMState* FSMStateFygarCharge::UpdateFirst(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(data);
	m_Accu += sceneData.GetTime()->GetDeltaTime();
	if (m_Accu >= m_Duration)
	{
		return GetState(m_FireState);
	}
	return this;
}
FSMState* FSMStateFygarCharge::OnNotify(ObservedEvent oevent, const ObservedData& odata, FSMData& data)
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
				return GetState(m_PumpState);
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












