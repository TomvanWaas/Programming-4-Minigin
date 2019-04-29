#include "MiniginPCH.h"
#include "MovementComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "AABBCollisionComponent.h"
#include "CollisionManager.h"

MovementComponent::MovementComponent(GameObject& gameObject)
	: BaseComponent(gameObject)
	, m_Interpolation(0)
	, m_MovementQueue(0.0f, 0.0f)
	, m_HasQueue(false)
	, m_pColliders()
{
}

void MovementComponent::Initialize(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);
	if (m_pGameObject == nullptr) return;
	m_pColliders = GetGameObject().GetComponents<AABBCollisionComponent>(true);
}

void MovementComponent::UpdateSecond(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);

	if (m_pGameObject != nullptr && m_HasQueue)
	{
		auto* pTransform = m_pGameObject->GetComponent<TransformComponent>();

		Vector2 interp = m_MovementQueue / int(m_Interpolation + 1);
		for (unsigned int i{0}; i < m_Interpolation+1; ++i)
		{
			pTransform->SetLocalPosition(pTransform->GetLocalPosition() + interp);

			//Collision Detection
			//If collide > previous and break
			for (AABBCollisionComponent* pCollider: m_pColliders)
			{
				if (sceneData.pCollisionManager->Collides(pCollider))
				{
					pTransform->SetLocalPosition(pTransform->GetLocalPosition() - interp);
					m_MovementQueue = Vector2();
					m_HasQueue = false;
					return;
				}
			}
		}

		m_MovementQueue = Vector2();
		m_HasQueue = false;
	}
}





void MovementComponent::QueueMovement(const Vector2& movement)
{
	m_HasQueue = true;
	m_MovementQueue += movement;
}


void MovementComponent::SetInterpolation(unsigned interpolation)
{
	m_Interpolation = interpolation;
}
unsigned MovementComponent::GetInterpolation() const
{
	return m_Interpolation;
}

