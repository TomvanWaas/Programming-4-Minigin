#pragma once
#include "AABBCollisionComponent.h"
class SpriteComponent;
class SpriteCollisionComponent final : public AABBCollisionComponent
{
public:
	explicit SpriteCollisionComponent(GameObject& gameObject);
	virtual ~SpriteCollisionComponent() = default;

	SpriteCollisionComponent(const SpriteCollisionComponent& other) = delete;
	SpriteCollisionComponent(SpriteCollisionComponent&& other) noexcept = delete;
	SpriteCollisionComponent& operator=(const SpriteCollisionComponent& other) = delete;
	SpriteCollisionComponent& operator=(SpriteCollisionComponent&& other) noexcept = delete;

	virtual void Initialize(const SceneData& sceneData) override;

	void SetOffset(float leftx, float rightx, float topy, float bottomy);
	virtual void CalculateCollider() override;

private:
	SpriteComponent* m_pSpriteComponent;


};

