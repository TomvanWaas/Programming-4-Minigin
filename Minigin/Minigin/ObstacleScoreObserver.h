#pragma once
#include "Observer.h"

namespace DigDug
{
	class ObstacleScoreObserver final : public Engine::Observer, public Engine::Subject
	{
	public:
		explicit ObstacleScoreObserver() = default;
		virtual ~ObstacleScoreObserver() = default;
		virtual void Notify(Engine::ObservedEvent event, const Engine::ObservedData& data) override;
	private:
		int m_HitAmount;

		int CalcScore(int n);
	};

}