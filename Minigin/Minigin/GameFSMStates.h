#pragma once
#include "FSMState.h"
namespace Minigin
{
	class GameObject;
}

namespace DigDug
{

	class FSMStateDelay final : public Minigin::FSMState
	{
	public:
		explicit FSMStateDelay(float delay, FSMState* pNext);
		virtual ~FSMStateDelay() = default;
		virtual void Enter(const Minigin::SceneData& sceneData, Minigin::FSMData& data) override;
		virtual FSMState* UpdateFirst(const Minigin::SceneData& sceneData, Minigin::FSMData& data) override;
	private:
		float m_Accu;
		float m_Delay;
		FSMState* m_pNextState;
	};

	class FSMStateDestroyObject final : public Minigin::FSMState
	{
	public:
		explicit FSMStateDestroyObject(Minigin::GameObject* pObject) : m_pObject(pObject) {}
		virtual ~FSMStateDestroyObject() = default;
		virtual FSMState* UpdateFirst(const Minigin::SceneData& sceneData, Minigin::FSMData& data) override;
	private:
		Minigin::GameObject* m_pObject;
	};
}