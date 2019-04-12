#include "MiniginPCH.h"
#include "AABBCollisionComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "ColliderCommand.h"

AABBCollisionComponent::AABBCollisionComponent(float w, float h, float offsetx, float offsety)
	: m_Rectangle(Rect{offsetx, offsety, w, h})
	, m_Collider()
	, m_pOnEnter(nullptr)
	, m_IsTrigger(false)
	, m_pOnExit(nullptr)
{
}




Rect AABBCollisionComponent::GetCollider() const
{
	return m_Collider;
}

void AABBCollisionComponent::CalculateCollider()
{
	if (m_pGameObject != nullptr)
	{
		const auto* pTransform = m_pGameObject->GetComponent<TransformComponent>();
		const auto world = pTransform->GetWorldPosition();
		const auto scale = pTransform->GetWorldScale();

		m_Collider.width = m_Rectangle.width * scale.x;
		m_Collider.height = m_Rectangle.height * scale.y;
		m_Collider.x = m_Rectangle.x + world.x - m_Collider.width*0.5f;
		m_Collider.y = m_Rectangle.y + world.y - m_Collider.height*0.5f;
	}
	else
	{
		m_Collider = m_Rectangle;
		m_Collider.x -= m_Collider.width*0.5f;
		m_Collider.y -= m_Collider.height*0.5f;
	}
}

void AABBCollisionComponent::Enter(AABBCollisionComponent* pOther)
{
	m_pOnEnter->Execute(pOther);
}

void AABBCollisionComponent::Exit(AABBCollisionComponent* pOther)
{
	m_pOnExit->Execute(pOther);
}
