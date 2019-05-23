#pragma once
#include "Scene.h"
#include "DigDugSettings.h"
struct LevelData;
class DigDugSingleplayerScene final : public Scene
{
	struct PlayerData
	{
		std::vector<GameObject*> pLifeObjects;
		Vector2 startPosition = Vector2::Zero;
		GameObject* pPlayer = nullptr;
	};
public:
	explicit DigDugSingleplayerScene(const std::string& name, const std::string& levelFile, int numLives = 3, int levelNumber = 1);
	virtual ~DigDugSingleplayerScene() = default;

	virtual void SceneInitialize() override;
	virtual void SceneNotify(ObservedEvent event, const ObservedData& data) override;
	virtual Scene* OnReload() const override;
private:
	DigDug::DigDugSettings m_GameSettings;
	std::string m_LevelFile;
	PlayerData m_PlayerData;
	int m_LevelNumber;
	int m_EnemyCount;
	std::shared_ptr<LevelData> m_pLevelData;


	GameObject* MakePlayer();
};

