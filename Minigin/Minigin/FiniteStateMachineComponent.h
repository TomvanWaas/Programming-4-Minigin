#pragma once
#include "BaseComponent.h"

class FSMState;
class FSMEvent;
class FSMCondition;
class FiniteStateMachineComponent final : public BaseComponent
{
public:
	explicit FiniteStateMachineComponent(const std::vector<FSMState*>& pStates, FSMState* pStartState);
	virtual ~FiniteStateMachineComponent();

	virtual void Initialize(const SceneData& sceneData) override;
	virtual void ReInitialize(const SceneData& sceneData) override;
	virtual void Update(const SceneData& sceneData) override;


	void SaveState(FSMState* pState);
	void SaveCondition(const std::shared_ptr<FSMCondition>& pCondition);
	void SaveEvent(const std::shared_ptr<FSMEvent>& pEvent);


private:
	FSMState* m_pCurrentState;

	std::vector<FSMState*> m_pSavedStates;
	std::vector<std::shared_ptr<FSMCondition>> m_pSavedConditions;
	std::vector<std::shared_ptr<FSMEvent>> m_pSavedEvents;


};

