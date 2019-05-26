#include "MiniginPCH.h"
#include "DigDugMultiplayerScene.h"
#include "ResourceManager.h"
#include "LevelLoader.h"
#include "LevelData.h"
#include "ObservedEvent.h"
#include "GameFiles.h"
#include "InputManager.h"
#include "DigDugGridComponent.h"
#include "GameObject.h"
#include "Engine.h"
#include "ScoreObserver.h"
#include "ObserverManager.h"
#include "PlayerManager.h"
#include "EnemyManager.h"
#include "RenderComponent.h"
#include "GameInputCommands.h"
#include "GameEvents.h"
#include "ObservedData.h"
#include "SceneManager.h"
#include "DigDugPlayerComponent.h"
#include "Time.h"
#include "DigDugEnemyComponent.h"
#include "WindowSettings.h"


using namespace DigDug;
using namespace Minigin;

DigDugMultiplayerScene::DigDugMultiplayerScene(const std::string& name, const std::string& levelFile, int numLivesP1,
	int numLivesP2, int levelNumber)
	: Scene(name)
	, m_pLevelData(ResourceManager::GetInstance().GetLoader<LevelData>()->GetResource(ResourceManager::GetInstance().GetBasePath() + levelFile))
	, m_LevelFile(levelFile)
	, m_LevelNumber(levelNumber)
	, m_EnemyCount(0)
{
	m_PlayerData[0].pLifeObjects.resize(numLivesP1);
	m_PlayerData[1].pLifeObjects.resize(numLivesP2);
}

