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
	const std::vector<GameObject*>& GetEnemies() const;
	const std::vector<Vector2>& GetInitPositions() const { return m_InitialPositions; }

	GameObject* GetClosestEnemy(const Vector2& t) const;
private:
	std::vector<GameObject*> m_pEnemies;
	std::vector<Vector2> m_InitialPositions;
};

