#include "MiniginPCH.h"
#include "GameObject.h"
#include "BaseComponent.h"
#include "Transform.h"
#include "Scene.h"
#include <algorithm>

GameObject::GameObject()
	: m_Transform(*this)
	, m_pComponents()
	, m_pChildren()
	, m_pParent(nullptr)
	, m_pScene(nullptr)
{
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
}


void GameObject::SetEnabled(bool e)
{
	m_IsEnabled = e;
	for (BaseComponent* pComp: m_pComponents)
	{
		if (pComp) pComp->SetEnabled(e);
	}
	for (GameObject* pChild: m_pChildren)
	{
		if (pChild) pChild->SetEnabled(e);
	}
}

void GameObject::Initialize(const SceneData& sceneData)
{
	for (BaseComponent* pComponent : m_pComponents)
	{
		if (pComponent != nullptr)
		{
			pComponent->Initialize(sceneData);
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
	if (m_IsEnabled == false) return;

	//UpdateFirstOverride Components
	for (BaseComponent* pComponent : m_pComponents)
	{
		if (pComponent != nullptr)
		{
			pComponent->UpdateFirst(sceneData);
		}
	}

	//UpdateFirstOverride Children
	for (GameObject* pChild : m_pChildren)
	{
		if (pChild != nullptr)
		{
			pChild->UpdateFirst(sceneData);
		}
	}

	//Erase if nullptr (deleted or removed)
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
	if (m_IsEnabled == false) return;

	//UpdateFirstOverride Components
	for (BaseComponent* pComponent : m_pComponents)
	{
		if (pComponent != nullptr)
		{
			pComponent->UpdateSecond(sceneData);
		}
	}

	//UpdateFirstOverride Children
	for (GameObject* pChild : m_pChildren)
	{
		if (pChild != nullptr)
		{
			pChild->UpdateSecond(sceneData);
		}
	}

	//Erase if nullptr (deleted or removed)
	m_pComponents.erase(std::remove_if(m_pComponents.begin(), m_pComponents.end(), [](const BaseComponent* pComponent)
	{
		return pComponent == nullptr;
	}), m_pComponents.end());
	m_pChildren.erase(std::remove_if(m_pChildren.begin(), m_pChildren.end(), [](const GameObject* pChild)
	{
		return pChild == nullptr;
	}), m_pChildren.end());
}

void GameObject::Destroy(const SceneData& sceneData)
{
	for (auto* pC: m_pComponents)
	{
		if (pC) pC->Destroy(sceneData);
	}
	for (auto* pC: m_pChildren)
	{
		if (pC) pC->Destroy(sceneData);
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
GameObject* GameObject::GetParent()
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
GameObject& GameObject::GetRoot()
{
	GameObject* pParent = this;
	while (pParent->GetParent() != nullptr)
	{
		pParent = pParent->GetParent();
	}
	return *pParent;
}
const Scene* GameObject::GetScene() const
{
	Scene* pScene = GetRoot().m_pScene;
	if (pScene != nullptr) return pScene;
	return m_pScene;
}
Scene* GameObject::GetScene()
{
	Scene* pScene = GetRoot().m_pScene;
	if (pScene != nullptr) return pScene;
	return m_pScene;
}
void GameObject::SetScene(Scene* pScene)
{
	if (m_pScene == pScene) return;
	Scene* pPrev = m_pScene;
	m_pScene = pScene;
	if (pPrev)
	{
		pPrev->RemoveGameObject(this);
	}
	if (pScene)
	{
		pScene->AddGameObject(this);
	}
	
}
const Transform& GameObject::GetTransform() const
{
	return m_Transform;
}
Transform& GameObject::GetTransform()
{
	return m_Transform;
}




GameObject* GameObject::CreateChild()
{
	GameObject* pChild = new GameObject();
	AddChild(pChild);
	return pChild;
}
bool GameObject::DeleteChild(GameObject*& pChild)
{
	if (pChild == nullptr) return false;
	if (RemoveChild(pChild))
	{
		GetScene()->DeleteGameObject(pChild);
		return true;
	}
	return false;
}
bool GameObject::RemoveChild(GameObject* pChild)
{
	if (pChild == nullptr) return false;
	auto i = std::find(m_pChildren.begin(), m_pChildren.end(), pChild);
	if (i != m_pChildren.end())
	{
		(*i)->m_pParent = nullptr;
		(*i) = nullptr;
		return true;
	}
	return false;
}
bool GameObject::AddChild(GameObject* pChild)
{
	if (pChild == nullptr) return false;
	auto i = std::find(m_pChildren.begin(), m_pChildren.end(), pChild);
	if (i == m_pChildren.end())
	{
		m_pChildren.push_back(pChild);
		pChild->m_pParent = this;
		pChild->m_pScene = m_pScene;
		return true;
	}
	return false;
}
void GameObject::SetParent(GameObject* pParent)
{
	if (pParent == m_pParent) return;

	//Scene as new parent
	if (pParent == nullptr)
	{
		//Prev is scene => Ignored
		//Prev is Object
		m_pParent->RemoveChild(this);
		m_pParent = nullptr;
		if (m_pScene != nullptr)
		{
			m_pScene->AddGameObject(this);
		}
	}
	//Object as new parent
	else
	{
		//Prev was scene
		if (m_pParent == nullptr)
		{
			if (m_pScene != nullptr)
			{
				m_pScene->RemoveGameObject(this);
				m_pParent = pParent;
				pParent->AddChild(this);
			}
		}
		//Prev was object
		else
		{
			m_pParent->RemoveChild(this);
			m_pParent = pParent;
			pParent->AddChild(this);
		}
	}
}





bool GameObject::AddComponent(BaseComponent* pComponent)
{
	if (pComponent == nullptr) return false;
	const type_info& ti = typeid(pComponent);

	for (const auto* pComp : m_pComponents)
	{
		if (pComp != nullptr && typeid(*pComp) == ti)
		{
			return false;
		}
	}
	m_pComponents.push_back(pComponent);
	pComponent->SetGameObject(this);
	return true;
}
bool GameObject::RemoveComponent(BaseComponent* pComponent)
{
	if (pComponent == nullptr) return false;
	auto i = std::find(m_pComponents.begin(), m_pComponents.end(), pComponent);
	if (i != m_pComponents.end())
	{
		(*i) = nullptr;
		return true;
	}
	return false;
}
bool GameObject::DeleteComponent(BaseComponent*& pComponent, const SceneData& sceneData)
{
	if (RemoveComponent(pComponent))
	{
		pComponent->Destroy(sceneData);
		SAFE_DELETE(pComponent);
		return true;
	}
	return false;
}









