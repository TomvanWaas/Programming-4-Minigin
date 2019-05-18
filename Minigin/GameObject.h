#pragma once
#include <vector>
#include "Transform.h"
#include "Observer.h"
class Scene;
class BaseComponent;
class SceneData;
class GameObject final : public Observer, public Subject
{
public:
	explicit GameObject();
	~GameObject();
	GameObject(const GameObject& other) = delete;
	GameObject(GameObject&& other) = delete;
	GameObject& operator=(const GameObject& other) = delete;
	GameObject& operator=(GameObject&& other) = delete;

	static void DeleteObject(GameObject*& pObject);

	void SetEnabled(bool e);
	bool IsEnabled() const { return IsState(State::Enabled); }

	void Initialize(const SceneData& sceneData);
	void UpdateFirst(const SceneData& sceneData);
	void UpdateSecond(const SceneData& sceneData);
	void Destroy(const SceneData& sceneData);

	const std::vector<BaseComponent*>& GetAllComponents() const;
	const std::vector<GameObject*>& GetAllChildren() const;

	//Observing
	virtual void Notify(ObservedEvent event, const ObservedData& data) override;
	void Notify(const std::string& component, ObservedEvent event, const ObservedData& data);
	void NotifyChildren(ObservedEvent event, const ObservedData& data);
	void NotifyChildren(const std::string& component, ObservedEvent event, const ObservedData& data);
	void NotifyParents(ObservedEvent event, const ObservedData& data);
	void NotifyParents(const std::string& component, ObservedEvent event, const ObservedData& data);


	//Hierarchy
	const GameObject* GetParent() const;
	GameObject* GetParent();
	const GameObject& GetRoot() const;
	GameObject& GetRoot();
	const Scene* GetScene() const;
	Scene* GetScene();
	void SetScene(Scene* pScene);

	GameObject* CreateChild();
	bool DeleteChild(GameObject*& pChild);
	bool RemoveChild(GameObject* pChild);
	bool AddChild(GameObject* pChild);
	void SetParent(GameObject* pParent);

	Transform& GetTransform();
	const Transform& GetTransform() const;

	//Components
	bool AddComponent(BaseComponent* pComponent);
	bool RemoveComponent(BaseComponent* pComponent);
	bool DeleteComponent(BaseComponent*& pComponent);


	//GetComponent
	template <class T>
	T* GetComponent() const
	{
		for (auto* pComp : m_pComponents)
		{
			T* pRes = dynamic_cast<T*>(pComp);
			if (pRes) return pRes;
		}
		return nullptr;
	}
	template <class T>
	T* GetComponentInChildren() const
	{
		T* pComp = nullptr;
		for (const GameObject* pChild: m_pChildren)
		{
			if (pChild == nullptr) continue;
			pComp = pChild->GetComponent<T>();
			if (pComp != nullptr) return pComp;
		}
		for (const GameObject* pChild : m_pChildren)
		{
			if (pChild == nullptr) continue;
			pComp = pChild->GetComponentInChildren<T>();
			if (pComp != nullptr) return pComp;
		}
		return nullptr;
	}
	template <class T>
	T* GetComponentInParents() const
	{
		if (m_pParent != nullptr)
		{
			T* pComp = m_pParent->GetComponent<T>();
			if (pComp != nullptr) return pComp;
			return m_pParent->GetComponentInParents<T>();
		}
		return nullptr;
	}

	template <class T>
	std::vector<T*> GetComponentsInChildren() const
	{
		std::vector<T*> pResult{};
		for (const GameObject* pChild : m_pChildren)
		{
			if (pChild == nullptr) continue;
			T* pComp = pChild->GetComponent<T>();
			if (pComp) pResult.push_back(pComp);
		}
		for (const GameObject* pChild : m_pChildren)
		{
			if (pChild == nullptr) continue;
			auto v = pChild->GetComponentsInChildren<T>();
			pResult.insert(pResult.end(), v.begin(), v.end());
		}
		return pResult;
	}
	template <class T>
	std::vector<T*> GetComponentsInParents() const
	{
		std::vector<T*> pResult{};
		if (m_pParent != nullptr)
		{
			T* pComp = m_pParent->GetComponent<T>();
			if (pComp != nullptr) pResult.push_back(pComp);
			auto v = m_pParent->GetComponentsInParents<T>();
			pResult.insert(pResult.begin(), v.begin(), v.end());
		}
		return pResult;
	}
	
private:
	enum class State : char
	{
		Enabled = 1,
		Initialized = 2,
		Destroyed = 4
	};

	std::vector<GameObject*> m_pChildren;
	std::vector<BaseComponent*> m_pComponents;

	Transform m_Transform;

	GameObject* m_pParent;
	Scene* m_pScene;

	char m_State = 1;



	void SetState(bool set, State state)
	{
		if (set)
		{
			m_State |= char(state);
		}
		else
		{
			m_State &= ~char(state);
		}
	}
	bool IsState(State state) const
	{
		return (m_State & char(state));
	}

};

