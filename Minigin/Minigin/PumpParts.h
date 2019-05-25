#pragma once
#include "FSMState.h"
#include "DigDugGridComponent.h"

namespace Minigin
{
	class GameObject;
}

namespace DigDug
{
	enum class Direction;

	class FSMStatePumpInactive final : public Minigin::FSMState
	{
	public:
		explicit FSMStatePumpInactive(FSMState* pActive, Minigin::GameObject* pObj);
		virtual ~FSMStatePumpInactive() = default;
		virtual void Enter(const Minigin::SceneData& sceneData, Minigin::FSMData& data) override;
		virtual FSMState* OnNotify(Minigin::ObservedEvent oevent, const Minigin::ObservedData& odata, Minigin::FSMData& data) override;
		virtual void Exit(const Minigin::SceneData& sceneData, Minigin::FSMData& data) override;
		void SetActiveState(FSMState* pS) { m_pActiveState = pS; }
	private:
		FSMState* m_pActiveState;
		Minigin::GameObject* m_pObject;
	};
	
	class FSMStatePumpActive final : public Minigin::FSMState
	{
	public:
		explicit FSMStatePumpActive(Minigin::GameObject* pObj, DigDugGridComponent* pGrid, float offset, float duration, FSMState* pInactive, FSMState* pHit);
		virtual ~FSMStatePumpActive() = default;
		virtual void Enter(const Minigin::SceneData& sceneData, Minigin::FSMData& data) override;
		virtual FSMState* OnNotify(Minigin::ObservedEvent oevent, const Minigin::ObservedData& odata, Minigin::FSMData& data) override;
		virtual FSMState* UpdateFirst(const Minigin::SceneData& sceneData, Minigin::FSMData& data) override;
		void SetPumpState(FSMState* pS) { m_pHitState = pS; }
	private:
		Direction m_CurrentDirection;
		Minigin::GameObject* m_pObject;
		FSMState* m_pInactiveState;
		FSMState* m_pHitState;
		float m_PlayerOffset;
		float m_Duration;
		float m_Accu;
		bool m_Deactivated;
		DigDugGridComponent* m_pGrid;
	};

	class FSMStatePumpPumping final : public Minigin::FSMState
	{
	public:
		explicit FSMStatePumpPumping(Minigin::GameObject* pObj, FSMState* pInactive);
		virtual ~FSMStatePumpPumping() = default;
		virtual void Enter(const Minigin::SceneData& sceneData, Minigin::FSMData& data) override;
		virtual void Exit(const Minigin::SceneData& sceneData, Minigin::FSMData& data) override;
		virtual FSMState* OnNotify(Minigin::ObservedEvent oevent, const Minigin::ObservedData& odata, Minigin::FSMData& data) override;
	private:
		Minigin::GameObject* m_pObject;
		FSMState* m_pInactiveState;
	};






















}