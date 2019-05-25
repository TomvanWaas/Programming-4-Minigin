#pragma once
#include "Observer.h"

namespace Minigin
{
	struct Vector2;
	class Font;
	class Scene;
}
namespace DigDug
{
	class EnemyScoreObserver final : public Minigin::Observer, public Minigin::Subject
	{

	public:
		enum class Type
		{
			Pooka,
			Fygar
		};
		explicit EnemyScoreObserver(Type type, Minigin::Scene* pScene = nullptr, float laneHeight = 0.0f);
		virtual ~EnemyScoreObserver() = default;
		virtual void Notify(Minigin::ObservedEvent event, const Minigin::ObservedData& data) override;
	private:
		int GetScore(const Minigin::Vector2& worldPos);

		Type m_Type;
		float m_LaneHeight;
		Minigin::Scene* m_pScene;
	};

}