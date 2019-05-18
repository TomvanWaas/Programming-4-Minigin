#include "MiniginPCH.h"
#include "EnemyManager.h"
#include <algorithm>
#include "GameObject.h"
#include "Transform.h"

void EnemyManager::LateInitialize(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);
	for( const auto* pEnemy : m_pEnemies)
	{
		if (pEnemy)
		{
			m_InitialPositions.push_back(pEnemy->GetTransform().GetWorldPosition());
		}
	}
}

bool EnemyManager::RegisterEnemy(GameObject* pEnemy)
{
	if (pEnemy != nullptr)
	{
		if (std::find(m_pEnemies.begin(), m_pEnemies.end(), pEnemy) == m_pEnemies.end())
		{
			m_pEnemies.push_back(pEnemy);
			return true;
		}
	}
	return false;
}

bool EnemyManager::UnregisterEnemy(GameObject* pEnemy)
{
	if (pEnemy != nullptr)
	{
		for (size_t i = 0; i < m_pEnemies.size(); ++i)
		{
			if (m_pEnemies[i] == pEnemy)
			{
				m_pEnemies.erase(m_pEnemies.begin() + i);
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
