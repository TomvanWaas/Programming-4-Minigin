#pragma once
#include "FSMState.h"

namespace Engine
{
	class GameObject;
}
namespace DigDug
{

	class DigDugGridComponent;

	class FSMStateObstacleIdle final : public Engine::FSMState
	{
	public:
		explicit FSMStateObstacleIdle(DigDugGridComponent* pGrid, Engine::GameObject* pObject, FSMState* pFallState);
		virtual ~FSMStateObstacleIdle() = default;
		virtual void Enter(const Engine::SceneData& sceneData, Engine::FSMData& data) override;
		virtual FSMState* OnNotify(Engine::ObservedEvent oevent, const Engine::ObservedData& odata, Engine::FSMData& data) override;
		virtual void Exit(const Engine::SceneData& sceneData, Engine::FSMData& data) override;
	private:
		DigDugGridComponent* m_pGrid;
		Engine::GameObject* m_pObject;
		FSMState* m_pFallState;
	};

	class FSMStateObstacleFall final : public Engine::FSMState
	{
	public:
		explicit FSMStateObstacleFall(Engine::GameObject* pObject, DigDugGridComponent* pGrid, float speed, FSMState* pNext);
		virtual ~FSMStateObstacleFall() = default;
		virtual void Enter(const Engine::SceneData& sceneData, Engine::FSMData& data) override;
		virtual FSMState* UpdateFirst(const Engine::SceneData& sceneData, Engine::FSMData& data) override;
		virtual FSMState* OnNotify(Engine::ObservedEvent oevent, const Engine::ObservedData& odata, Engine::FSMData& data) override;
		virtual void Exit(const Engine::SceneData& sceneData, Engine::FSMData& data) override;
	private:
		FSMState* m_pDestrState;
		float m_Speed;
		Engine::GameObject* m_pObject;
		DigDugGridComponent* m_pGrid;
	};

	

}