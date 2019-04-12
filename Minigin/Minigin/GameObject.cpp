#include "MiniginPCH.h"
#include "GameObject.h"
#include "BaseComponent.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "Scene.h"
#include "Logger.h"
#include <algorithm>

GameObject::GameObject()
	: m_pTransformComponent(new TransformComponent{})
	, m_pComponents()
	, m_pChildren()
	, m_pParent(nullptr)
	, m_pScene(nullptr)
{
}
GameObject::~GameObject()
{
	SAFE_DELETE(m_pTransformComponent);

	for (BaseComponent* pComponent : m_pComponents)
	{
		SAFE_DELETE(pComponent);
	}

	for (GameObject* pChild : m_pChildren)
	{
		SAFE_DELETE(pChild);
	}
}


void GameObject::Initialize(const SceneData& sceneData)
{
	m_pTransformComponent->SetGameObject(this);
	m_pTransformComponent->Initialize(sceneData);

	for (BaseComponent* pComponent : m_pComponents)
	{
		pComponent->Initialize(sceneData);
	}
	for (GameObject* pChild : m_pChildren)
	{
		pChild->Initialize(sceneData);
	}
}
void GameObject::Update(const SceneData& sceneData)
{
	m_pTransformComponent->Update(sceneData);

	for (BaseComponent* pComponent : m_pComponents)
	{
		pComponent->Update(sceneData);
	}
	for (GameObject* pChild : m_pChildren)
	{
		pChild->Update(sceneData);
	}
}
void GameObject::Destroy()
{
	//Destroy in loop

	SAFE_DELETE(m_pTransformComponent);
	if (m_pScene) m_pScene->RemoveChild(this);

	for (BaseComponent* pComponent : m_pComponents)
	{
		SAFE_DELETE(pComponent);
	}

	for (GameObject* pChild : m_pChildren)
	{
		SAFE_DELETE(pChild);
	}
}


void GameObject::AddComponent(BaseComponent* pComponent)
{
	//TransformComponent?
	if (typeid(*pComponent) == typeid(TransformComponent))
	{
		if (m_pTransformComponent != nullptr)
		{
			Logger::GetInstance().LogWarning("GameObject::AddComponent > Already owns a TransformComponent");
			return;
		}
		m_pTransformComponent = static_cast<TransformComponent*>(pComponent);
		return;
	}

	//Check if already owns comp in vector
	auto i = std::find_if(m_pComponents.begin(), m_pComponents.end(), [pComponent](BaseComponent* pComp)
	{
		return (typeid(*pComponent) == typeid(*pComp));
	});

	if (i == m_pComponents.end())
	{
		m_pComponents.push_back(pComponent);
		pComponent->SetGameObject(this);
	}
	else
	{
		std::string name = std::string(typeid(*pComponent).name());
		Logger::GetInstance().LogWarning(("GameObject::AddComponent > Already owns a " + name));
	}
}
void GameObject::RemoveCompoment(BaseComponent* pComponent)
{
	auto i = std::find(m_pComponents.begin(), m_pComponents.end(), pComponent);

	if (i != m_pComponents.end())
	{
		m_pComponents.erase(i);
		pComponent->SetGameObject(nullptr);
	}
	else
	{
		std::string name = std::string(typeid(*pComponent).name());
		Logger::GetInstance().LogWarning(("GameObject::RemoveComponent > Does not own component " + name));
	}
}
const std::vector<BaseComponent*>& GameObject::GetComponents() const
{
	return m_pComponents;
}


void GameObject::AddChild(GameObject* pChild)
{
	if (pChild != nullptr)
	{
		auto i = std::find(m_pChildren.begin(), m_pChildren.end(), pChild);

		if (i == m_pChildren.end())
		{
			//Adopt parent
			if (pChild->m_pParent != nullptr)
			{
				pChild->m_pParent->RemoveChild(pChild);
			}

			//Adopt scene
			if (pChild->m_pScene != nullptr)
			{
				pChild->m_pScene->RemoveChild(pChild);
			}

			//Push
			m_pChildren.push_back(pChild);
			pChild->m_pParent = this;
		}
	}
}
void GameObject::RemoveChild(GameObject* pChild)
{
	auto i = std::find(m_pChildren.begin(), m_pChildren.end(), pChild);

	if (i != m_pChildren.end() && pChild != nullptr)
	{
		m_pChildren.erase(i);
		pChild->m_pParent = nullptr;
	}
}
const std::vector<GameObject*>& GameObject::GetChildren() const
{
	return m_pChildren;
}


const GameObject* GameObject::GetParent() const
{
	return m_pParent;
}

const GameObject* GameObject::GetRoot() const
{
	const GameObject* pGo = this;
	while (pGo->GetParent() != nullptr)
	{
		pGo = pGo->GetParent();
	}
	return pGo;
}

void GameObject::SetScene(Scene* pScene)
{
	m_pScene = pScene;
}
Scene* GameObject::GetScene(bool inroot) const
{
	if (inroot)
	{
		return GetRoot()->m_pScene;
	}
	return m_pScene;
}

