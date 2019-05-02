#pragma once
struct WindowSettings;
class SceneManager;

class Application abstract
{
public:
	explicit Application() = default;
	virtual ~Application() = default;

	virtual void Initialize(SceneManager& sceneManager, const WindowSettings& window) = 0;

private:


};

