#include "MiniginPCH.h"
#include "GameFSMClasses.h"
#include "GameObject.h"
#include "Components.h"
#include "Time.h"
#include "DigDugGridComponent.h"
#include "InputManager.h"

FSMGridCondition::FSMGridCondition(DigDugGridComponent& grid, GameObject& go)
	: m_pGrid(&grid)
	, m_pTransform(go.GetComponent<TransformComponent>())
{
}
bool FSMGridCondition::Execute(const SceneData& sceneData) const
{
	UNREFERENCED_PARAMETER(sceneData);
	return (m_pGrid != nullptr && m_pTransform != nullptr &&
		m_pGrid->IsOnPoint(m_pTransform->GetWorldPosition()));
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



FSMSpriteEvent::FSMSpriteEvent(unsigned spriteID, GameObject& go)
	: m_SpriteID(spriteID)
	, m_pSpriteComp(go.GetComponent<SpriteComponent>())
{
}
void FSMSpriteEvent::Execute(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);
	if (m_pSpriteComp != nullptr)
	{
		m_pSpriteComp->SetCurrentSource(m_SpriteID);
	}
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



FSMGridMarkerEvent::FSMGridMarkerEvent(GameObject& go, DigDugGridComponent& grid)
	: m_pGrid(&grid)
	, m_pTransform(go.GetComponent<TransformComponent>())
{
}
void FSMGridMarkerEvent::Execute(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);
	if (m_pGrid != nullptr && m_pTransform != nullptr)
	{
		m_pGrid->Mark(m_pTransform->GetWorldPosition());
	}
}













