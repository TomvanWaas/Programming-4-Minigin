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

	Time* pTime;
	RenderManager* pRenderManager;
	InputManager* pInput;
	CollisionManager* pCollisionManager;

	//Others
	//...

};
