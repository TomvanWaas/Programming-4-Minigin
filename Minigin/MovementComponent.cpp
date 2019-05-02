#include "MiniginPCH.h"
#include "MovementComponent.h"
#include "Transform.h"
#include "GameObject.h"
#include "AABBCollisionComponent.h"
#include "CollisionManager.h"
#include "SceneData.h"

MovementComponent::MovementComponent(unsigned int interpolation )
	: m_Interpolation(interpolation)
	, m_MovementQueue(0.0f, 0.0f)
	, m_HasQueue(false)
	, m_pColliders()
{
}

void MovementComponent::InitializeOverride(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);
	if (GetGameObject() == nullptr) return;
	m_pColliders = GetGameObject()->GetRoot().GetComponentsInChildren<AABBCollisionComponent>();
	auto comp = GetGameObject()->GetRoot().GetComponent<AABBCollisionComponent>();
	m_pColliders.push_back(comp);
}

void MovementComponent::UpdateSecondOverride(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);

	if (GetGameObject() != nullptr && m_HasQueue)
	{
		auto& transform = GetGameObject()->GetTransform();

		Vector2 interp = m_MovementQueue / int(m_Interpolation + 1);
		Vector2 local{};
		for (unsigned int i{0}; i < m_Interpolation+1; ++i)
		{
			local = transform.GetLocalPosition();
			transform.SetLocalPosition((transform.GetLocalPosition() + interp));

			//Collision Detection
			//If collide > previous and break
			for (AABBCollisionComponent* pCollider: m_pColliders)
			{
				if (pCollider != nullptr)
				{
					pCollider->CalculateCollider();
					if (sceneData.pCollisionManager->Collides(pCollider))
					{
						transform.SetLocalPosition(local);
						m_MovementQueue = Vector2();
						m_HasQueue = false;
						return;
					}
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

