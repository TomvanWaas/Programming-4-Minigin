#pragma once
class Time;
class RenderManager;
class InputManager;
class CollisionManager;

struct SceneData final
{
	SceneData()
		: pTime{nullptr}
		, pRenderManager{nullptr}
		, pInput(nullptr)
		, pCollisionManager(nullptr)
	{}

	void Initialize(const SceneData& other);
	void Update(float elapsed);

	Time* pTime;
	RenderManager* pRenderManager;
	InputManager* pInput;
	CollisionManager* pCollisionManager;

	//Others
	//...
private:
	bool m_Initialized = false;

};
