#pragma once
#include <memory>
#include "SceneData.h"
class FSMCondition;
class FSMEvent;
class FSMState;

struct FSMTransition
{
	FSMTransition(FSMState* pState, const std::shared_ptr<FSMCondition>& pCondition)
		: pState(pState)
		, pCondition(pCondition)
	{}

	FSMTransition()
		: FSMTransition(nullptr, {})
	{}

	FSMState* pState;
	std::shared_ptr<FSMCondition> pCondition;
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

	void Initialize(const SceneData& sceneData);
	FSMState* UpdateFirst(const SceneData& sceneData);
	FSMState* UpdateSecond(const SceneData& sceneData);

	void AddTransition(const std::shared_ptr<FSMTransition>& pTransition);
	void SetUpdateFirstEvent(const std::shared_ptr<FSMEvent>& pEvent);
	void SetUpdateSecondEvent(const std::shared_ptr<FSMEvent>& pEvent);
	void SetEnterEvent(const std::shared_ptr<FSMEvent>& pEvent);
	void SetExitEvent(const std::shared_ptr<FSMEvent>& pEvent);


	void Enter(const SceneData& sceneData);
	void Exit(const SceneData& sceneData);
private:
	std::vector<std::shared_ptr<FSMTransition>> m_pTransitions;
	std::shared_ptr<FSMEvent> m_pOnEnterEvent;
	std::shared_ptr<FSMEvent> m_pOnExitEvent;
	std::shared_ptr<FSMEvent> m_pUpdateFirstEvent;
	std::shared_ptr<FSMEvent> m_pUpdateSecondEvent;


	
};
