#include "MiniginPCH.h"
#include "AutokillComponent.h"
#include "SceneData.h"
#include "Time.h"
#include "GameObject.h"


void DigDug::AutokillComponent::UpdateFirstOverride(const Engine::SceneData& sceneData)
{
	m_Delay -= sceneData.GetTime()->GetDeltaTime();
	if (m_Delay <= 0)
	{
		auto* pObject = GetGameObject();
		Engine::GameObject::DeleteObject(pObject);
	}
}
