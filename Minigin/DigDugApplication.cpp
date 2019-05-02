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

using namespace DigDug;

void DigDugApplication::Initialize(SceneManager& sceneManager, const WindowSettings& windowSettings)
{
	UNREFERENCED_PARAMETER(windowSettings);
	UNREFERENCED_PARAMETER(sceneManager);

	Vector2 objectScale(2.0f, 2.0f);

	auto pGameScene = sceneManager.CreateScene("GameScene");
	UNREFERENCED_PARAMETER(pGameScene);

	DigDugSettings settings;
	settings.spriteHeight = 16;
	settings.spriteWidth = 16;
	settings.spriteSpeed = 0.25f;
	settings.spriteTexture = ResourceManager::GetInstance().LoadTexture("DigDugSprites_Usable.png");
	settings.backgroundTexture = ResourceManager::GetInstance().LoadTexture("background.png");
	settings.cavesTexture = ResourceManager::GetInstance().LoadTexture("Cave.png");
	

	//Background
	auto pBackgroundObject = MakeBackground(*pGameScene, settings);
	settings.pGrid = pBackgroundObject->GetComponentInChildren<DigDugGridComponent>();
	if (settings.pGrid == nullptr) return;
	pBackgroundObject->GetTransform().SetLocalPosition(windowSettings.width*0.5f, windowSettings.height*0.5f);


	//Stone TEST
	for (int w = 4; w <= 16; w+=2)
	{
		auto pStone = MakeStone(*pGameScene, settings);
		pStone->GetTransform().SetWorldPosition(settings.pGrid->GetPosition(w, 4));
		pStone->GetTransform().SetWorldScale(objectScale);
	}


	//Make player
	auto pPlayer = MakePlayerObject(*pGameScene, settings, 'W', 'S', 'A', 'D', 'E');
	auto& pPlayerTransform = pPlayer->GetTransform();
	pPlayerTransform.SetWorldPosition(settings.pGrid->GetPosition(2,2));
	pPlayerTransform.SetWorldScale(objectScale);
}
