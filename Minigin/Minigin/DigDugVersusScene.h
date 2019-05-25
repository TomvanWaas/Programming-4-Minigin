#pragma once
#include "Scene.h"
#include "DigDugSettings.h"
namespace DigDug
{
	struct LevelData;
	class DigDugVersusScene final : public Minigin::Scene
	{
		struct PlayerData
		{
			std::vector<Minigin::GameObject*> pLifeObjects;
			Minigin::Vector2 startPosition = Minigin::Vector2::Zero;
			float accuTime = 0.0f;
			Minigin::GameObject* pPlayer = nullptr;
		};
	public:
		explicit DigDugVersusScene(const std::string& name, const std::string& levelFile, int numLives, int levelNumber);
		virtual ~DigDugVersusScene() = default;

		virtual void SceneInitialize() override;
		virtual void SceneNotify(Minigin::ObservedEvent event, const Minigin::ObservedData& data) override;
		virtual void SceneUpdate() override;
		virtual Scene* OnReload() const override;

	private:
		int m_LevelNumber;
		std::string m_LevelFile;
		DigDugSettings m_GameSettings;
		PlayerData m_PlayerData[2];
		std::shared_ptr<LevelData> m_pLevelData;
		int m_EnemyCount;

		Minigin::GameObject* MakeFygar();
		Minigin::GameObject* MakeDigDug();
	};

}
