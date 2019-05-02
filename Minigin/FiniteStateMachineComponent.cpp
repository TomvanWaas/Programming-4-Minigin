#include "MiniginPCH.h"
#include "FiniteStateMachineComponent.h"
#include "FSMState.h"
#include "FSMEvent.h"
#include "FSMCondition.h"


FiniteStateMachineComponent::~FiniteStateMachineComponent()
{
	for (auto& pState : m_pSavedStates)
	{
		SAFE_DELETE(pState);
	}
	m_pSavedStates.clear();
	m_pCurrentState = nullptr;
}


void FiniteStateMachineComponent::UpdateFirstOverride(const SceneData& sceneData)
{
	m_StateChanged = false;
	if (m_pCurrentState != nullptr)
	{
		SetState(m_pCurrentState->UpdateFirst(sceneData), sceneData);
	}
}
void FiniteStateMachineComponent::UpdateSecondOverride(const SceneData& sceneData)
{
	if (m_pCurrentState != nullptr && !m_StateChanged)
	{
		SetState(m_pCurrentState->UpdateSecond(sceneData), sceneData);
	}
}

void FiniteStateMachineComponent::SetState(FSMState* pState, const SceneData& sceneData)
{
	if (pState == m_pCurrentState) return;
	if (m_pCurrentState != nullptr)
	{
		m_pCurrentState->Exit(sceneData);
	}
	if (pState != nullptr)
	{
		pState->Initialize(sceneData);
		pState->Enter(sceneData);
	}
	m_pCurrentState = pState;
	m_StateChanged = true;
}


void FiniteStateMachineComponent::SaveState(FSMState* pState)
{
	if (pState == nullptr) return;
	if (std::find(m_pSavedStates.begin(), m_pSavedStates.end(), pState) == m_pSavedStates.end())
	{
		m_pSavedStates.push_back(pState);
	}
}
void FiniteStateMachineComponent::SaveStates(const std::vector<FSMState*>& pStates)
{
	for (const auto& pState : pStates)
	{
		SaveState(pState);
	}
}

void FiniteStateMachineComponent::SaveCondition(const std::shared_ptr<FSMCondition>& pCondition)
{
	if (std::find(m_pSavedConditions.begin(), m_pSavedConditions.end(), pCondition) == m_pSavedConditions.end())
	{
		m_pSavedConditions.push_back(pCondition);
	}
}
void FiniteStateMachineComponent::SaveConditions(const std::vector<std::shared_ptr<FSMCondition>>& pConditions)
{
	for (const auto& pCondition : pConditions)
	{
		SaveCondition(pCondition);
	}
}

void FiniteStateMachineComponent::SaveEvent(const std::shared_ptr<FSMEvent>& pEvent)
{
	if (std::find(m_pSavedEvents.begin(), m_pSavedEvents.end(), pEvent) == m_pSavedEvents.end())
	{
		m_pSavedEvents.push_back(pEvent);
	}
}
void FiniteStateMachineComponent::SaveEvents(const std::vector<std::shared_ptr<FSMEvent>>& pEvents)
{
	for (const auto& pEvent: pEvents)
	{
		SaveEvent(pEvent);
	}
}
