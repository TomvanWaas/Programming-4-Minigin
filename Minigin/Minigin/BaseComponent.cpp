#include "MiniginPCH.h"
#include "BaseComponent.h"
#include "GameObject.h"
#include "Scene.h"

void BaseComponent::Initialize(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);
}

void BaseComponent::ReInitialize(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);
}

void BaseComponent::Update(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);
}

void BaseComponent::SetGameObject(GameObject* gameObject)
{
	GameObject* pPrevious = m_pGameObject;
	m_pGameObject = gameObject;

	//If previous was not nullptr => Reinitialize
	if (pPrevious != nullptr && gameObject != nullptr)
	{
		const auto* scene = gameObject->GetScene(true);
		if (scene != nullptr)
		{
			ReInitialize(scene->GetSceneData());
		}
	}
}
GameObject* BaseComponent::GetGameObject() const
{
	return m_pGameObject;
}
