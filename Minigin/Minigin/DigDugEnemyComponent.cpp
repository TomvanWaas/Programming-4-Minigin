#include "MiniginPCH.h"
#include "DigDugEnemyComponent.h"
#include "SceneData.h"
#include "EnemyManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "GameEvents.h"
#include "ObservedData.h"

using namespace DigDug;
using namespace Minigin;

void DigDugEnemyComponent::InitializeOverride(const SceneData& sceneData)
{
	auto pm = sceneData.GetManager<EnemyManager>();
	if (pm)
	{
		if (!pm->RegisterEnemy(GetGameObject(), m_Id)) Logger::GetInstance().LogWarning("DigDugEnemyComponent::InitializeOverride > Failed to register enemy");
	}
}

void DigDugEnemyComponent::DestroyOverride(const SceneData& sceneData)
{
	if (GetGameObject() && GetGameObject()->GetScene())
	{
		GetGameObject()->GetScene()->NotifyAll(GameEvent::EnemyDied, ObservedData{});
	}
	auto pm = sceneData.GetManager<EnemyManager>();
	if (pm)
	{
		pm->UnregisterEnemy(GetGameObject());
	}
}