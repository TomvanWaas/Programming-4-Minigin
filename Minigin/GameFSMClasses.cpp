#include "MiniginPCH.h"
#include "GameFSMClasses.h"
#include "GameObject.h"
#include "Components.h"
#include "Time.h"
#include "DigDugGridComponent.h"
#include "InputManager.h"
#include "SceneData.h"
#include "Scene.h"
#include "GameFiles.h"

using namespace DigDug;


FSMGridPositionWalkablePointCondition::FSMGridPositionWalkablePointCondition(DigDugGridComponent& grid, GameObject& go, bool target)
	: m_pGrid(&grid)
	, m_pTransform(&go.GetTransform())
	, m_Target(target)
{
}
bool FSMGridPositionWalkablePointCondition::Execute(const SceneData& sceneData) const
{
	UNREFERENCED_PARAMETER(sceneData);
	return (m_pGrid != nullptr &&
		m_pTransform != nullptr &&
		(m_pGrid->IsOnWalkablePoint(m_pTransform->GetWorldPosition()) == m_Target));
}


bool FSMGridPositionPointCondition::Execute(const SceneData& sceneData) const
{
	UNREFERENCED_PARAMETER(sceneData);
	return (m_pGrid != nullptr &&
		m_pTransform != nullptr &&
		(m_pGrid->IsOnPoint(m_pTransform->GetWorldPosition()) == m_Target));
}

FSMGridMarkCondition::FSMGridMarkCondition(DigDugGridComponent& grid, GameObject& go, const Vector2& offset,
	bool target)
	: m_Offset(offset)
	, m_pGrid(&grid)
	, m_pTransform(&go.GetTransform())
	, m_Target(target)
{
}
bool FSMGridMarkCondition::Execute(const SceneData& sceneData) const
{
	UNREFERENCED_PARAMETER(sceneData);
	if  (m_pGrid != nullptr && m_pTransform != nullptr)
	{
		const auto& s = m_pTransform->GetWorldScale();
		const auto& p = m_pTransform->GetWorldPosition();
		return (m_pGrid->IsMarked(m_pGrid->ClosestPoint(p + m_Offset * s)) == m_Target);
	}
	return false;
}









FSMMoveEvent::FSMMoveEvent(const Vector2& velocity, GameObject& go)
	: m_pMovementComp(go.GetComponent<MovementComponent>())
	, m_Velocity(velocity)
{
}
void FSMMoveEvent::Execute(const SceneData& sceneData)
{
	if (m_pMovementComp != nullptr)
	{
		m_pMovementComp->QueueMovement(m_Velocity * sceneData.pTime->GetDeltaTime());
	}
}
void FSMMoveEvent::SetVelocity(const Vector2& velocity)
{
	m_Velocity = velocity;
}
const Vector2& FSMMoveEvent::GetVelocity() const
{
	return m_Velocity;
}
Vector2& FSMMoveEvent::GetVelocity()
{
	return m_Velocity;
}




FSMFunctionEvent::FSMFunctionEvent(GameObject& go, void func(GameObject*, const SceneData&))
	: m_pGameObject(&go)
	, m_Function(func)
{
}
void FSMFunctionEvent::Execute(const SceneData& sceneData)
{
	if (m_pGameObject != nullptr && m_Function != nullptr)
	{
		m_Function(m_pGameObject, sceneData);
	}
}



FSMGridMarkerEvent::FSMGridMarkerEvent(GameObject& go, DigDugGridComponent& grid, const Vector2& offset)
	: m_pGrid(&grid)
	, m_pTransform(&go.GetTransform())
	, m_Offset(offset)
{
}
void FSMGridMarkerEvent::Execute(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);
	if (m_pGrid != nullptr && m_pTransform != nullptr)
	{
		m_pGrid->Mark(m_pTransform->GetWorldPosition() + m_Offset);
	}
}

void FSMEnableComponentEvent::Execute(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);
	if (m_pComponent != nullptr)
	{
		m_pComponent->SetEnabled(m_Enabled);
	}
}



void FSMSetLocalPosition::Execute(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);
	if (m_pTransform != nullptr)
	{
		m_pTransform->SetLocalPosition(m_Local);
	}
}

void FSMForceMoveEvent::Execute(const SceneData& sceneData)
{
	if (m_pTransform != nullptr)
	{
		m_pTransform->SetLocalPosition(m_pTransform->GetLocalPosition() + m_Velocity * sceneData.pTime->GetDeltaTime());
	}
}

void FSMTriggerEvent::Execute(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);
	if (m_pCollider)m_pCollider->SetTrigger(m_Target);
}












FSMEventPumpSprite::FSMEventPumpSprite(Direction& dir, SpriteComponent* pSprite)
	: m_pComponent(pSprite)
	, m_pDirection(&dir)
{
}

void FSMEventPumpSprite::Execute(const SceneData& sceneData)
{
	if (m_pComponent != nullptr && m_pDirection != nullptr)
	{
		UNREFERENCED_PARAMETER(sceneData);
		m_pComponent->SetCurrentSource(unsigned int(*m_pDirection));
	}
}


FSMEventGridMove::FSMEventGridMove(Direction& dir, MovementComponent* pMove, float speed)
	: m_pComponent(pMove)
	, m_pDirection(&dir)
	, m_Speed(speed)
{
}
void FSMEventGridMove::Execute(const SceneData& sceneData)
{
	if (m_pComponent != nullptr && m_pDirection != nullptr)
	{
		switch (*m_pDirection)
		{
		case Direction::Down:
			m_pComponent->QueueMovement(Vector2(0, m_Speed)*sceneData.pTime->GetDeltaTime());
			break;
		case Direction::Right:
			m_pComponent->QueueMovement(Vector2(m_Speed, 0)*sceneData.pTime->GetDeltaTime());
			break;
		case Direction::Up:
			m_pComponent->QueueMovement(Vector2(0, -m_Speed)*sceneData.pTime->GetDeltaTime());
			break;
		case Direction::Left:
			m_pComponent->QueueMovement(Vector2(-m_Speed, 0)*sceneData.pTime->GetDeltaTime());
			break;
		}
	}
}




FSMEventPlayerSprite::FSMEventPlayerSprite(Direction& dir, SpriteComponent* pSprite, DigDug::PlayerState& state)
	: m_pDirection(&dir)
	, m_pComponent(pSprite)
	, m_pState(&state)
{
}
void FSMEventPlayerSprite::Execute(const SceneData& sceneData)
{
	if (m_pComponent != nullptr && m_pDirection != nullptr && m_pState != nullptr)
	{
		UNREFERENCED_PARAMETER(sceneData);
		//4 Directions for each state
		m_pComponent->SetCurrentSource(unsigned int(*m_pState)*4 + unsigned int(*m_pDirection));
	}
}









