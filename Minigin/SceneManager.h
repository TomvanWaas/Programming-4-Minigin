#pragma once
class Scene;
class SceneManager final
{
public:
	SceneManager();
	~SceneManager();
	SceneManager(const SceneManager& other) = delete;
	SceneManager(SceneManager&& other) = delete;
	SceneManager& operator=(const SceneManager& other) = delete;
	SceneManager& operator=(SceneManager&& other) = delete;

	bool AddScene(Scene* pScene);

	void Initialize();
	void Update(float deltaTime);
	void Render() const;

	Scene* GetActiveScene() const;
	void SetActiveScene(const std::string& name);

	void NextScene();
	void PreviousScene();
	void ReloadScene();

private:
	std::vector<Scene*> m_pScenes;
	size_t m_ActiveSceneId;
	bool m_NeedsReload = false;



};


	