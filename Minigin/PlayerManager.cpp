#include "MiniginPCH.h"
#include "PlayerManager.h"
#include <algorithm>
#include "GameObject.h"
#include "Transform.h"

void PlayerManager::LateInitialize(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);
	for (const auto* pPlayer : m_pPlayers)
	{
		if (pPlayer)
		{
			m_InitialPositions.push_back(pPlayer->GetTransform().GetWorldPosition());
		}
	}
}

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
		for (size_t i = 0; i < m_pPlayers.size(); ++i)
		{
			if (m_pPlayers[i] == pPlayer)
			{
				m_pPlayers.erase(m_pPlayers.begin() + i);
				if (m_InitialPositions.size() > i)
				{
					m_InitialPositions.erase(m_InitialPositions.begin() + i);
				}
				return true;
			}
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
