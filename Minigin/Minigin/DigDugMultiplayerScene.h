#pragma once
#include "Scene.h"
#include "DigDugSettings.h"
namespace DigDug 
{
	struct LevelData;
	class DigDugMultiplayerScene final : public Minigin::Scene
	{
		struct PlayerData
		{
			std::vector<Minigin::GameObject*> pLifeObjects;
			Minigin::Vector2 startPosition = Minigin::Vector2::Zero;
			float accuTime = 0.0f;
			Minigin::GameObject* pPlayer = nullptr;
		};
	public:
		explicit DigDugMultiplayerScene(const std::string& name, const std::string& levelFile, int numLivesP1 = 3, int numLivesP2 = 3, int levelNumber = 1);
		virtual ~DigDugMultiplayerScene() = default;
		virtual void SceneInitialize() override;
		virtual void SceneNotify(Minigin::ObservedEvent event, const Minigin::ObservedData& data) override;
		virtual void SceneUpdate() override;
		virtual Scene* OnReload() const override;
	private:
		static const int NUMPLAYERS = 2;

		DigDugSettings m_GameSettings;
		std::string m_LevelFile;
		PlayerData m_PlayerData[NUMPLAYERS];
		int m_LevelNumber;
		int m_EnemyCount;
		std::shared_ptr<LevelData> m_pLevelData;


		Minigin::GameObject* MakePlayer1(const DigDug::DigDugSettings& settings);
		Minigin::GameObject* MakePlayer2(const DigDug::DigDugSettings& settings);
	};
}