void DigDugMultiplayerScene::SceneInitialize()
{
#pragma region Settings
	Vector2 scale{ 2.0f, 2.0f };
	const WindowSettings& wsettings = Minigin::Engine::GetWindowSettings();

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
	m_GameSettings.gridOffset = Vector2{ (wsettings.width - 50) / m_pLevelData->size.first, (wsettings.width - 50) / m_pLevelData->size.first };
#pragma endregion Settings


	//Managers
	GetSceneData().AddManager(new EnemyManager());
	GetSceneData().AddManager(new PlayerManager());
	auto pObserverManager = new ObserverManager();
	GetSceneData().AddManager(pObserverManager);

	//Score Counter
	auto pScoreObserver = new ScoreObserver(m_GameSettings.pScoreFont, Color4(), this);
	{
		auto pScoreObject = DigDug::CreateScoreManager(*this, m_GameSettings.pScoreFont, Color4(), pScoreObserver);
		auto pScoreRender = pScoreObject->GetComponent<RenderComponent>();
		if (pScoreRender) pScoreRender->SetPivot(Vector2(0, 1));
		pScoreObject->GetTransform().SetWorldPosition(wsettings.width*0.5f, wsettings.height);
	}

	//Background & Grid
	auto pBackgroundObject = MakeBackground(*this, m_GameSettings);
	m_GameSettings.pGrid = pBackgroundObject->GetComponentInChildren<DigDugGridComponent>();
	if (m_GameSettings.pGrid == nullptr)
	{
		Logger::GetInstance().LogError("DigDugMultiplayerScene::SceneInitialize > Failed to make grid");
		return;
	}
	pBackgroundObject->GetTransform().SetLocalPosition(Minigin::Engine::GetWindowSettings().width*0.5f, Minigin::Engine::GetWindowSettings().height*0.5f);


	//LevelData
	m_GameSettings.pGrid->Initialize(GetSceneData());
	if (m_GameSettings.pGrid)
	{
		int enemyId = 0;

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
			auto* pPooka = CreateAIPooka(*this, m_GameSettings, enemyId,pScoreObserver);
			pPooka->GetTransform().SetWorldPosition(m_GameSettings.pGrid->GetPosition(p.first, p.second));
			pPooka->GetTransform().SetWorldScale(scale);
			++m_EnemyCount;
			++enemyId;
		}
		//Fygars
		for (const auto& p : m_pLevelData->fygar)
		{
			auto* pFygar = CreateAIFygar(*this, m_GameSettings, enemyId,pScoreObserver);
			pFygar->GetTransform().SetWorldPosition(m_GameSettings.pGrid->GetPosition(p.first, p.second));
			pFygar->GetTransform().SetWorldScale(scale);
			++m_EnemyCount;
			++enemyId;
		}
	}

	//SetPath Lives
	for (size_t i = 0; i < m_PlayerData[0].pLifeObjects.size(); ++i)
	{
		auto* pObj = CreateGameObject();
		auto* pRender = new RenderComponent(m_GameSettings.pOtherTexture);
		pRender->SetRenderPriority(-26, GetSceneData());
		pRender->SetSource(Rect{ 4 * m_GameSettings.spriteWidth, 0, m_GameSettings.spriteWidth, m_GameSettings.spriteHeight });
		pObj->AddComponent(pRender);
		auto& t = pObj->GetTransform();
		t.SetWorldScale(scale);
		t.SetWorldPosition(((i + 0.5f)*m_GameSettings.spriteWidth)*scale.x, (wsettings.height - 0.5f*m_GameSettings.spriteHeight*scale.y));
		m_PlayerData[0].pLifeObjects[i] = pObj;
	}
	for (size_t i = 0; i < m_PlayerData[1].pLifeObjects.size(); ++i)
	{
		auto* pObj = CreateGameObject();
		auto* pRender = new RenderComponent(m_GameSettings.pOtherTexture);
		pRender->SetRenderPriority(-26, GetSceneData());
		pRender->SetSource(Rect{ 4 * m_GameSettings.spriteWidth, 0, m_GameSettings.spriteWidth, m_GameSettings.spriteHeight });
		pObj->AddComponent(pRender);
		auto& t = pObj->GetTransform();
		t.SetWorldScale(scale);
		t.SetWorldPosition(wsettings.width - ((i + 0.5f)*m_GameSettings.spriteWidth)*scale.x, (wsettings.height - 0.5f*m_GameSettings.spriteHeight*scale.y));
		m_PlayerData[1].pLifeObjects[i] = pObj;
	}
	//SetPath Flowers (LevelNumber)
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


	//SetPath MenuInput
	auto pAction = std::make_shared<DigDug::SceneSetInput>("Pause", GetSceneManager());
	InputAction menu1{ InputTriggerState::Pressed, pAction, -1, VK_ESCAPE, int(GamepadCode::GAMEPAD_START), 0 };
	InputAction menu2{ InputTriggerState::Pressed, pAction, -1, -1, int(GamepadCode::GAMEPAD_START), 1 };
	GetSceneData().GetInput()->AddInputAction(menu1);
	GetSceneData().GetInput()->AddInputAction(menu2);

	//Players
	m_PlayerData[0].pPlayer = MakePlayer1(m_GameSettings);
	m_PlayerData[1].pPlayer = MakePlayer2(m_GameSettings);

	auto& t1 = m_PlayerData[0].pPlayer->GetTransform();
	t1.SetWorldScale(scale);
	m_PlayerData[0].startPosition = m_GameSettings.pGrid->GetPosition(0, 0);
	t1.SetWorldPosition(m_PlayerData[0].startPosition);
	auto& t2 = m_PlayerData[1].pPlayer->GetTransform();
	t2.SetWorldScale(scale);
	m_PlayerData[1].startPosition = m_GameSettings.pGrid->GetPosition(2, 0);
	t2.SetWorldPosition(m_PlayerData[1].startPosition);
}


void DigDugMultiplayerScene::SceneNotify(ObservedEvent event, const ObservedData& data)
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
			//Get PlayerID
			DigDugPlayerComponent* pPlayerComp = pPlayer->GetComponent<DigDugPlayerComponent>();
			if (pPlayerComp)
			{
				int id = pPlayerComp->GetID();
				if (id >= 0 && id < NUMPLAYERS)
				{
					Logger::GetInstance().LogInfo("DigDugMultiplayerScene::SceneNotify > Player Died: ID = " + std::to_string(id));

					auto& pdata = m_PlayerData[id];

					//Remove one lifepoint
					if (pdata.pLifeObjects.size() > 0)
					{
						auto pObj = pdata.pLifeObjects[pdata.pLifeObjects.size() - 1];
						if (pObj) GameObject::DeleteObject(pObj);
						pdata.pLifeObjects.erase(pdata.pLifeObjects.end() - 1);
					}
					pdata.pPlayer = nullptr;

					//Check if all players are dead for ever => To Start
					bool areDead = true;
					for (int i = 0; i < NUMPLAYERS; ++i)
					{
						areDead = areDead && (m_PlayerData[i].pLifeObjects.size() == 0);
					}
					if (areDead)
					{
						GetSceneManager()->SetActiveScene("Menu");
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
							std::string name = "Coop";
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
				}
			}
		}
		break;
	}
}

