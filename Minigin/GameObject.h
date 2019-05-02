#pragma once
#include <vector>
#include "Subject.h"
#include "Transform.h"
class Scene;
class BaseComponent;
struct SceneData;
class GameObject final : public Subject
{
public:
	explicit GameObject();
	virtual ~GameObject();
	GameObject(const GameObject& other) = delete;
	GameObject(GameObject&& other) = delete;
	GameObject& operator=(const GameObject& other) = delete;
	GameObject& operator=(GameObject&& other) = delete;

	void SetEnabled(bool e);
	bool IsEnabled() const { return m_IsEnabled; }

	void Initialize(const SceneData& sceneData);
	void UpdateFirst(const SceneData& sceneData);
	void UpdateSecond(const SceneData& sceneData);
	void Destroy(const SceneData& sceneData);

	const std::vector<BaseComponent*>& GetAllComponents() const;
	const std::vector<GameObject*>& GetAllChildren() const;

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
	bool DeleteComponent(BaseComponent*& pComponent, const SceneData& sceneData);


	//GetComponent
	/////////////////////////////////////////////////////////
	//BASED ON OVERLORD_ENGINE (from Graphics Programming 2)
	/////////////////////////////////////////////////////////
	template <class T>
	T* GetComponent() const
	{
		const type_info& ti = typeid(T);

		for (auto* pComp : m_pComponents)
		{
			if (pComp != nullptr && typeid(*pComp) == ti) return static_cast<T*>(pComp);
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
	Scene* m_pScene;
	GameObject* m_pParent;
	std::vector<GameObject*> m_pChildren;

	Transform m_Transform;
	std::vector<BaseComponent*> m_pComponents;
	bool m_IsEnabled = true;
};

