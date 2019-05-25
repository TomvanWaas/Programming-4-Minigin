#pragma once
#include "Scene.h"
#include "DigDugSettings.h"

namespace DigDug
{
	struct LevelData;
	class DigDugSingleplayerScene final : public Minigin::Scene
	{
		struct PlayerData
		{
			std::vector<Minigin::GameObject*> pLifeObjects;
			Minigin::Vector2 startPosition = Minigin::Vector2::Zero;
			Minigin::GameObject* pPlayer = nullptr;
		};
	public:
		explicit DigDugSingleplayerScene(const std::string& name, const std::string& levelFile, int numLives = 3, int levelNumber = 1);
		virtual ~DigDugSingleplayerScene() = default;

		virtual void SceneInitialize() override;
		virtual void SceneNotify(Minigin::ObservedEvent event, const Minigin::ObservedData& data) override;
		virtual void SceneUpdate() override;
		virtual Scene* OnReload() const override;
	private:
		DigDug::DigDugSettings m_GameSettings;
		std::string m_LevelFile;
		PlayerData m_PlayerData;
		int m_LevelNumber;
		int m_EnemyCount;
		std::shared_ptr<LevelData> m_pLevelData;


		Minigin::GameObject* MakePlayer();
	};

}