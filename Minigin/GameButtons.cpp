#include "MiniginPCH.h"
#include "GameButtons.h"
#include "SceneManager.h"
#include "Scene.h"
#include "InputManager.h"

using namespace DigDug;

SceneSwapButton::SceneSwapButton(SceneManager& sceneManager, const std::string& name)
	: ButtonAction()
	, m_pSceneManager(&sceneManager)
	, m_SceneName(name)
{
}
SceneSwapButton::SceneSwapButton(SceneManager& sceneManager, Scene& scene)
	: SceneSwapButton(sceneManager, scene.GetName())
{
}
void DigDug::SceneSwapButton::Execute()
{
	if (m_pSceneManager)
	{
		m_pSceneManager->SetActiveScene(m_SceneName);
	}
}

void QuitButton::Execute()
{
	InputManager::Quit();
}
