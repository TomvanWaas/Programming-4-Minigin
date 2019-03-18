#include "MiniginPCH.h"
#include "GameObject.h"
#include "BaseComponent.h"
#include "TransformComponent.h"

dae::GameObject::GameObject()
	: m_pTransform(new TransformComponent{})
	, m_pParent(nullptr)
{
}

dae::GameObject::~GameObject()
{
	delete m_pTransform;
	for (BaseComponent* pComponent : m_pComponents)
	{
		delete pComponent;
	}
	for (GameObject* pChild : m_pChildren)
	{
		delete pChild;
	}
}

void dae::GameObject::Initialize()
{
	for (BaseComponent* pComponent : m_pComponents)
	{
		pComponent->Initialize();
	}
	for (GameObject* pChild : m_pChildren)
	{
		pChild->Initialize();
	}
}


void dae::GameObject::Update()
{
	for (BaseComponent* pComponent : m_pComponents)
	{
		pComponent->Update();
	}
	for (GameObject* pChild : m_pChildren)
	{
		pChild->Update();
	}
}

void dae::GameObject::Render() const
{
	for (const BaseComponent* pComponent : m_pComponents)
	{
		pComponent->Render();
	}
	for (GameObject* pChild : m_pChildren)
	{
		pChild->Render();
	}
}

void dae::GameObject::AddComponent(BaseComponent* pComponent)
{
	m_pComponents.push_back(pComponent);
	pComponent->SetOwner(this);
}

void dae::GameObject::AddChild(GameObject* pChild)
{
	m_pChildren.push_back(pChild);
	pChild->SetParent(this);
}

void dae::GameObject::SetParent(GameObject* pParent)
{
	m_pParent = pParent;
}

TransformComponent* dae::GameObject::GetTransform()
{
	return m_pTransform;
}
const TransformComponent* dae::GameObject::GetTransform() const
{
	return m_pTransform;
}




