#pragma once
#include "Scene.h"
#include "DigDugSettings.h"
struct LevelData;
namespace DigDug 
{
	class DigDugMultiplayerScene final : public Scene
	{
		struct PlayerData
		{
			std::vector<GameObject*> pLifeObjects;
			Vector2 startPosition = Vector2::Zero;
			float accuTime = 0.0f;
			GameObject* pPlayer = nullptr;
		};
	public:
		explicit DigDugMultiplayerScene(const std::string& name, const std::string& levelFile, int numLivesP1 = 3, int numLivesP2 = 3, int levelNumber = 1);
		virtual ~DigDugMultiplayerScene() = default;
		virtual void SceneInitialize() override;
		virtual void SceneNotify(ObservedEvent event, const ObservedData& data) override;
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




		GameObject* MakePlayer1(const DigDug::DigDugSettings& settings);
		GameObject* MakePlayer2(const DigDug::DigDugSettings& settings);
	};
}

