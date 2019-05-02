#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"
#include <algorithm>
#include "InputManager.h"
#include "CollisionManager.h"
#include "RenderManager.h"

Scene::Scene(const std::string& name)
	: m_Name(name)
	, m_pGameObjects()
	, m_SceneData()
	, m_NeedsInit(true)
{
}
Scene::~Scene()
{
	for (GameObject* pObject : m_pGameObjects)
	{
		SAFE_DELETE(pObject);
	}
	m_pGameObjects.clear();
	for (GameObject* pObject : m_pMarkedForDelete)
	{
		SAFE_DELETE(pObject);
	}
	m_pMarkedForDelete.clear();

	SAFE_DELETE(m_SceneData.pInput);
	SAFE_DELETE(m_SceneData.pCollisionManager);
	SAFE_DELETE(m_SceneData.pRenderManager);
}

void Scene::InitSceneData(const SceneData& sceneData)
{
	m_SceneData.Initialize(sceneData);
}



GameObject* Scene::CreateGameObject()
{
	GameObject* pObject = new GameObject();
	AddGameObject(pObject);
	return pObject;
}
bool Scene::DeleteGameObject(GameObject* pObject)
{
	if (pObject == nullptr) return false;
	pObject->SetParent(nullptr);
	RemoveGameObject(pObject);
	m_pMarkedForDelete.push_back(pObject);
	pObject->Destroy(m_SceneData);
	pObject->SetEnabled(false);
	return true;
}
bool Scene::RemoveGameObject(GameObject* pObject)
{
	if (pObject == nullptr) return false;
	auto i = std::find(m_pGameObjects.begin(), m_pGameObjects.end(), pObject);
	if (i != m_pGameObjects.end())
	{
		(*i) = nullptr;
	}
	return false;
}
bool Scene::AddGameObject(GameObject* pObject)
{
	auto i = std::find(m_pGameObjects.begin(), m_pGameObjects.end(), pObject);
	if (i == m_pGameObjects.end() && pObject != nullptr)
	{
		m_pGameObjects.push_back(pObject);
		pObject->SetScene(this);
		return true;
	}
	return false;
}



void Scene::Initialize()
{
	//InitializeOverride all
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
	//UpdateFirstOverride sceneData
	m_SceneData.Update(elapsed);
	
	//UpdateFirstOverride all
	for (GameObject* pObject: m_pGameObjects)
	{
		if (pObject != nullptr)
		{
			pObject->UpdateFirst(m_SceneData);
		}
	}

	//UpdateFirstOverride all
	for (GameObject* pObject : m_pGameObjects)
	{
		if (pObject != nullptr)
		{
			pObject->UpdateSecond(m_SceneData);
		}
	}

	//If Deleted
	if (m_pMarkedForDelete.size() > 0)
	{
		for (GameObject* pObject : m_pMarkedForDelete)
		{
			SAFE_DELETE(pObject);
		}
		m_pMarkedForDelete.clear();
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



