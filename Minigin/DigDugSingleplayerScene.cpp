#include "MiniginPCH.h"
#include "DigDugSingleplayerScene.h"
#include "LevelData.h"
#include "ResourceManager.h"
#include "LevelLoader.h"
#include "ObservedEvent.h"
#include "ObservedData.h"
#include "GameFiles.h"
#include "WindowSettings.h"
#include "Minigin.h"
#include "DigDugGridComponent.h"
#include "GameObject.h"
#include "EnemyManager.h"
#include "PlayerManager.h"
#include "GameEvents.h"
#include "FiniteStateMachineComponent.h"
#include "SceneManager.h"
#include "RenderComponent.h"
#include "ObserverManager.h"
#include "ScoreObserver.h"
#include "InputManager.h"
#include "GameInputCommands.h"
#include "DigDugEnemyComponent.h"
#include "DigDugPlayerComponent.h"

DigDugSingleplayerScene::DigDugSingleplayerScene(const std::string& name, const std::string& levelFile, int numLives, int levelNumber)
	: Scene(name)
	, m_pLevelData(ResourceManager::GetInstance().GetLoader<LevelData>()->GetResource(ResourceManager::GetInstance().GetBasePath() + levelFile))
	, m_LevelFile(levelFile)
	, m_LevelNumber(levelNumber)
	, m_EnemyCount(0)
{
	m_PlayerData.pLifeObjects.resize(numLives);
}




