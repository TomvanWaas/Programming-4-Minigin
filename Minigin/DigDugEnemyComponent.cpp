#include "MiniginPCH.h"
#include "DigDugEnemyComponent.h"
#include "SceneData.h"
#include "EnemyManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "GameEvents.h"
#include "ObservedData.h"

void DigDugEnemyComponent::InitializeOverride(const SceneData& sceneData)
{
	auto pm = sceneData.GetManager<EnemyManager>();
	if (pm)
	{
		pm->RegisterEnemy(GetGameObject());
	}
}

void DigDugEnemyComponent::DestroyOverride(const SceneData& sceneData)
{
	if (GetGameObject() && GetGameObject()->GetScene())
	{
		GetGameObject()->GetScene()->Notify(GameEvent::EnemyDied, ObservedData{});
	}
	auto pm = sceneData.GetManager<EnemyManager>();
	if (pm)
	{
		pm->UnregisterEnemy(GetGameObject());
	}
}