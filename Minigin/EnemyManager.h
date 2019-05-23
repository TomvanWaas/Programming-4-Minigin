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

	virtual void LateInitialize(const SceneData& sceneData) override;

	bool RegisterEnemy(GameObject* pEnemy);
	bool UnregisterEnemy(GameObject* pEnemy);
	const std::vector<std::pair<Vector2, GameObject*>>& GetEnemies() const { return m_Enemies; }

	GameObject* GetClosestEnemy(const Vector2& t) const;
private:
	std::vector<std::pair<Vector2, GameObject*>> m_Enemies;
};

