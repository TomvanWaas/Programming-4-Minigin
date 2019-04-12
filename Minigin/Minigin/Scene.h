#pragma once
#include "SceneManager.h"
#include "SceneData.h"

class GameObject;
class Renderer;
class RenderComponent;
class Scene final
{
	friend Scene* SceneManager::CreateScene(const std::string& name);
public:
	~Scene();
	Scene(const Scene& other) = delete;
	Scene(Scene&& other) = delete;
	Scene& operator=(const Scene& other) = delete;
	Scene& operator=(Scene&& other) = delete;

	void AddChild(GameObject* pObject);
	void RemoveChild(GameObject* pObject);

	const std::string& GetName() const { return m_Name; }

	void Initialize();
	void Update();
	void Render() const;

	const SceneData& GetSceneData() const;

private: 
	explicit Scene(const std::string& name, Time* pTime);

	std::string m_Name;
	std::vector<GameObject*> m_pGameObjects;
	SceneData m_SceneData;
};


