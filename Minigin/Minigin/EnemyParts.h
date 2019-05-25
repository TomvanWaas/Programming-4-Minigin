#pragma once
#include "FSMState.h"
#include "Vector2.h"
namespace Minigin
{
	class GameObject;
}
namespace DigDug
{
	class DigDugGridComponent;
	enum class Direction;


	//General
	class FSMStateEnemyMove final : public Minigin::FSMState
	{
	public:
		explicit FSMStateEnemyMove(float speed, DigDugGridComponent* pGrid, float ghostCooldown, float fireCooldown, const std::string& ghostState, const std::string& fireState, const std::string& pumpState, const std::string& crushState);
		virtual ~FSMStateEnemyMove() = default;
		virtual void Enter(const Minigin::SceneData& sceneData, Minigin::FSMData& data) override;
		virtual FSMState* OnNotify(Minigin::ObservedEvent oevent, const Minigin::ObservedData& odata, Minigin::FSMData& data) override;
		virtual FSMState* UpdateSecond(const Minigin::SceneData& sceneData, Minigin::FSMData& data) override;
	private:
		std::string m_GhostState;
		std::string m_FireState;
		std::string m_PumpState;
		std::string m_CrushState;
		float m_AccuGhost;
		float m_GhostCooldown;
		float m_AccuFire;
		float m_FireCooldown;
		float m_Speed;
		Direction m_Direction;
		Direction m_NonZero;
		Direction m_Desired;
		DigDugGridComponent* m_pGrid;

		void HandleDesired(Direction d);
		bool Opposite(Direction a, Direction b) const;
	};

	class FSMStateEnemyGhost final : public Minigin::FSMState
	{
	public:
		explicit FSMStateEnemyGhost(float speed, float dur, DigDugGridComponent* pGrid, const std::string& move, const std::string& pump, const std::string& crush);
		virtual ~FSMStateEnemyGhost() = default;
		virtual void Enter(const Minigin::SceneData& sceneData, Minigin::FSMData& data) override;
		virtual FSMState* OnNotify(Minigin::ObservedEvent oevent, const Minigin::ObservedData& odata, Minigin::FSMData& data) override;
		virtual FSMState* UpdateSecond(const Minigin::SceneData& sceneData, Minigin::FSMData& data) override;
	private:
		float m_Speed;
		float m_Duration;
		float m_Accu;
		std::string m_MoveState;
		std::string m_PumpState;
		std::string m_CrushState;
		DigDugGridComponent* m_pGrid;
		Minigin::Vector2 m_Direction;
		bool m_RequestedMoveState;
	};

	class FSMStateEnemyPumping final : public Minigin::FSMState
	{
	public:
		explicit FSMStateEnemyPumping(Minigin::GameObject* pObject, DigDugGridComponent* pGrid, float targetTime,float deflateSpeed, FSMState* pMove, FSMState* pDead, FSMState* pCrushed);
		virtual ~FSMStateEnemyPumping() = default;

		virtual void Enter(const Minigin::SceneData& sceneData, Minigin::FSMData& data) override;
		virtual FSMState* UpdateFirst(const Minigin::SceneData& sceneData, Minigin::FSMData& data) override;
		virtual FSMState* OnNotify(Minigin::ObservedEvent oevent, const Minigin::ObservedData& odata, Minigin::FSMData& data) override;
		virtual void Exit(const Minigin::SceneData& sceneData, Minigin::FSMData& data) override;

		void SetMoveState(FSMState* pMove) { m_pMovingState = pMove; }
		void SetCrushState(FSMState* pCrush) { m_pCrushedState = pCrush; }
		void SetDeadState(FSMState* pDead) { m_pDeadState = pDead; }
	private:
		Minigin::GameObject* m_pObject;
		FSMState* m_pMovingState;
		FSMState* m_pDeadState;
		FSMState* m_pCrushedState;
		float m_DeflateSpeed;
		float m_AccuPump;
		float m_TargetPump;
		bool m_IsBeingPumped;
		DigDugGridComponent* m_pGrid;
	};

