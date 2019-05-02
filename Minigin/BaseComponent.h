#pragma once
class GameObject;
struct SceneData;
class BaseComponent abstract
{
public:
	explicit BaseComponent()
		: m_pGameObject(nullptr)
		, m_State(char(State::enabled))
	{
	}
	virtual ~BaseComponent() = default;

	BaseComponent(const BaseComponent& other) = delete;
	BaseComponent(BaseComponent&& other) noexcept = delete;
	BaseComponent& operator=(const BaseComponent& other) = delete;
	BaseComponent& operator=(BaseComponent&& other) noexcept = delete;

	void Initialize(const SceneData& sceneData);
	void UpdateFirst(const SceneData& sceneData);
	void UpdateSecond(const SceneData& sceneData);
	void Destroy(const SceneData& sceneData);

	const GameObject* GetGameObject() const;
	GameObject* GetGameObject();
	void SetGameObject(GameObject* pObject);

	void SetEnabled(bool e);
	bool IsEnabled() const;

protected:
	virtual void InitializeOverride(const SceneData& sceneData);
	virtual void UpdateFirstOverride(const SceneData& sceneData);
	virtual void UpdateSecondOverride(const SceneData& sceneData);
	virtual void DestroyOverride(const SceneData& sceneData);
private:
	enum class State
	{
		constructed = 1,
		enabled = 2,
		destroyed = 4
	};

	char m_State;
	GameObject* m_pGameObject;


	bool IsConstructed() const;
	void SetConstructed(bool c);
	bool IsDestroyed() const;
	void SetDestroyed(bool d);
};

