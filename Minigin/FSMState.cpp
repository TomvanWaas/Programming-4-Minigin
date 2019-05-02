#include "MiniginPCH.h"
#include "FSMState.h"
#include "FSMEvent.h"
#include "FSMCondition.h"


void FSMState::Initialize(const SceneData& sceneData)
{
	if (m_pOnEnterEvent != nullptr) m_pOnEnterEvent->Initialize(sceneData);
	if (m_pOnExitEvent != nullptr) m_pOnExitEvent->Initialize(sceneData);
	if (m_pUpdateFirstEvent != nullptr)m_pUpdateFirstEvent->Initialize(sceneData);
	if (m_pUpdateSecondEvent != nullptr) m_pUpdateSecondEvent->Initialize(sceneData);
	for (auto& trans: m_pTransitions)
	{
		if (trans != nullptr && trans->pCondition != nullptr) trans->pCondition->Initialize(sceneData);
	}
}

FSMState* FSMState::UpdateFirst(const SceneData& sceneData)
{
	//Check Transitions
	for (const auto& pTransition : m_pTransitions)
	{
		if (pTransition->pCondition != nullptr && pTransition->pCondition->Execute(sceneData))
		{
			return pTransition->pState;
		}
	}

	//Else UpdateFirstOverride Events
	if (m_pUpdateFirstEvent != nullptr) m_pUpdateFirstEvent->Execute(sceneData);
	return this;
}
FSMState* FSMState::UpdateSecond(const SceneData& sceneData)
{
	//Check Transitions
	for (const auto& pTransition : m_pTransitions)
	{
		if (pTransition->pCondition != nullptr && pTransition->pCondition->Execute(sceneData))
		{
			return pTransition->pState;
		}
	}

	//Else UpdateFirstOverride Events
	if (m_pUpdateSecondEvent != nullptr) m_pUpdateSecondEvent->Execute(sceneData);
	return this;
}


void FSMState::AddTransition(const std::shared_ptr<FSMTransition>& pTransition)
{
	if (std::find(m_pTransitions.begin(), m_pTransitions.end(), pTransition) == m_pTransitions.end())
	{
		m_pTransitions.push_back(pTransition);
	}
}
void FSMState::SetUpdateFirstEvent(const std::shared_ptr<FSMEvent>& pEvent)
{
	m_pUpdateFirstEvent = pEvent;
}
void FSMState::SetUpdateSecondEvent(const std::shared_ptr<FSMEvent>& pEvent)
{
	m_pUpdateSecondEvent = pEvent;
}
void FSMState::SetEnterEvent(const std::shared_ptr<FSMEvent>& pEvent)
{
	m_pOnEnterEvent = pEvent;
}
void FSMState::SetExitEvent(const std::shared_ptr<FSMEvent>& pEvent)
{
	m_pOnExitEvent = pEvent;
}



void FSMState::Enter(const SceneData& sceneData)
{
	if (m_pOnEnterEvent != nullptr) m_pOnEnterEvent->Execute(sceneData);
}
void FSMState::Exit(const SceneData& sceneData)
{
	if (m_pOnExitEvent != nullptr) m_pOnExitEvent->Execute(sceneData);
}