	class FSMStateEnemyCrushed final : public Minigin::FSMState
	{
	public:
		explicit FSMStateEnemyCrushed(DigDugGridComponent* pGrid, Minigin::GameObject* pObj, float fallspeed, FSMState* pDead);
		virtual ~FSMStateEnemyCrushed() = default;
		virtual void Enter(const Minigin::SceneData& sceneData, Minigin::FSMData& data) override;
		virtual FSMState* UpdateFirst(const Minigin::SceneData& sceneData, Minigin::FSMData& data) override;
		virtual void Exit(const Minigin::SceneData& sceneData, Minigin::FSMData& data) override;

		void SetDeadState(FSMState* pDead) { m_pDeadState = pDead; }
	private:
		DigDugGridComponent* m_pGrid;
		Minigin::GameObject* m_pObject;
		float m_FallSpeed;
		FSMState* m_pDeadState;
	};
	

	//Fygar specific
	class FSMStateFygarFire final : public Minigin::FSMState
	{
	public:
		explicit FSMStateFygarFire(float time, const std::string& pumpedState, const std::string& crushedState, const std::string& moveState);
		virtual ~FSMStateFygarFire() = default;
		virtual void Enter(const Minigin::SceneData& sceneData, Minigin::FSMData& data) override;
		virtual FSMState* UpdateFirst(const Minigin::SceneData& sceneData, Minigin::FSMData& data) override;
		virtual FSMState* OnNotify(Minigin::ObservedEvent oevent, const Minigin::ObservedData& odata, Minigin::FSMData& data) override;
	private:
		std::string m_PumpedState;
		std::string m_CrushedState;
		std::string m_MoveState;
		float m_Time;
		float m_Accu;
	};

	class FSMStateFygarCharge final : public Minigin::FSMState
	{
	public:
		explicit FSMStateFygarCharge(float dur, const std::string& pumpState, const std::string& crushState, const std::string& fireState);
		virtual ~FSMStateFygarCharge() = default;
		virtual void Enter(const Minigin::SceneData& sceneData, Minigin::FSMData& data) override;
		virtual FSMState* UpdateFirst(const Minigin::SceneData& sceneData, Minigin::FSMData& data) override;
		virtual FSMState* OnNotify(Minigin::ObservedEvent oevent, const Minigin::ObservedData& odata, Minigin::FSMData& data) override;
	private:
		float m_Duration;
		float m_Accu;
		std::string m_PumpState;
		std::string m_CrushedState;
		std::string m_FireState;
	};


	//Fire Object
	class FSMStateFireIdle final : public Minigin::FSMState
	{
	public:
		explicit FSMStateFireIdle(const std::string& activeState);
		virtual ~FSMStateFireIdle() = default;
		virtual void Enter(const Minigin::SceneData& sceneData, Minigin::FSMData& data) override;
		virtual FSMState* OnNotify(Minigin::ObservedEvent oevent, const Minigin::ObservedData& odata, Minigin::FSMData& data) override;
	private:
		std::string m_ActiveState;
	};
	class FSMStateFireActive final : public Minigin::FSMState
	{
	public:
		explicit FSMStateFireActive(DigDugGridComponent* pGrid, const std::string& inactiveState, float duration);
		virtual ~FSMStateFireActive() = default;
		virtual void Enter(const Minigin::SceneData& sceneData, Minigin::FSMData& data) override;
		virtual FSMState* OnNotify(Minigin::ObservedEvent oevent, const Minigin::ObservedData& odata, Minigin::FSMData& data) override;
		virtual FSMState* UpdateFirst(const Minigin::SceneData& sceneData, Minigin::FSMData& data) override;
	private:
		std::string m_InactiveState;
		float m_Duration;
		float m_Accu;
		Direction m_Direction;
		DigDugGridComponent* m_pGrid;
	};




}