void DigDugMultiplayerScene::SceneUpdate()
{
	for (int i = 0; i < NUMPLAYERS; ++i)
	{
		PlayerData& pdata = m_PlayerData[i];
		if (pdata.pPlayer == nullptr && pdata.pLifeObjects.size() > 0)
		{
			//
			Vector2 scale{ 2,2 };
			float respawnDelay = 3.0f;

			//
			pdata.accuTime += GetSceneData().GetTime()->GetDeltaTime();
			if (pdata.accuTime >= respawnDelay)
			{
				pdata.accuTime = 0.0f;

				switch(i)
				{
				case 0:
					pdata.pPlayer = MakePlayer1(m_GameSettings);
					break;
				case 1:
					pdata.pPlayer = MakePlayer2(m_GameSettings);
					break;
				}

				if (pdata.pPlayer)
				{
					pdata.pPlayer->GetTransform().SetWorldPosition(pdata.startPosition);
					pdata.pPlayer->GetTransform().SetWorldScale(scale);
				}
				Logger::GetInstance().LogInfo("DigDugMultiplayerScene::SceneUpdate > Player Respawned: ID = " + std::to_string(i));
			}
		}
	}
}

Scene* DigDugMultiplayerScene::OnReload() const
{
	return new DigDugMultiplayerScene(GetName(), m_LevelFile, 3, 3, m_LevelNumber);
}










GameObject* DigDugMultiplayerScene::MakePlayer1(const DigDug::DigDugSettings& settings)
{
	//SetPath Player
	InputAction up{ InputTriggerState::Pressed, nullptr, -1, 'W', int(GamepadCode::GAMEPAD_RIGHTSTICK_UP), 0 };
	InputAction down{ InputTriggerState::Pressed, nullptr, -1, 'S', int(GamepadCode::GAMEPAD_RIGHTSTICK_DOWN), 0 };
	InputAction left{ InputTriggerState::Pressed, nullptr, -1, 'A', int(GamepadCode::GAMEPAD_RIGHTSTICK_LEFT), 0 };
	InputAction right{ InputTriggerState::Pressed, nullptr, -1, 'D', int(GamepadCode::GAMEPAD_RIGHTSTICK_RIGHT), 0 };
	InputAction pump{ InputTriggerState::Pressed, nullptr, -1, 'E', int(GamepadCode::GAMEPAD_RIGHT_THUMB) , 0 };


	auto* pPlayer = CreatePlayer(*this, settings, up, down, left, right, pump, 0);
	return pPlayer;
}

GameObject* DigDugMultiplayerScene::MakePlayer2(const DigDug::DigDugSettings& settings)
{
	//SetPath Player
	InputAction up{ InputTriggerState::Pressed, nullptr, -1, VK_UP , int(GamepadCode::GAMEPAD_RIGHTSTICK_UP), 1 };
	InputAction down{ InputTriggerState::Pressed, nullptr, -1, VK_DOWN, int(GamepadCode::GAMEPAD_RIGHTSTICK_DOWN), 1 };
	InputAction left{ InputTriggerState::Pressed, nullptr, -1, VK_LEFT , int(GamepadCode::GAMEPAD_RIGHTSTICK_LEFT), 1 };
	InputAction right{ InputTriggerState::Pressed, nullptr, -1, VK_RIGHT, int(GamepadCode::GAMEPAD_RIGHTSTICK_RIGHT), 1 };
	InputAction pump{ InputTriggerState::Pressed, nullptr, -1, VK_SHIFT, int(GamepadCode::GAMEPAD_RIGHT_THUMB) , 1 };


	auto* pPlayer = CreatePlayer(*this, settings, up, down, left, right, pump, 1);
	return pPlayer;
}

