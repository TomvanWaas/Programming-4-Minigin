#pragma once
class Scene;
class Time;
#include "SceneData.h"
class SceneManager final
{
public:
	SceneManager();
	virtual ~SceneManager();
	SceneManager(const SceneManager& other) = delete;
	SceneManager(SceneManager&& other) = delete;
	SceneManager& operator=(const SceneManager& other) = delete;
	SceneManager& operator=(SceneManager&& other) = delete;

	Scene* CreateScene(const std::string& name);

	void Initialize();
	void Update(float deltaTime);
	void Render() const;

	Scene* GetActiveScene() const;
	void SetActiveScene(const std::string& name);
	void NextScene();
	void PreviousScene();


private:
	std::vector<Scene*> m_pScenes;
	size_t m_ActiveSceneId;


	//SceneManager::SceneData variables that are used by all scenes
	SceneData m_SceneData;


};


	