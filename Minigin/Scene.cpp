#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"

dae::Scene::Scene(const std::string& name) : m_Name(name)
{
}


dae::Scene::~Scene()
{
	for (GameObject* pObject : m_pObjects)
	{
		delete pObject;
	}
	m_pObjects.clear();
}

void dae::Scene::Add(GameObject* pObject)
{
	m_pObjects.push_back(pObject);
}

void dae::Scene::Initialize()
{
	for (GameObject* pObject : m_pObjects)
	{
		pObject->Initialize();
	}
}

void dae::Scene::Update()
{
	for(auto gameObject : m_pObjects)
	{
		gameObject->Update();
	}
}

void dae::Scene::Render() const
{
	for (const auto gameObject : m_pObjects)
	{
		gameObject->Render();
	}
}

