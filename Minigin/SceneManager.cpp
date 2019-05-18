#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"
#include <algorithm>
#include "Logger.h"
#include "Deletor.h"

SceneManager::SceneManager()
	: m_pScenes()
	, m_ActiveSceneId(0)
{
}
SceneManager::~SceneManager()
{
	for (Scene* pScene : m_pScenes)
	{
		SAFE_DELETE(pScene);
	}
	m_pScenes.clear();
}

void SceneManager::Initialize()
{
	if (m_pScenes.size() <= m_ActiveSceneId) return;
	for (auto pScene : m_pScenes)
	{
		pScene->Initialize();
	}
}
void SceneManager::Update(float deltaTime)
{

	//Reload?
	if (m_NeedsReload)
	{
		m_NeedsReload = false;
		if (m_pScenes.size() > m_ActiveSceneId && m_pScenes[m_ActiveSceneId])
		{
			auto* pScene = m_pScenes[m_ActiveSceneId];
			auto* pNew = pScene->GetNew();
			Deletor::GetInstance().StoreDelete<Scene>(pScene);
			m_pScenes[m_ActiveSceneId] = pNew;
			if (pNew)
			{
				pNew->SetSceneManager(this);
				pNew->Initialize();
			}
		}
	}

	//Valid Scene?
	if (m_pScenes.size() <= m_ActiveSceneId) return;
	//Update scene
	if (m_pScenes[m_ActiveSceneId] != nullptr)
	{
		m_pScenes[m_ActiveSceneId]->Update(deltaTime);
	}
}
void SceneManager::Render() const
{
	if (m_pScenes.size() <= m_ActiveSceneId) return;

	if (m_pScenes[m_ActiveSceneId] != nullptr)
	{
		m_pScenes[m_ActiveSceneId]->Render();
	}
}

Scene* SceneManager::GetActiveScene() const
{
	if (m_pScenes.size() > m_ActiveSceneId)
	{
		return m_pScenes[m_ActiveSceneId];
	}
	return nullptr;
}


void SceneManager::SetActiveScene(const std::string& name)
{
	for (size_t i{0}; i < m_pScenes.size(); ++i)
	{
		if (m_pScenes[i] != nullptr && m_pScenes[i]->GetName() == name)
		{
			m_ActiveSceneId = i;
			return;
		}
	}
}
void SceneManager::NextScene()
{
	++m_ActiveSceneId;
	if (m_ActiveSceneId >= m_pScenes.size()) m_ActiveSceneId = 0;
}
void SceneManager::PreviousScene()
{
	--m_ActiveSceneId;
	if (m_ActiveSceneId >= m_pScenes.size()) m_ActiveSceneId = m_pScenes.size() - 1;
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


