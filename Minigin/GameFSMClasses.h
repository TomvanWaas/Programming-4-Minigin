#pragma once
#include "FSMEvent.h"
#include "FSMCondition.h"
#include "Vector2.h"


struct InputAction;
class GameObject;
class MovementComponent;
class SpriteComponent;
class DigDugGridComponent;
class TransformComponent;

//////////////
//Conditions//
//////////////


template <class T>
class FSMDataRefCondition final : public FSMCondition
{
public:
	explicit FSMDataRefCondition(const T& data, const T& target)
		: m_pData(&data)
		, m_Target(target)
	{}
	virtual ~FSMDataRefCondition() = default;
	virtual bool Execute(const SceneData& sceneData) const override
	{
		UNREFERENCED_PARAMETER(sceneData);
		return (m_pData != nullptr &&
			*m_pData == m_Target);
	}
private:
	const T* m_pData;
	T m_Target;
};

template <class T>
class FSMDataValCondition final : public FSMCondition
{
public:
	explicit FSMDataValCondition(const T& data, const T& target)
		: m_Data(data)
		, m_Target(target)
	{}
	virtual ~FSMDataValCondition() = default;
	virtual bool Execute(const SceneData& sceneData) const override
	{
		return (m_Data == m_Target);
	}
	void SetData(const T& data) { m_Data = data; }
	const T& GetData() const { return m_Data; }
	T& GetData() { return m_Data; }
	void SetTarget(const T& target) { m_Target = target; }
	const T& GetTarget() const { return m_Target; }
	T& GetTarget() { return m_Target; }
private:
	T m_Data;
	T m_Target;
};

class FSMGridCondition final : public FSMCondition
{
public:
	FSMGridCondition(DigDugGridComponent& grid, GameObject& go);
	virtual ~FSMGridCondition() = default;
	virtual bool Execute(const SceneData& sceneData) const override;
private:
	DigDugGridComponent* m_pGrid;
	TransformComponent* m_pTransform;
};


//////////////
//Events//////
//////////////

class FSMMoveEvent final : public FSMEvent
{
public:
	explicit FSMMoveEvent(const Vector2& velocity, GameObject& go);
	virtual ~FSMMoveEvent() = default;
	virtual void Execute(const SceneData& sceneData) override;
	void SetVelocity(const Vector2& velocity);
	const Vector2& GetVelocity() const;
	Vector2& GetVelocity();
private:
	Vector2 m_Velocity;
	MovementComponent* m_pMovementComp;
};

class FSMSpriteEvent final : public FSMEvent
{
public:
	explicit FSMSpriteEvent(unsigned int spriteID, GameObject& go);
	virtual ~FSMSpriteEvent() = default;
	virtual void Execute(const SceneData& sceneData) override;
private:
	unsigned int m_SpriteID;
	SpriteComponent* m_pSpriteComp;
};

class FSMFunctionEvent final : public FSMEvent
{
public:
	explicit FSMFunctionEvent(GameObject& go, void func(GameObject*, const SceneData&));
	virtual ~FSMFunctionEvent() = default;
	virtual void Execute(const SceneData& sceneData) override;
private:
	GameObject* m_pGameObject;
	void(* m_Function)(GameObject*, const SceneData&);
};

class FSMGridMarkerEvent final : public FSMEvent
{
public:
	explicit FSMGridMarkerEvent(GameObject& go, DigDugGridComponent& grid);
	virtual ~FSMGridMarkerEvent() = default;
	virtual void Execute(const SceneData& sceneData) override;
private:
	TransformComponent* m_pTransform;
	DigDugGridComponent* m_pGrid;
};