#pragma once
#include "SceneData.h"
#include "Vector2.h"

class ObservedEvent;
class ObservedData;
class GameObject;
class SceneManager;
class Scene abstract
{
public:
	explicit Scene(const std::string& name);
	virtual ~Scene();
	Scene(const Scene& other) = delete;
	Scene(Scene&& other) = delete;
	Scene& operator=(const Scene& other) = delete;
	Scene& operator=(Scene&& other) = delete;

	GameObject* CreateGameObject();
	bool DeleteGameObject(GameObject*& pObject);
	bool RemoveGameObject(GameObject* pObject);
	bool AddGameObject(GameObject* pObject);

	void Notify(ObservedEvent event, const ObservedData& data);

	void Initialize();
	void Update(float elapsed);
	void Render() const;

	const SceneData& GetSceneData() const;
	SceneData& GetSceneData() { return m_SceneData; }
	const std::string& GetName() const;

	const Vector2& GetSceneScale() const { return m_SceneScale; }
	void SetSceneScale(const Vector2& s) { m_SceneScale = s; }

	virtual Scene* GetNew() const = 0;

	void SetSceneManager(SceneManager* pManager) { m_pSceneManager = pManager; }
	SceneManager* GetSceneManager() const { return m_pSceneManager; }

protected:
	virtual void SceneInitialize() {}
	virtual void SceneUpdate() {}
	virtual void SceneNotify(ObservedEvent event, const ObservedData& data);
private:
	std::vector<GameObject*> m_pGameObjects;
	std::string m_Name;
	SceneData m_SceneData;
	Vector2 m_SceneScale;
	SceneManager* m_pSceneManager;
	bool m_IsInitialized = false;
};


