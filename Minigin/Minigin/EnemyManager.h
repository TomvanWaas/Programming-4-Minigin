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
	class EnemyManager final : public Minigin::Manager
	{
	public:
		explicit EnemyManager() = default;
		virtual ~EnemyManager() = default;

		virtual void LateInitialize(const Minigin::SceneData& sceneData) override;

		bool RegisterEnemy(Minigin::GameObject* pEnemy, int id);
		bool UnregisterEnemy(Minigin::GameObject* pEnemy);
		const std::map<int, std::pair<Minigin::Vector2, Minigin::GameObject*>>& GetEnemies() const { return m_Enemies; }
		Minigin::GameObject* GetEnemy(int id) const;
		Minigin::GameObject* GetClosestEnemy(const Minigin::Vector2& t) const;
	private:
		std::map<int, std::pair<Minigin::Vector2, Minigin::GameObject*>> m_Enemies;
	};

}