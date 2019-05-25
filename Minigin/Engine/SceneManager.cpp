#include "stdafx.h"
#include "SceneManager.h"
#include "Scene.h"
#include <algorithm>
#include "Logger.h"
#include "Deletor.h"
#include "RenderManager.h"

using namespace Minigin;

SceneManager::SceneManager()
	: m_pScenes()
	, m_pActiveScene(nullptr)
{
}
SceneManager::~SceneManager()
{
	for (Scene* pScene : m_pScenes)
	{
		SAFE_DELETE(pScene);
	}
	m_pScenes.clear();
	m_pActiveScene = nullptr;
}

void SceneManager::Initialize()
{
	for (auto pScene : m_pScenes)
	{
		if (pScene) pScene->Initialize();
	}
}
void SceneManager::Update(float deltaTime)
{
	//Reload?
	if (m_NeedsReload)
	{
		m_NeedsReload = false;

		if (m_pActiveScene)
		{
			//Get Scenes
			Scene* pOldScene = m_pActiveScene;
			Scene* pNewScene = m_pActiveScene->OnReload();

			if (pOldScene != pNewScene)
			{
				if (pNewScene) pNewScene->SetSceneManager(this);

				//Set In Vector
				for (Scene*& pScene : m_pScenes)
				{
					if (pScene == pOldScene) pScene = pNewScene;
				}

				//Init New
				if (pNewScene) pNewScene->Initialize();

				//Enter/Exit
				if (pOldScene) pOldScene->Exit(pNewScene);
				if (pNewScene) pNewScene->Enter(pOldScene);

				//Delete Old
				Deletor::GetInstance().StoreDelete<Scene>(pOldScene);

				m_pActiveScene = pNewScene;
			}
		}		
	}

	//Update scene
	if (m_pActiveScene) m_pActiveScene->Update(deltaTime);
}

void SceneManager::FixedUpdate(float fixedDeltaTime)
{
	if (m_pActiveScene) m_pActiveScene->FixedUpdate(fixedDeltaTime);
}

void SceneManager::Render() const
{
	if (m_pActiveScene) m_pActiveScene->Render();
	else RenderManager::ClearScreen();
}

Scene* SceneManager::GetActiveScene() const
{
	return m_pActiveScene;
}

Scene* SceneManager::GetPreviousScene() const
{
	for (size_t i = 0, j = m_pScenes.size(); i < j; ++i)
	{
		if (m_pScenes[i] == m_pActiveScene)
		{
			int idx = (int(i) - 1);
			if (idx < 0) idx = int(j - 1);
			return m_pScenes[idx];
		}
	}
	return nullptr;
}

Scene* SceneManager::GetNextScene() const
{
	for (size_t i = 0, j = m_pScenes.size(); i < j; ++i)
	{
		if (m_pScenes[i] == m_pActiveScene)
		{
			return m_pScenes[(i + 1) % j];
		}
	}
	return nullptr;
}

void SceneManager::SetActiveScene(const std::string& name)
{
	for (size_t i = 0, j = m_pScenes.size(); i < j; ++i)
	{
		if (m_pScenes[i] && m_pScenes[i]->GetName() == name)
		{
			SetActiveScene(m_pScenes[i]);
			return;
		}
	}
	SetActiveScene(nullptr);
}
void SceneManager::SetActiveScene(Scene* pScene)
{
	if (!pScene)
	{
		Logger::GetInstance().LogInfo("SceneManager::SetActiveScene > Scene set to nullptr");
		if (m_pActiveScene) m_pActiveScene->Exit(pScene);
		m_pActiveScene = pScene;
		return;
	}

	//Find In Vector (add if necessary)
	auto i = std::find(m_pScenes.begin(), m_pScenes.end(), pScene);
	if (i == m_pScenes.end() && AddScene(pScene))
	{
		Logger::GetInstance().LogWarning("SceneManager::SetActiveScene > Added scene as it was not in vector");
		if (m_pActiveScene) m_pActiveScene->Exit(pScene);
		if (pScene) (pScene)->Enter(m_pActiveScene);
		m_pActiveScene = pScene;
	}
	else if (i != m_pScenes.end())
	{
		if (m_pActiveScene) m_pActiveScene->Exit(pScene);
		if (pScene) (pScene)->Enter(m_pActiveScene);
		m_pActiveScene = pScene;
	}
}
void SceneManager::SetActiveScene(size_t idx)
{
	if (m_pScenes.size() > idx)
	{
		SetActiveScene(m_pScenes[idx]);
		return;
	}
	Logger::GetInstance().LogWarning("SceneManager::SetActiveScene > Index out of bounds resulted in Active Scene = nullptr");
	SetActiveScene(nullptr);
}


void SceneManager::NextScene()
{
	SetActiveScene(GetNextScene());
}
void SceneManager::PreviousScene()
{
	SetActiveScene(GetPreviousScene());
}

void SceneManager::ReloadScene()
{
	m_NeedsReload = true;
}


bool SceneManager::AddScene(Scene* pScene)
{
	if (pScene == nullptr) return false;
	auto i = std::find_if(m_pScenes.begin(), m_pScenes.end(), [&pScene](const Scene* pOther)
	{
		return (pOther && pOther->GetName() == pScene->GetName());
	});
	if (i != m_pScenes.end())
	{
		Logger::GetInstance().LogWarning("SceneManager::AddScene > Failed to add scene '" + pScene->GetName() + "' as it already exists.\n");
		return false;
	}
	m_pScenes.push_back(pScene);
	pScene->SetSceneManager(this);
	return true;
}

Scene* SceneManager::GetScene(const std::string& name) const
{
	auto i = std::find_if(m_pScenes.begin(), m_pScenes.end(), [&name](const Scene* pScene)
	{
		return (pScene && pScene->GetName() == name);
	});
	if (i != m_pScenes.end())
	{
		return (*i);
	}
	return nullptr;
}


