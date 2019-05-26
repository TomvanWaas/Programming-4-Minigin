#pragma once
#include "Scene.h"
#include "DigDugSettings.h"
namespace DigDug
{
	struct LevelData;
	class DigDugVersusScene final : public Engine::Scene
	{
		struct PlayerData
		{
			std::vector<Engine::GameObject*> pLifeObjects;
			Engine::Vector2 startPosition = Engine::Vector2::Zero;
			float accuTime = 0.0f;
			Engine::GameObject* pPlayer = nullptr;
		};
	public:
		explicit DigDugVersusScene(const std::string& name, const std::string& levelFile, int numLives, int levelNumber);
		virtual ~DigDugVersusScene() = default;

		virtual void SceneInitialize() override;
		virtual void SceneNotify(Engine::ObservedEvent event, const Engine::ObservedData& data) override;
		virtual void SceneUpdate() override;
		virtual Scene* OnReload() const override;

	private:
		int m_LevelNumber;
		std::string m_LevelFile;
		DigDugSettings m_GameSettings;
		PlayerData m_PlayerData[2];
		std::shared_ptr<LevelData> m_pLevelData;
		int m_EnemyCount;

		Engine::GameObject* MakeFygar();
		Engine::GameObject* MakeDigDug();
	};

}
