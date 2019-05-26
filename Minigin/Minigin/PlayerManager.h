#pragma once
#include "Manager.h"
#include <map>

namespace Engine
{
	struct Vector2;
	class Transform;
	class GameObject;
}
namespace DigDug
{
	class PlayerManager final : public Engine::Manager
	{
	public:
		explicit PlayerManager() = default;
		virtual ~PlayerManager() = default;
		virtual void LateInitialize(const Engine::SceneData& sceneData) override;
		bool RegisterPlayer(Engine::GameObject* pPlayer, int id);
		bool UnregisterPlayer(Engine::GameObject* pPlayer);
		Engine::GameObject* GetPlayer(int id) const;
		Engine::Vector2 GetInitialPosition(int id) const;
		const std::map<int, std::pair<Engine::Vector2, Engine::GameObject*>>& GetPlayers() const { return m_Players; }


		Engine::GameObject* GetClosestPlayer(const Engine::Vector2& t) const;
	private:
		std::map<int, std::pair<Engine::Vector2, Engine::GameObject*>> m_Players;
	};

}