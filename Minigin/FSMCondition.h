#pragma once

struct SceneData;
class FSMCondition abstract
{
public:
	explicit FSMCondition() = default;
	virtual ~FSMCondition() = default;

	FSMCondition(const FSMCondition& other) = delete;
	FSMCondition(FSMCondition&& other) noexcept = delete;
	FSMCondition& operator=(const FSMCondition& other) = delete;
	FSMCondition& operator=(FSMCondition&& other) noexcept = delete;

	virtual void Initialize(const SceneData& sceneData) { UNREFERENCED_PARAMETER(sceneData); }
	virtual bool Execute(const SceneData& sceneData) const = 0;
};


class FSMMultiAndCondition final : public FSMCondition
{
public:
	explicit FSMMultiAndCondition(const std::vector<std::shared_ptr<FSMCondition>>& pConditions)
		: m_pConditions(pConditions)
	{}
	virtual ~FSMMultiAndCondition() = default;

	FSMMultiAndCondition(const FSMMultiAndCondition& other) = delete;
	FSMMultiAndCondition(FSMMultiAndCondition&& other) noexcept = delete;
	FSMMultiAndCondition& operator=(const FSMMultiAndCondition& other) = delete;
	FSMMultiAndCondition& operator=(FSMMultiAndCondition&& other) noexcept = delete;

	virtual bool Execute(const SceneData& sceneData) const override
	{
		bool cond = true;
		for (const auto& pCondition : m_pConditions)
		{
			if (pCondition == nullptr) continue;
			cond = cond && pCondition->Execute(sceneData);
		}
		return cond;
	}

private:
	std::vector<std::shared_ptr<FSMCondition>> m_pConditions;
};

class FSMMultiOrCondition final : public FSMCondition
{
public:
	explicit FSMMultiOrCondition(const std::vector<std::shared_ptr<FSMCondition>>& pConditions)
		: m_pConditions(pConditions)
	{}
	virtual ~FSMMultiOrCondition() = default;

	FSMMultiOrCondition(const FSMMultiOrCondition& other) = delete;
	FSMMultiOrCondition(FSMMultiOrCondition&& other) noexcept = delete;
	FSMMultiOrCondition& operator=(const FSMMultiOrCondition& other) = delete;
	FSMMultiOrCondition& operator=(FSMMultiOrCondition&& other) noexcept = delete;

	virtual bool Execute(const SceneData& sceneData) const override
	{
		bool cond = false;
		for (const auto& pCondition : m_pConditions)
		{
			if (pCondition == nullptr) continue;
			cond = cond || pCondition->Execute(sceneData);
		}
		return cond;
	}

private:
	std::vector<std::shared_ptr<FSMCondition>> m_pConditions;
};




