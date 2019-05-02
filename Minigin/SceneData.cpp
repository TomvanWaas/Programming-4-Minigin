#include "MiniginPCH.h"
#include "SceneData.h"
#include "Time.h"
#include "RenderManager.h"
#include "CollisionManager.h"
#include "InputManager.h"

void SceneData::Initialize(const SceneData& other)
{
	if (m_Initialized == false)
	{
		m_Initialized = true;
		pTime = (other.pTime != nullptr) ? other.pTime : new Time();
		pCollisionManager = (other.pCollisionManager != nullptr) ? other.pCollisionManager : new CollisionManager();
		pInput = (other.pInput != nullptr) ? other.pInput : new InputManager();
		pRenderManager = (other.pRenderManager != nullptr) ? other.pRenderManager : new RenderManager();

		//InitializeOverride
		pCollisionManager->Initialize(*this);

	}
}


void SceneData::Update(float elapsed)
{
	pTime->Update(elapsed);
	pCollisionManager->UpdateBuffers();
	pInput->UpdateActions();
	pRenderManager->Update();
}
