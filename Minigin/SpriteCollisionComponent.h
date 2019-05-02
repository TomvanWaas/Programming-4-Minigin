#pragma once
#include "AABBCollisionComponent.h"
class SpriteComponent;
class SpriteCollisionComponent final : public AABBCollisionComponent
{
public:
	explicit SpriteCollisionComponent(const Rect& extents = Rect{}, bool isTrigger = false, const std::string& tag = "Tag",
		const std::shared_ptr<ColliderCommand>& enter = nullptr, const std::shared_ptr<ColliderCommand>& exit = nullptr);
	virtual ~SpriteCollisionComponent() = default;

	SpriteCollisionComponent(const SpriteCollisionComponent& other) = delete;
	SpriteCollisionComponent(SpriteCollisionComponent&& other) noexcept = delete;
	SpriteCollisionComponent& operator=(const SpriteCollisionComponent& other) = delete;
	SpriteCollisionComponent& operator=(SpriteCollisionComponent&& other) noexcept = delete;

	virtual void InitializeOverride(const SceneData& sceneData) override;

	void SetOffset(float leftx, float rightx, float topy, float bottomy);
	virtual void CalculateCollider() override;

private:
	SpriteComponent* m_pSpriteComponent;


};

