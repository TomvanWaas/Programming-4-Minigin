#include "MiniginPCH.h"
#include "DigDugApplication.h"
#include "GameFiles.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "InputManager.h"
#include "DigDugGridComponent.h"
#include "TransformComponent.h"
#include "Components.h"
#include "ResourceManager.h"

using namespace DigDug;

void DigDugApplication::Initialize(SceneManager& sceneManager)
{
	auto pGameScene = sceneManager.CreateScene("GameScene");

	DigDugSettings settings;
	settings.spriteHeight = 16;
	settings.spriteWidth = 16;
	settings.spriteSpeed = 0.25f;
	settings.spriteTexture = ResourceManager::GetInstance().LoadTexture("DigDugSprites_Usable.png");
	

	//Background
	auto pBackgroundObject = MakeBackground(*pGameScene);
	settings.pGrid = pBackgroundObject->GetComponent<DigDugGridComponent>(true);
	if (settings.pGrid == nullptr) return;

	//Player Controls
	InputAction moveLeft{ InputTriggerState::Down, nullptr, -1, 'A', -1 };
	InputAction moveRight{ InputTriggerState::Down, nullptr, -1, 'D', -1 };
	InputAction moveUp{ InputTriggerState::Down, nullptr, -1, 'W', -1 };
	InputAction moveDown{ InputTriggerState::Down, nullptr, -1, 'S', -1 };
	InputAction throwLasso{ InputTriggerState::Down, nullptr, -1, 'E', -1 };



	auto pPlayer = MakePlayer(*pGameScene, moveLeft, moveRight, moveUp, moveDown, throwLasso, settings);
	auto pPlayerTransform = pPlayer->GetComponent<TransformComponent>();
	pPlayerTransform->SetWorldPosition(settings.pGrid->GetGameObject().GetComponent<TransformComponent>()->GetWorldPosition());
	
}
