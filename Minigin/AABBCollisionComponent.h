#pragma once
#include "BaseComponent.h"
#include "Rect.h"
class ColliderCommand;
class AABBCollisionComponent : public BaseComponent
{
public:
	explicit AABBCollisionComponent(const Rect& extents = Rect{ 0,0,1,1 },
		bool isTrigger = false,
		const std::string& tag = "Tag",
		const std::shared_ptr<ColliderCommand>& enter = nullptr, 
		const std::shared_ptr<ColliderCommand>& exit = nullptr);
	~AABBCollisionComponent() = default;

	AABBCollisionComponent(const AABBCollisionComponent& other) = delete;
	AABBCollisionComponent(AABBCollisionComponent&& other) noexcept = delete;
	AABBCollisionComponent& operator=(const AABBCollisionComponent& other) = delete;
	AABBCollisionComponent& operator=(AABBCollisionComponent&& other) noexcept = delete;

	virtual void InitializeOverride(const SceneData& sceneData) override;
	virtual void DestroyOverride(const SceneData& sceneData) override;

	bool CollidesWith(AABBCollisionComponent* pCollider) const;



	virtual void CalculateCollider();
	void SetCollider(float w, float h, float offsetx = 0, float offsety = 0);
	const Rect& GetCollider() const;

	void Enter(AABBCollisionComponent* pOther);
	void Exit(AABBCollisionComponent* pOther);

	bool IsTrigger() const { return m_IsTrigger; }
	void SetTrigger(bool trigger) { m_IsTrigger = trigger; }

	void SetTag(const std::string& tag);
	const std::string& GetTag() const;

	void SetEnterCommand(const std::shared_ptr<ColliderCommand>& pCommand);
	void SetExitCommand(const std::shared_ptr<ColliderCommand>& pCommand);


protected:
	Rect m_Rectangle;
	Rect m_Collider;

private:

	bool m_IsTrigger;
	std::string m_Tag;

	std::shared_ptr<ColliderCommand> m_pOnEnter;
	std::shared_ptr<ColliderCommand> m_pOnExit;
};

