#pragma once

namespace Minigin
{
	class FSMData;
	class SceneData;




	class FSMCondition abstract
	{
	public:
		explicit FSMCondition() = default;
		virtual ~FSMCondition() = default;

		FSMCondition(const FSMCondition& other) = delete;
		FSMCondition(FSMCondition&& other) noexcept = delete;
		FSMCondition& operator=(const FSMCondition& other) = delete;
		FSMCondition& operator=(FSMCondition&& other) noexcept = delete;

		virtual void Initialize(const SceneData& sceneData, FSMData& data) { UNREFERENCED_PARAMETER(sceneData); UNREFERENCED_PARAMETER(data); }
		virtual bool Execute(const SceneData& sceneData, const FSMData& data) const = 0;
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

		virtual bool Execute(const SceneData& sceneData, const FSMData& data) const override
		{
			bool cond = true;
			for (const auto& pCondition : m_pConditions)
			{
				if (pCondition == nullptr) continue;
				cond = cond && pCondition->Execute(sceneData, data);
			}
			return cond;
		}
		virtual void Initialize(const SceneData& sceneData, FSMData& data) override
		{
			for (auto& pCondition : m_pConditions)
			{
				if (pCondition != nullptr) pCondition->Initialize(sceneData, data);
			}
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

		virtual bool Execute(const SceneData& sceneData, const FSMData& data) const override
		{
			bool cond = false;
			for (const auto& pCondition : m_pConditions)
			{
				if (pCondition == nullptr) continue;
				cond = cond || pCondition->Execute(sceneData, data);
			}
			return cond;
		}

	private:
		std::vector<std::shared_ptr<FSMCondition>> m_pConditions;
	};

	class FSMNotCondition final : public FSMCondition
	{
	public:
		explicit FSMNotCondition(const std::shared_ptr<FSMCondition>& pCondition)
			: m_pCondition(pCondition)
		{}
		virtual bool Execute(const SceneData& sceneData, const FSMData& data) const override
		{
			return (m_pCondition != nullptr && !m_pCondition->Execute(sceneData, data));
		}
	private:
		std::shared_ptr<FSMCondition> m_pCondition;
	};




}