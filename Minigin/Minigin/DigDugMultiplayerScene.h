#pragma once
#include "Scene.h"
#include "DigDugSettings.h"
namespace DigDug 
{
	struct LevelData;
	class DigDugMultiplayerScene final : public Engine::Scene
	{
		struct PlayerData
		{
			std::vector<Engine::GameObject*> pLifeObjects;
			Engine::Vector2 startPosition = Engine::Vector2::Zero;
			float accuTime = 0.0f;
			Engine::GameObject* pPlayer = nullptr;
		};
	public:
		explicit DigDugMultiplayerScene(const std::string& name, const std::string& levelFile, int numLivesP1 = 3, int numLivesP2 = 3, int levelNumber = 1);
		virtual ~DigDugMultiplayerScene() = default;
		virtual void SceneInitialize() override;
		virtual void SceneNotify(Engine::ObservedEvent event, const Engine::ObservedData& data) override;
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


		Engine::GameObject* MakePlayer1(const DigDug::DigDugSettings& settings);
		Engine::GameObject* MakePlayer2(const DigDug::DigDugSettings& settings);
	};
}

