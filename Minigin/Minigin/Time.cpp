#include "MiniginPCH.h"
#include "Time.h"


Time::Time()
	: m_TotalTime(0)
	, m_DeltaTime(1)
{
}


Time::~Time()
{
}

void Time::Update(float elapsed)
{
	m_TotalTime += elapsed;
	m_DeltaTime = elapsed;
}