#pragma once
#include "FSMState.h"
#include "DigDugGridComponent.h"

namespace Engine
{
	class GameObject;
}

namespace DigDug
{
	enum class Direction;

	class FSMStatePumpInactive final : public Engine::FSMState
	{
	public:
		explicit FSMStatePumpInactive(FSMState* pActive, Engine::GameObject* pObj);
		virtual ~FSMStatePumpInactive() = default;
		virtual void Enter(const Engine::SceneData& sceneData, Engine::FSMData& data) override;
		virtual FSMState* OnNotify(Engine::ObservedEvent oevent, const Engine::ObservedData& odata, Engine::FSMData& data) override;
		virtual void Exit(const Engine::SceneData& sceneData, Engine::FSMData& data) override;
		void SetActiveState(FSMState* pS) { m_pActiveState = pS; }
	private:
		FSMState* m_pActiveState;
		Engine::GameObject* m_pObject;
	};
	
	class FSMStatePumpActive final : public Engine::FSMState
	{
	public:
		explicit FSMStatePumpActive(Engine::GameObject* pObj, DigDugGridComponent* pGrid, float offset, float duration, FSMState* pInactive, FSMState* pHit);
		virtual ~FSMStatePumpActive() = default;
		virtual void Enter(const Engine::SceneData& sceneData, Engine::FSMData& data) override;
		virtual FSMState* OnNotify(Engine::ObservedEvent oevent, const Engine::ObservedData& odata, Engine::FSMData& data) override;
		virtual FSMState* UpdateFirst(const Engine::SceneData& sceneData, Engine::FSMData& data) override;
		void SetPumpState(FSMState* pS) { m_pHitState = pS; }
	private:
		Direction m_CurrentDirection;
		Engine::GameObject* m_pObject;
		FSMState* m_pInactiveState;
		FSMState* m_pHitState;
		float m_PlayerOffset;
		float m_Duration;
		float m_Accu;
		bool m_Deactivated;
		DigDugGridComponent* m_pGrid;
	};

	class FSMStatePumpPumping final : public Engine::FSMState
	{
	public:
		explicit FSMStatePumpPumping(Engine::GameObject* pObj, FSMState* pInactive);
		virtual ~FSMStatePumpPumping() = default;
		virtual void Enter(const Engine::SceneData& sceneData, Engine::FSMData& data) override;
		virtual void Exit(const Engine::SceneData& sceneData, Engine::FSMData& data) override;
		virtual FSMState* OnNotify(Engine::ObservedEvent oevent, const Engine::ObservedData& odata, Engine::FSMData& data) override;
	private:
		Engine::GameObject* m_pObject;
		FSMState* m_pInactiveState;
	};






















}