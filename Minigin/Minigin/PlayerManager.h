#pragma once
#include "Manager.h"
#include <map>

namespace Minigin
{
	struct Vector2;
	class Transform;
	class GameObject;
}
namespace DigDug
{
	class PlayerManager final : public Minigin::Manager
	{
	public:
		explicit PlayerManager() = default;
		virtual ~PlayerManager() = default;
		virtual void LateInitialize(const Minigin::SceneData& sceneData) override;
		bool RegisterPlayer(Minigin::GameObject* pPlayer, int id);
		bool UnregisterPlayer(Minigin::GameObject* pPlayer);
		Minigin::GameObject* GetPlayer(int id) const;
		Minigin::Vector2 GetInitialPosition(int id) const;
		const std::map<int, std::pair<Minigin::Vector2, Minigin::GameObject*>>& GetPlayers() const { return m_Players; }


		Minigin::GameObject* GetClosestPlayer(const Minigin::Vector2& t) const;
	private:
		std::map<int, std::pair<Minigin::Vector2, Minigin::GameObject*>> m_Players;
	};

}