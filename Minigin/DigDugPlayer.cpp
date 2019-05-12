#include "MiniginPCH.h"
#include "DigDugPlayer.h"
#include "PlayerManager.h"
#include "SceneData.h"

void DigDugPlayer::InitializeOverride(const SceneData& sceneData)
{
	auto* pManager = sceneData.GetManager<PlayerManager>();
	if (pManager)
	{
		pManager->RegisterPlayer(GetGameObject());
	}
}

void DigDugPlayer::DestroyOverride(const SceneData& sceneData)
{
	auto* pManager = sceneData.GetManager<PlayerManager>();
	if (pManager)
	{
		pManager->UnregisterPlayer(GetGameObject());
	}
}
