#pragma once
#include "BaseComponent.h"
#include "FSMData.h"

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
	virtual void OnNotify(ObservedEvent event, const ObservedData& data) override;
	void SetState(FSMState* pState);

	void SaveState(FSMState* pState);
	void SaveStates(const std::vector<FSMState*>& pStates);

	FSMData& GetData() { return m_Data; }
	const FSMData& GetData() const { return m_Data; }

private:
	FSMState* m_pCurrentState;
	bool m_StateChanged;
	FSMData m_Data;

	//So they are guaranteed to exist when used
	std::vector<FSMState*> m_pSavedStates;
};

