#pragma once
#include "FSMState.h"
class GameObject;


namespace DigDug
{
	enum class Direction;

	class FSMStatePumpInactive final : public FSMState
	{
	public:
		explicit FSMStatePumpInactive(FSMState* pActive, GameObject* pObj);
		virtual ~FSMStatePumpInactive() = default;
		virtual void Enter(const SceneData& sceneData, FSMData& data) override;
		virtual FSMState* OnNotify(ObservedEvent oevent, const ObservedData& odata, FSMData& data) override;
		virtual void Exit(const SceneData& sceneData, FSMData& data) override;
		void SetActiveState(FSMState* pS) { m_pActiveState = pS; }
	private:
		FSMState* m_pActiveState;
		GameObject* m_pObject;
	};
	
	class FSMStatePumpActive final : public FSMState
	{
	public:
		explicit FSMStatePumpActive(GameObject* pObj, float offset, float duration, FSMState* pInactive, FSMState* pHit);
		virtual ~FSMStatePumpActive() = default;
		virtual void Enter(const SceneData& sceneData, FSMData& data) override;
		virtual FSMState* OnNotify(ObservedEvent oevent, const ObservedData& odata, FSMData& data) override;
		virtual FSMState* UpdateFirst(const SceneData& sceneData, FSMData& data) override;
		void SetPumpState(FSMState* pS) { m_pHitState = pS; }
	private:
		Direction m_CurrentDirection;
		GameObject* m_pObject;
		FSMState* m_pInactiveState;
		FSMState* m_pHitState;
		float m_PlayerOffset;
		float m_Duration;
		float m_Accu;
	};

	class FSMStatePumpPumping final : public FSMState
	{
	public:
		explicit FSMStatePumpPumping(GameObject* pObj, FSMState* pInactive);
		virtual ~FSMStatePumpPumping() = default;
		virtual void Enter(const SceneData& sceneData, FSMData& data) override;
		virtual void Exit(const SceneData& sceneData, FSMData& data) override;
		virtual FSMState* OnNotify(ObservedEvent oevent, const ObservedData& odata, FSMData& data) override;
	private:
		GameObject* m_pObject;
		FSMState* m_pInactiveState;
	};






















}