#pragma once
#include "Observer.h"
class ScoreObserver final : public Observer
{
public:
	explicit ScoreObserver() = default;
	virtual ~ScoreObserver() = default;

	virtual void Notify(ObservedEvent event, const ObservedData& data) override;
private:
	int m_Score = 0;
	

};

