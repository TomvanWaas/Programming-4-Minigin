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
	virtual void ReInitialize(const SceneData& sceneData) { UNREFERENCED_PARAMETER(sceneData); }
	virtual bool Execute(const SceneData& sceneData) const = 0;

private:



};
