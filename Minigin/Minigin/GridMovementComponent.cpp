#include "MiniginPCH.h"
#include "GridMovementComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "PositionGridComponent.h"
#include "Vector2.h"
#include "SceneData.h"
#include "Time.h"
#include "MovementComponent.h"

GridMovementComponent::GridMovementComponent(PositionGridComponent* pGrid, float movespeed)
	: m_pGrid(pGrid)
	, m_MoveSpeed(movespeed)
	, m_CurrentDir(Direction::None)
	, m_MoveDir(Direction::Down)
	, m_pMovementComponent(nullptr)
{
}


void GridMovementComponent::Initialize(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);

	if (m_pGameObject != nullptr)
	{
		m_pMovementComponent = m_pGameObject->GetComponent<MovementComponent>();
	}
}

void GridMovementComponent::Update(const SceneData& sceneData)
{
	if (m_pGameObject != nullptr && m_pGrid != nullptr && m_pMovementComponent != nullptr)
	{
		auto* pTransform = m_pGameObject->GetComponent<TransformComponent>();
		const Vector2 worldpos = pTransform->GetWorldPosition();

		//Change MoveDir according to CurrDir
		if (m_CurrentDir != Direction::None && m_CurrentDir != m_MoveDir)
		{
			//Same Axis?
			if (((m_CurrentDir == Direction::Left || m_CurrentDir == Direction::Right) && (m_MoveDir == Direction::Left || m_MoveDir == Direction::Right))
				||
				((m_CurrentDir == Direction::Up || m_CurrentDir == Direction::Down) && (m_MoveDir == Direction::Up || m_MoveDir == Direction::Down)))
			{
				m_MoveDir = m_CurrentDir;
			}
			//Different Axis? (Only allowed on gridpoint)
			else if (m_pGrid->IsOnPoint(worldpos))
			{
				m_MoveDir = m_CurrentDir;
			}
		}

		//Update Movement
		if (m_CurrentDir != Direction::None && m_MoveDir != Direction::None)
		{
			//Move
			Vector2 move(0, 0);

			switch (m_MoveDir)
			{
			case Direction::Up:
				move.y -= m_MoveSpeed * sceneData.pTime->GetDeltaTime();
				break;
			case Direction::Down:
				move.y += m_MoveSpeed * sceneData.pTime->GetDeltaTime();
				break;
			case Direction::Left:
				move.x -= m_MoveSpeed * sceneData.pTime->GetDeltaTime();
				break;
			case Direction::Right:
				move.x += m_MoveSpeed * sceneData.pTime->GetDeltaTime();
				break;
			}

			move = m_pGrid->ClampToGrid(m_pGrid->ClampToLine(worldpos + move));
			m_pMovementComponent->MovePosition(move - worldpos);
		}
	}
}

void GridMovementComponent::ReInitialize(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);

	if (m_pGameObject != nullptr)
	{
		m_pMovementComponent = m_pGameObject->GetComponent<MovementComponent>();
	}
}

void GridMovementComponent::SetMoveSpeed(float speed)
{
	m_MoveSpeed = speed;
}
float GridMovementComponent::GetMoveSpeed() const
{
	return m_MoveSpeed;
}


bool GridMovementComponent::Move(Direction dir)
{
	m_CurrentDir = dir;

	return true;
}

Direction GridMovementComponent::GetCurrentDirection()
{
	return m_CurrentDir;
}


