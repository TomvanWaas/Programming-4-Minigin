#pragma once
#include <memory>
#include "SceneData.h"
class GameObject;
class ObservedData;
class FSMCondition;
class FSMEvent;
class FSMNotifiedEvent;
class FSMData;
class FiniteStateMachineComponent;
#include "ObservedEvent.h"

class FSMState abstract
{
public:
	explicit FSMState() = default;
	virtual ~FSMState() = default;

	void SetFSMComponent(FiniteStateMachineComponent* pComponent);

	FSMState(const FSMState& other) = delete;
	FSMState(FSMState&& other) noexcept = delete;
	FSMState& operator=(const FSMState& other) = delete;
	FSMState& operator=(FSMState&& other) noexcept = delete;

	virtual void Initialize(const SceneData& sceneData, FSMData& data) { UNREFERENCED_PARAMETER(sceneData); UNREFERENCED_PARAMETER(data); }
	virtual FSMState* UpdateFirst(const SceneData& sceneData, FSMData& data) { UNREFERENCED_PARAMETER(sceneData);  UNREFERENCED_PARAMETER(data); return this; }
	virtual FSMState* UpdateSecond(const SceneData& sceneData, FSMData& data) { UNREFERENCED_PARAMETER(sceneData); UNREFERENCED_PARAMETER(data); return this; }
	virtual void Enter(const SceneData& sceneData, FSMData& data) { UNREFERENCED_PARAMETER(sceneData);  UNREFERENCED_PARAMETER(data); }
	virtual void Exit(const SceneData& sceneData, FSMData& data) { UNREFERENCED_PARAMETER(sceneData); UNREFERENCED_PARAMETER(data); }
	virtual FSMState* OnNotify(ObservedEvent oevent, const ObservedData& odata, FSMData& data) { UNREFERENCED_PARAMETER(oevent); UNREFERENCED_PARAMETER(odata); UNREFERENCED_PARAMETER(data); return this; };

protected:
	FSMState* GetState(const std::string& name) const;
	GameObject* GetGameObject() const;
private:
	FiniteStateMachineComponent* m_pFSMComponent;

};

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



class FSMStateDefault final : public FSMState
{
public:
	explicit FSMStateDefault() = default;
	virtual ~FSMStateDefault() = default;

	FSMStateDefault(const FSMStateDefault& other) = delete;
	FSMStateDefault(FSMStateDefault&& other) noexcept = delete;
	FSMStateDefault& operator=(const FSMStateDefault& other) = delete;
	FSMStateDefault& operator=(FSMStateDefault&& other) noexcept = delete;

	virtual void Initialize(const SceneData& sceneData, FSMData& data) override;
	virtual FSMState* UpdateFirst(const SceneData& sceneData, FSMData& data) override;
	virtual FSMState* UpdateSecond(const SceneData& sceneData, FSMData& data) override;
	virtual FSMState* OnNotify(ObservedEvent oevent, const ObservedData& odata, FSMData& data) override;

	void AddTransition(const std::shared_ptr<FSMTransition>& pTransition);
	void SetUpdateFirstEvent(const std::shared_ptr<FSMEvent>& pEvent);
	void SetUpdateSecondEvent(const std::shared_ptr<FSMEvent>& pEvent);
	void SetEnterEvent(const std::shared_ptr<FSMEvent>& pEvent);
	void SetExitEvent(const std::shared_ptr<FSMEvent>& pEvent);
	void SetNotifiedEvent(const std::shared_ptr<FSMNotifiedEvent>& pEvent);

	virtual void Enter(const SceneData& sceneData, FSMData& data) override;
	virtual void Exit(const SceneData& sceneData, FSMData& data) override;
private:
	std::vector<std::shared_ptr<FSMTransition>> m_pTransitions;
	std::shared_ptr<FSMEvent> m_pOnEnterEvent;
	std::shared_ptr<FSMEvent> m_pOnExitEvent;
	std::shared_ptr<FSMEvent> m_pUpdateFirstEvent;
	std::shared_ptr<FSMEvent> m_pUpdateSecondEvent;
	std::shared_ptr<FSMNotifiedEvent> m_pNotifiedEvent;

	
};
