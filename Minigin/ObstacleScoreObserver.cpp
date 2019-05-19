#include "MiniginPCH.h"
#include "ObstacleScoreObserver.h"
#include "ObservedData.h"
#include "GameEvents.h"
#include "GameObject.h"


void ObstacleScoreObserver::Notify(ObservedEvent event, const ObservedData& data)
{
	switch (event)
	{
	case GameEvent::ObstacleHit:
	{
		++m_HitAmount;
	}
	break;
	case ObservedEvent::Destroyed:
	{
		GameObject* pSelf = nullptr;
		if (data.GetData<GameObject*>("GameObject", pSelf) && pSelf)
		{
			//Calc Score
			int score = CalcScore(m_HitAmount);

			//Notify
			ObservedData d{};
			d.AddData<int>("Score", score);
			d.AddData<GameObject*>("GameObject", pSelf);
			NotifyObservers(GameEvent::GainedScore, d);


		}
	}
	break;
	}
}

int ObstacleScoreObserver::CalcScore(int n)
{
	switch (n)
	{
	case 0:
		return 0;
	case 1:
		return 1000;
	case 2:
		return 2500;
	case 3:
		return 4000;
	case 4:
		return 6000;
	case 5:
		return 8000;
	case 6:
		return 10000;
	case 7:
		return 12000;
	default:
		return 15000;
	}
}
