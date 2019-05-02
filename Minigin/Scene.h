#pragma once
#include "SceneManager.h"
#include "SceneData.h"

class GameObject;
class Renderer;
class RenderComponent;
class Scene final
{
	friend Scene* SceneManager::CreateScene(const std::string& name);
	explicit Scene(const std::string& name);


public:
	~Scene();
	Scene(const Scene& other) = delete;
	Scene(Scene&& other) = delete;
	Scene& operator=(const Scene& other) = delete;
	Scene& operator=(Scene&& other) = delete;

	GameObject* CreateGameObject();
	bool DeleteGameObject(GameObject* pObject);
	bool RemoveGameObject(GameObject* pObject);
	bool AddGameObject(GameObject* pObject);

	void InitSceneData(const SceneData& sceneData);
	void Initialize();
	void Update(float elapsed);
	void Render() const;

	const SceneData& GetSceneData() const;
	const std::string& GetName() const;

	void SetInitialize();

private: 
	std::string m_Name;
	std::vector<GameObject*> m_pGameObjects;
	SceneData m_SceneData;
	bool m_NeedsInit = true;

	std::vector<GameObject*> m_pMarkedForDelete;
};


