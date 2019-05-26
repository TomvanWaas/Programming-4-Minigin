#include "stdafx.h"
#include "SceneData.h"
#include "Time.h"
#include "RenderManager.h"
#include "CollisionManager.h"
#include "InputManager.h"
#include "Manager.h"

using namespace Engine;

SceneData::~SceneData()
{
	SAFE_DELETE(m_pCollisionManager);
	SAFE_DELETE(m_pInputManager);
	SAFE_DELETE(m_pRenderManager);
	SAFE_DELETE(m_pTimeManager);
	for (Manager* pManager : m_pManagers)
	{
		SAFE_DELETE(pManager);
	}
	m_pManagers.clear();
}

void SceneData::Initialize()
{
	if (m_Initialized == false)
	{
		m_Initialized = true;
		m_pTimeManager =  new Time();
		m_pCollisionManager =  new CollisionManager();
		m_pInputManager =  new InputManager();
		m_pRenderManager = new RenderManager();

		m_pCollisionManager->Initialize(*this);
		m_pInputManager->Initialize(*this);
		m_pRenderManager->Initialize(*this);
	}

	for (Manager* pManager : m_pManagers)
	{
		if (pManager) pManager->Initialize(*this);
	}
}


void SceneData::Update(float elapsed)
{
	m_pTimeManager->Update(elapsed);
	m_pCollisionManager->Update(*this);
	m_pInputManager->Update(*this);
	m_pRenderManager->Update(*this);

	for (Manager* pManager : m_pManagers)
	{
		if (pManager) pManager->Update(*this);
	}
}

void SceneData::UpdateFixed(float elapsed)
{
	m_pTimeManager->UpdateFixed(elapsed);
	m_pCollisionManager->UpdateFixed(*this);
	m_pInputManager->UpdateFixed(*this);
	m_pRenderManager->UpdateFixed(*this);

	for (Manager* pManager : m_pManagers)
	{
		if (pManager) pManager->UpdateFixed(*this);
	}
}

void SceneData::Notify(ObservedEvent event, const ObservedData& data)
{
	m_pCollisionManager->Notify(event, data);
	m_pInputManager->Notify(event, data);
	m_pRenderManager->Notify(event, data);
	for (Manager* pManager : m_pManagers)
	{
		if (pManager) pManager->Notify(event, data);
	}
}

void SceneData::LateInitialize()
{
	for (Manager* pManager : m_pManagers)
	{
		if (pManager) pManager->LateInitialize(*this);
	}
}
