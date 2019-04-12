#include "MiniginPCH.h"
#include "FSMState.h"
#include "FSMEvent.h"
#include "FSMCondition.h"


bool FSMTransition::MeetsConditions(const SceneData& sceneData) const
{
	bool b = true;
	for (const auto& pCondition : m_pConditions)
	{
		b = b && pCondition->Execute(sceneData);
	}
	return b;
}

FSMState* FSMState::Update(const SceneData& sceneData)
{
	//Check Transitions
	for (const auto& pTransition: m_pTransitions)
	{
		if (pTransition->MeetsConditions(sceneData))
		{
			FSMState* pState = pTransition->m_pState;

			//Exit this
			Exit(sceneData);

			//Enter new stat
			if (pState != nullptr)
			{
				pState->Enter(sceneData);
			}

			//Return
			return pState;
		}
	}

	//Else Update Events
	for (auto& pEvent: m_pUpdateEvents)
	{
		pEvent->Execute(sceneData);		
	}
	return this;
}



void FSMState::AddTransition(const std::shared_ptr<FSMTransition>& pTransition)
{
	if (std::find(m_pTransitions.begin(), m_pTransitions.end(), pTransition) == m_pTransitions.end())
	{
		m_pTransitions.push_back(pTransition);
	}
}
void FSMState::AddUpdateEvent(const std::shared_ptr<FSMEvent>& pEvent)
{
	if (std::find(m_pUpdateEvents.begin(), m_pUpdateEvents.end(), pEvent) == m_pUpdateEvents.end())
	{
		m_pUpdateEvents.push_back(pEvent);
	}
}
void FSMState::AddEnterEvent(const std::shared_ptr<FSMEvent>& pEvent)
{
	if (std::find(m_pOnEnterEvents.begin(), m_pOnEnterEvents.end(), pEvent) == m_pOnEnterEvents.end())
	{
		m_pOnEnterEvents.push_back(pEvent);
	}
}
void FSMState::AddExitEvent(const std::shared_ptr<FSMEvent>& pEvent)
{
	if (std::find(m_pOnExitEvents.begin(), m_pOnExitEvents.end(), pEvent) == m_pOnExitEvents.end())
	{
		m_pOnExitEvents.push_back(pEvent);
	}
}



void FSMState::Enter(const SceneData& sceneData)
{
	for (auto& pEvent: m_pOnEnterEvents)
	{
		pEvent->Execute(sceneData);
	}
}
void FSMState::Exit(const SceneData& sceneData)
{
	for (auto& pEvent: m_pOnExitEvents)
	{
		pEvent->Execute(sceneData);
	}
}




