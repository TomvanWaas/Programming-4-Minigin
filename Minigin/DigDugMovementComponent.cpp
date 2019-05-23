#include "MiniginPCH.h"
#include "DigDugMovementComponent.h"
#include "Enumerations.h"
#include "GameObject.h"
#include "DigDugGridComponent.h"
#include "MovementComponent.h"
#include "SceneData.h"
#include "Time.h"

using namespace DigDug;

DigDug::DigDugMovementComponent::DigDugMovementComponent(DigDugGridComponent& grid, float speed)
	: m_pGrid(&grid)
	, m_pMovement(nullptr)
	, m_CurrentDirection(Direction::None)
	, m_Speed(speed)
	, m_WantedDirection(Direction::None)
	, m_PreviousDirection(Direction::Right)
{
}

void DigDug::DigDugMovementComponent::InitializeOverride(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);
	if (GetGameObject() != nullptr)
	{
		m_pMovement = GetGameObject()->GetComponent<MovementComponent>();
	}
}

void DigDug::DigDugMovementComponent::UpdateFirstOverride(const SceneData& sceneData)
{
	m_CurrentDirection = Direction::None;
	if (m_pGrid != nullptr && m_pMovement != nullptr && GetGameObject() != nullptr)
	{
		//Process Wanted
		if (m_WantedDirection != Direction::None)
		{
			//Same line
			if (m_WantedDirection == m_PreviousDirection
				|| Opposite(m_WantedDirection, m_PreviousDirection))
			{
				m_CurrentDirection = m_WantedDirection;
			}
			//Cross
			else
			{
				if (m_pGrid->IsOnWalkablePoint(GetGameObject()->GetTransform().GetWorldPosition(), m_Speed * 0.05f))
				{
					m_CurrentDirection = m_WantedDirection;
					GetGameObject()->GetTransform().SetWorldPosition(m_pGrid->ClosestPoint(GetGameObject()->GetTransform().GetWorldPosition()));
				}
				else
				{
					m_CurrentDirection = m_PreviousDirection;
				}
			}



			if (m_CurrentDirection != Direction::None) m_PreviousDirection = m_CurrentDirection;

			//Process Direction
			Vector2 move = Vector2::Zero;
			switch (m_CurrentDirection)
			{
			case Direction::Right:
				move = Vector2(m_Speed*sceneData.GetTime()->GetDeltaTime(), 0);
				break;
			case Direction::Down:
				move = Vector2(0, m_Speed*sceneData.GetTime()->GetDeltaTime());
				break;
			case Direction::Left:
				move = Vector2(-m_Speed * sceneData.GetTime()->GetDeltaTime(), 0);
				break;
			case Direction::Up:
				move = Vector2(0, -m_Speed * sceneData.GetTime()->GetDeltaTime());
				break;
			}

			const auto wp = GetGameObject()->GetTransform().GetWorldPosition();

			//Clamp to grid
			move = m_pGrid->ClosestWalkableGrid(move + wp) - wp;
			m_pMovement->QueueMovement(move);
		}
	}
	m_WantedDirection = Direction::None;
}


void DigDug::DigDugMovementComponent::Move(Direction d)
{
	if (d != Direction::None)
	{
		//Same as current
		if (m_PreviousDirection == d) m_WantedDirection = d;
		else if (Opposite(m_PreviousDirection, d) && m_WantedDirection != m_PreviousDirection) m_WantedDirection = d;
		else if (m_WantedDirection == Direction::None) m_WantedDirection = d;
	}
}

bool DigDug::DigDugMovementComponent::Opposite(Direction a, Direction b) const
{
	return( (a == Direction::Right && b == Direction::Left)
		|| (a == Direction::Left && b == Direction::Right)
		|| (a == Direction::Down && b == Direction::Up)
		|| (a == Direction::Up && b == Direction::Down));
}
