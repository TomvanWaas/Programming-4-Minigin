#pragma once
#include "Observer.h"
#include "GameObject.h"
#include <map>

class ObstacleScoreObserver final : public Observer, public Subject
{
public:
	explicit ObstacleScoreObserver() = default;
	virtual ~ObstacleScoreObserver() = default;
	virtual void Notify(ObservedEvent event, const ObservedData& data) override;
private:
	std::map<GameObject*, int> m_Scores;

	int CalcScore(int n);
};

