#include "MiniginPCH.h"
#include "FiniteStateMachineComponent.h"
#include "FSMState.h"
#include "Scene.h"
#include "GameObject.h"

FiniteStateMachineComponent::~FiniteStateMachineComponent()
{
	for (auto& pState : m_pSavedStates)
	{
		SAFE_DELETE(pState.second);
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


bool FiniteStateMachineComponent::SaveState(const std::string& name, FSMState* pState)
{
	if (pState == nullptr) return false;
	auto i = m_pSavedStates.find(name);
	if (i == m_pSavedStates.end())
	{
		m_pSavedStates[name] = pState;
		pState->SetFSMComponent(this);
		return true;
	}
	return false;
}

FSMState* FiniteStateMachineComponent::GetState(const std::string& name) const
{
	auto i = m_pSavedStates.find(name);
	if (i != m_pSavedStates.end())
	{
		return (*i).second;
	}
	return nullptr;
}
