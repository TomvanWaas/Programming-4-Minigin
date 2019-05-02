#pragma once
#include "FSMCondition.h"

class FSMTimeCondition final : public FSMCondition
{
	float m_StartTime;
	float m_DeltaTime;
public:
	explicit FSMTimeCondition(float dtime);
	virtual ~FSMTimeCondition() = default;
	virtual void Initialize(const SceneData& sceneData) override;
	virtual bool Execute(const SceneData& sceneData) const override;
};

template <class T>
class FSMDataCondition final : public FSMCondition
{
	const T* m_pReference;
	T m_Value;
	bool m_Equals;
public:
	explicit FSMDataCondition(const T& reference, const T& target, bool equals = true)
		: m_pReference(&reference)
		, m_Value(target)
		, m_Equals(equals)
	{}
	virtual ~FSMDataCondition() = default;
	virtual bool Execute(const SceneData& sceneData) const override
	{
		UNREFERENCED_PARAMETER(sceneData);
		return (m_pReference != nullptr && ((*m_pReference == m_Value) == m_Equals));
	}
};
