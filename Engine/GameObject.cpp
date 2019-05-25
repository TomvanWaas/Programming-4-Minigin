#include "stdafx.h"
#include "GameObject.h"
#include "BaseComponent.h"
#include "Transform.h"
#include "Scene.h"
#include <algorithm>
#include "Deletor.h"
#include "ObservedData.h"

using namespace Minigin;

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
	for (GameObject*& pChild : m_pChildren)
	{
		SAFE_DELETE(pChild);
	}
	m_pChildren.clear();
	for (BaseComponent*& pComponent : m_pComponents)
	{
		SAFE_DELETE(pComponent);
	}
	m_pComponents.clear();
}


void GameObject::DeleteObject(GameObject*& pObject)
{
	if (!pObject) return;

	////Delete Children
	auto children = pObject->GetAllChildren();
	for (GameObject*& pChild : children)
	{
		DeleteObject(pChild);
	}

	////Delete Self
	//Destroy
	if (pObject->m_pScene)
	{
		pObject->Destroy(pObject->m_pScene->GetSceneData());
	}

	//Remove in parent
	if (pObject->m_pParent)
	{
		auto i = std::find(pObject->m_pParent->m_pChildren.begin(), pObject->m_pParent->m_pChildren.end(), pObject);
		if (i != pObject->m_pParent->m_pChildren.end())
		{
			(*i) = nullptr;
		}
	}
	pObject->m_pParent = nullptr;

	//Remove in Scene
	if (pObject->m_pScene)
	{
		pObject->m_pScene->RemoveGameObject(pObject);
	}	
	pObject->m_pScene = nullptr;

	//Set State
	pObject->SetState(false, State::Enabled);

	Deletor::GetInstance().StoreDelete(pObject);
}



