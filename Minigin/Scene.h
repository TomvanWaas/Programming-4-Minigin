#pragma once
#include "SceneManager.h"

namespace dae
{
	class GameObject;
	class Scene
	{
		friend Scene* SceneManager::CreateScene(const std::string& name);
	public:
		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		void Add(GameObject* pObject);
		const std::string& GetName() const { return m_Name; }

		void Initialize();
		void Update();
		void Render() const;
	private: 
		explicit Scene(const std::string& name);

		std::string m_Name{};
		std::vector<GameObject*> m_pObjects{};
	};

}
