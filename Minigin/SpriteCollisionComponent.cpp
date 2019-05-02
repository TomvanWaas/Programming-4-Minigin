#include "MiniginPCH.h"
#include "SpriteCollisionComponent.h"
#include "SpriteComponent.h"
#include "Transform.h"
#include "GameObject.h"

SpriteCollisionComponent::SpriteCollisionComponent(const Rect& extents, bool isTrigger, const std::string& tag,
	const std::shared_ptr<ColliderCommand>& enter, const std::shared_ptr<ColliderCommand>& exit)
	: AABBCollisionComponent(extents, isTrigger, tag, enter, exit)
	, m_pSpriteComponent(nullptr)
{
}

void SpriteCollisionComponent::InitializeOverride(const SceneData& sceneData)
{
	if (GetGameObject() != nullptr)
	{
		m_pSpriteComponent = GetGameObject()->GetComponent<SpriteComponent>();
	}
	AABBCollisionComponent::InitializeOverride(sceneData);
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
	if (GetGameObject() != nullptr)
	{
		const auto& transform = GetGameObject()->GetTransform();
		const auto wpos = transform.GetWorldPosition();
		const auto wscale = transform.GetWorldScale();
		m_Collider.x += wpos.x;
		m_Collider.y += wpos.y;
		m_Collider.width *= wscale.x;
		m_Collider.height *= wscale.y;
	}
	m_Collider.x -= m_Collider.width * 0.5f;
	m_Collider.y -= m_Collider.height * 0.5f;
}
