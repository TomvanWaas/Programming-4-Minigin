#pragma once
#include "FSMState.h"

namespace Minigin
{
	class GameObject;
}
namespace DigDug
{

	class DigDugGridComponent;

	class FSMStateObstacleIdle final : public Minigin::FSMState
	{
	public:
		explicit FSMStateObstacleIdle(DigDugGridComponent* pGrid, Minigin::GameObject* pObject, FSMState* pFallState);
		virtual ~FSMStateObstacleIdle() = default;
		virtual void Enter(const Minigin::SceneData& sceneData, Minigin::FSMData& data) override;
		virtual FSMState* OnNotify(Minigin::ObservedEvent oevent, const Minigin::ObservedData& odata, Minigin::FSMData& data) override;
		virtual void Exit(const Minigin::SceneData& sceneData, Minigin::FSMData& data) override;
	private:
		DigDugGridComponent* m_pGrid;
		Minigin::GameObject* m_pObject;
		FSMState* m_pFallState;
	};

	class FSMStateObstacleFall final : public Minigin::FSMState
	{
	public:
		explicit FSMStateObstacleFall(Minigin::GameObject* pObject, DigDugGridComponent* pGrid, float speed, FSMState* pNext);
		virtual ~FSMStateObstacleFall() = default;
		virtual void Enter(const Minigin::SceneData& sceneData, Minigin::FSMData& data) override;
		virtual FSMState* UpdateFirst(const Minigin::SceneData& sceneData, Minigin::FSMData& data) override;
		virtual FSMState* OnNotify(Minigin::ObservedEvent oevent, const Minigin::ObservedData& odata, Minigin::FSMData& data) override;
		virtual void Exit(const Minigin::SceneData& sceneData, Minigin::FSMData& data) override;
	private:
		FSMState* m_pDestrState;
		float m_Speed;
		Minigin::GameObject* m_pObject;
		DigDugGridComponent* m_pGrid;
	};

	

}