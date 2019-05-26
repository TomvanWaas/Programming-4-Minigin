#pragma once
#include "FSMState.h"

namespace DigDug
{
	class DigDugGridComponent;



	class FSMStatePlayerIdle final : public Engine::FSMState
	{
	public:
		explicit FSMStatePlayerIdle(const std::string& deadState, const std::string& crushedState, const std::string& throwState, const std::string& moveState);
		virtual ~FSMStatePlayerIdle() = default;
		virtual void Enter(const Engine::SceneData& sceneData, Engine::FSMData& data) override;
		virtual FSMState* OnNotify(Engine::ObservedEvent oevent, const Engine::ObservedData& odata, Engine::FSMData& data) override;
	private:
		std::string m_DeadState;
		std::string m_CrushedState;
		std::string m_ThrowState;
		std::string m_MoveState;
	};
	
	class FSMStatePlayerMoving final : public Engine::FSMState
	{
	public:
		explicit FSMStatePlayerMoving(const std::string& deadState, const std::string& crushedState, const std::string& throwState, const std::string& idleState, DigDugGridComponent* pGrid);
		virtual ~FSMStatePlayerMoving() = default;
		virtual void Enter(const Engine::SceneData& sceneData, Engine::FSMData& data) override;
		virtual void Exit(const Engine::SceneData& sceneData, Engine::FSMData& data) override;
		virtual FSMState* UpdateSecond(const Engine::SceneData& sceneData, Engine::FSMData& data) override;
		virtual FSMState* OnNotify(Engine::ObservedEvent oevent, const Engine::ObservedData& odata, Engine::FSMData& data) override;
	private:
		std::string m_DeadState;
		std::string m_CrushedState;
		std::string m_IdleState;
		std::string m_ThrowState;
		DigDugGridComponent* m_pGrid;
	};
	
	class FSMStatePlayerThrowing final : public Engine::FSMState
	{
	public:
		explicit FSMStatePlayerThrowing(const std::string& deadState, const std::string& crushedState, const std::string& idleState, const std::string& pumpState);
		virtual ~FSMStatePlayerThrowing() = default;
		virtual void Enter(const Engine::SceneData& sceneData, Engine::FSMData& data) override;
		virtual FSMState* OnNotify(Engine::ObservedEvent oevent, const Engine::ObservedData& odata, Engine::FSMData& data) override;
	private:
		std::string m_DeadState;
		std::string m_CrushedState;
		std::string m_IdleState;
		std::string m_PumpState;

	};

	class FSMStatePlayerPumping final : public Engine::FSMState
	{
	public:
		explicit FSMStatePlayerPumping(const std::string& deadState, const std::string& crushedState, const std::string& idleState, const std::string& moveState);
		virtual ~FSMStatePlayerPumping() = default;
		virtual void Enter(const Engine::SceneData& sceneData, Engine::FSMData& data) override;
		virtual FSMState* OnNotify(Engine::ObservedEvent oevent, const Engine::ObservedData& odata, Engine::FSMData& data) override;
	private:
		std::string m_DeadState;
		std::string m_CrushedState;
		std::string m_IdleState;
		std::string m_MoveState;
	};

	class FSMStatePlayerCrushed final : public Engine::FSMState
	{
	public:
		explicit FSMStatePlayerCrushed(const std::string& deadState, DigDugGridComponent* pGrid, float fallspeed);
		virtual ~FSMStatePlayerCrushed() = default;
		virtual void Enter(const Engine::SceneData& sceneData, Engine::FSMData& data) override;
		virtual void Exit(const Engine::SceneData& sceneData, Engine::FSMData& data) override;
		virtual FSMState* UpdateFirst(const Engine::SceneData& sceneData, Engine::FSMData& data) override;
	private:
		std::string m_DeadState;
		DigDugGridComponent* m_pGrid;
		float m_Speed;
	};

	class FSMStatePlayerDead final : public Engine::FSMState
	{
	public:
		explicit FSMStatePlayerDead() = default;
		virtual ~FSMStatePlayerDead() = default;
		virtual void Enter(const Engine::SceneData& sceneData, Engine::FSMData& data) override;
		virtual FSMState* UpdateFirst(const Engine::SceneData& sceneData, Engine::FSMData& data) override;
	private:
		float m_Delay = 2.0f;
		float m_Accu = 0.0f;
	};



}