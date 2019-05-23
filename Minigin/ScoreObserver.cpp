#include "MiniginPCH.h"
#include "ScoreObserver.h"
#include "GameEvents.h"
#include "ObservedData.h"
#include "GameObject.h"
#include "GameFiles.h"
#include "SDL.h"
#include "TextComponent.h"

ScoreObserver::ScoreObserver(const std::shared_ptr<Font>& pFont, const Color4& color, Scene* pScene)
	: m_pScene(pScene)
	, m_Color(color)
	, m_pFont(pFont)
	, m_Score(0)
{
}

void ScoreObserver::Notify(ObservedEvent event, const ObservedData& data)
{
	switch (event)
	{
	case GameEvent::GainedScore:
	{
		//Get Data
		GameObject* pObject = nullptr;
		int score = 0;
		if (m_pScene && data.GetData<int>("Score", score) && data.GetData<GameObject*>("GameObject", pObject) && pObject)
		{
			//Use Data
			if (score > 0)
			{
				//Create ScoreObject
				auto* pScoreObject = DigDug::CreateScore(*m_pScene, m_pFont, m_Color.SDLColor(), score);
				pScoreObject->GetTransform().SetWorldPosition(pObject->GetTransform().GetWorldPosition());
			}

			//Add Score
			m_Score += score;

			//Set Score in TextComponent
			if (GetGameObject())
			{
				TextComponent* pTextComponent = GetGameObject()->GetComponent<TextComponent>();
				if (pTextComponent)
				{
					pTextComponent->SetText(std::to_string(m_Score));
				}
			}

			//Log
			std::stringstream ss{};
			ss << "[Score] >> Gained Score: " << score << ", Total now counts: " << m_Score;
			Logger::GetInstance().LogInfo(ss.str());

		}
	}
	break;
	}


}
