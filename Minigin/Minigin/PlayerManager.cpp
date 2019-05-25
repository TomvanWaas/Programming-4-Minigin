#include "MiniginPCH.h"
#include "PlayerManager.h"
#include <algorithm>
#include "GameObject.h"
#include "Transform.h"

using namespace Minigin;
using namespace DigDug;

void PlayerManager::LateInitialize(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);
	for (auto& player : m_Players)
	{
		if (player.second.second)
		{
			player.second.first = player.second.second->GetTransform().GetWorldPosition();
		}
	}
}

bool PlayerManager::RegisterPlayer(GameObject* pPlayer, int id)
{
	if (pPlayer != nullptr)
	{
		auto i = m_Players.find(id);
		if (i == m_Players.end())
		{
			m_Players[id] = std::pair<Vector2, GameObject*>(Vector2::Zero, pPlayer);
			return true;
		}
	}
	return false;
}

bool PlayerManager::UnregisterPlayer(GameObject* pPlayer)
{
	if (pPlayer != nullptr)
	{
		auto i = std::find_if(m_Players.begin(), m_Players.end(), [&pPlayer](const std::pair<int, std::pair<Vector2, GameObject*>>& pair)
		{
			return pair.second.second == pPlayer;
		});
		if (i != m_Players.end())
		{
			m_Players.erase(i);
			return true;
		}
	}
	return false;
}

GameObject* PlayerManager::GetPlayer(int id) const
{
	auto i = m_Players.find(id);
	if (i != m_Players.end())
	{
		return (*i).second.second;
	}
	return nullptr;
}

Vector2 PlayerManager::GetInitialPosition(int id) const
{
	auto i = m_Players.find(id);
	if (i != m_Players.end())
	{
		return (*i).second.first;
	}
	return Vector2::Zero;
}



GameObject* PlayerManager::GetClosestPlayer(const Vector2& t) const
{
	GameObject* pClosest = nullptr;
	float sqDistance = std::numeric_limits<float>::infinity();
	for (const auto& pPlayer : m_Players)
	{
		if (pPlayer.second.second != nullptr)
		{
			float sqD = t.SqrDistance(pPlayer.second.second->GetTransform().GetWorldPosition());
			if (sqD < sqDistance)
			{
				sqDistance = sqD;
				pClosest = pPlayer.second.second;
			}
		}
	}
	return pClosest;
}
