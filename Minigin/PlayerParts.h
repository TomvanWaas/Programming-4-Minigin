#pragma once
#include "FSMState.h"
class DigDugGridComponent;

namespace DigDug
{



	class FSMStatePlayerIdle final : public FSMState
	{
	public:
		explicit FSMStatePlayerIdle(const std::string& deadState, const std::string& crushedState, const std::string& throwState, const std::string& moveState);
		virtual ~FSMStatePlayerIdle() = default;
		virtual void Enter(const SceneData& sceneData, FSMData& data) override;
		virtual FSMState* OnNotify(ObservedEvent oevent, const ObservedData& odata, FSMData& data) override;
	private:
		std::string m_DeadState;
		std::string m_CrushedState;
		std::string m_ThrowState;
		std::string m_MoveState;
	};
	
	class FSMStatePlayerMoving final : public FSMState
	{
	public:
		explicit FSMStatePlayerMoving(const std::string& deadState, const std::string& crushedState, const std::string& throwState, const std::string& idleState, DigDugGridComponent* pGrid);
		virtual ~FSMStatePlayerMoving() = default;
		virtual void Enter(const SceneData& sceneData, FSMData& data) override;
		virtual void Exit(const SceneData& sceneData, FSMData& data) override;
		virtual FSMState* UpdateSecond(const SceneData& sceneData, FSMData& data) override;
		virtual FSMState* OnNotify(ObservedEvent oevent, const ObservedData& odata, FSMData& data) override;
	private:
		std::string m_DeadState;
		std::string m_CrushedState;
		std::string m_IdleState;
		std::string m_ThrowState;
		DigDugGridComponent* m_pGrid;
	};
	
	class FSMStatePlayerThrowing final : public FSMState
	{
	public:
		explicit FSMStatePlayerThrowing(const std::string& deadState, const std::string& crushedState, const std::string& idleState, const std::string& pumpState);
		virtual ~FSMStatePlayerThrowing() = default;
		virtual void Enter(const SceneData& sceneData, FSMData& data) override;
		virtual FSMState* OnNotify(ObservedEvent oevent, const ObservedData& odata, FSMData& data) override;
	private:
		std::string m_DeadState;
		std::string m_CrushedState;
		std::string m_IdleState;
		std::string m_PumpState;

	};

	class FSMStatePlayerPumping final : public FSMState
	{
	public:
		explicit FSMStatePlayerPumping(const std::string& deadState, const std::string& crushedState, const std::string& idleState, const std::string& moveState);
		virtual ~FSMStatePlayerPumping() = default;
		virtual void Enter(const SceneData& sceneData, FSMData& data) override;
		virtual FSMState* OnNotify(ObservedEvent oevent, const ObservedData& odata, FSMData& data) override;
	private:
		std::string m_DeadState;
		std::string m_CrushedState;
		std::string m_IdleState;
		std::string m_MoveState;
	};

	class FSMStatePlayerCrushed final : public FSMState
	{
	public:
		explicit FSMStatePlayerCrushed(const std::string& deadState, DigDugGridComponent* pGrid, float fallspeed);
		virtual ~FSMStatePlayerCrushed() = default;
		virtual void Enter(const SceneData& sceneData, FSMData& data) override;
		virtual void Exit(const SceneData& sceneData, FSMData& data) override;
		virtual FSMState* UpdateFirst(const SceneData& sceneData, FSMData& data) override;
	private:
		std::string m_DeadState;
		DigDugGridComponent* m_pGrid;
		float m_Speed;
	};

	class FSMStatePlayerDead final : public FSMState
	{
	public:
		explicit FSMStatePlayerDead() = default;
		virtual ~FSMStatePlayerDead() = default;
		virtual void Enter(const SceneData& sceneData, FSMData& data) override;
		virtual FSMState* UpdateFirst(const SceneData& sceneData, FSMData& data) override;
		virtual void Exit(const SceneData& sceneData, FSMData& data) override;
	private:
		float m_Delay = 2.0f;
		float m_Accu = 0.0f;
	};



}