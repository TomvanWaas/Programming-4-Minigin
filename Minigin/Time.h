#pragma once
class Time final
{
public:
	~Time();

	float GetDeltaTime() const { return m_DeltaTime; }
	float GetTotalTime() const { return m_TotalTime; }
	Time();

	void Update(float elapsed);

private:
	float m_TotalTime;
	float m_DeltaTime;

};

