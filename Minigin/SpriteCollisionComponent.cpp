#include "MiniginPCH.h"
#include "SpriteCollisionComponent.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"

SpriteCollisionComponent::SpriteCollisionComponent(GameObject& gameObject)
	: AABBCollisionComponent(gameObject)
	, m_pSpriteComponent(nullptr)
{
}


void SpriteCollisionComponent::Initialize(const SceneData& sceneData)
{
	if (m_pGameObject != nullptr)
	{
		m_pSpriteComponent = m_pGameObject->GetComponent<SpriteComponent>();
	}
	AABBCollisionComponent::Initialize(sceneData);
}

void SpriteCollisionComponent::SetOffset(float leftx, float rightx, float topy, float bottomy)
{
	m_Rectangle.x = leftx;
	m_Rectangle.y = topy;
	m_Rectangle.width = rightx;
	m_Rectangle.height = bottomy;
}

void SpriteCollisionComponent::CalculateCollider()
{
	Rect sprite{};

	//Sprite
	if (m_pSpriteComponent != nullptr)
	{
		sprite = m_pSpriteComponent->GetCurrentSource();
	}

	//
	m_Collider.width = sprite.width + m_Rectangle.x + m_Rectangle.width;
	m_Collider.height = sprite.height + m_Rectangle.y + m_Rectangle.height;
	m_Collider.x = -m_Rectangle.x;
	m_Collider.y = -m_Rectangle.y;

	//Position
	if (m_pGameObject != nullptr)
	{
		const auto* pTransform = m_pGameObject->GetComponent<TransformComponent>();
		const auto wpos = pTransform->GetWorldPosition();
		const auto wscale = pTransform->GetWorldScale();
		m_Collider.x += wpos.x;
		m_Collider.y += wpos.y;
		m_Collider.width *= wscale.x;
		m_Collider.height *= wscale.y;
	}
	m_Collider.x -= m_Collider.width * 0.5f;
	m_Collider.y -= m_Collider.height * 0.5f;
}
