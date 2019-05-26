#include "stdafx.h"
#include "FSMEvents.h"
#include "GameObject.h"
#include "Scene.h"
#include "BaseComponent.h"
#include "SpriteComponent.h"

using namespace Engine;

//FSMDeleteGameObjectEvent
FSMDeleteGameObjectEvent::FSMDeleteGameObjectEvent(GameObject* pObject)
	: m_pObject(pObject)
{
}
void FSMDeleteGameObjectEvent::Execute(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(sceneData);
	UNREFERENCED_PARAMETER(data);
	//Deletor by using parent
	if (m_pObject != nullptr && m_pObject->GetParent() != nullptr)
	{
		m_pObject->GetParent()->DeleteChild(m_pObject);
		m_pObject = nullptr;
	}
	//Deletor by using Scene
	else if (m_pObject != nullptr && m_pObject->GetScene() != nullptr)
	{
		m_pObject->GetScene()->DeleteGameObject(m_pObject);
		m_pObject = nullptr;
	}
}



//FSMComponentEnableEvent
FSMComponentEnableEvent::FSMComponentEnableEvent(BaseComponent* pComponent, Operation operation)
	: m_pComponent(pComponent)
	, m_Operation(operation)
{
}
void FSMComponentEnableEvent::Execute(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(sceneData);
	UNREFERENCED_PARAMETER(data);
	if (m_pComponent != nullptr)
	{
		switch (m_Operation)
		{
		case Operation::SetTrue:
			m_pComponent->SetEnabled(true);
			break;
		case Operation::SetFalse:
			m_pComponent->SetEnabled(false);
			break;
		case Operation::Toggle:
			m_pComponent->SetEnabled(!m_pComponent->IsEnabled());
			break;
		}
	}
}



//FSMSpriteSourceEvent
FSMSpriteSourceEvent::FSMSpriteSourceEvent(unsigned id, SpriteComponent* pSpriteComponent)
	: m_pSpriteComponent(pSpriteComponent)
	, m_ID(id)
{
}
void FSMSpriteSourceEvent::Execute(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(sceneData);
	UNREFERENCED_PARAMETER(data);
	if (m_pSpriteComponent != nullptr)
	{
		m_pSpriteComponent->SetCurrentSprite(m_ID);
	}
}



//FSMDoOnceEvent
void FSMDoOnceEvent::Initialize(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(sceneData);
	UNREFERENCED_PARAMETER(data);
	m_Done = false;
}
void FSMDoOnceEvent::Execute(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(data);
	if (!m_Done)
	{
		m_Done = true;
		if (m_pEvent != nullptr) m_pEvent->Execute(sceneData, data);
	}
}
