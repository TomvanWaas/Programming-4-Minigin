#include "MiniginPCH.h"
#include "FSMConditions.h"
#include "SceneData.h"
#include "Time.h"


//FSMTimeCondition
FSMTimeCondition::FSMTimeCondition(float dtime)
	: m_StartTime(0.0f)
	, m_DeltaTime(dtime)
{
}
void FSMTimeCondition::Initialize(const SceneData& sceneData)
{
	m_StartTime = sceneData.pTime->GetTotalTime();
}
bool FSMTimeCondition::Execute(const SceneData& sceneData) const
{
	return (m_StartTime + m_DeltaTime <= sceneData.pTime->GetTotalTime());
}
