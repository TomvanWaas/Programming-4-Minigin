#pragma once
#include "Manager.h"
struct Vector2;
class Transform;
class GameObject;
class EnemyManager final : public Manager
{
public:
	explicit EnemyManager() = default;
	virtual ~EnemyManager() = default;

	bool RegisterEnemy(GameObject* pPlayer);
	bool UnregisterEnemy(GameObject* pPlayer);
	const std::vector<GameObject*>& GetEnemies() const;
	GameObject* GetClosestEnemy(const Vector2& t) const;
private:
	std::vector<GameObject*> m_pEnemies;
};

