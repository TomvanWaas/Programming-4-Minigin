#pragma once
#include "Observer.h"

namespace Engine
{
	struct Vector2;
	class Font;
	class Scene;
}
namespace DigDug
{
	class EnemyScoreObserver final : public Engine::Observer, public Engine::Subject
	{

	public:
		enum class Type
		{
			Pooka,
			Fygar
		};
		explicit EnemyScoreObserver(Type type, Engine::Scene* pScene = nullptr, float laneHeight = 0.0f);
		virtual ~EnemyScoreObserver() = default;
		virtual void Notify(Engine::ObservedEvent event, const Engine::ObservedData& data) override;
	private:
		int GetScore(const Engine::Vector2& worldPos);

		Type m_Type;
		float m_LaneHeight;
		Engine::Scene* m_pScene;
	};

}