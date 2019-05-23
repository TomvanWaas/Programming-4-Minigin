#include "MiniginPCH.h"
#include "PlayerManager.h"
#include <algorithm>
#include "GameObject.h"
#include "Transform.h"

void PlayerManager::LateInitialize(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);
	for (const auto& pPlayer : m_pPlayers)
	{
		if (pPlayer.second) m_InitialPositions[pPlayer.first] = pPlayer.second->GetTransform().GetWorldPosition();
	}
}

bool PlayerManager::RegisterPlayer(GameObject* pPlayer, int id)
{
	if (pPlayer != nullptr)
	{
		auto i = m_pPlayers.find(id);
		if (i == m_pPlayers.end())
		{
			m_pPlayers[id] = pPlayer;
			return true;
		}
	}
	return false;
}

bool PlayerManager::UnregisterPlayer(GameObject* pPlayer)
{
	if (pPlayer != nullptr)
	{
		auto i = std::find_if(m_pPlayers.begin(), m_pPlayers.end(), [&pPlayer](const std::pair<int,GameObject*>& p)
		{
			return p.second == pPlayer;
		});
		if (i != m_pPlayers.end())
		{
			m_pPlayers.erase(i);
			return true;
		}
	}
	return false;
}

GameObject* PlayerManager::GetPlayer(int id) const
{
	if (m_pPlayers.find(id) != m_pPlayers.end())
	{
		return m_pPlayers.at(id);
	}
	return nullptr;
}

Vector2 PlayerManager::GetInitialPosition(int id) const
{
	if (m_InitialPositions.find(id) != m_InitialPositions.end())
	{
		return m_InitialPositions.at(id);
	}
	return Vector2::Zero;
}

const std::map<int,GameObject*>& PlayerManager::GetPlayers() const
{
	return m_pPlayers;
}


GameObject* PlayerManager::GetClosestPlayer(const Vector2& t) const
{
	GameObject* pClosest = nullptr;
	float sqDistance = std::numeric_limits<float>::infinity();
	for (const auto& pPlayer : m_pPlayers)
	{
		if (pPlayer.second != nullptr)
		{
			float sqD = t.SqrDistance(pPlayer.second->GetTransform().GetWorldPosition());
			if (sqD < sqDistance)
			{
				sqDistance = sqD;
				pClosest = pPlayer.second;
			}
		}
	}
	return pClosest;
}
