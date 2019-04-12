#include "MiniginPCH.h"
#include "FiniteStateMachineComponent.h"
#include "FSMState.h"
#include "FSMEvent.h"
#include "FSMCondition.h"


FiniteStateMachineComponent::FiniteStateMachineComponent(const std::vector<FSMState*>& pStates,
	FSMState* pStartState)
	: BaseComponent()
	, m_pSavedStates(pStates)
	, m_pCurrentState(pStartState)
{}
FiniteStateMachineComponent::~FiniteStateMachineComponent()
{
	for (auto& pState : m_pSavedStates)
	{
		SAFE_DELETE(pState);
	}
	m_pSavedStates.clear();
	m_pCurrentState = nullptr;
}


void FiniteStateMachineComponent::Initialize(const SceneData& sceneData)
{
	for (auto& pEvent: m_pSavedEvents)
	{
		pEvent->Initialize(sceneData);
	}
	for (auto& pCondition: m_pSavedConditions)
	{
		pCondition->Initialize(sceneData);
	}
}
void FiniteStateMachineComponent::ReInitialize(const SceneData& sceneData)
{
	for (auto& pEvent : m_pSavedEvents)
	{
		pEvent->ReInitialize(sceneData);
	}
	for (auto& pCondition : m_pSavedConditions)
	{
		pCondition->ReInitialize(sceneData);
	}
}
void FiniteStateMachineComponent::Update(const SceneData& sceneData)
{
	if (m_pCurrentState != nullptr)
	{
		m_pCurrentState = m_pCurrentState->Update(sceneData);
	}
}



void FiniteStateMachineComponent::SaveState(FSMState* pState)
{
	if (pState == nullptr) return;
	if (std::find(m_pSavedStates.begin(), m_pSavedStates.end(), pState) == m_pSavedStates.end())
	{
		m_pSavedStates.push_back(pState);
	}
}
void FiniteStateMachineComponent::SaveCondition(const std::shared_ptr<FSMCondition>& pCondition)
{
	if (std::find(m_pSavedConditions.begin(), m_pSavedConditions.end(), pCondition) == m_pSavedConditions.end())
	{
		m_pSavedConditions.push_back(pCondition);
	}
}
void FiniteStateMachineComponent::SaveEvent(const std::shared_ptr<FSMEvent>& pEvent)
{
	if (std::find(m_pSavedEvents.begin(), m_pSavedEvents.end(), pEvent) == m_pSavedEvents.end())
	{
		m_pSavedEvents.push_back(pEvent);
	}
}
