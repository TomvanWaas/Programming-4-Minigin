#pragma once
#include <vector>
class BaseComponent;
class TransformComponent;
namespace dae
{
	class GameObject final
	{
	public:
		void Initialize();
		virtual void Update();
		virtual void Render() const;

		void AddComponent(BaseComponent* pComponent);
		void AddChild(GameObject* pChild);
		void SetParent(GameObject* pParent);

		template <typename T>
		T* GetComponent(bool searchchildren = false)
		{
			//BASED ON OVERLORD_ENGINE (from Graphics Programming 2)

			//Search Self
			const type_info& ti = typeid(T);
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
		const T* GetComponent(bool searchchildren = false) const
		{
			//BASED ON OVERLORD_ENGINE (from Graphics Programming 2)

			//Search Self
			const type_info& ti = typeid(T);
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
		TransformComponent* GetTransform();
		const TransformComponent* GetTransform() const;


		GameObject();
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		TransformComponent* m_pTransform;
		std::vector<BaseComponent*> m_pComponents;
		std::vector<GameObject*> m_pChildren;
		GameObject* m_pParent;
	};
}
