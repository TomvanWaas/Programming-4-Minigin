#pragma once
#include "Scene.h"
struct LevelData;
class DigDugSingleplayerScene final : public Scene
{
public:
	explicit DigDugSingleplayerScene(const std::string& name, const std::string& levelFile, int numLives = 3, int levelNumber = 1);
	virtual ~DigDugSingleplayerScene() = default;

	virtual void SceneInitialize() override;
	virtual void SceneNotify(ObservedEvent event, const ObservedData& data) override;
	virtual Scene* GetNew() const override;
private:
	std::string m_LevelFile;
	std::shared_ptr<LevelData> m_pLevelData;
	int m_NumberOfLives;
	int m_LevelNumber;


};

