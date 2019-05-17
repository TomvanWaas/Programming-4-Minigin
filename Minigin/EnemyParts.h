#pragma once
#include "FSMState.h"
class DigDugGridComponent;
class GameObject;

namespace DigDug
{
	enum class Direction;

	class FSMStatePookaMoving final : public FSMState
	{
	public:
		explicit FSMStatePookaMoving(float speed, DigDugGridComponent* pGrid, GameObject* pObject, FSMState* pPumped, FSMState* pGhost, FSMState* pCrushed, float ghostDelay);
		virtual ~FSMStatePookaMoving() = default;
		virtual void Enter(const SceneData& sceneData, FSMData& data) override;
		virtual FSMState* UpdateFirst(const SceneData& sceneData, FSMData& data) override;
		virtual FSMState* OnNotify(ObservedEvent oevent, const ObservedData& odata, FSMData& data) override;
		virtual void Exit(const SceneData& sceneData, FSMData& data) override;

		void SetPumpState(FSMState* pPump) { m_pPumpedState = pPump; }
		void SetCrushState(FSMState* pCrush) { m_pCrushedState = pCrush; }
		void SetGhostState(FSMState* pGhost) { m_pGhostState = pGhost; }
	private:
		float m_Speed;
		DigDugGridComponent* m_pGrid;
		GameObject* m_pObject;
		Direction m_CurrentDirection;

		FSMState* m_pPumpedState;
		FSMState* m_pGhostState;
		FSMState* m_pCrushedState;

		float m_AccuTime;
		float m_GhostTime;

		void SetDirection(Direction d, FSMData& data);
		Direction GetRandomDirection(bool* marks);
		void SetSprite(unsigned i);
	};

	class FSMStateEnemyPumping final : public FSMState
	{
	public:
		explicit FSMStateEnemyPumping(GameObject* pObject, float targetTime,float deflateSpeed, FSMState* pMove, FSMState* pDead, FSMState* pCrushed);
		virtual ~FSMStateEnemyPumping() = default;

		virtual void Enter(const SceneData& sceneData, FSMData& data) override;
		virtual FSMState* UpdateFirst(const SceneData& sceneData, FSMData& data) override;
		virtual FSMState* OnNotify(ObservedEvent oevent, const ObservedData& odata, FSMData& data) override;

		void SetMoveState(FSMState* pMove) { m_pMovingState = pMove; }
		void SetCrushState(FSMState* pCrush) { m_pCrushedState = pCrush; }
		void SetDeadState(FSMState* pDead) { m_pDeadState = pDead; }
	private:
		GameObject* m_pObject;
		FSMState* m_pMovingState;
		FSMState* m_pDeadState;
		FSMState* m_pCrushedState;
		float m_DeflateSpeed;
		float m_AccuPump;
		float m_TargetPump;
		bool m_IsBeingPumped;
	};

	class FSMStateEnemyCrushed final : public FSMState
	{
	public:
		explicit FSMStateEnemyCrushed(DigDugGridComponent* pGrid, GameObject* pObj, float fallspeed, FSMState* pDead);
		virtual ~FSMStateEnemyCrushed() = default;
		virtual void Enter(const SceneData& sceneData, FSMData& data) override;
		virtual FSMState* UpdateFirst(const SceneData& sceneData, FSMData& data) override;
		virtual void Exit(const SceneData& sceneData, FSMData& data) override;

		void SetDeadState(FSMState* pDead) { m_pDeadState = pDead; }
	private:
		DigDugGridComponent* m_pGrid;
		GameObject* m_pObject;
		float m_FallSpeed;
		FSMState* m_pDeadState;
	};
	
