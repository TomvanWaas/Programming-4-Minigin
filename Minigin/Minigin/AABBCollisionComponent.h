#pragma once
#include "BaseComponent.h"
#include "Rect.h"
class ColliderCommand;
class AABBCollisionComponent final : public BaseComponent
{
public:
	explicit AABBCollisionComponent(float w, float h, float offsetx = 0, float offsety = 0);
	~AABBCollisionComponent() = default;

	AABBCollisionComponent(const AABBCollisionComponent& other) = delete;
	AABBCollisionComponent(AABBCollisionComponent&& other) noexcept = delete;
	AABBCollisionComponent& operator=(const AABBCollisionComponent& other) = delete;
	AABBCollisionComponent& operator=(AABBCollisionComponent&& other) noexcept = delete;

	Rect GetCollider() const;
	void CalculateCollider();

	void Enter(AABBCollisionComponent* pOther);
	void Exit(AABBCollisionComponent* pOther);

	bool IsTrigger() const { return m_IsTrigger; }

private:
	Rect m_Rectangle;
	Rect m_Collider;
	bool m_IsTrigger;

	std::shared_ptr<ColliderCommand> m_pOnEnter;
	std::shared_ptr<ColliderCommand> m_pOnExit;
};

