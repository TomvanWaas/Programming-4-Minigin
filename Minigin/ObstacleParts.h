#pragma once
#include "FSMState.h"
class GameObject;
class DigDugGridComponent;

namespace DigDug
{
	class FSMStateObstacleIdle final : public FSMState
	{
	public:
		explicit FSMStateObstacleIdle(DigDugGridComponent* pGrid, GameObject* pObject, FSMState* pFallState);
		virtual ~FSMStateObstacleIdle() = default;
		virtual void Enter(const SceneData& sceneData, FSMData& data) override;
		virtual FSMState* OnNotify(ObservedEvent oevent, const ObservedData& odata, FSMData& data) override;
		virtual void Exit(const SceneData& sceneData, FSMData& data) override;
	private:
		DigDugGridComponent* m_pGrid;
		GameObject* m_pObject;
		FSMState* m_pFallState;
	};

	class FSMStateObstacleFall final : public FSMState
	{
	public:
		explicit FSMStateObstacleFall(GameObject* pObject, DigDugGridComponent* pGrid, float speed, FSMState* pNext);
		virtual ~FSMStateObstacleFall() = default;
		virtual void Enter(const SceneData& sceneData, FSMData& data) override;
		virtual FSMState* UpdateFirst(const SceneData& sceneData, FSMData& data) override;
		virtual FSMState* OnNotify(ObservedEvent oevent, const ObservedData& odata, FSMData& data) override;
		virtual void Exit(const SceneData& sceneData, FSMData& data) override;
	private:
		FSMState* m_pDestrState;
		float m_Speed;
		GameObject* m_pObject;
		DigDugGridComponent* m_pGrid;
	};

	

}