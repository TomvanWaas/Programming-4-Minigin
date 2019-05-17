#pragma once
#include "Manager.h"
#include "LevelData.h"

class EnemyManager;
class PlayerManager;

namespace DigDug
{

	class GameManager final : public Manager
	{
	public:
		explicit GameManager(unsigned int numLives, const std::string& levelFile);
		virtual ~GameManager() = default;

		virtual void Initialize(const SceneData& sceneData) override;
		virtual void OnNotify(ObservedEvent event, const ObservedData& data) override;
	private:
		unsigned int m_NrLives;
		std::shared_ptr<LevelData> m_pLevelData;

	};

}
