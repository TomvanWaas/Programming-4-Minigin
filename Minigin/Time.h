#pragma once
#include "Singleton.h"
#include "Minigin.h"

class Time : public dae::Singleton<Time>
{
	//So that only the Run() can call the Update() from Time
	friend void dae::Minigin::Run();

public:
	~Time();

	float GetDeltaTime() const { return m_DeltaTime; }
	float GetTotalTime() const { return m_TotalTime; }
	Time();

protected:
	void Update(const float& elapsed);

private:

	float m_TotalTime;
	float m_DeltaTime;

};

