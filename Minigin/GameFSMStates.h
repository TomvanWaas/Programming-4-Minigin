#pragma once
#include "FSMState.h"
class GameObject;
class FSMStateDelay final : public FSMState
{
public:
	explicit FSMStateDelay(float delay, FSMState* pNext);
	virtual ~FSMStateDelay() = default;
	virtual void Initialize(const SceneData& sceneData, FSMData& data) override;
	virtual FSMState* UpdateFirst(const SceneData& sceneData, FSMData& data) override;
private:
	float m_Accu;
	float m_Delay;
	FSMState* m_pNextState;
};

class FSMStateDestroyObject final : public FSMState
{
public:
	explicit FSMStateDestroyObject(GameObject* pObject) : m_pObject(pObject) {}
	virtual ~FSMStateDestroyObject() = default;
	virtual FSMState* UpdateFirst(const SceneData& sceneData, FSMData& data) override;
private:
	GameObject* m_pObject;
};