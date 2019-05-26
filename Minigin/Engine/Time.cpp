#include "stdafx.h"
#include "Time.h"

using namespace Engine;

Time::Time()
	: m_TotalTime(0)
	, m_DeltaTime(1)
{
}


void Time::Update(float elapsed)
{
	m_TotalTime += elapsed;
	m_DeltaTime = elapsed;
}

void Time::UpdateFixed(float elapsed)
{
	m_FixedDeltaTime = elapsed;
	m_FixedTotalTime += elapsed;
}
