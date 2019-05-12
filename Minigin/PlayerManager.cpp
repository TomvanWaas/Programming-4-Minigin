#include "MiniginPCH.h"
#include "PlayerManager.h"
#include <algorithm>
#include "GameObject.h"
#include "Transform.h"

bool PlayerManager::RegisterPlayer(GameObject* pPlayer)
{
	if (pPlayer != nullptr)
	{
		if (std::find(m_pPlayers.begin(), m_pPlayers.end(), pPlayer) == m_pPlayers.end())
		{
			m_pPlayers.push_back(pPlayer);
			return true;
		}
	}
	return false;
}

bool PlayerManager::UnregisterPlayer(GameObject* pPlayer)
{
	if (pPlayer != nullptr)
	{
		auto i = std::find(m_pPlayers.begin(), m_pPlayers.end(), pPlayer);
		if (i != m_pPlayers.end())
		{
			m_pPlayers.erase(i);
			return true;
		}
	}
	return false;
}

const std::vector<GameObject*>& PlayerManager::GetPlayers() const
{
	return m_pPlayers;
}


GameObject* PlayerManager::GetClosestPlayer(const Vector2& t) const
{
	GameObject* pClosest = nullptr;
	float sqDistance = std::numeric_limits<float>::infinity();
	for (GameObject* pPlayer : m_pPlayers)
	{
		if (pPlayer != nullptr)
		{
			float sqD = t.SqrDistance(pPlayer->GetTransform().GetWorldPosition());
			if (sqD < sqDistance)
			{
				sqDistance = sqD;
				pClosest = pPlayer;
			}
		}
	}
	return pClosest;
}
