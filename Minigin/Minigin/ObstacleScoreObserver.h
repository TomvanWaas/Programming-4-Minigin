#pragma once
#include "Observer.h"

namespace DigDug
{
	class ObstacleScoreObserver final : public Minigin::Observer, public Minigin::Subject
	{
	public:
		explicit ObstacleScoreObserver() = default;
		virtual ~ObstacleScoreObserver() = default;
		virtual void Notify(Minigin::ObservedEvent event, const Minigin::ObservedData& data) override;
	private:
		int m_HitAmount;

		int CalcScore(int n);
	};

}