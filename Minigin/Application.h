#pragma once
class SceneManager;

class Application abstract
{
public:
	explicit Application() = default;
	virtual ~Application() = default;

	virtual void Initialize(SceneManager& sceneManager) = 0;

private:


};

