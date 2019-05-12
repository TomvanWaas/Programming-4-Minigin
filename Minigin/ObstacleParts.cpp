#include "MiniginPCH.h"
#include "ObstacleParts.h"
#include "GameObject.h"
#include "Components.h"
#include "DigDugGridComponent.h"
#include "Scene.h"
#include "Enumerations.h"
#include "Time.h"
#include "ObservedData.h"

using namespace DigDug;

FSMStateObstacleIdle::FSMStateObstacleIdle(DigDugGridComponent* pGrid, GameObject* pObject, FSMState* pFallState)
	: m_pGrid(pGrid)
	, m_pObject(pObject)
	, m_pFallState(pFallState)
{
}

void FSMStateObstacleIdle::Enter(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(sceneData);
	UNREFERENCED_PARAMETER(data);

	if (m_pObject)
	{
		auto* pSprite = m_pObject->GetComponent<SpriteComponent>();
		if (pSprite)
		{
			pSprite->SetCurrentSource(unsigned int(ObstacleSprite::Idle));
		}

		auto* pCollider = m_pObject->GetComponent<AABBCollisionComponent>();
		if (pCollider)
		{
			pCollider->SetTrigger(false);
		}
	}
}

FSMState* FSMStateObstacleIdle::OnNotify(ObservedEvent oevent, const ObservedData& odata, FSMData& data)
{
	UNREFERENCED_PARAMETER(odata);
	UNREFERENCED_PARAMETER(data);

	//Check if below marked on marked event
	if (oevent == ObservedEvent::GridMarked && m_pObject && m_pGrid)
	{
		const auto& t = m_pObject->GetTransform();
		auto o = m_pGrid->GetOffset();
		if (m_pGrid->IsMarked(m_pGrid->ClosestPoint(t.GetWorldPosition() + Vector2(0, o.y))))
		{
			return m_pFallState;
		}
	}
	return this;
}

void FSMStateObstacleIdle::Exit(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(sceneData);
	UNREFERENCED_PARAMETER(data);

	if (m_pObject)
	{
		auto* pSprite = m_pObject->GetComponent<SpriteComponent>();
		if (pSprite) pSprite->SetCurrentSource(unsigned int(ObstacleSprite::Moving));
	}
}


FSMStateObstacleFall::FSMStateObstacleFall(GameObject* pObject, DigDugGridComponent* pGrid, float speed,
	FSMState* pNext)
	: m_pObject(pObject)
	, m_pGrid(pGrid)
	, m_Speed(speed)
	, m_pDestrState(pNext)
{
}

void FSMStateObstacleFall::Enter(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(sceneData);
	UNREFERENCED_PARAMETER(data);

	if (m_pObject)
	{
		auto* pSprite = m_pObject->GetComponent<SpriteComponent>();
		if (pSprite) pSprite->SetCurrentSource(unsigned int(ObstacleSprite::Moving));

		auto* pCol = m_pObject->GetComponent<AABBCollisionComponent>();
		if (pCol) pCol->SetTrigger(true);

		//Mark self and below
		if (m_pGrid)
		{
			const auto& t = m_pObject->GetTransform();
			Vector2 offset = m_pGrid->GetOffset()*0.5f;
			m_pGrid->Mark(m_pGrid->ClosestPoint(t.GetWorldPosition()));
			m_pGrid->Mark(m_pGrid->ClosestPoint(t.GetWorldPosition() + Vector2(0, offset.y)));
		}
	}
}

FSMState* FSMStateObstacleFall::UpdateFirst(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(data);

	if (m_pGrid && m_pObject)
	{
		auto& t = m_pObject->GetTransform();
		t.SetLocalPosition(t.GetLocalPosition() + Vector2(0, m_Speed * sceneData.GetTime()->GetDeltaTime()));

		//ToNext if !Marked below
		Vector2 offset = m_pGrid->GetOffset()*0.5f;
		if (!m_pGrid->IsMarked(m_pGrid->ClosestPoint(t.GetWorldPosition() + Vector2(0, offset.y))) && m_pGrid->IsOnPoint(t.GetWorldPosition()))
		{
			return m_pDestrState;
		}
	}
	return this;
}

FSMState* FSMStateObstacleFall::OnNotify(ObservedEvent oevent, const ObservedData& odata, FSMData& data)
{
	if (oevent == ObservedEvent::ColliderEntered)
	{
		AABBCollisionComponent* pCollider = nullptr;
		if (odata.GetData<AABBCollisionComponent*>("Collider", pCollider) && pCollider != nullptr && pCollider->GetGameObject())
		{
			if (pCollider->GetTag() == "Player"  || pCollider->GetTag() == "Enemy")
			{
				pCollider->GetGameObject()->GetRoot().Notify(ObservedEvent::HitByObstacle, ObservedData{});
				pCollider->GetGameObject()->GetRoot().NotifyChildren(ObservedEvent::HitByObstacle, ObservedData{});
			}
		}
	}
	UNREFERENCED_PARAMETER(data);
	return this;
}

void FSMStateObstacleFall::Exit(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(sceneData);
	UNREFERENCED_PARAMETER(data);

	//Disable collider, set sprite
	if (m_pObject)
	{
		auto* pSprite = m_pObject->GetComponent<SpriteComponent>();
		if (pSprite) pSprite->SetCurrentSource(unsigned int(ObstacleSprite::Destroyed));

		auto* pCol = m_pObject->GetComponent<AABBCollisionComponent>();
		if (pCol) pCol->SetEnabled(false);
	}
}

