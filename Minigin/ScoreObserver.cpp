#include "MiniginPCH.h"
#include "ScoreObserver.h"
#include "GameEvents.h"
#include "ObservedData.h"

void ScoreObserver::Notify(ObservedEvent event, const ObservedData& data)
{
	switch (event)
	{
	case GameEvent::GainedScore:
	{
		int score = 0;
		if (data.GetData<int>("Score", score))
		{
			m_Score += score;
			std::stringstream ss{};
			ss << "[Score] >> Gained Score: " << score << ", Total now counts: " << m_Score;
			Logger::GetInstance().LogInfo(ss.str());
		}
	}
	break;
	}


}
