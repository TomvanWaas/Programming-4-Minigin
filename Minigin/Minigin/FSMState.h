#pragma once
#include <vector>
#include <memory>
#include "SceneData.h"
class FSMCondition;
class FSMEvent;
class FSMState;

struct FSMTransition
{
	FSMState* m_pState;
	std::vector<std::shared_ptr<FSMCondition>> m_pConditions;
	bool MeetsConditions(const SceneData& sceneData) const;

};

class FSMState final
{
public:
	explicit FSMState() = default;
	~FSMState() = default;

	FSMState(const FSMState& other) = delete;
	FSMState(FSMState&& other) noexcept = delete;
	FSMState& operator=(const FSMState& other) = delete;
	FSMState& operator=(FSMState&& other) noexcept = delete;

	FSMState* Update(const SceneData& sceneData);

	void AddTransition(const std::shared_ptr<FSMTransition>& pTransition);
	void AddUpdateEvent(const std::shared_ptr<FSMEvent>& pEvent);
	void AddEnterEvent(const std::shared_ptr<FSMEvent>& pEvent);
	void AddExitEvent(const std::shared_ptr<FSMEvent>& pEvent);



private:
	std::vector<std::shared_ptr<FSMTransition>> m_pTransitions;
	std::vector<std::shared_ptr<FSMEvent>> m_pOnEnterEvents;
	std::vector<std::shared_ptr<FSMEvent>> m_pOnExitEvents;
	std::vector<std::shared_ptr<FSMEvent>> m_pUpdateEvents;

	void Enter(const SceneData& sceneData);
	void Exit(const SceneData& sceneData);
};
