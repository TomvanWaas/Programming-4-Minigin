#include "MiniginPCH.h"
#include "AABBCollisionComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "ColliderCommand.h"
#include "CollisionManager.h"

AABBCollisionComponent::AABBCollisionComponent(GameObject& gameObject)
	: BaseComponent(gameObject) 
	, m_Rectangle()
	, m_pOnEnter(nullptr)
	, m_IsTrigger(false)
	, m_pOnExit(nullptr)
{
}


void AABBCollisionComponent::Initialize(const SceneData& sceneData)
{
	sceneData.pCollisionManager->RegisterCollision(*this);
}

bool AABBCollisionComponent::CollidesWith(AABBCollisionComponent* pCollider) const
{
	if (pCollider == nullptr || pCollider == this	//Null Or same collider?
		|| pCollider->IsTrigger() || IsTrigger()	//One is trigger?
		|| &pCollider->GetGameObject().GetRoot() == &GetGameObject().GetRoot()) //Same Object?
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
	if (m_pGameObject == nullptr)return;
	const auto* pTransform = GetGameObject().GetComponent<TransformComponent>();
	if (pTransform != nullptr)
	{
		m_Collider.x += pTransform->GetWorldPosition().x;
		m_Collider.y += pTransform->GetWorldPosition().y;
		m_Collider.width *= pTransform->GetWorldScale().x;
		m_Collider.height *= pTransform->GetWorldScale().y;
	}
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
	m_pOnEnter->Execute(pOther);
}
void AABBCollisionComponent::Exit(AABBCollisionComponent* pOther)
{
	m_pOnExit->Execute(pOther);
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
