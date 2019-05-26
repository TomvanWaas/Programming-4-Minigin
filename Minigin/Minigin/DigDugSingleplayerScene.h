#pragma once
#include "Scene.h"
#include "DigDugSettings.h"

namespace DigDug
{
	struct LevelData;
	class DigDugSingleplayerScene final : public Engine::Scene
	{
		struct PlayerData
		{
			std::vector<Engine::GameObject*> pLifeObjects;
			Engine::Vector2 startPosition = Engine::Vector2::Zero;
			Engine::GameObject* pPlayer = nullptr;
		};
	public:
		explicit DigDugSingleplayerScene(const std::string& name, const std::string& levelFile, int numLives = 3, int levelNumber = 1);
		virtual ~DigDugSingleplayerScene() = default;

		virtual void SceneInitialize() override;
		virtual void SceneNotify(Engine::ObservedEvent event, const Engine::ObservedData& data) override;
		virtual void SceneUpdate() override;
		virtual Scene* OnReload() const override;
	private:
		DigDug::DigDugSettings m_GameSettings;
		std::string m_LevelFile;
		PlayerData m_PlayerData;
		int m_LevelNumber;
		int m_EnemyCount;
		std::shared_ptr<LevelData> m_pLevelData;


		Engine::GameObject* MakePlayer();
	};

}