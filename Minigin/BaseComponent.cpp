#include "MiniginPCH.h"
#include "BaseComponent.h"
#include "GameObject.h"
#include "Scene.h"


void BaseComponent::Initialize(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);
}


void BaseComponent::Construct(const SceneData& sceneData)
{
	if (m_IsConstructed == false)
	{
		m_IsConstructed = true;
		Initialize(sceneData);
	}
}

void BaseComponent::UpdateFirst(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);
}

void BaseComponent::UpdateSecond(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);
}


const GameObject& BaseComponent::GetGameObject() const
{
	return *m_pGameObject;
}
GameObject& BaseComponent::GetGameObject()
{
	return *m_pGameObject;
}

BaseComponent::BaseComponent(GameObject& gameObject)
	: m_pGameObject(&gameObject)
{
}
