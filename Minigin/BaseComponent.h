#pragma once
#include "GameObject.h"
struct SceneData;
class BaseComponent abstract
{
	friend BaseComponent* GameObject::CreateComponent<BaseComponent>();

public:
	virtual ~BaseComponent() = default;

	BaseComponent(const BaseComponent& other) = delete;
	BaseComponent(BaseComponent&& other) noexcept = delete;
	BaseComponent& operator=(const BaseComponent& other) = delete;
	BaseComponent& operator=(BaseComponent&& other) noexcept = delete;

	void Construct(const SceneData& sceneData);
	virtual void UpdateFirst(const SceneData& sceneData);
	virtual void UpdateSecond(const SceneData& sceneData);

	const GameObject& GetGameObject() const;
	GameObject& GetGameObject();



protected:
	virtual void Initialize(const SceneData& sceneData);
	explicit BaseComponent(GameObject& gameObject);

	GameObject* m_pGameObject;

private:
	bool m_IsConstructed = false;
};

