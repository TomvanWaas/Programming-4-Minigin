#pragma once
#include <vector>
#include "Subject.h"
struct SceneData;
class BaseComponent;
class TransformComponent;
class RenderComponent;
class Scene;

class GameObject final : public Subject
{
public:
	void Initialize(const SceneData& sceneData);
	void Update(const SceneData& sceneData);
	void Destroy();

	void AddComponent(BaseComponent* pComponent);
	void RemoveCompoment(BaseComponent* pComponent);
	const std::vector<BaseComponent*>& GetComponents() const;

	template <typename T>
	T* GetComponent(bool searchchildren = false) const
	{
		//BASED ON OVERLORD_ENGINE (from Graphics Programming 2)

		const type_info& ti = typeid(T);

		//Search Self
		for (BaseComponent* pComponent : m_pComponents)
		{
			if (pComponent != nullptr && typeid(*pComponent) == ti)
			{
				return static_cast<T*>(pComponent);
			}
		}

		if (searchchildren)
		{
			//Search Children
			for (GameObject* pChild : m_pChildren)
			{
				if (pChild == nullptr) continue;
				T* pComponent = pChild->GetComponent<T>(true);
				if (pComponent != nullptr) return pComponent;
			}
		}

		//Return NULLPTR
		return nullptr;
	}
	template <typename T>
	std::vector<T*> GetComponents(bool searchchildren = false) const
	{
		std::vector<T*> pResult{};

		const type_info& ti = typeid(T);

		//Search Self
		for (BaseComponent* pComponent : m_pComponents)
		{
			if (pComponent != nullptr && typeid(*pComponent) == ti)
			{
				pResult.push_back(static_cast<T*>(pComponent));
			}
		}

		if (searchchildren)
		{
			//Search Children
			for (GameObject* pChild : m_pChildren)
			{
				if (pChild == nullptr) continue;
				std::vector<T*> pComponents = pChild->GetComponents<T>(true);
				pResult.insert(pComponents.begin(), pComponents.end());
			}
		}

		return pResult;
	}
	template <>
	TransformComponent* GetComponent(bool searchchildren)const
	{
		UNREFERENCED_PARAMETER(searchchildren);
		return m_pTransformComponent;
	}

	void AddChild(GameObject* pChild);
	void RemoveChild(GameObject* pChild);
	const std::vector<GameObject*>& GetChildren() const;

	const GameObject* GetParent() const;
	const GameObject* GetRoot() const;

	void SetScene(Scene* pScene);
	Scene* GetScene(bool inroot = false) const;

	explicit GameObject();
	~GameObject();
	GameObject(const GameObject& other) = delete;
	GameObject(GameObject&& other) = delete;
	GameObject& operator=(const GameObject& other) = delete;
	GameObject& operator=(GameObject&& other) = delete;

private:
	TransformComponent* m_pTransformComponent;
	std::vector<BaseComponent*> m_pComponents;
	std::vector<GameObject*> m_pChildren;
	GameObject* m_pParent;
	Scene* m_pScene;

};

