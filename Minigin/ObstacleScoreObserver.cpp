#include "MiniginPCH.h"
#include "ObstacleScoreObserver.h"
#include "AABBCollisionComponent.h"
#include "ObservedData.h"
#include "GameEvents.h"

void ObstacleScoreObserver::Notify(ObservedEvent event, const ObservedData& data)
{
	switch (event)
	{
	case ObservedEvent::ColliderEntered:
	{
		AABBCollisionComponent* pCollider = nullptr;
		if (data.GetData<AABBCollisionComponent*>("Collider", pCollider) && pCollider != nullptr)
		{
			if (pCollider->GetTag() == "Enemy" && pCollider->GetGameObject() != nullptr)
			{
				if (m_Scores.find(pCollider->GetGameObject()) != m_Scores.end())
				{
					++m_Scores[pCollider->GetGameObject()];
				}
				else
				{
					m_Scores[pCollider->GetGameObject()] = 1;
				}
			}
		}
	}
	break;
	case ObservedEvent::Destroyed:
	{
		GameObject* pObject = nullptr;
		if (data.GetData<GameObject*>("GameObject", pObject) && pObject != nullptr && m_Scores.find(pObject) != m_Scores.end())
		{
			ObservedData d{};
			d.AddData<int>("Score", CalcScore(m_Scores[pObject]));
			NotifyObservers(GameEvent::GainedScore, d);
			if (m_Scores.find(pObject) != m_Scores.end()) m_Scores.erase(m_Scores.find(pObject));
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
