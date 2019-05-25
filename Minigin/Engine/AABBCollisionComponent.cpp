#include "stdafx.h"
#include "AABBCollisionComponent.h"
#include "Transform.h"
#include "GameObject.h"
#include "CollisionManager.h"
#include "SceneData.h"

using namespace Minigin;

AABBCollisionComponent::AABBCollisionComponent(const Rect& extents, bool isTrigger, const std::string& tag)
	: m_Rectangle(extents)
	, m_IsTrigger(isTrigger)
	, m_Collider({})
	, m_Tag(tag)
{
}

void AABBCollisionComponent::InitializeOverride(const SceneData& sceneData)
{
	m_Collider = m_Rectangle;
	if (GetGameObject() != nullptr)
	{
		const auto& transform = GetGameObject()->GetTransform();
		m_Collider.x += transform.GetWorldPosition().x;
		m_Collider.y += transform.GetWorldPosition().y;
		m_Collider.width *= transform.GetWorldScale().x;
		m_Collider.height *= transform.GetWorldScale().y;
	}
	m_Collider.x -= m_Collider.width*0.5f;
	m_Collider.y -= m_Collider.height*0.5f;

	sceneData.GetCollision()->RegisterCollision(*this);
}
void AABBCollisionComponent::DestroyOverride(const SceneData& sceneData)
{
	sceneData.GetCollision()->UnRegisterCollision(*this);
}
void AABBCollisionComponent::OnNotify(ObservedEvent event, const ObservedData& data)
{
	UNREFERENCED_PARAMETER(data);
	switch (event)
	{
	case ObservedEvent::ScaleChanged:
	case ObservedEvent::PositionChanged:

		//Recalc collider
		m_Collider = m_Rectangle;
		if (GetGameObject() != nullptr)
		{
			const auto& transform = GetGameObject()->GetTransform();
			m_Collider.x += transform.GetWorldPosition().x;
			m_Collider.y += transform.GetWorldPosition().y;
			m_Collider.width *= transform.GetWorldScale().x;
			m_Collider.height *= transform.GetWorldScale().y;
		}
		m_Collider.x -= m_Collider.width*0.5f;
		m_Collider.y -= m_Collider.height*0.5f;
		
		break;
	}
}




bool AABBCollisionComponent::CollidesWith(AABBCollisionComponent* pCollider) const
{
	if (GetGameObject() == nullptr || pCollider->GetGameObject() == nullptr ||
		pCollider == nullptr || pCollider == this	//Null Or same collider?
		|| pCollider->IsTrigger() || IsTrigger()	//One is trigger?
		|| &pCollider->GetGameObject()->GetRoot() == &GetGameObject()->GetRoot()
		|| pCollider->IsEnabled() == false || IsEnabled() == false) //Same Object?
		return false; 

	const auto rect = pCollider->GetCollider();
	const auto own = GetCollider();

	return (rect.x <= own.x + own.width &&
		rect.x + rect.width >= own.x &&
		rect.y <= own.y + own.height &&
		rect.y + rect.height >= own.y);
}



void AABBCollisionComponent::SetCollider(float w, float h, float offsetx, float offsety)
{
	m_Rectangle = Rect{ offsetx, offsety, w, h };

	//Recalc collider
	m_Collider = m_Rectangle;
	if (GetGameObject() != nullptr)
	{
		const auto& transform = GetGameObject()->GetTransform();
		m_Collider.x += transform.GetWorldPosition().x;
		m_Collider.y += transform.GetWorldPosition().y;
		m_Collider.width *= transform.GetWorldScale().x;
		m_Collider.height *= transform.GetWorldScale().y;
	}
	m_Collider.x -= m_Collider.width*0.5f;
	m_Collider.y -= m_Collider.height*0.5f;
}
const Rect& AABBCollisionComponent::GetCollider() const
{
	return m_Collider;
}



void AABBCollisionComponent::SetTag(const std::string& tag)
{
	m_Tag = tag;
}
const std::string& AABBCollisionComponent::GetTag() const
{
	return m_Tag;
}