void DigDugSingleplayerScene::SceneInitialize()
{
#pragma region Settings
	Vector2 scale{ 2.0f, 2.0f };
	const WindowSettings& wsettings = dae::Minigin::GetWindowSettings();

	//Settings
	m_GameSettings.spriteHeight = 16;
	m_GameSettings.spriteWidth = 16;
	m_GameSettings.spriteSpeed = 0.25f;

	m_GameSettings.pBackgroundTexture = ResourceManager::GetInstance().LoadTexture("background.png");
	m_GameSettings.pDigDugTexture = ResourceManager::GetInstance().LoadTexture("Sprites_DigDug.png");
	m_GameSettings.pFygarTexture = ResourceManager::GetInstance().LoadTexture("Sprites_Fygar.png");
	m_GameSettings.pOtherTexture = ResourceManager::GetInstance().LoadTexture("Sprites_Other.png");
	m_GameSettings.pPookaTexture = ResourceManager::GetInstance().LoadTexture("Sprites_Pooka.png");
	m_GameSettings.pPumpTexture = ResourceManager::GetInstance().LoadTexture("Sprites_Pump.png");
	m_GameSettings.pScoreFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	m_GameSettings.gridHeight = m_pLevelData->size.second;
	m_GameSettings.gridWidth = m_pLevelData->size.first;
	m_GameSettings.gridOffset = Vector2{ (wsettings.width - 25) / m_pLevelData->size.first, (wsettings.height - 25) / m_pLevelData->size.second };
#pragma endregion Settings


	//Managers
	GetSceneData().AddManager(new EnemyManager());
	GetSceneData().AddManager(new PlayerManager());
	GetSceneData().AddManager(new ObserverManager());



	//Score Counter
	auto pScoreObserver = new ScoreObserver(m_GameSettings.pScoreFont, Color4(), this);
	auto pScoreObject = DigDug::CreateScoreManager(*this, m_GameSettings.pScoreFont, Color4(), pScoreObserver);
	pScoreObject->GetTransform().SetWorldPosition(wsettings.width, wsettings.height);


	//Background & Grid
	auto pBackgroundObject = MakeBackground(*this, m_GameSettings);
	m_GameSettings.pGrid = pBackgroundObject->GetComponentInChildren<DigDugGridComponent>();
	if (m_GameSettings.pGrid == nullptr)
	{
		Logger::GetInstance().LogError("DigDugSingleplayerScene::SceneInitialize > Failed to make grid");
		return;
	}
	pBackgroundObject->GetTransform().SetLocalPosition(dae::Minigin::GetWindowSettings().width*0.5f, dae::Minigin::GetWindowSettings().height*0.5f);

	//LevelData
	m_GameSettings.pGrid->Initialize(GetSceneData());
	if (m_GameSettings.pGrid)
	{
		//Marks
		for (const auto& p : m_pLevelData->marked)
		{
			m_GameSettings.pGrid->Mark(m_GameSettings.pGrid->GetPosition(p.first, p.second));
		}
		//Obstacles
		for (const auto& p : m_pLevelData->obstacle)
		{
			auto* pObstacle = CreateObstacle(*this, m_GameSettings, pScoreObserver);
			pObstacle->GetTransform().SetWorldPosition(m_GameSettings.pGrid->GetPosition(p.first, p.second));
			pObstacle->GetTransform().SetWorldScale(scale);
		}
		//Pookas
		for (const auto& p : m_pLevelData->pooka)
		{
			auto* pPooka = CreatePooka(*this, m_GameSettings, pScoreObserver);
			pPooka->GetTransform().SetWorldPosition(m_GameSettings.pGrid->GetPosition(p.first, p.second));
			pPooka->GetTransform().SetWorldScale(scale);
			++m_EnemyCount;
		}
		//Fygars
		for (const auto& p : m_pLevelData->fygar)
		{
			auto* pFygar = CreateFygar(*this, m_GameSettings, pScoreObserver);
			pFygar->GetTransform().SetWorldPosition(m_GameSettings.pGrid->GetPosition(p.first, p.second));
			pFygar->GetTransform().SetWorldScale(scale);
			++m_EnemyCount;
		}
	}


	//Make Player
	auto* pPlayer = MakePlayer();
	m_PlayerData.startPosition = m_GameSettings.pGrid->GetPosition(m_pLevelData->size.first / 4 * 2, 0);
	pPlayer->GetTransform().SetWorldPosition(m_PlayerData.startPosition);
	pPlayer->GetTransform().SetWorldScale(scale);

	//Make Lives
	for (int i = 0; i < m_PlayerData.pLifeObjects.size(); ++i)
	{
		auto* pObj = CreateGameObject();
		auto* pRender = new RenderComponent(m_GameSettings.pOtherTexture);
		pRender->SetRenderPriority(-26, GetSceneData());
		pRender->SetSource(Rect{ 4 * m_GameSettings.spriteWidth, 0, m_GameSettings.spriteWidth, m_GameSettings.spriteHeight });
		pObj->AddComponent(pRender);
		auto& t = pObj->GetTransform();
		t.SetWorldScale(scale);
		t.SetWorldPosition( ((i+0.5f)*m_GameSettings.spriteWidth)*scale.x, (wsettings.height - 0.5f*m_GameSettings.spriteHeight*scale.y));
		m_PlayerData.pLifeObjects[i] = pObj;
	}
	//Make Flowers (LevelNumber)
	for (int i = 0; i < m_LevelNumber; ++i)
	{
		auto* pObj = CreateGameObject();
		auto* pRender = new RenderComponent(m_GameSettings.pOtherTexture);
		pRender->SetRenderPriority(-26, GetSceneData());
		pRender->SetSource(Rect{ 5 * m_GameSettings.spriteWidth, 0, m_GameSettings.spriteWidth, m_GameSettings.spriteHeight });
		pObj->AddComponent(pRender);
		auto& t = pObj->GetTransform();
		t.SetWorldScale(scale);
		t.SetWorldPosition(wsettings.width - ((i + 0.5f)*m_GameSettings.spriteWidth)*scale.x, (m_GameSettings.spriteHeight*2.5f)*scale.y);
	}


	//Make MenuInput
	InputAction menu{ InputTriggerState::Pressed, std::make_shared<DigDug::SceneSetInput>("Pause", GetSceneManager()), -1, VK_ESCAPE, int(GamepadCode::GAMEPAD_START), 0 };
	GetSceneData().GetInput()->AddInputAction(menu);

}


