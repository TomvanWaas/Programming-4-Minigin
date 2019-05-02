#pragma once
#include "FSMEvent.h"
#include "FSMCondition.h"
#include "Vector2.h"
#include "Enumerations.h"

class SpriteComponent;
struct InputAction;
class GameObject;
class MovementComponent;
class DigDugGridComponent;
class Transform;
class BaseComponent;
class AABBCollisionComponent;

//////////////
//Conditions//
//////////////


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

class FSMGridPositionWalkablePointCondition final : public FSMCondition
{
public:
	FSMGridPositionWalkablePointCondition(DigDugGridComponent& grid, GameObject& go, bool target = true);
	virtual ~FSMGridPositionWalkablePointCondition() = default;
	virtual bool Execute(const SceneData& sceneData) const override;
private:
	DigDugGridComponent* m_pGrid;
	Transform* m_pTransform;
	bool m_Target;
};
class FSMGridPositionPointCondition final : public FSMCondition
{
public:
	FSMGridPositionPointCondition(DigDugGridComponent& grid, Transform& t, bool target = true)
		:m_pGrid(&grid)
		, m_pTransform(&t)
		, m_Target(target)
	{}
	virtual ~FSMGridPositionPointCondition() = default;
	virtual bool Execute(const SceneData& sceneData) const override;
private:
	DigDugGridComponent* m_pGrid;
	Transform* m_pTransform;
	bool m_Target;
};

class FSMGridMarkCondition final : public FSMCondition
{
public:
	FSMGridMarkCondition(DigDugGridComponent& grid, GameObject& go, const Vector2& offset = {}, bool target = true);
	virtual ~FSMGridMarkCondition() = default;
	virtual bool Execute(const SceneData& sceneData) const override;
private:
	DigDugGridComponent* m_pGrid;
	Transform* m_pTransform;
	Vector2 m_Offset;
	bool m_Target;
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
	explicit FSMGridMarkerEvent(GameObject& go, DigDugGridComponent& grid, const Vector2& offset = {});
	virtual ~FSMGridMarkerEvent() = default;
	virtual void Execute(const SceneData& sceneData) override;
private:
	Transform* m_pTransform;
	DigDugGridComponent* m_pGrid;
	Vector2 m_Offset;
};

class FSMEnableComponentEvent final : public FSMEvent
{
public:
	FSMEnableComponentEvent(BaseComponent* pComp, bool enable)
		: m_pComponent(pComp)
		, m_Enabled(enable)
	{}
	virtual void Execute(const SceneData& sceneData) override;
private:
	BaseComponent* m_pComponent;
	bool m_Enabled;
};

class FSMSetLocalPosition final : public FSMEvent
{
public:
	FSMSetLocalPosition(const Vector2& p, Transform& t)
		: m_pTransform(&t)
		, m_Local(p)
	{}
	virtual void Execute(const SceneData& sceneData) override;
private:
	Transform* m_pTransform;
	Vector2 m_Local;
};

class FSMForceMoveEvent final : public FSMEvent
{
public:
	explicit FSMForceMoveEvent(const Vector2& vel, Transform& t)
		: m_pTransform(&t)
		, m_Velocity(vel)
	{}
	virtual void Execute(const SceneData& sceneData) override;
private:
	Vector2 m_Velocity;
	Transform* m_pTransform;
};

class FSMTriggerEvent final : public FSMEvent
{
	AABBCollisionComponent* m_pCollider;
	bool m_Target;
public:
	explicit FSMTriggerEvent(AABBCollisionComponent& col, bool tar)
		: m_Target(tar)
		, m_pCollider(&col)
	{}
	virtual void Execute(const SceneData& sceneData) override;
};
















//Pump
class FSMEventPumpSprite final : public FSMEvent
{
	DigDug::Direction* m_pDirection;
	SpriteComponent* m_pComponent;
public:
	explicit FSMEventPumpSprite(DigDug::Direction& dir, SpriteComponent* pSprite);
	virtual void Execute(const SceneData& sceneData) override;
};
class FSMEventGridMove final : public FSMEvent
{
	DigDug::Direction* m_pDirection;
	MovementComponent* m_pComponent;
	float m_Speed;
public:
	explicit FSMEventGridMove(DigDug::Direction& dir, MovementComponent* pMove, float speed);
	virtual void Execute(const SceneData& sceneData) override;
};

//Player
class FSMEventPlayerSprite final : public FSMEvent
{
	DigDug::Direction* m_pDirection;
	SpriteComponent* m_pComponent;
	DigDug::PlayerState* m_pState;
public:
	explicit FSMEventPlayerSprite(DigDug::Direction& dir, SpriteComponent* pSprite, DigDug::PlayerState& state);
	virtual void Execute(const SceneData& sceneData) override;

};




