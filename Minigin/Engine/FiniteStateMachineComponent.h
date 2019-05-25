#pragma once
#include "BaseComponent.h"
#include "FSMData.h"

namespace Minigin
{
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
		virtual void UpdateFixedOverride(const SceneData& sceneData) override;
		virtual void OnNotify(ObservedEvent event, const ObservedData& data) override;
		void SetState(FSMState* pState);
		void SetState(const std::string& state);
		FSMState* GetState() const { return m_pCurrentState; }
		std::string GetStateName() const;

		bool SaveState(const std::string& name, FSMState* pState);
		FSMState* GetStateByName(const std::string& name) const;

		FSMData& GetData() { return m_Data; }
		const FSMData& GetData() const { return m_Data; }

	private:
		FSMState* m_pCurrentState;
		bool m_StateChanged;
		FSMData m_Data;

		//So they are guaranteed to exist when used
		std::map<std::string, FSMState*> m_pSavedStates;
	};

}