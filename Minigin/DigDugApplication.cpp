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
	auto* pScene = sceneManager.CreateScene("Menu");

	auto* pButtonManager = new ButtonManager();
	pScene->GetSceneData().AddManager(pButtonManager);

	auto pFontNone = ResourceManager::GetInstance().LoadFont("DIGDUG.ttf", 50);
	auto pFontHover = ResourceManager::GetInstance().LoadFont("DIGDUG.ttf", 75);
	SDL_Color colorNone = { 255,0,0,255 };
	SDL_Color colorHover = { 255,127,0,0 };

	//Make Buttons
	//
	auto* pPlayAloneButton = pScene->CreateGameObject();
	{
		auto* pButton = new ButtonComponent();
		pPlayAloneButton->AddComponent(pButton);
		pButton->SetStateNone(pFontNone, colorNone);
		pButton->SetStateHover(pFontHover, colorHover);
		auto* pText = new TextComponent();
		pText->SetText("Play - One player");
		pPlayAloneButton->AddComponent(pText);
		pPlayAloneButton->AddComponent(new RenderComponent());
		pButton->SetAction(std::make_shared<SceneSwapButton>(sceneManager, "SingleplayerScene"));
	}
	//
	auto* pPlayTogetherButton = pScene->CreateGameObject();
	{
		auto* pButton = new ButtonComponent();
		pPlayTogetherButton->AddComponent(pButton);
		pButton->SetStateNone(pFontNone, colorNone);
		pButton->SetStateHover(pFontHover, colorHover);
		auto* pText = new TextComponent();
		pText->SetText("Play - Two players");
		pPlayTogetherButton->AddComponent(pText);
		pPlayTogetherButton->AddComponent(new RenderComponent());
		pButton->SetAction(std::make_shared<SceneSwapButton>(sceneManager, "MultiplayerScene"));
	}
	//
	auto* pQuitButton = pScene->CreateGameObject();
	{
		auto* pButton = new ButtonComponent();
		pQuitButton->AddComponent(pButton);
		pButton->SetStateNone(pFontNone, colorNone);
		pButton->SetStateHover(pFontHover, colorHover);
		auto* pText = new TextComponent();
		pText->SetText("Quit");
		pQuitButton->AddComponent(pText);
		pQuitButton->AddComponent(new RenderComponent());
		pButton->SetAction(std::make_shared<QuitButton>());
	}


	//Position
	pPlayAloneButton->GetTransform().SetWorldPosition(windowSettings.width*0.5f, windowSettings.height*0.25f);
	pPlayTogetherButton->GetTransform().SetWorldPosition(windowSettings.width*0.5f, windowSettings.height*0.5f);
	pQuitButton->GetTransform().SetWorldPosition(windowSettings.width*0.5f, windowSettings.height*0.75f);

	//Make Input
	auto* input = pScene->GetSceneData().GetInput();

	InputAction next{};
	next.pCommand = std::make_shared<InputButtonSet>(pButtonManager, InputButtonSet::Action::Next);
	next.keyboardCode = VK_DOWN;
	InputAction prev{};
	prev.pCommand = std::make_shared<InputButtonSet>(pButtonManager, InputButtonSet::Action::Previous);
	prev.keyboardCode = VK_UP;
	InputAction select{};
	select.pCommand = std::make_shared<InputButtonSet>(pButtonManager, InputButtonSet::Action::Select);
	select.keyboardCode = VK_SPACE;

	input->AddInputAction(next);
	input->AddInputAction(prev);
	input->AddInputAction(select);

	//Background
	auto* pBackground = pScene->CreateGameObject();
	{
		auto* pRender = new ScreenRenderComponent();
		pRender->SetTexture("background.png");
		pBackground->AddComponent(pRender);
		pRender->SetRenderPriority(-10, pScene->GetSceneData());
	}

	return pScene;
}

Scene* DigDugApplication::CreateGameScene(SceneManager& sceneManager, const WindowSettings& windowSettings)
{
	Vector2 objectScale(2.0f, 2.0f);

	auto pGameScene = sceneManager.CreateScene("SingleplayerScene");

	//Settings
	DigDugSettings settings;
	settings.spriteHeight = 16;
	settings.spriteWidth = 16;
	settings.spriteSpeed = 0.25f;
	settings.spriteTexture = ResourceManager::GetInstance().LoadTexture("DigDugSprites_Usable.png");
	settings.backgroundTexture = ResourceManager::GetInstance().LoadTexture("background.png");
	settings.cavesTexture = ResourceManager::GetInstance().LoadTexture("Cave.png");

	//Managers
	auto* pPManager = new PlayerManager();
	auto* pEManager = new EnemyManager();
	pGameScene->GetSceneData().AddManager(pPManager);
	pGameScene->GetSceneData().AddManager(pEManager);


	//Background
	auto pBackgroundObject = MakeBackground(*pGameScene, settings);
	settings.pGrid = pBackgroundObject->GetComponentInChildren<DigDugGridComponent>();
	if (settings.pGrid == nullptr) return pGameScene;
	pBackgroundObject->GetTransform().SetLocalPosition(windowSettings.width*0.5f, windowSettings.height*0.5f);


	//Stone TEST
	for (int w = 4; w <= 16; w += 2)
	{
		auto pStone = CreateObstacle(*pGameScene, settings);
		pStone->GetTransform().SetWorldPosition(settings.pGrid->GetPosition(w, 4));
		pStone->GetTransform().SetWorldScale(objectScale);
	}


	//Make player
	auto pPlayer = MakePlayerObject(*pGameScene, settings, 'W', 'S', 'A', 'D', 'E');
	auto& pPlayerTransform = pPlayer->GetTransform();
	pPlayerTransform.SetWorldPosition(settings.pGrid->GetPosition(2, 2));
	pPlayerTransform.SetWorldScale(objectScale);
	pPManager->RegisterPlayer(pPlayer);


	auto* pPooka = CreatePooka(*pGameScene, settings);
	pPooka->GetTransform().SetWorldScale(objectScale);
	pPooka->GetTransform().SetWorldPosition(settings.pGrid->GetPosition(10, 10));
	pEManager->RegisterEnemy(pPooka);

	return pGameScene;
}
