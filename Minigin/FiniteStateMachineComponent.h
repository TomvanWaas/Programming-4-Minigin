#pragma once
#include "BaseComponent.h"

class FSMState;
class FSMEvent;
class FSMCondition;
class FiniteStateMachineComponent final : public BaseComponent
{
public:
	explicit FiniteStateMachineComponent() = default;
	virtual ~FiniteStateMachineComponent();

	virtual void UpdateFirstOverride(const SceneData& sceneData) override;
	virtual void UpdateSecondOverride(const SceneData& sceneData) override;

	void SetState(FSMState* pState, const SceneData& sceneData);

	void SaveState(FSMState* pState);
	void SaveStates(const std::vector<FSMState*>& pStates);

	void SaveCondition(const std::shared_ptr<FSMCondition>& pCondition);
	void SaveConditions(const std::vector<std::shared_ptr<FSMCondition>>& pConditions);


	void SaveEvent(const std::shared_ptr<FSMEvent>& pEvent);
	void SaveEvents(const std::vector<std::shared_ptr<FSMEvent>>& pEvents);


private:
	FSMState* m_pCurrentState;
	bool m_StateChanged;

	//So they are guaranteed to exist when used
	std::vector<FSMState*> m_pSavedStates;
	std::vector<std::shared_ptr<FSMCondition>> m_pSavedConditions;
	std::vector<std::shared_ptr<FSMEvent>> m_pSavedEvents;


};

