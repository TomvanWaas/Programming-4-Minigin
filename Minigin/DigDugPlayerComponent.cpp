#include "MiniginPCH.h"
#include "DigDugPlayerComponent.h"
#include "PlayerManager.h"
#include "SceneData.h"
#include "GameObject.h"
#include "Scene.h"
#include "GameEvents.h"
#include "ObservedData.h"

void DigDugPlayerComponent::InitializeOverride(const SceneData& sceneData)
{
	auto pm = sceneData.GetManager<PlayerManager>();
	if (pm)
	{
		pm->RegisterPlayer(GetGameObject());
	}
}

void DigDugPlayerComponent::DestroyOverride(const SceneData& sceneData)
{
	if (GetGameObject() && GetGameObject()->GetScene())
	{
		GetGameObject()->GetScene()->Notify(GameEvent::PlayerDied, ObservedData{});
	}
	auto pm = sceneData.GetManager<PlayerManager>();
	if (pm)
	{
		pm->UnregisterPlayer(GetGameObject());
	}
}
