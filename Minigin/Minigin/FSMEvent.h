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
	virtual void ReInitialize(const SceneData& sceneData) { UNREFERENCED_PARAMETER(sceneData); }
	virtual void Execute(const SceneData& sceneData) = 0;

private:


};
