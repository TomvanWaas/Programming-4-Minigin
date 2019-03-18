#pragma once
#include "Singleton.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
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
		void Update();
		void Render();

		void SetActiveScene(const std::string& name);
		void NextScene();
		void PreviousScene();

	private:
		std::vector<Scene*> m_pScenes;
		size_t m_ActiveSceneId;


	};

}
