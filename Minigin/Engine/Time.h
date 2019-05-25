#pragma once

namespace Minigin
{
	class Time final
	{
	public:
		~Time() = default;

		float GetDeltaTime() const { return m_DeltaTime; }
		float GetTotalTime() const { return m_TotalTime; }
		Time();

		void Update(float elapsed);

	private:
		float m_TotalTime;
		float m_DeltaTime;

	};
}
