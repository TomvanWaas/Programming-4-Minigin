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

	bool RegisterPlayer(GameObject* pPlayer);
	bool UnregisterPlayer(GameObject* pPlayer);
	const std::vector<GameObject*>& GetPlayers() const;
	GameObject* GetClosestPlayer(const Vector2& t) const;
private:
	std::vector<GameObject*> m_pPlayers;
};

