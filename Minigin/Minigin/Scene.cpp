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
	m_SceneData.pInput = new InputManager();
	m_SceneData.pCollisionManager = new CollisionManager();
	m_SceneData.pRenderManager = new RenderManager{};
	m_SceneData.pTime = pTime;
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

void Scene::AddChild(GameObject* pObject)
{
	auto i = std::find(m_pGameObjects.begin(), m_pGameObjects.end(), pObject);
	
	if (i == m_pGameObjects.end())
	{
		m_pGameObjects.push_back(pObject);
		pObject->SetScene(this);
	}
}
void Scene::RemoveChild(GameObject* pObject)
{
	auto i = std::find(m_pGameObjects.begin(), m_pGameObjects.end(), pObject);

	if (i != m_pGameObjects.end())
	{
		m_pGameObjects.erase(i);
		pObject->SetScene(nullptr);
	}
}

void Scene::Initialize()
{
	for (GameObject* pObject : m_pGameObjects)
	{
		if (pObject == nullptr) continue;
		pObject->Initialize(m_SceneData);
	}
}

void Scene::Update()
{
	//Update sceneData
	m_SceneData.pInput->UpdateActions();
	m_SceneData.pRenderManager->Update();

	//Update scene
	for(auto* gameObject : m_pGameObjects)
	{
		if (gameObject == nullptr) continue;
		gameObject->Update(m_SceneData);
	}
}

void Scene::Render() const
{
	m_SceneData.pRenderManager->Render();
}

const SceneData& Scene::GetSceneData() const
{
	return m_SceneData;
}
