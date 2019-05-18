#pragma once
#include "Manager.h"
struct Vector2;
class Transform;
class GameObject;
class PlayerManager final : public Manager
{
public:
	explicit PlayerManager() = default;
	virtual ~PlayerManager() = default;
	virtual void LateInitialize(const SceneData& sceneData) override;
	bool RegisterPlayer(GameObject* pPlayer);
	bool UnregisterPlayer(GameObject* pPlayer);
	const std::vector<GameObject*>& GetPlayers() const;
	const std::vector<Vector2>& GetInitPositions() const { return m_InitialPositions; }

	GameObject* GetClosestPlayer(const Vector2& t) const;
private:
	std::vector<GameObject*> m_pPlayers;
	std::vector<Vector2> m_InitialPositions;
};

