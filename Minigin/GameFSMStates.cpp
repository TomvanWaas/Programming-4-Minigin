#include "MiniginPCH.h"
#include "GameFSMStates.h"
#include "Time.h"
#include "GameObject.h"
#include "Scene.h"


FSMStateDelay::FSMStateDelay(float delay, FSMState* pNext)
	: m_Accu(0)
	, m_Delay(delay)
	, m_pNextState(pNext)
{
}



void FSMStateDelay::Enter(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(data);
	UNREFERENCED_PARAMETER(sceneData);
	m_Accu = 0;
}

FSMState* FSMStateDelay::UpdateFirst(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(data);

	m_Accu += sceneData.GetTime()->GetDeltaTime();
	if (m_Accu >= m_Delay)
	{
		m_Accu = 0;
		return m_pNextState;
	}
	return this;
}


FSMState* FSMStateDestroyObject::UpdateFirst(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(sceneData);
	UNREFERENCED_PARAMETER(data);

	GameObject::DeleteObject(m_pObject);
	return nullptr;
}

