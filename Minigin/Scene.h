#pragma once
#include "SceneManager.h"
#include "SceneData.h"

class GameObject;
class Renderer;
class RenderComponent;
class Scene final
{
	friend Scene* SceneManager::CreateScene(const std::string& name);
	explicit Scene(const std::string& name, Time* pTime);


public:
	enum class Stage
	{
		Initialize,
		Update,
		Render
	};

	~Scene();
	Scene(const Scene& other) = delete;
	Scene(Scene&& other) = delete;
	Scene& operator=(const Scene& other) = delete;
	Scene& operator=(Scene&& other) = delete;

	GameObject* CreateGameObject();
	void DeleteGameObject(GameObject* pObject);
	void RemoveGameObject(GameObject* pObject);
	void AddGameObject(GameObject* pObject);

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
	
};


