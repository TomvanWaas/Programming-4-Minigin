#pragma once
#include "AABBCollisionComponent.h"
class SpriteComponent;
class SpriteCollisionComponent final : public AABBCollisionComponent
{
public:
	explicit SpriteCollisionComponent(const Rect& extents = Rect{}, bool isTrigger = false, const std::string& tag = "Tag");
	virtual ~SpriteCollisionComponent() = default;

	SpriteCollisionComponent(const SpriteCollisionComponent& other) = delete;
	SpriteCollisionComponent(SpriteCollisionComponent&& other) noexcept = delete;
	SpriteCollisionComponent& operator=(const SpriteCollisionComponent& other) = delete;
	SpriteCollisionComponent& operator=(SpriteCollisionComponent&& other) noexcept = delete;

	virtual void UpdateSecondOverride(const SceneData& sceneData) override;
	virtual void DestroyOverride(const SceneData& sceneData) override;

	void SetOffset(float leftx, float rightx, float topy, float bottomy);

private:


};

