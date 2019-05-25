#pragma once

namespace Minigin
{
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
		Scene* GetScene(const std::string& name) const;

		void Initialize();
		void Update(float deltaTime);
		void Render() const;

		Scene* GetActiveScene() const;
		Scene* GetPreviousScene() const;
		Scene* GetNextScene() const;

		void SetActiveScene(const std::string& name);
		void SetActiveScene(Scene* pScene);
		void SetActiveScene(size_t idx);

		void NextScene();
		void PreviousScene();
		void ReloadScene();

	private:
		std::vector<Scene*> m_pScenes;
		Scene* m_pActiveScene;
		bool m_NeedsReload = false;



	};
}

	