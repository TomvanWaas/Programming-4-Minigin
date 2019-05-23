#include "MiniginPCH.h"
#include "EnemyManager.h"
#include <algorithm>
#include "GameObject.h"
#include "Transform.h"

void EnemyManager::LateInitialize(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);
	for( auto& enemy : m_Enemies)
	{
		if (enemy.second)
		{
			enemy.first = enemy.second->GetTransform().GetWorldPosition();
		}
	}
}

bool EnemyManager::RegisterEnemy(GameObject* pEnemy)
{
	if (pEnemy != nullptr)
	{
		if (std::find_if(m_Enemies.begin(), m_Enemies.end(), [&pEnemy](const std::pair<Vector2, GameObject*>& pair)
		{
			return pair.second == pEnemy;
		}) == m_Enemies.end())
		{
			m_Enemies.push_back(std::pair<Vector2, GameObject*>(Vector2::Zero, pEnemy));
			return true;
		}
	}
	return false;
}

bool EnemyManager::UnregisterEnemy(GameObject* pEnemy)
{
	if (pEnemy != nullptr)
	{
		auto i = std::find_if(m_Enemies.begin(), m_Enemies.end(), [&pEnemy](const std::pair<Vector2, GameObject*>& pair)
		{
			return pair.second == pEnemy;
		});
		if (i != m_Enemies.end())
		{
			m_Enemies.erase(i);
		}
	}
	return false;
}

GameObject* EnemyManager::GetClosestEnemy(const Vector2& t) const
{
	GameObject* pClosest = nullptr;
	float sqDistance = std::numeric_limits<float>::infinity();
	for (const auto& enemy : m_Enemies)
	{
		if (enemy.second != nullptr)
		{
			float sqD = t.SqrDistance(enemy.second->GetTransform().GetWorldPosition());
			if (sqD < sqDistance)
			{
				sqDistance = sqD;
				pClosest = enemy.second;
			}
		}
	}
	return pClosest;
}
