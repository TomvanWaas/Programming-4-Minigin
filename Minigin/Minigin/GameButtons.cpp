#include "MiniginPCH.h"
#include "GameButtons.h"
#include "SceneManager.h"
#include "Scene.h"
#include "InputManager.h"

using namespace Minigin;
using namespace DigDug;

SceneSwapButton::SceneSwapButton(SceneManager& sceneManager, const std::string& name, bool reload )
	: ButtonAction()
	, m_pSceneManager(&sceneManager)
	, m_Reload(reload)
	, m_SceneName(name)
{
}

SceneSwapButton::SceneSwapButton(SceneManager& sceneManager, Scene& scene, bool reload)
	: m_pSceneManager(&sceneManager)
	, m_Reload(reload)
	, m_SceneName(scene.GetName())
{
}
void DigDug::SceneSwapButton::Execute()
{
	if (m_pSceneManager)
	{
		m_pSceneManager->SetActiveScene(m_SceneName);
		if (m_Reload) m_pSceneManager->ReloadScene();
	}
}

void QuitButton::Execute()
{
	InputManager::StaticQuit();
}
