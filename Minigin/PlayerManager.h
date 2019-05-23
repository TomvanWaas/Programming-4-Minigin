#pragma once
#include "Manager.h"
#include <map>
struct Vector2;
class Transform;
class GameObject;
class PlayerManager final : public Manager
{
public:
	explicit PlayerManager() = default;
	virtual ~PlayerManager() = default;
	virtual void LateInitialize(const SceneData& sceneData) override;
	bool RegisterPlayer(GameObject* pPlayer, int id = 0);
	bool UnregisterPlayer(GameObject* pPlayer);
	GameObject* GetPlayer(int id) const;
	Vector2 GetInitialPosition(int id) const;
	const std::map<int, GameObject*>& GetPlayers() const;
	const std::map<int,Vector2>& GetInitPositions() const { return m_InitialPositions; }


	GameObject* GetClosestPlayer(const Vector2& t) const;
private:
	std::map<int, GameObject*> m_pPlayers;
	std::map<int,Vector2> m_InitialPositions;
};

