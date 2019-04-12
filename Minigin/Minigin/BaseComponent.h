#pragma once
struct SceneData;
class GameObject;
class BaseComponent abstract
{
public:
	explicit BaseComponent() = default;
	virtual ~BaseComponent() = default;

	BaseComponent(const BaseComponent& other) = delete;
	BaseComponent(BaseComponent&& other) noexcept = delete;
	BaseComponent& operator=(const BaseComponent& other) = delete;
	BaseComponent& operator=(BaseComponent&& other) noexcept = delete;

	virtual void Initialize(const SceneData& sceneData);
	virtual void ReInitialize(const SceneData& sceneData);
	virtual void Update(const SceneData& sceneData);

	void SetGameObject(GameObject* gameObject);
	GameObject* GetGameObject() const;



protected:
	GameObject* m_pGameObject;

};

