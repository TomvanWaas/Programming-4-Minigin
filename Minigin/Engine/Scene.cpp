#include "stdafx.h"
#include "Scene.h"
#include "GameObject.h"
#include <algorithm>
#include "InputManager.h"
#include "CollisionManager.h"
#include "RenderManager.h"
#include "Deletor.h"

using namespace Minigin;

Scene::Scene(const std::string& name)
	: m_Name(name)
	, m_pGameObjects()
	, m_SceneData(this)
	, m_IsInitialized(false)
	, m_SceneScale(1, 1)
{
	m_SceneData.Initialize();
}
Scene::~Scene()
{
	for (GameObject* pObject : m_pGameObjects)
	{
		SAFE_DELETE(pObject);
	}
	m_pGameObjects.clear();
}



GameObject* Scene::CreateGameObject()
{
	auto pObject = new GameObject();
	AddGameObject(pObject);
	if (m_IsInitialized) pObject->Initialize(m_SceneData);
	return pObject;
}
bool Scene::DeleteGameObject(GameObject*& pObject)
{
	if (pObject == nullptr) return false;
	pObject->SetParent(nullptr);
	RemoveGameObject(pObject);	//If possible	
	pObject->Destroy(m_SceneData);
	Deletor::GetInstance().StoreDelete(pObject);
	pObject->SetEnabled(false);
	pObject = nullptr;
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
		if (m_IsInitialized) pObject->Initialize(m_SceneData);
		return true;
	}
	return false;
}



void Scene::Initialize()
{
	if (m_IsInitialized == false)
	{
		SceneInitialize();
		m_SceneData.Initialize();

		//InitializeOverride all
		for (GameObject* pObject : m_pGameObjects)
		{
			if (pObject != nullptr)
			{
				pObject->Initialize(m_SceneData);
			}
		}

		m_SceneData.LateInitialize();
		m_IsInitialized = true;
	}
}
void Scene::Update(float elapsed)
{
	//UpdateFirst sceneData
	m_SceneData.Update(elapsed);

	SceneUpdate();

	//UpdateFirst all
	for (size_t i = 0; i < m_pGameObjects.size(); ++i)
	{
		auto* pObject = m_pGameObjects[i];
		if (pObject != nullptr)
		{
			pObject->UpdateFirst(m_SceneData);
		}
	}

	//UpdateSecond all
	for (size_t i = 0; i < m_pGameObjects.size(); ++i)
	{
		auto* pObject = m_pGameObjects[i];
		if (pObject != nullptr)
		{
			pObject->UpdateSecond(m_SceneData);
		}
	}

	//Remove all nullptr from vector (if EG deleted)
	m_pGameObjects.erase(std::remove_if(m_pGameObjects.begin(), m_pGameObjects.end(), [](const GameObject* pObject)
	{
		return pObject == nullptr;
	}), m_pGameObjects.end());
}

void Scene::FixedUpdate(float fixedElapsed)
{
	m_SceneData.UpdateFixed(fixedElapsed);
	SceneUpdateFixed();

	//Fixed Update all
	for (size_t i = 0; i < m_pGameObjects.size(); ++i)
	{
		auto* pObject = m_pGameObjects[i];
		if (pObject != nullptr)
		{
			pObject->UpdateFixed(m_SceneData);
		}
	}

}

void Scene::Render() const
{
	m_SceneData.GetRender()->Render(m_SceneScale);
}



const SceneData& Scene::GetSceneData() const
{
	return m_SceneData;
}


const std::string& Scene::GetName() const
{
	return m_Name;
}

void Scene::SceneNotify(ObservedEvent event, const ObservedData& data)
{
	UNREFERENCED_PARAMETER(event); UNREFERENCED_PARAMETER(data);
}


void Scene::NotifyAll(ObservedEvent event, const ObservedData& data)
{
	SceneNotify(event, data);
	m_SceneData.Notify(event, data);
	for (GameObject* pObject : m_pGameObjects)
	{
		if (pObject) pObject->Notify(event, data);
	}
}

void Scene::Notify(ObservedEvent event, const ObservedData& data)
{
	SceneNotify(event, data);
	m_SceneData.Notify(event, data);
}



