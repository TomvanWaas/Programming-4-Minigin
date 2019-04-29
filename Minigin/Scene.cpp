#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"
#include <algorithm>
#include "InputManager.h"
#include "CollisionManager.h"
#include "RenderManager.h"

Scene::Scene(const std::string& name, Time* pTime)
	: m_Name(name)
	, m_pGameObjects()
{
	m_SceneData.Initialize(pTime, nullptr, nullptr, nullptr);
	m_SceneData.pCollisionManager->Initialize(m_SceneData);
}
Scene::~Scene()
{
	for (GameObject* pObject : m_pGameObjects)
	{
		SAFE_DELETE(pObject);
	}
	m_pGameObjects.clear();

	SAFE_DELETE(m_SceneData.pInput);
	SAFE_DELETE(m_SceneData.pCollisionManager);
	SAFE_DELETE(m_SceneData.pRenderManager);
}



GameObject* Scene::CreateGameObject()
{
	GameObject* pObject = new GameObject();
	pObject->m_pScene = this;

	m_pGameObjects.push_back(pObject);
	return pObject;
}
void Scene::DeleteGameObject(GameObject* pObject)
{
	if (pObject == nullptr) return;

	for (GameObject* pGameObject: m_pGameObjects)
	{
		if (pGameObject == pObject)
		{
			SAFE_DELETE(pGameObject);
			break;
		}
	}
}

void Scene::RemoveGameObject(GameObject* pObject)
{
	auto i = std::find(m_pGameObjects.begin(), m_pGameObjects.end(), pObject);
	if (i != m_pGameObjects.end() && pObject != nullptr)
	{
		(*i) = nullptr;
	}
}

void Scene::AddGameObject(GameObject* pObject)
{
	auto i = std::find(m_pGameObjects.begin(), m_pGameObjects.end(), pObject);
	if (i == m_pGameObjects.end() && pObject != nullptr)
	{
		m_pGameObjects.push_back(pObject);
		pObject->SetParent(nullptr);
	}
}


void Scene::Initialize()
{
	//Initialize all
	for (GameObject* pObject: m_pGameObjects)
	{
		if (pObject != nullptr)
		{
			pObject->Initialize(m_SceneData);
		}
	}
	m_NeedsInit = false;
}
void Scene::Update(float elapsed)
{
	//UpdateFirst sceneData
	m_SceneData.Update(elapsed);
	
	//UpdateFirst all
	for (GameObject* pObject: m_pGameObjects)
	{
		if (pObject != nullptr)
		{
			pObject->UpdateFirst(m_SceneData);
		}
	}
	//Intialize if new things appeared
	if (m_NeedsInit)
	{
		Initialize();
	}

	//UpdateFirst all
	for (GameObject* pObject : m_pGameObjects)
	{
		if (pObject != nullptr)
		{
			pObject->UpdateSecond(m_SceneData);
		}
	}
	//Intialize if new things appeared
	if (m_NeedsInit)
	{
		Initialize();
	}

	//Remove all nullptr from vector (if EG deleted)
	m_pGameObjects.erase(std::remove_if(m_pGameObjects.begin(), m_pGameObjects.end(), [](const GameObject* pObject)
	{
		return pObject == nullptr;
	}), m_pGameObjects.end());
}
void Scene::Render() const
{
	m_SceneData.pRenderManager->Render();
}



const SceneData& Scene::GetSceneData() const
{
	return m_SceneData;
}
const std::string& Scene::GetName() const
{
	return m_Name;
}

void Scene::SetInitialize()
{
	m_NeedsInit = true;
}



