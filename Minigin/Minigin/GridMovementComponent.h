#pragma once
#include "BaseComponent.h"
#include "Direction.h"

class MovementComponent;
class PositionGridComponent;
class GridMovementComponent final : public BaseComponent
{
public:
	explicit GridMovementComponent(PositionGridComponent* pGrid, float movespeed);
	virtual ~GridMovementComponent() override = default;

	GridMovementComponent(const GridMovementComponent& other) = delete;
	GridMovementComponent(GridMovementComponent&& other) noexcept = delete;
	GridMovementComponent& operator=(const GridMovementComponent& other) = delete;
	GridMovementComponent& operator=(GridMovementComponent&& other) noexcept = delete;

	virtual void Initialize(const SceneData& sceneData) override;
	virtual void Update(const SceneData& sceneData) override;
	virtual void ReInitialize(const SceneData& sceneData) override;

	void SetMoveSpeed(float speed);
	float GetMoveSpeed() const;
	bool Move(Direction dir);

	Direction GetCurrentDirection();

private:
	PositionGridComponent* m_pGrid;
	float m_MoveSpeed;
	MovementComponent* m_pMovementComponent;


	Direction m_CurrentDir;	//Direction that player wants
	Direction m_MoveDir;	//Direction (non-None) that keeps track of direction on grid

};

