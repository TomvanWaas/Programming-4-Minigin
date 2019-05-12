#include "MiniginPCH.h"
#include "EnemyManager.h"
#include <algorithm>
#include "GameObject.h"
#include "Transform.h"

bool EnemyManager::RegisterEnemy(GameObject* pPlayer)
{
	if (pPlayer != nullptr)
	{
		if (std::find(m_pEnemies.begin(), m_pEnemies.end(), pPlayer) == m_pEnemies.end())
		{
			m_pEnemies.push_back(pPlayer);
			return true;
		}
	}
	return false;
}

bool EnemyManager::UnregisterEnemy(GameObject* pPlayer)
{
	if (pPlayer != nullptr)
	{
		auto i = std::find(m_pEnemies.begin(), m_pEnemies.end(), pPlayer);
		if (i != m_pEnemies.end())
		{
			m_pEnemies.erase(i);
			return true;
		}
	}
	return false;
}

const std::vector<GameObject*>& EnemyManager::GetEnemies() const
{
	return m_pEnemies;
}

GameObject* EnemyManager::GetClosestEnemy(const Vector2& t) const
{
	GameObject* pClosest = nullptr;
	float sqDistance = std::numeric_limits<float>::infinity();
	for (GameObject* pPlayer : m_pEnemies)
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
