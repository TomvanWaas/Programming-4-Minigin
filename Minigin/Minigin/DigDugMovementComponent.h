#pragma once
#include "BaseComponent.h"
namespace Engine
{
	class MovementComponent;
}
namespace DigDug
{
	class DigDugGridComponent;
	enum class Direction;


	class DigDugMovementComponent final : public Engine::BaseComponent
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
		virtual void InitializeOverride(const Engine::SceneData& sceneData) override;
		virtual void UpdateFirstOverride(const Engine::SceneData& sceneData) override;
	private:
		Engine::MovementComponent* m_pMovement;
		DigDugGridComponent* m_pGrid;
		Direction m_PreviousDirection;
		Direction m_CurrentDirection;
		Direction m_WantedDirection;
		float m_Speed;

		bool Opposite(Direction a, Direction b) const;
	};

}