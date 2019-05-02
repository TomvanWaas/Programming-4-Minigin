#include "MiniginPCH.h"
#include "DigDugMovementComponent.h"
#include "Enumerations.h"
#include "GameObject.h"
#include "DigDugGridComponent.h"
#include "MovementComponent.h"
#include "SceneData.h"
#include "Time.h"

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
	if (m_pGrid != nullptr && m_pMovement != nullptr)
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
				if (m_pGrid->IsOnWalkablePoint(GetGameObject()->GetTransform().GetWorldPosition()))
				{
					m_CurrentDirection = m_WantedDirection;
				}
				else
				{
					m_CurrentDirection = m_PreviousDirection;
				}
			}



			if (m_CurrentDirection != Direction::None) m_PreviousDirection = m_CurrentDirection;

			//Process Direction
			switch (m_CurrentDirection)
			{
			case Direction::Right:
				m_pMovement->QueueMovement(Vector2(m_Speed*sceneData.pTime->GetDeltaTime(), 0));
				break;
			case Direction::Down:
				m_pMovement->QueueMovement(Vector2(0, m_Speed*sceneData.pTime->GetDeltaTime()));
				break;
			case Direction::Left:
				m_pMovement->QueueMovement(Vector2(-m_Speed * sceneData.pTime->GetDeltaTime(), 0));
				break;
			case Direction::Up:
				m_pMovement->QueueMovement(Vector2(0, -m_Speed * sceneData.pTime->GetDeltaTime()));
				break;
			}
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
