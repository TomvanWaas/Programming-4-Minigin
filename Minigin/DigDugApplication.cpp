#include "MiniginPCH.h"
#include "DigDugApplication.h"
#include "GameFiles.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "InputManager.h"
#include "DigDugGridComponent.h"
#include "Transform.h"
#include "Components.h"
#include "ResourceManager.h"
#include "WindowSettings.h"
#include "Scene.h"
#include "PlayerManager.h"
#include "EnemyManager.h"
#include "Button.h"
#include "GameButtons.h"
#include "GameInputCommands.h"
#include "ScreenRenderComponent.h"
#include "LevelLoader.h"
#include "DigDugSingleplayerScene.h"
#include "DigDugMenuScene.h"

using namespace DigDug;

#include "ObservedData.h"
#include "AABBCollisionComponent.h"
#include "SpriteCollisionComponent.h"
void DigDugApplication::Initialize(SceneManager& sceneManager, const WindowSettings& windowSettings)
{
	UNREFERENCED_PARAMETER(windowSettings);
	auto* pGameScene = CreateGameScene(sceneManager, windowSettings);
	auto* pMenuScene = CreateMenuScene(sceneManager, windowSettings);
	sceneManager.SetActiveScene("Menu");
	UNREFERENCED_PARAMETER(pMenuScene);
	UNREFERENCED_PARAMETER(pGameScene);
}



Scene* DigDugApplication::CreateMenuScene(SceneManager& sceneManager, const WindowSettings& windowSettings)
{
	UNREFERENCED_PARAMETER(windowSettings);

	auto pScene = new DigDugMenuScene("Menu");
	sceneManager.AddScene(pScene);
	return pScene;
}

Scene* DigDugApplication::CreateGameScene(SceneManager& sceneManager, const WindowSettings& windowSettings)
{
	UNREFERENCED_PARAMETER(windowSettings);

	auto pScene = new DigDugSingleplayerScene("SingleplayerScene", "Level.txt");
	sceneManager.AddScene(pScene);
	return pScene;
}