void DigDugSingleplayerScene::SceneNotify(ObservedEvent event, const ObservedData& data)
{
	UNREFERENCED_PARAMETER(event);
	UNREFERENCED_PARAMETER(data);

	switch (event)
	{
		//Dead Player
	case GameEvent::PlayerDied:
	{
		//Get Object
		GameObject* pPlayer = nullptr;
		if (data.GetData<GameObject*>("GameObject", pPlayer) && pPlayer)
		{
			Logger::GetInstance().LogInfo("DigDugSingleplayerScene::SceneNotify > Player Died");

			//Remove one lifepoint
			if (m_PlayerData.pLifeObjects.size() > 0)
			{
				auto pObj = m_PlayerData.pLifeObjects[m_PlayerData.pLifeObjects.size() - 1];
				if (pObj) GameObject::DeleteObject(pObj);
				m_PlayerData.pLifeObjects.erase(m_PlayerData.pLifeObjects.end() - 1);
			}
			m_PlayerData.pPlayer = nullptr;

			//Check if player is dead for ever => To Start
			if (m_PlayerData.pLifeObjects.size() == 0)
			{
				GetSceneManager()->SetActiveScene("Menu");
			}
			//Reset Scene
			else if (m_PlayerData.pPlayer == nullptr && m_PlayerData.pLifeObjects.size() > 0)
			{
				//Reset Player
				//
				Vector2 scale{ 2,2 };

				//
				m_PlayerData.pPlayer = MakePlayer();


				if (m_PlayerData.pPlayer)
				{
					m_PlayerData.pPlayer->GetTransform().SetWorldPosition(m_PlayerData.startPosition);
					m_PlayerData.pPlayer->GetTransform().SetWorldScale(scale);
				}
				Logger::GetInstance().LogInfo("DigDugMultiplayerScene::SceneUpdate > Player Respawned");

				//Reset Enemy positions
				auto pEnemyManager = GetSceneData().GetManager<EnemyManager>();
				if (pEnemyManager)
				{
					auto enemies = pEnemyManager->GetEnemies();
					for (const auto& enemy : enemies)
					{
						if (enemy.second)
						{
							enemy.second->GetTransform().SetWorldPosition(enemy.first);
						}
					}
				}
			}
		}
	}
	break;

	//Dead enemy
	case ObservedEvent::Destroyed:
	{
		GameObject* pObject = nullptr;
		if (data.GetData<GameObject*>("GameObject", pObject) && pObject)
		{
			auto pEnemy = pObject->GetComponent<DigDugEnemyComponent>();
			if (pEnemy)
			{
				--m_EnemyCount;
				//No more enemies? => Next Level
				if (m_EnemyCount <= 0)
				{
					auto* pNext = GetSceneManager()->GetNextScene();
					if (pNext)
					{
						//Check name
						std::string name = "Singleplayer";
						auto valid = true;
						for (size_t i = 0; i < name.size(); ++i)
						{
							valid = valid && pNext->GetName()[i] == name[i];
						}
						if (valid)
						{
							GetSceneManager()->NextScene();
						}
						else
						{
							GetSceneManager()->SetActiveScene("Menu");
						}
					}
					else
					{
						GetSceneManager()->SetActiveScene("Menu");
					}
				}
				Logger::GetInstance().LogInfo("DigDugSingleplayerScene::SceneNotify > Enemy died: Remaining = " + std::to_string(m_EnemyCount));
			}
		}
	}
	break;
	}
}

Scene* DigDugSingleplayerScene::OnReload() const
{
	return new DigDugSingleplayerScene(GetName(), m_LevelFile, 3, m_LevelNumber);
}

GameObject* DigDugSingleplayerScene::MakePlayer()
{
	//Make Player
	InputAction up{ InputTriggerState::Pressed, nullptr, -1, 'W', int(GamepadCode::GAMEPAD_RIGHTSTICK_UP), 0 };
	InputAction down{ InputTriggerState::Pressed, nullptr, -1, 'S', int(GamepadCode::GAMEPAD_RIGHTSTICK_DOWN), 0 };
	InputAction left{ InputTriggerState::Pressed, nullptr, -1, 'A', int(GamepadCode::GAMEPAD_RIGHTSTICK_LEFT), 0 };
	InputAction right{ InputTriggerState::Pressed, nullptr, -1, 'D', int(GamepadCode::GAMEPAD_RIGHTSTICK_RIGHT), 0 };
	InputAction pump{ InputTriggerState::Pressed, nullptr, -1, 'E', int(GamepadCode::GAMEPAD_RIGHT_THUMB) , 0 };


	auto* pPlayer = DigDug::CreatePlayer(*this, m_GameSettings, up, down, left, right, pump, 0);
	return pPlayer;
}

