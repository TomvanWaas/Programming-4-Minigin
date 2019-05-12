#include "MiniginPCH.h"
#include "FiniteStateMachineComponent.h"
#include "FSMState.h"
#include "Scene.h"
#include "GameObject.h"

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
		SetState(m_pCurrentState->UpdateFirst(sceneData, m_Data));
	}
}
void FiniteStateMachineComponent::UpdateSecondOverride(const SceneData& sceneData)
{
	if (m_pCurrentState != nullptr && !m_StateChanged)
	{
		SetState(m_pCurrentState->UpdateSecond(sceneData, m_Data));
	}
}
void FiniteStateMachineComponent::OnNotify(ObservedEvent event, const ObservedData& data)
{
	if (m_pCurrentState)
	{
		SetState(m_pCurrentState->OnNotify(event, data, m_Data));
	}
}

void FiniteStateMachineComponent::SetState(FSMState* pState)
{
	if (pState == m_pCurrentState) return;
	if (GetGameObject() && GetGameObject()->GetScene())
	{
		const auto& sceneData = GetGameObject()->GetScene()->GetSceneData();
		if (m_pCurrentState != nullptr )
		{
			m_pCurrentState->Exit(sceneData, m_Data);
		}
		if (pState != nullptr)
		{
			pState->Initialize(sceneData, m_Data);
			pState->Enter(sceneData, m_Data);
		}
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
