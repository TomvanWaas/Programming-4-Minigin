#include "MiniginPCH.h"
#include "AutokillComponent.h"
#include "SceneData.h"
#include "Time.h"
#include "GameObject.h"


void AutokillComponent::UpdateFirstOverride(const SceneData& sceneData)
{
	m_Delay -= sceneData.GetTime()->GetDeltaTime();
	if (m_Delay <= 0)
	{
		auto* pObject = GetGameObject();
		GameObject::DeleteObject(pObject);
	}
}
