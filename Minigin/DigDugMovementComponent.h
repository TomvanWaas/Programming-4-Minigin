#pragma once
#include "BaseComponent.h"
class MovementComponent;
class DigDugGridComponent;
namespace DigDug
{
	enum class Direction;


	class DigDugMovementComponent final : public BaseComponent
	{
	public:
		explicit DigDugMovementComponent(DigDugGridComponent& grid, float speed);
		virtual  ~DigDugMovementComponent() = default;


		void Move(Direction d);
		void SetSpeed(float s) { m_Speed = s; }
		float GetSpeed() const { return m_Speed; }

		Direction& GetLookDirection() { return m_PreviousDirection; }
		Direction& GetMoveDirection() { return m_CurrentDirection; }

	protected:
		virtual void InitializeOverride(const SceneData& sceneData) override;
		virtual void UpdateFirstOverride(const SceneData& sceneData) override;
	private:
		MovementComponent* m_pMovement;
		DigDugGridComponent* m_pGrid;
		Direction m_PreviousDirection;
		Direction m_CurrentDirection;
		Direction m_WantedDirection;
		float m_Speed;

		bool Opposite(Direction a, Direction b) const;
	};

}