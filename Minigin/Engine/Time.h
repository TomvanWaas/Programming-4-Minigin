#pragma once

namespace Engine
{
	class Time final
	{
	public:
		~Time() = default;
		explicit Time();

		float GetFixedDeltaTime() const { return m_FixedDeltaTime; }
		float GetFixedTotalTime() const { return m_FixedTotalTime; }
		float GetDeltaTime() const { return m_DeltaTime; }
		float GetTotalTime() const { return m_TotalTime; }

		void Update(float elapsed);
		void UpdateFixed(float elapsed);

	private:
		float m_TotalTime;
		float m_DeltaTime;
		float m_FixedDeltaTime;
		float m_FixedTotalTime;
	};
}
