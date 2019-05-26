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
	class EnemyManager final : public Engine::Manager
	{
	public:
		explicit EnemyManager() = default;
		virtual ~EnemyManager() = default;

		virtual void LateInitialize(const Engine::SceneData& sceneData) override;

		bool RegisterEnemy(Engine::GameObject* pEnemy, int id);
		bool UnregisterEnemy(Engine::GameObject* pEnemy);
		const std::map<int, std::pair<Engine::Vector2, Engine::GameObject*>>& GetEnemies() const { return m_Enemies; }
		Engine::GameObject* GetEnemy(int id) const;
		Engine::GameObject* GetClosestEnemy(const Engine::Vector2& t) const;
	private:
		std::map<int, std::pair<Engine::Vector2, Engine::GameObject*>> m_Enemies;
	};

}