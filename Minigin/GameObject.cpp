#include "MiniginPCH.h"
#include "GameObject.h"
//#include "BaseComponent.h"
#include "TransformComponent.h"
#include "Scene.h"
#include <algorithm>

GameObject::GameObject()
	: m_pTransformComponent(nullptr)
	, m_pComponents()
	, m_pChildren()
	, m_pParent(nullptr)
	, m_pScene(nullptr)
{
	CreateComponent<TransformComponent>();
}
GameObject::~GameObject()
{
	for (GameObject* pChild : m_pChildren)
	{
		SAFE_DELETE(pChild);
	}
	for (BaseComponent* pComponent : m_pComponents)
	{
		SAFE_DELETE(pComponent);
	}
	SAFE_DELETE(m_pTransformComponent);
}




void GameObject::Initialize(const SceneData& sceneData)
{
	m_pTransformComponent->Construct(sceneData);

	for (BaseComponent* pComponent : m_pComponents)
	{
		if (pComponent != nullptr)
		{
			pComponent->Construct(sceneData);
		}
	}
	for (GameObject* pChild : m_pChildren)
	{
		if (pChild != nullptr)
		{
			pChild->Initialize(sceneData);
		}
	}
}

void GameObject::UpdateFirst(const SceneData& sceneData)
{
	//UpdateFirst Components
	m_pTransformComponent->UpdateFirst(sceneData);
	for (BaseComponent* pComponent : m_pComponents)
	{
		if (pComponent != nullptr)
		{
			pComponent->UpdateFirst(sceneData);
		}
	}

	//UpdateFirst Children
	for (GameObject* pChild : m_pChildren)
	{
		if (pChild != nullptr)
		{
			pChild->UpdateFirst(sceneData);
		}
	}

	//Erase if nullptr (deleted or...)
	m_pComponents.erase(std::remove_if(m_pComponents.begin(), m_pComponents.end(), [](const BaseComponent* pComponent)
	{
		return pComponent == nullptr;
	}), m_pComponents.end());
	m_pChildren.erase(std::remove_if(m_pChildren.begin(), m_pChildren.end(), [](const GameObject* pChild)
	{
		return pChild == nullptr;
	}), m_pChildren.end());
}
void GameObject::UpdateSecond(const SceneData& sceneData)
{
	//UpdateFirst Components
	m_pTransformComponent->UpdateFirst(sceneData);
	for (BaseComponent* pComponent : m_pComponents)
	{
		if (pComponent != nullptr)
		{
			pComponent->UpdateSecond(sceneData);
		}
	}

	//UpdateFirst Children
	for (GameObject* pChild : m_pChildren)
	{
		if (pChild != nullptr)
		{
			pChild->UpdateSecond(sceneData);
		}
	}

	//Erase if nullptr (deleted or...)
	m_pComponents.erase(std::remove_if(m_pComponents.begin(), m_pComponents.end(), [](const BaseComponent* pComponent)
	{
		return pComponent == nullptr;
	}), m_pComponents.end());
	m_pChildren.erase(std::remove_if(m_pChildren.begin(), m_pChildren.end(), [](const GameObject* pChild)
	{
		return pChild == nullptr;
	}), m_pChildren.end());
}





GameObject* GameObject::CreateChild()
{
	GameObject* pChild = new GameObject();
	pChild->m_pScene = m_pScene;
	pChild->m_pParent = this;
	if (m_pScene)m_pScene->SetInitialize();
	m_pChildren.push_back(pChild);
	return pChild;
}
void GameObject::DeleteChild(GameObject* pChild)
{
	if (pChild == nullptr) return;
	auto i = std::find(m_pChildren.begin(), m_pChildren.end(), pChild);

	if (i != m_pChildren.end())
	{
		SAFE_DELETE(pChild);
	}
}

void GameObject::SetParent(GameObject* pParent)
{
	if ( (m_pParent == nullptr && pParent == nullptr)
		|| (m_pParent == pParent)) return;

	//Previous Parent was Scene
	if (m_pParent == nullptr
		&& m_pScene != nullptr)
	{
		//New Parent is Object
		m_pScene->RemoveGameObject(this);
		pParent->m_pChildren.push_back(this);
		m_pParent = pParent;
		//New Parent is Scene
		//- Ignored as Previous and New are Scene => No Change
	}
	//Previous Parent was Object
	else if (m_pParent != nullptr)
	{
		//New Parent is Object
		if (pParent != nullptr)
		{
			//Remove from Previous
			auto i = std::find(m_pParent->m_pChildren.begin(), m_pParent->m_pChildren.end(), this);
			if (i != m_pParent->m_pChildren.end())
			{
				(*i) = nullptr;
			}
			//Register on New
			pParent->m_pChildren.push_back(this);
			m_pParent = pParent;
		}
		//New Parent is Scene
		else
		{
			//Remove from Previous
			auto i = std::find(m_pParent->m_pChildren.begin(), m_pParent->m_pChildren.end(), this);
			if (i != m_pParent->m_pChildren.end())
			{
				(*i) = nullptr;
			}
			//Register on Scene
			m_pParent = nullptr;
			m_pScene->AddGameObject(this);
		}
	}	
}



void GameObject::DeleteComponent(BaseComponent* pComponent)
{
	if (pComponent == nullptr) return;
	auto i = std::find(m_pComponents.begin(), m_pComponents.end(), pComponent);

	if (i != m_pComponents.end())
	{
		SAFE_DELETE(pComponent);
	}
}


const std::vector<BaseComponent*>& GameObject::GetAllComponents() const
{
	return m_pComponents;
}
const std::vector<GameObject*>& GameObject::GetAllChildren() const
{
	return m_pChildren;
}


const GameObject* GameObject::GetParent() const
{
	return m_pParent;
}
const GameObject& GameObject::GetRoot() const
{
	const GameObject* pParent = this;
	while (pParent->GetParent() != nullptr)
	{
		pParent = pParent->GetParent();
	}
	return *pParent;
}


const Scene& GameObject::GetScene() const
{
	return *m_pScene;
}
Scene& GameObject::GetScene()
{
	return *m_pScene;
}

