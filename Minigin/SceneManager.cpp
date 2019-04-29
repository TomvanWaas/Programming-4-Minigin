#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"
#include <algorithm>
#include "Logger.h"
#include "Time.h"

SceneManager::SceneManager()
	: m_pScenes()
	, m_ActiveSceneId(0)
//Init sceneData values
	, m_pTime(new Time())
{
}

SceneManager::~SceneManager()
{
	for (Scene* pScene : m_pScenes)
	{
		SAFE_DELETE(pScene);
	}
	m_pScenes.clear();

	//Delete sceneData values
	SAFE_DELETE(m_pTime);
}

void SceneManager::Initialize()
{
	for (auto pScene : m_pScenes)
	{
		pScene->Initialize();
	}
}
void SceneManager::Update(float deltaTime)
{
	//Update scene
	if (m_pScenes[m_ActiveSceneId] != nullptr)
	{
		m_pScenes[m_ActiveSceneId]->Update(deltaTime);
	}
}
void SceneManager::Render() const
{
	if (m_pScenes[m_ActiveSceneId] != nullptr)
	{
		m_pScenes[m_ActiveSceneId]->Render();
	}
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

	//Cannot go negative, if so => Huge number
	if (m_ActiveSceneId >= m_pScenes.size()) m_ActiveSceneId = m_pScenes.size() - 1;
}


Scene* SceneManager::CreateScene(const std::string& name)
{
	if (m_pScenes.end() != std::find_if(m_pScenes.begin(), m_pScenes.end(), [name](Scene* pScene)
	{
		return (pScene != nullptr && pScene->GetName() == name);
	}))
	{
		Logger::GetInstance().LogWarning("Failed to create scene '" + name + "' as it already exists.\n");
		return nullptr;
	}

	Scene* pScene = new Scene(name, m_pTime);
	m_pScenes.push_back(pScene);
	return pScene;
}

