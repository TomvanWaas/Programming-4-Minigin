#pragma once

struct SceneData;
class FSMEvent abstract
{
public:
	explicit FSMEvent() = default;
	virtual ~FSMEvent() = default;

	FSMEvent(const FSMEvent& other) = delete;
	FSMEvent(FSMEvent&& other) noexcept = delete;
	FSMEvent& operator=(const FSMEvent& other) = delete;
	FSMEvent& operator=(FSMEvent&& other) noexcept = delete;

	virtual void Initialize(const SceneData& sceneData) { UNREFERENCED_PARAMETER(sceneData); }
	virtual void Execute(const SceneData& sceneData) = 0;
};

class FSMMultiEvent final : public FSMEvent
{
public:
	explicit FSMMultiEvent(const std::vector<std::shared_ptr<FSMEvent>>& pEvents)
		: m_pEvents(pEvents)
	{}
	virtual ~FSMMultiEvent() = default;

	FSMMultiEvent(const FSMMultiEvent& other) = delete;
	FSMMultiEvent(FSMMultiEvent&& other) noexcept = delete;
	FSMMultiEvent& operator=(const FSMMultiEvent& other) = delete;
	FSMMultiEvent& operator=(FSMMultiEvent&& other) noexcept = delete;

	virtual void Execute(const SceneData& sceneData) override
	{
		for (auto& pEvent : m_pEvents)
		{
			if (pEvent == nullptr) continue;
			pEvent->Execute(sceneData);
		}
	}
	virtual void Initialize(const SceneData& sceneData) override
	{
		for (auto& pEvent : m_pEvents)
		{
			if (pEvent == nullptr) continue;
			pEvent->Initialize(sceneData);
		}
	}
private:
	std::vector<std::shared_ptr<FSMEvent>> m_pEvents;
};