void GameObject::SetEnabled(bool e)
{
	SetState(e, State::Enabled);
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
	if (!IsState(State::Initialized))
	{
		SetState(true, State::Initialized);
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
}
void GameObject::UpdateFirst(const SceneData& sceneData)
{
	if (IsState(State::Enabled) && IsState(State::Initialized) && !IsState(State::Destroyed))
	{
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
		m_pComponents.erase(std::remove_if(m_pComponents.begin(), m_pComponents.end(), [](const BaseComponent* pComp)
		{
			return pComp == nullptr;
		}), m_pComponents.end());
		m_pChildren.erase(std::remove_if(m_pChildren.begin(), m_pChildren.end(), [](const GameObject* pChild)
		{
			return pChild == nullptr;
		}), m_pChildren.end());
	}
}
void GameObject::UpdateSecond(const SceneData& sceneData)
{
	if (IsState(State::Enabled) && IsState(State::Initialized) && !IsState(State::Destroyed))
	{
		//UpdateSecondOverride Components
		for (BaseComponent* pComponent : m_pComponents)
		{
			if (pComponent != nullptr)
			{
				pComponent->UpdateSecond(sceneData);
			}
		}

		//UpdateSecondOverride Children
		for (GameObject* pChild : m_pChildren)
		{
			if (pChild != nullptr)
			{
				pChild->UpdateSecond(sceneData);
			}
		}

		//Erase if nullptr (deleted or removed)
		m_pComponents.erase(std::remove_if(m_pComponents.begin(), m_pComponents.end(), [](const BaseComponent* pComp)
		{
			return pComp == nullptr;
		}), m_pComponents.end());
		m_pChildren.erase(std::remove_if(m_pChildren.begin(), m_pChildren.end(), [](const GameObject* pChild)
		{
			return pChild == nullptr;
		}), m_pChildren.end());
	}
}
void GameObject::Destroy(const SceneData& sceneData)
{
	if (!IsState(State::Destroyed))
	{
		//Only Notify Observers as components & children get also destroyed and notify themselves also
		ObservedData d{};
		d.AddData<GameObject*>("GameObject", this);
		NotifyObservers(ObservedEvent::Destroyed, d);
		if (GetScene()) GetScene()->Notify(ObservedEvent::Destroyed, d);


		SetState(true, State::Destroyed);
		for (BaseComponent* pC : m_pComponents)
		{
			if (pC) pC->Destroy(sceneData);
		}
		for (GameObject* pC : m_pChildren)
		{
			if (pC) pC->Destroy(sceneData);
		}
		
	}
	DestroyObserver();
	DestroySubject();
}


const std::vector<BaseComponent*>& GameObject::GetAllComponents() const
{
	return m_pComponents;
}
const std::vector<GameObject*>& GameObject::GetAllChildren() const
{
	return m_pChildren;
}




void GameObject::Notify(ObservedEvent event, const ObservedData& data)
{

	m_Transform.OnNotify(event, data);
	for (BaseComponent* pComponent : m_pComponents)
	{
		if (pComponent) pComponent->Notify(event, data);
	}
	NotifyObservers(event, data);

}
void GameObject::Notify(const std::string& component, ObservedEvent event, const ObservedData& data)
{

	if (component == typeid(Transform).name())
	{
		m_Transform.OnNotify(event, data);
	}
	else
	{
		for (BaseComponent* pComponent : m_pComponents)
		{
			if (pComponent && typeid(*pComponent).name() == component)
			{
				pComponent->Notify(event, data);
				return;
			}
		}
	}

}
void GameObject::NotifyChildren(ObservedEvent event, const ObservedData& data)
{

	for (GameObject* pChild : m_pChildren)
	{
		if (pChild)
		{
			pChild->Notify(event, data);
			pChild->NotifyChildren(event, data);
		}
	}

}
void GameObject::NotifyChildren(const std::string& component, ObservedEvent event, const ObservedData& data)
{

	for (GameObject* pChild : m_pChildren)
	{
		if (pChild)
		{
			pChild->Notify(component, event, data);
			pChild->NotifyChildren(component, event, data);
		}
	}

}
void GameObject::NotifyParents(ObservedEvent event, const ObservedData& data)
{
	if (m_pParent)
	{
		m_pParent->Notify(event, data);
		m_pParent->NotifyParents(event, data);
	}
}
void GameObject::NotifyParents(const std::string& component, ObservedEvent event, const ObservedData& data)
{
	if (m_pParent)
	{
		m_pParent->Notify(component, event, data);
		m_pParent->NotifyParents(component, event, data);
	}
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
	ObservedData d{};
	pChild->Notify(ObservedEvent::ScaleChanged, d);
	pChild->Notify(ObservedEvent::PositionChanged, d);
	pChild->Notify(ObservedEvent::RotationChanged, d);
	return pChild;
}
bool GameObject::DeleteChild(GameObject*& pChild)
{
	if (pChild == nullptr) return false;
	if (RemoveChild(pChild))
	{
		if (m_pScene) pChild->Destroy(m_pScene->GetSceneData());
		Deletor::GetInstance().StoreDelete(pChild);
		pChild->SetEnabled(false);
		pChild = nullptr;
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

		ObservedData d{};
		pChild->Notify(ObservedEvent::ScaleChanged, d);
		pChild->NotifyChildren(ObservedEvent::ScaleChanged, d);
		pChild->Notify(ObservedEvent::RotationChanged, d);
		pChild->NotifyChildren(ObservedEvent::RotationChanged, d);
		pChild->Notify(ObservedEvent::PositionChanged, d);
		pChild->NotifyChildren(ObservedEvent::PositionChanged, d);
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
		if (IsState(State::Initialized) && m_pScene != nullptr)
		{
			pChild->Initialize(m_pScene->GetSceneData());
		}
		ObservedData d{};
		pChild->Notify(ObservedEvent::ScaleChanged, d);
		pChild->NotifyChildren(ObservedEvent::ScaleChanged, d);
		pChild->Notify(ObservedEvent::RotationChanged, d);
		pChild->NotifyChildren(ObservedEvent::RotationChanged, d);
		pChild->Notify(ObservedEvent::PositionChanged, d);
		pChild->NotifyChildren(ObservedEvent::PositionChanged, d);
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
	const type_info& ti = typeid(*pComponent);

	//Already owns Component?
	for (BaseComponent* pComp : m_pComponents)
	{
		if (pComp && typeid(pComp) == ti) return false;
	}

	//Else => Add
	m_pComponents.push_back(pComponent);
	pComponent->SetGameObject(this);
	if (IsState(State::Initialized) && m_pScene)
	{
		pComponent->Initialize(m_pScene->GetSceneData());
	}
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
bool GameObject::DeleteComponent(BaseComponent*& pComponent)
{
	if (RemoveComponent(pComponent))
	{
		if (m_pScene) pComponent->Destroy(m_pScene->GetSceneData());
		Deletor::GetInstance().StoreDelete(pComponent);
		pComponent->SetEnabled(false);
		pComponent = nullptr;
		return true;
	}
	return false;
}













