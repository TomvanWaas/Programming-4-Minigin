#include "MiniginPCH.h"
#include "GameManager.h"
#include "SceneData.h"
#include "PlayerManager.h"
#include "GameEvents.h"
#include "ResourceManager.h"
#include "LevelLoader.h"
#include "GameFiles.h"
#include "GameObject.h"
#include "DigDugGridComponent.h"
#include "Minigin.h"

using namespace DigDug;

GameManager::GameManager(unsigned numLives, const std::string& levelFile)
	: m_NrLives(numLives)
	, m_pLevelData(ResourceManager::GetInstance().GetLoader<LevelData>()->GetResource(ResourceManager::GetInstance().GetBasePath() + levelFile))
{
}

void GameManager::Initialize(const SceneData& sceneData)
{
	Vector2 scale{ 2,2 };
	const WindowSettings& wsettings = dae::Minigin::GetWindowSettings();

	//Settings
	DigDugSettings settings{};
	settings.spriteHeight = 16;
	settings.spriteWidth = 16;
	settings.spriteSpeed = 0.25f;

	settings.pBackgroundTexture = ResourceManager::GetInstance().LoadTexture("background.png");
	settings.pCaveTexture = ResourceManager::GetInstance().LoadTexture("Cave.png");
	settings.pDigDugTexture = ResourceManager::GetInstance().LoadTexture("Sprites_DigDug.png");
	settings.pFygarTexture = ResourceManager::GetInstance().LoadTexture("Sprites_Fygar.png");
	settings.pOtherTexture = ResourceManager::GetInstance().LoadTexture("Sprites_Other.png");
	settings.pPookaTexture = ResourceManager::GetInstance().LoadTexture("Sprites_Pooka.png");
	settings.pPumpTexture = ResourceManager::GetInstance().LoadTexture("Sprites_Pump.png");

	settings.gridHeight = m_pLevelData->size.second;
	settings.gridWidth = m_pLevelData->size.first;
	settings.gridOffset = Vector2{ (wsettings.width - 25) / m_pLevelData->size.first, (wsettings.height - 25) / m_pLevelData->size.second };

	//Background
	auto pBackgroundObject = MakeBackground(*sceneData.GetScene(), settings);
	settings.pGrid = pBackgroundObject->GetComponentInChildren<DigDugGridComponent>();
	if (settings.pGrid == nullptr) return;
	pBackgroundObject->GetTransform().SetLocalPosition(dae::Minigin::GetWindowSettings().width*0.5f, dae::Minigin::GetWindowSettings().height*0.5f);

	settings.pGrid->Initialize(sceneData);
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
			auto* pObstacle = CreateObstacle(*sceneData.GetScene(), settings);
			pObstacle->GetTransform().SetWorldPosition(settings.pGrid->GetPosition(p.first, p.second));
			pObstacle->GetTransform().SetWorldScale(scale);
		}
		//Pookas
		for (const auto& p : m_pLevelData->pooka)
		{
			auto* pPooka = CreatePooka(*sceneData.GetScene(), settings);
			pPooka->GetTransform().SetWorldPosition(settings.pGrid->GetPosition(p.first, p.second));
			pPooka->GetTransform().SetWorldScale(scale);
		}
		//Fygars
		for (const auto& p : m_pLevelData->fygar)
		{
			auto* pFygar = CreateFygar(*sceneData.GetScene(), settings);
			pFygar->GetTransform().SetWorldPosition(settings.pGrid->GetPosition(p.first, p.second));
			pFygar->GetTransform().SetWorldScale(scale);
		}
	}

	//Make Player
	auto* pPlayer = CreatePlayer(*sceneData.GetScene(), settings, 'W', 'S', 'A', 'D', 'E');
	pPlayer->GetTransform().SetWorldPosition(settings.pGrid->GetPosition(m_pLevelData->size.first / 4 * 2, 0));
	pPlayer->GetTransform().SetWorldScale(scale);

}

void GameManager::OnNotify(ObservedEvent event, const ObservedData& data)
{
	UNREFERENCED_PARAMETER(event);
	UNREFERENCED_PARAMETER(data);
}

