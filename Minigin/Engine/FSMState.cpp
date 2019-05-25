#include "stdafx.h"
#include "FSMState.h"
#include "FSMEvent.h"
#include "FSMCondition.h"
#include "FiniteStateMachineComponent.h"

using namespace Minigin;

void Minigin::FSMState::SetFSMComponent(FiniteStateMachineComponent* pComponent)
{
	m_pFSMComponent = pComponent;
}

FSMState* Minigin::FSMState::GetState(const std::string& name) const
{
	if (m_pFSMComponent) return m_pFSMComponent->GetStateByName(name);
	return nullptr;
}

GameObject* FSMState::GetGameObject() const
{
	if (m_pFSMComponent) return m_pFSMComponent->GetGameObject();
	return nullptr;
}

void FSMStateDefault::Initialize(const SceneData& sceneData, FSMData& data)
{
	//Initialize when this state is set
	if (m_pOnEnterEvent != nullptr) m_pOnEnterEvent->Initialize(sceneData, data);
	if (m_pOnExitEvent != nullptr) m_pOnExitEvent->Initialize(sceneData, data);
	if (m_pUpdateFirstEvent != nullptr)m_pUpdateFirstEvent->Initialize(sceneData, data);
	if (m_pUpdateSecondEvent != nullptr) m_pUpdateSecondEvent->Initialize(sceneData, data);
	if (m_pNotifiedEvent != nullptr) m_pNotifiedEvent->Initialize(sceneData, data);
	for (auto& trans: m_pTransitions)
	{
		if (trans != nullptr && trans->pCondition != nullptr) trans->pCondition->Initialize(sceneData, data);
	}
}

FSMState* FSMStateDefault::UpdateFirst(const SceneData& sceneData, FSMData& data)
{
	//Check Transitions
	for (const auto& pTransition : m_pTransitions)
	{
		if (pTransition->pCondition != nullptr && pTransition->pCondition->Execute(sceneData, data))
		{
			return pTransition->pState;
		}
	}

	//Else UpdateFirstOverride Events
	if (m_pUpdateFirstEvent != nullptr) m_pUpdateFirstEvent->Execute(sceneData, data);
	return this;
}
FSMState* FSMStateDefault::UpdateSecond(const SceneData& sceneData, FSMData& data)
{
	//Check Transitions
	for (const auto& pTransition : m_pTransitions)
	{
		if (pTransition->pCondition != nullptr && pTransition->pCondition->Execute(sceneData, data))
		{
			return pTransition->pState;
		}
	}

	//Else UpdateFirstOverride Events
	if (m_pUpdateSecondEvent != nullptr) m_pUpdateSecondEvent->Execute(sceneData, data);
	return this;
}

FSMState* FSMStateDefault::OnNotify(ObservedEvent oevent, const ObservedData& odata, FSMData& data)
{
	if (m_pNotifiedEvent) m_pNotifiedEvent->Execute(oevent, odata, data);
	return this;
}


void FSMStateDefault::AddTransition(const std::shared_ptr<FSMTransition>& pTransition)
{
	if (std::find(m_pTransitions.begin(), m_pTransitions.end(), pTransition) == m_pTransitions.end())
	{
		m_pTransitions.push_back(pTransition);
	}
}
void FSMStateDefault::SetUpdateFirstEvent(const std::shared_ptr<FSMEvent>& pEvent)
{
	m_pUpdateFirstEvent = pEvent;
}
void FSMStateDefault::SetUpdateSecondEvent(const std::shared_ptr<FSMEvent>& pEvent)
{
	m_pUpdateSecondEvent = pEvent;
}
void FSMStateDefault::SetEnterEvent(const std::shared_ptr<FSMEvent>& pEvent)
{
	m_pOnEnterEvent = pEvent;
}
void FSMStateDefault::SetExitEvent(const std::shared_ptr<FSMEvent>& pEvent)
{
	m_pOnExitEvent = pEvent;
}

void FSMStateDefault::SetNotifiedEvent(const std::shared_ptr<FSMNotifiedEvent>& pEvent)
{
	m_pNotifiedEvent = pEvent;
}


void FSMStateDefault::Enter(const SceneData& sceneData, FSMData& data)
{
	if (m_pOnEnterEvent != nullptr) m_pOnEnterEvent->Execute(sceneData, data);
}
void FSMStateDefault::Exit(const SceneData& sceneData, FSMData& data)
{
	if (m_pOnExitEvent != nullptr) m_pOnExitEvent->Execute(sceneData, data);
}




