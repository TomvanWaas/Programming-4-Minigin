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

DigDugSingleplayerScene::DigDugSingleplayerScene(const std::string& name, const std::string& levelFile, int numLives, int levelNumber)
	: Scene(name)
	, m_pLevelData(ResourceManager::GetInstance().GetLoader<LevelData>()->GetResource(ResourceManager::GetInstance().GetBasePath() + levelFile))
	, m_LevelFile(levelFile)
	, m_NumberOfLives(numLives)
	, m_LevelNumber(levelNumber)
{
}


void DigDugSingleplayerScene::SceneInitialize()
{
	Vector2 scale{ 2.0f, 2.0f };
	const WindowSettings& wsettings = dae::Minigin::GetWindowSettings();

	//Settings
	DigDug::DigDugSettings settings{};
	settings.spriteHeight = 16;
	settings.spriteWidth = 16;
	settings.spriteSpeed = 0.25f;

	settings.pBackgroundTexture = ResourceManager::GetInstance().LoadTexture("background.png");
	settings.pDigDugTexture = ResourceManager::GetInstance().LoadTexture("Sprites_DigDug.png");
	settings.pFygarTexture = ResourceManager::GetInstance().LoadTexture("Sprites_Fygar.png");
	settings.pOtherTexture = ResourceManager::GetInstance().LoadTexture("Sprites_Other.png");
	settings.pPookaTexture = ResourceManager::GetInstance().LoadTexture("Sprites_Pooka.png");
	settings.pPumpTexture = ResourceManager::GetInstance().LoadTexture("Sprites_Pump.png");
	settings.pScoreFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	settings.gridHeight = m_pLevelData->size.second;
	settings.gridWidth = m_pLevelData->size.first;
	settings.gridOffset = Vector2{ (wsettings.width - 25) / m_pLevelData->size.first, (wsettings.height - 25) / m_pLevelData->size.second };

	//Managers
	GetSceneData().AddManager(new EnemyManager());
	GetSceneData().AddManager(new PlayerManager());
	auto pObserverManager = new ObserverManager();
	GetSceneData().AddManager(pObserverManager);

	//Observers
	auto pScoreObserver = new ScoreObserver(settings.pScoreFont, Color4(), this);

	pObserverManager->AddUnownedObserver(pScoreObserver);





	//Background
	auto pBackgroundObject = MakeBackground(*this, settings);
	settings.pGrid = pBackgroundObject->GetComponentInChildren<DigDugGridComponent>();
	if (settings.pGrid == nullptr) return;
	pBackgroundObject->GetTransform().SetLocalPosition(dae::Minigin::GetWindowSettings().width*0.5f, dae::Minigin::GetWindowSettings().height*0.5f);

	settings.pGrid->Initialize(GetSceneData());
	if (settings.pGrid)
	{
		//Marks
		for (const auto& p : m_pLevelData->marked)
		{
			settings.pGrid->Mark(settings.pGrid->GetPosition(p.first, p.second));
		}
		//Obstacles
		for (const auto& p : m_pLevelData->obstacle)
		{
			auto* pObstacle = CreateObstacle(*this, settings, pScoreObserver);
			pObstacle->GetTransform().SetWorldPosition(settings.pGrid->GetPosition(p.first, p.second));
			pObstacle->GetTransform().SetWorldScale(scale);
		}
		//Pookas
		for (const auto& p : m_pLevelData->pooka)
		{
			auto* pPooka = CreatePooka(*this, settings, pScoreObserver);
			pPooka->GetTransform().SetWorldPosition(settings.pGrid->GetPosition(p.first, p.second));
			pPooka->GetTransform().SetWorldScale(scale);
		}
		//Fygars
		for (const auto& p : m_pLevelData->fygar)
		{
			auto* pFygar = CreateFygar(*this, settings, pScoreObserver);
			pFygar->GetTransform().SetWorldPosition(settings.pGrid->GetPosition(p.first, p.second));
			pFygar->GetTransform().SetWorldScale(scale);
		}
	}

	//Make Player
	auto* pPlayer = CreatePlayer(*this, settings, 'W', 'S', 'A', 'D', 'E');
	pPlayer->GetTransform().SetWorldPosition(settings.pGrid->GetPosition(m_pLevelData->size.first / 4 * 2, 0));
	pPlayer->GetTransform().SetWorldScale(scale);

	//Make Lives
	for (int i = 0; i < m_NumberOfLives; ++i)
	{
		auto* pObj = CreateGameObject();
		auto* pRender = new RenderComponent(settings.pOtherTexture);
		pRender->SetRenderPriority(-26, GetSceneData());
		pRender->SetSource(Rect{ 4 * settings.spriteWidth, 0, settings.spriteWidth, settings.spriteHeight });
		pObj->AddComponent(pRender);
		auto& t = pObj->GetTransform();
		t.SetWorldScale(scale);
		t.SetWorldPosition( ((i+0.5f)*settings.spriteWidth)*scale.x, (wsettings.height - 0.5f*settings.spriteHeight*scale.y));
	}
	//Make Flowers (LevelNumber)
	for (int i = 0; i < m_LevelNumber; ++i)
	{
		auto* pObj = CreateGameObject();
		auto* pRender = new RenderComponent(settings.pOtherTexture);
		pRender->SetRenderPriority(-26, GetSceneData());
		pRender->SetSource(Rect{ 5 * settings.spriteWidth, 0, settings.spriteWidth, settings.spriteHeight });
		pObj->AddComponent(pRender);
		auto& t = pObj->GetTransform();
		t.SetWorldScale(scale);
		t.SetWorldPosition(wsettings.width - ((i + 0.5f)*settings.spriteWidth)*scale.x, (settings.spriteHeight*2.5f)*scale.y);
	}
}


void DigDugSingleplayerScene::SceneNotify(ObservedEvent event, const ObservedData& data)
{
	UNREFERENCED_PARAMETER(event);
	UNREFERENCED_PARAMETER(data);

	switch (event)
	{
	case GameEvent::PlayerDied:
		--m_NumberOfLives;
		//Reload Scene
		if (m_NumberOfLives > 0)
		{
			if (GetSceneManager())
			{
				GetSceneManager()->ReloadScene();
			}
		}
		else
		{
			//Reload entire game
			dae::Minigin::Reload();
		}

	}
}

Scene* DigDugSingleplayerScene::GetNew() const
{
	return new DigDugSingleplayerScene(GetName(), m_LevelFile, m_NumberOfLives, m_LevelNumber);
}

