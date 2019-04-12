#include "MiniginPCH.h"
#include "MovementComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"

MovementComponent::MovementComponent(unsigned int interpolation)
	: m_Interpolation(interpolation)
	, m_Movement(0.0f, 0.0f)
	, m_HasMoved(false)
{
}

void MovementComponent::Initialize(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);
}

void MovementComponent::Update(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);

	if (m_pGameObject != nullptr && m_HasMoved)
	{
		auto* pTransform = m_pGameObject->GetComponent<TransformComponent>();
		pTransform->SetLocalPosition(pTransform->GetLocalPosition() + m_Movement);

		//TODO : Collision Detection

		m_Movement = Vector2();
		m_HasMoved = false;
	}
}

void MovementComponent::MovePosition(const Vector2& movement)
{
	m_HasMoved = true;
	m_Movement = movement;
}
