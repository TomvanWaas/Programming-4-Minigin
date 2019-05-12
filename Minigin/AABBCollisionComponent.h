#pragma once
#include "BaseComponent.h"
#include "Rect.h"
class ColliderCommand;
class AABBCollisionComponent : public BaseComponent
{
public:
	explicit AABBCollisionComponent(const Rect& extents = Rect{ 0,0,1,1 }, bool isTrigger = false, const std::string& tag = "Tag");
	virtual ~AABBCollisionComponent() = default;

	AABBCollisionComponent(const AABBCollisionComponent& other) = delete;
	AABBCollisionComponent(AABBCollisionComponent&& other) noexcept = delete;
	AABBCollisionComponent& operator=(const AABBCollisionComponent& other) = delete;
	AABBCollisionComponent& operator=(AABBCollisionComponent&& other) noexcept = delete;

	virtual void InitializeOverride(const SceneData& sceneData) override;
	virtual void DestroyOverride(const SceneData& sceneData) override;
	virtual void OnNotify(ObservedEvent event, const ObservedData& data) override;

	bool CollidesWith(AABBCollisionComponent* pCollider) const;



	void SetCollider(float w, float h, float offsetx = 0, float offsety = 0);
	const Rect& GetCollider() const;

	bool IsTrigger() const { return m_IsTrigger; }
	void SetTrigger(bool trigger) { m_IsTrigger = trigger; }

	void SetTag(const std::string& tag);
	const std::string& GetTag() const;


protected:
	Rect m_Rectangle;
	Rect m_Collider;

private:
	bool m_IsTrigger;
	std::string m_Tag;

};

