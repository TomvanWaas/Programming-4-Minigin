#include "MiniginPCH.h"
#include "EnemyScoreObserver.h"
#include "GameObject.h"
#include "Transform.h"
#include "ObservedData.h"
#include "GameEvents.h"
#include "Minigin.h"

void EnemyScoreObserver::Notify(ObservedEvent event, const ObservedData& data)
{
	switch (event)
	{
	case ObservedEvent::Destroyed:
		{
		GameObject* pObject = nullptr;
			if (data.GetData("GameObject", pObject) && pObject != nullptr)
			{
				ObservedData d{};
				d.AddData<int>("Score", GetScore(pObject->GetTransform().GetWorldPosition()));
				NotifyObservers(GameEvent::GainedScore, d);
			}
		}
		break;
	}
}

int EnemyScoreObserver::GetScore(const Vector2& worldPos)
{
	const auto& wsettings = dae::Minigin::GetWindowSettings();
	float frac = worldPos.y / wsettings.height;
	int score = 0;

	//Pixels (in Resource Image)
	if (frac <= 0.3472f) //100 / 288
	{
		score = 200;
	}
	else if (frac <= 0.566f) //163 / 288
	{
		score = 300;
	}
	else if (frac <= 0.7882f) //227 / 288
	{
		score = 400;
	}
	else
	{
		score = 500;
	}
	if (m_Type == Type::Fygar) score *= 2;
	return score;
}
