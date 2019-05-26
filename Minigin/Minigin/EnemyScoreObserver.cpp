#include "MiniginPCH.h"
#include "EnemyScoreObserver.h"
#include "GameObject.h"
#include "Transform.h"
#include "ObservedData.h"
#include "GameEvents.h"
#include "EngineObject.h"
#include "GameFiles.h"
#include "SDL.h"
#include "PlayerManager.h"
#include "Scene.h"
#include "WindowSettings.h"

using namespace Engine;
using namespace DigDug;

EnemyScoreObserver::EnemyScoreObserver(Type type, Engine::Scene* pScene, float laneHeight)
	: m_Type(type)
	, m_LaneHeight(laneHeight)
	, m_pScene(pScene)
{
	
}

void EnemyScoreObserver::Notify(ObservedEvent event, const ObservedData& data)
{
	switch (event)
	{
	case ObservedEvent::Destroyed:
		{
		Engine::GameObject* pObject = nullptr;
			if (data.GetData("GameObject", pObject) && pObject != nullptr)
			{
				ObservedData d{};
				int score = GetScore(pObject->GetTransform().GetWorldPosition());
				d.AddData<int>("Score", score);
				d.AddData<Engine::GameObject*>("GameObject", pObject);
				NotifyObservers(GameEvent::GainedScore, d);
			}
		}
		break;
	}
}

int EnemyScoreObserver::GetScore(const Engine::Vector2& worldPos)
{
	const auto& wsettings = Engine::EngineObject::GetWindowSettings();
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
	if (m_Type == Type::Fygar)
	{
		//Check if on same lane
		if (m_pScene)
		{
			auto pm = m_pScene->GetSceneData().GetManager<PlayerManager>();
			if (pm)
			{
				auto p = pm->GetClosestPlayer(worldPos);
				if (p)
				{
					float d = p->GetTransform().GetWorldPosition().y - worldPos.y;
					if (d*d <= m_LaneHeight*m_LaneHeight)
					{
						//Mul 2
						score *= 2;
					}
				}
			}
		}
	}
	return score;
}
