#pragma once
#include <vector>
#include "Scene.h"
#include "Subject.h"
class BaseComponent;
struct SceneData;
class TransformComponent;
class GameObject final : public Subject
{

	friend GameObject* Scene::CreateGameObject();

	explicit GameObject();

public:

	virtual ~GameObject();
	GameObject(const GameObject& other) = delete;
	GameObject(GameObject&& other) = delete;
	GameObject& operator=(const GameObject& other) = delete;
	GameObject& operator=(GameObject&& other) = delete;

	void Initialize(const SceneData& sceneData);
	void UpdateFirst(const SceneData& sceneData);
	void UpdateSecond(const SceneData& sceneData);



	const std::vector<BaseComponent*>& GetAllComponents() const;
	const std::vector<GameObject*>& GetAllChildren() const;

	const GameObject* GetParent() const;
	const GameObject& GetRoot() const;

	const Scene& GetScene() const;
	Scene& GetScene();



	//Creates new gameobject and adds it as child
	GameObject* CreateChild();
	void DeleteChild(GameObject* pChild);
	void SetParent(GameObject* pParent);

	//Creates new Component and adds it to vector
	template <class T>
	T* CreateComponent()
	{
		T* pComponent = GetComponent<T>();
		if (pComponent == nullptr)
		{
			//Tell scene to initialize
			if (m_pScene) m_pScene->SetInitialize();

			pComponent = new T(*this);

			//If transformComp
			if (typeid(T) == typeid(TransformComponent))
			{
				m_pTransformComponent = dynamic_cast<TransformComponent*>(pComponent);
				return pComponent;
			}
			//Else add to vector
			m_pComponents.push_back(dynamic_cast<BaseComponent*>(pComponent));
			return pComponent;
		}
		return pComponent;
	}
	void DeleteComponent(BaseComponent* pComponent);


	//Returns component in self and or children
	template <class T>
	T* GetComponent(bool searchChildren = false) const
	{
		/////////////////////////////////////////////////////////
		//BASED ON OVERLORD_ENGINE (from Graphics Programming 2)
		/////////////////////////////////////////////////////////

		const type_info& ti = typeid(T);

		//Search Self
		for (BaseComponent* pComponent : m_pComponents)
		{
			if (pComponent != nullptr && typeid(*pComponent) == ti)
			{
				return static_cast<T*>(pComponent);
			}
		}

		if (searchChildren)
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

	//Base get function for transformComponent (Specialized)
	template <>
	TransformComponent* GetComponent<TransformComponent>(bool searchChildren) const
	{
		UNREFERENCED_PARAMETER(searchChildren);
		return m_pTransformComponent;
	}

	template <class T>
	std::vector<T*> GetComponents(bool searchChildren = true) const
	{
		std::vector<T*> pResult;

		pResult.push_back(GetComponent<T>());

		if (searchChildren)
		{
			for (GameObject* pChild: m_pChildren)
			{
				auto v = pChild->GetComponents<T>();
				pResult.insert(pResult.end(), v.begin(), v.end());
			}
		}

		return pResult;
	}

private:
	Scene* m_pScene;

	GameObject* m_pParent;
	std::vector<GameObject*> m_pChildren;

	TransformComponent* m_pTransformComponent;
	std::vector<BaseComponent*> m_pComponents;
};

