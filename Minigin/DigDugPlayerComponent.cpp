#include "MiniginPCH.h"
#include "DigDugPlayerComponent.h"
#include "PlayerManager.h"
#include "SceneData.h"
#include "GameObject.h"
#include "Scene.h"
#include "GameEvents.h"
#include "ObservedData.h"
#include "ObservedData.h"

DigDugPlayerComponent::DigDugPlayerComponent(int id)
	: m_Id(id)
{
}

void DigDugPlayerComponent::InitializeOverride(const SceneData& sceneData)
{
	auto pm = sceneData.GetManager<PlayerManager>();
	if (pm)
	{
		pm->RegisterPlayer(GetGameObject(), m_Id);
	}
}

void DigDugPlayerComponent::DestroyOverride(const SceneData& sceneData)
{
	if (GetGameObject() && GetGameObject()->GetScene())
	{
		ObservedData d{};
		d.AddData<GameObject*>("GameObject", GetGameObject());
		GetGameObject()->GetScene()->NotifyAll(GameEvent::PlayerDied, d);
	}
	auto pm = sceneData.GetManager<PlayerManager>();
	if (pm)
	{
		pm->UnregisterPlayer(GetGameObject());
	}
}
