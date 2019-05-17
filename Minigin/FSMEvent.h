#pragma once
class ObservedData;
class SceneData;
class FSMData;
#include "ObservedEvent.h"

class FSMEvent abstract
{
public:
	explicit FSMEvent() = default;
	virtual ~FSMEvent() = default;

	FSMEvent(const FSMEvent& other) = delete;
	FSMEvent(FSMEvent&& other) noexcept = delete;
	FSMEvent& operator=(const FSMEvent& other) = delete;
	FSMEvent& operator=(FSMEvent&& other) noexcept = delete;

	virtual void Initialize(const SceneData& sceneData, FSMData& data) { UNREFERENCED_PARAMETER(sceneData); UNREFERENCED_PARAMETER(data); }
	virtual void Execute(const SceneData& sceneData, FSMData& data) = 0;
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

	virtual void Execute(const SceneData& sceneData, FSMData& data) override
	{
		for (auto& pEvent : m_pEvents)
		{
			if (pEvent == nullptr) continue;
			pEvent->Execute(sceneData, data);
		}
	}
	virtual void Initialize(const SceneData& sceneData, FSMData& data) override
	{
		for (auto& pEvent : m_pEvents)
		{
			if (pEvent == nullptr) continue;
			pEvent->Initialize(sceneData, data);
		}
	}
private:
	std::vector<std::shared_ptr<FSMEvent>> m_pEvents;
};

class FSMNotifiedEvent abstract
{
public:
	explicit FSMNotifiedEvent() = default;
	virtual ~FSMNotifiedEvent() = default;

	FSMNotifiedEvent(const FSMNotifiedEvent& other) = delete;
	FSMNotifiedEvent(FSMNotifiedEvent&& other) noexcept = delete;
	FSMNotifiedEvent& operator=(const FSMNotifiedEvent& other) = delete;
	FSMNotifiedEvent& operator=(FSMNotifiedEvent&& other) noexcept = delete;

	virtual void Initialize(const SceneData& sceneData, FSMData& data) { UNREFERENCED_PARAMETER(sceneData); UNREFERENCED_PARAMETER(data);}
	virtual void Execute(ObservedEvent oevent, const ObservedData& odata, FSMData& data) { UNREFERENCED_PARAMETER(oevent); UNREFERENCED_PARAMETER(odata); UNREFERENCED_PARAMETER(data); }
private:
};

class FSMMultiNotifiedEvent final : public FSMNotifiedEvent
{
public:
	explicit FSMMultiNotifiedEvent(const std::vector<std::shared_ptr<FSMNotifiedEvent>>& pEvents)
		: m_pEvents(pEvents)
	{}
	virtual ~FSMMultiNotifiedEvent() = default;

	FSMMultiNotifiedEvent(const FSMMultiNotifiedEvent& other) = delete;
	FSMMultiNotifiedEvent(FSMMultiNotifiedEvent&& other) noexcept = delete;
	FSMMultiNotifiedEvent& operator=(const FSMMultiNotifiedEvent& other) = delete;
	FSMMultiNotifiedEvent& operator=(FSMMultiNotifiedEvent&& other) noexcept = delete;

	virtual void Execute(ObservedEvent oevent, const ObservedData& odata, FSMData& data) override
	{
		for (auto& pEvent : m_pEvents)
		{
			if (pEvent == nullptr) continue;
			pEvent->Execute(oevent, odata, data);
		}
	}
	virtual void Initialize(const SceneData& sceneData, FSMData& data) override
	{
		for (auto& pEvent : m_pEvents)
		{
			if (pEvent == nullptr) continue;
			pEvent->Initialize(sceneData, data);
		}
	}
private:
	std::vector<std::shared_ptr<FSMNotifiedEvent>> m_pEvents;
};