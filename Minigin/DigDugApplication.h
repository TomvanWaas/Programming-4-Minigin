#pragma once
#include "Application.h"
class Scene;

class DigDugApplication final : public Application
{
public:
	DigDugApplication() = default;
	virtual ~DigDugApplication() = default;

	virtual void Initialize(SceneManager& sceneManager, const WindowSettings& windowSettings) override;

private:
	Scene* CreateMenuScene(SceneManager& sceneManager, const WindowSettings& windowSettings);
	Scene* CreateGameScene(SceneManager& sceneManager, const WindowSettings& windowSettings);

};