	class FSMStateEnemyGhost final : public FSMState
	{
	public:
		explicit FSMStateEnemyGhost(GameObject* pObj, float speed, float duration, DigDugGridComponent* pGrid, FSMState* pMove, FSMState* pCrushed, FSMState* pPumped);
		virtual void Enter(const SceneData& sceneData, FSMData& data) override;
		virtual FSMState* UpdateFirst(const SceneData& sceneData, FSMData& data) override;
		virtual FSMState* OnNotify(ObservedEvent oevent, const ObservedData& odata, FSMData& data) override;

		void SetMoveState(FSMState* pMove) { m_pMoveState = pMove; }
		void SetCrushState(FSMState* pCrush) { m_pCrushedState = pCrush; }
		void SetPumpState(FSMState* pPump) { m_pPumpedState = pPump; }
	private:
		GameObject* m_pObject;
		GameObject* m_pClosestPlayer;
		float m_Speed;
		DigDugGridComponent* m_pGrid;
		FSMState* m_pMoveState;
		FSMState* m_pCrushedState;
		FSMState* m_pPumpedState;
		float m_MinDuration;
		float m_Accu;

	};






	class FSMStateFygarMoving final : public FSMState
	{
	public:
		explicit FSMStateFygarMoving(float speed, float fireDelay, float ghostDelay, DigDugGridComponent* pGrid, 
			const std::string& pumpedState, const std::string& ghostState, 
			const std::string& crushedState, const std::string& fireState);
		virtual ~FSMStateFygarMoving() = default;
		virtual void Enter(const SceneData& sceneData, FSMData& data) override;
		virtual FSMState* UpdateFirst(const SceneData& sceneData, FSMData& data) override;
		virtual FSMState* OnNotify(ObservedEvent oevent, const ObservedData& odata, FSMData& data) override;
		virtual void Exit(const SceneData& sceneData, FSMData& data) override;
	private:	
		std::string m_PumpedState;
		std::string m_GhostState;
		std::string m_CrushedState;
		std::string m_FireState;
		float m_Speed;
		float m_GhostAccu;
		float m_FireAccu;
		float m_FireTime;
		float m_GhostTime;
		DigDugGridComponent* m_pGrid;
		Direction m_CurrentDirection;


		void SetDirection(Direction d, FSMData& data);
		Direction GetRandomDirection(bool* marks);
		void SetSprite(unsigned i);
	};
	
	class FSMStateFygarFire final : public FSMState
	{
	public:
		explicit FSMStateFygarFire(float time, const std::string& pumpedState, const std::string& crushedState, const std::string& moveState);
		virtual ~FSMStateFygarFire() = default;
		virtual void Enter(const SceneData& sceneData, FSMData& data) override;
		virtual FSMState* UpdateFirst(const SceneData& sceneData, FSMData& data) override;
		virtual FSMState* OnNotify(ObservedEvent oevent, const ObservedData& odata, FSMData& data) override;
	private:
		std::string m_PumpedState;
		std::string m_CrushedState;
		std::string m_MoveState;
		float m_Time;
		float m_Accu;
	};








	class FSMStateFireIdle final : public FSMState
	{
	public:
		explicit FSMStateFireIdle(const std::string& activeState);
		virtual ~FSMStateFireIdle() = default;
		virtual void Enter(const SceneData& sceneData, FSMData& data) override;
		virtual FSMState* OnNotify(ObservedEvent oevent, const ObservedData& odata, FSMData& data) override;
	private:
		std::string m_ActiveState;
	};
	class FSMStateFireActive final : public FSMState
	{
	public:
		explicit FSMStateFireActive(const std::string& inactiveState, float duration);
		virtual ~FSMStateFireActive() = default;
		virtual void Enter(const SceneData& sceneData, FSMData& data) override;
		virtual FSMState* OnNotify(ObservedEvent oevent, const ObservedData& odata, FSMData& data) override;
		virtual FSMState* UpdateFirst(const SceneData& sceneData, FSMData& data) override;
	private:
		std::string m_InactiveState;
		float m_Duration;
		float m_Accu;
		Direction m_Direction;
	};



}
