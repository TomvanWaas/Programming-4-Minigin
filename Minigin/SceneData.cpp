#include "MiniginPCH.h"
#include "SceneData.h"
#include "Time.h"
#include "RenderManager.h"
#include "CollisionManager.h"
#include "InputManager.h"

void SceneData::Initialize(Time* time, RenderManager* rendermanager, InputManager* inputmanager,
	CollisionManager* collisionmanager)
{
	if (m_Initialized == false)
	{
		pTime = (time != nullptr) ? time : new Time();
		pCollisionManager = (collisionmanager != nullptr) ? collisionmanager : new CollisionManager();
		pInput = (inputmanager != nullptr) ? inputmanager : new InputManager();
		pRenderManager = (rendermanager != nullptr) ? rendermanager : new RenderManager();
		m_Initialized = true;
	}
}

void SceneData::Update(float elapsed)
{
	pTime->Update(elapsed);
	pCollisionManager->UpdateBuffers();
	pInput->UpdateActions();
	pRenderManager->Update();
}
