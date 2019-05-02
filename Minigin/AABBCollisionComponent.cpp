#include "MiniginPCH.h"
#include "AABBCollisionComponent.h"
#include "Transform.h"
#include "GameObject.h"
#include "ColliderCommand.h"
#include "CollisionManager.h"
#include "SceneData.h"

AABBCollisionComponent::AABBCollisionComponent(const Rect& extents, bool isTrigger, const std::string& tag,
	const std::shared_ptr<ColliderCommand>& enter, const std::shared_ptr<ColliderCommand>& exit)
	: m_Rectangle(extents)
	, m_IsTrigger(isTrigger)
	, m_pOnEnter(enter)
	, m_pOnExit(exit)
	, m_Collider({})
	, m_Tag(tag)
{
}

void AABBCollisionComponent::InitializeOverride(const SceneData& sceneData)
{
	sceneData.pCollisionManager->RegisterCollision(*this);
}

void AABBCollisionComponent::DestroyOverride(const SceneData& sceneData)
{
	sceneData.pCollisionManager->UnRegisterCollision(*this);
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



void AABBCollisionComponent::CalculateCollider()
{
	m_Collider = m_Rectangle;
	if (GetGameObject() == nullptr)return;
	const auto& transform = GetGameObject()->GetTransform();

	m_Collider.x += transform.GetWorldPosition().x;
	m_Collider.y += transform.GetWorldPosition().y;
	m_Collider.width *= transform.GetWorldScale().x;
	m_Collider.height *= transform.GetWorldScale().y;
	m_Collider.x -= m_Collider.width*0.5f;
	m_Collider.y -= m_Collider.height*0.5f;
}
void AABBCollisionComponent::SetCollider(float w, float h, float offsetx, float offsety)
{
	m_Rectangle = Rect{ offsetx, offsety, w, h };
}
const Rect& AABBCollisionComponent::GetCollider() const
{
	return m_Collider;
}



void AABBCollisionComponent::Enter(AABBCollisionComponent* pOther)
{
	if (m_pOnEnter) m_pOnEnter->Execute(pOther);
}
void AABBCollisionComponent::Exit(AABBCollisionComponent* pOther)
{
	if (m_pOnExit) m_pOnExit->Execute(pOther);
}



void AABBCollisionComponent::SetTag(const std::string& tag)
{
	m_Tag = tag;
}
const std::string& AABBCollisionComponent::GetTag() const
{
	return m_Tag;
}



void AABBCollisionComponent::SetEnterCommand(const std::shared_ptr<ColliderCommand>& pCommand)
{
	m_pOnEnter = pCommand;
}
void AABBCollisionComponent::SetExitCommand(const std::shared_ptr<ColliderCommand>& pCommand)
{
	m_pOnExit = pCommand;
}
