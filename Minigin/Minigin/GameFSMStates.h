#pragma once
#include "FSMState.h"
namespace Engine
{
	class GameObject;
}

namespace DigDug
{

	class FSMStateDelay final : public Engine::FSMState
	{
	public:
		explicit FSMStateDelay(float delay, FSMState* pNext);
		virtual ~FSMStateDelay() = default;
		virtual void Enter(const Engine::SceneData& sceneData, Engine::FSMData& data) override;
		virtual FSMState* UpdateFirst(const Engine::SceneData& sceneData, Engine::FSMData& data) override;
	private:
		float m_Accu;
		float m_Delay;
		FSMState* m_pNextState;
	};

	class FSMStateDestroyObject final : public Engine::FSMState
	{
	public:
		explicit FSMStateDestroyObject(Engine::GameObject* pObject) : m_pObject(pObject) {}
		virtual ~FSMStateDestroyObject() = default;
		virtual FSMState* UpdateFirst(const Engine::SceneData& sceneData, Engine::FSMData& data) override;
	private:
		Engine::GameObject* m_pObject;
	};
}