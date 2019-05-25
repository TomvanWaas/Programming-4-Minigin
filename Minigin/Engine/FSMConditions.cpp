#include "stdafx.h"
#include "FSMConditions.h"
#include "SceneData.h"
#include "Time.h"

using namespace Minigin;

//FSMTimeCondition
FSMTimeCondition::FSMTimeCondition(float dtime)
	: m_StartTime(0.0f)
	, m_DeltaTime(dtime)
{
}
void FSMTimeCondition::Initialize(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(data);
	m_StartTime = sceneData.GetTime()->GetTotalTime();
}
bool FSMTimeCondition::Execute(const SceneData& sceneData, const FSMData& data) const
{
	UNREFERENCED_PARAMETER(data);
	return (m_StartTime + m_DeltaTime <= sceneData.GetTime()->GetTotalTime());
}
