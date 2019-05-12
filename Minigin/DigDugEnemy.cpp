#include "MiniginPCH.h"
#include "DigDugEnemy.h"
#include "SceneData.h"
#include "EnemyManager.h"


void DigDugEnemy::InitializeOverride(const SceneData& sceneData)
{
	auto* pManager = sceneData.GetManager<EnemyManager>();
	if (pManager)
	{
		pManager->RegisterEnemy(GetGameObject());
	}
}

void DigDugEnemy::DestroyOverride(const SceneData& sceneData)
{
	auto* pManager = sceneData.GetManager<EnemyManager>();
	if (pManager)
	{
		pManager->UnregisterEnemy(GetGameObject());
	}
}
