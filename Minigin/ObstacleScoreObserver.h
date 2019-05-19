#pragma once
#include "Observer.h"
class ObstacleScoreObserver final : public Observer, public Subject
{
public:
	explicit ObstacleScoreObserver() = default;
	virtual ~ObstacleScoreObserver() = default;
	virtual void Notify(ObservedEvent event, const ObservedData& data) override;
private:
	int m_HitAmount;

	int CalcScore(int n);
};

