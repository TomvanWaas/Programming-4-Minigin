#include "MiniginPCH.h"
#include "EnemyManager.h"
#include <algorithm>
#include "GameObject.h"
#include "Transform.h"

using namespace Engine;
using namespace DigDug;

void EnemyManager::LateInitialize(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);
	for( auto& enemy : m_Enemies)
	{
		if (enemy.second.second)
		{
			enemy.second.first = enemy.second.second->GetTransform().GetWorldPosition();
		}
	}
}

bool EnemyManager::RegisterEnemy(GameObject* pEnemy, int id)
{
	if (pEnemy != nullptr)
	{
		auto i = m_Enemies.find(id);
		if (i == m_Enemies.end())
		{
			m_Enemies[id] = std::pair<Vector2, GameObject*>(Vector2::Zero, pEnemy);
			return true;
		}
	}
	return false;
}

bool EnemyManager::UnregisterEnemy(GameObject* pEnemy)
{
	if (pEnemy != nullptr)
	{
		auto i = std::find_if(m_Enemies.begin(), m_Enemies.end(), [&pEnemy](const std::pair<int,std::pair<Vector2, GameObject*>>& pair)
		{
			return pair.second.second == pEnemy;
		});
		if (i != m_Enemies.end())
		{
			m_Enemies.erase(i);
		}
	}
	return false;
}

GameObject* EnemyManager::GetEnemy(int id) const
{
	if (m_Enemies.find(id) != m_Enemies.end())
	{
		return m_Enemies.at(id).second;
	}
	return nullptr;
}

GameObject* EnemyManager::GetClosestEnemy(const Vector2& t) const
{
	GameObject* pClosest = nullptr;
	float sqDistance = std::numeric_limits<float>::infinity();
	for (const auto& enemy : m_Enemies)
	{
		if (enemy.second.second != nullptr)
		{
			float sqD = t.SqrDistance(enemy.second.second->GetTransform().GetWorldPosition());
			if (sqD < sqDistance)
			{
				sqDistance = sqD;
				pClosest = enemy.second.second;
			}
		}
	}
	return pClosest;
}
