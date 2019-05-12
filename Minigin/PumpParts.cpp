#include "MiniginPCH.h"
#include "PumpParts.h"
#include "ObserverEvents.h"
#include "GameObject.h"
#include "Components.h"
#include "Enumerations.h"
#include "ObservedData.h"
#include "Time.h"

using namespace DigDug;


FSMStatePumpInactive::FSMStatePumpInactive(FSMState* pActive, GameObject* pObj)
	: m_pActiveState(pActive)
	, m_pObject(pObj)
{
}
void FSMStatePumpInactive::Enter(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(sceneData);
	UNREFERENCED_PARAMETER(data);
	if (m_pObject)
	{
		//Sprite
		auto* pSprite = m_pObject->GetComponent<SpriteComponent>();
		if (pSprite) pSprite->SetCurrentSource(unsigned int(PumpSpriteID::None));

		//Collider
		auto* pCollider = m_pObject->GetComponent<AABBCollisionComponent>();
		if (pCollider) pCollider->SetEnabled(false);

		//Transform
		m_pObject->GetTransform().SetLocalPosition(0, 0);
	}
}
FSMState* FSMStatePumpInactive::OnNotify(ObservedEvent oevent, const ObservedData& odata, FSMData& data)
{
	UNREFERENCED_PARAMETER(data);
	UNREFERENCED_PARAMETER(odata);
	if (oevent == ObservedEvent::InputPumpPressed)
	{
		return m_pActiveState;
	}
	return this;
}
void FSMStatePumpInactive::Exit(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(sceneData);
	UNREFERENCED_PARAMETER(data);
	if (m_pObject)
	{
		auto* pCollider = m_pObject->GetComponent<AABBCollisionComponent>();
		if (pCollider) pCollider->SetEnabled(true);
	}
}



FSMStatePumpActive::FSMStatePumpActive(GameObject* pObj, float offset, float duration, FSMState* pInactive,
	FSMState* pHit)
	: m_pObject(pObj)
	, m_CurrentDirection(Direction::None)
	, m_Accu(0)
	, m_PlayerOffset(offset)
	, m_Duration(duration)
	, m_pInactiveState(pInactive)
	, m_pHitState(pHit)
{
}
void FSMStatePumpActive::Enter(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(sceneData);
	if (m_pObject)
	{
		m_Accu = 0;

		//Sprite & Direction
		if (!data.GetData("Direction", m_CurrentDirection)) m_CurrentDirection = Direction::Right;

		auto* pSprite = m_pObject->GetComponent<SpriteComponent>();
		pSprite->SetEnabled(true);
		pSprite->SetFreezed(false);
		if (pSprite)
		{
			switch (m_CurrentDirection)
			{
			case Direction::Right:
				pSprite->SetCurrentSource(unsigned int(PumpSpriteID::Right));
				break;
			case Direction::Left:
				pSprite->SetCurrentSource(unsigned int(PumpSpriteID::Left));
				break;
			case Direction::Down:
				pSprite->SetCurrentSource(unsigned int(PumpSpriteID::Down));
				break;
			case Direction::Up:
				pSprite->SetCurrentSource(unsigned int(PumpSpriteID::Up));
				break;
			}
		}

		auto* pCollider = m_pObject->GetComponent<AABBCollisionComponent>();
		if (pCollider)
		{
			pCollider->SetTrigger(true);
			pCollider->SetEnabled(true);
		}
	}
}
FSMState* FSMStatePumpActive::OnNotify(ObservedEvent oevent, const ObservedData& odata, FSMData& data)
{
	UNREFERENCED_PARAMETER(data);
	switch (oevent)
	{
	case ObservedEvent::ColliderEntered:
		{
		AABBCollisionComponent* pOther = nullptr;
			if (odata.GetData<AABBCollisionComponent*>("Collider", pOther) && pOther)
			{
				if (pOther->GetTag() == "Enemy")
				{
					//Notify enemy
					ObservedData d{};
					if (pOther->GetGameObject())
					{
						pOther->GetGameObject()->GetRoot().Notify(ObservedEvent::PumpHit, d);
						pOther->GetGameObject()->GetRoot().NotifyChildren(ObservedEvent::PumpHit, d);

						//Save Enemy in FSMData
						if (!data.AddData<GameObject*>("Enemy", pOther->GetGameObject()))
						{
							data.SetData("Enemy", pOther->GetGameObject());
						}
					}
					//Notify player
					if (m_pObject && m_pObject->GetParent())
					{
						m_pObject->GetRoot().Notify(ObservedEvent::PumpHit, d);
						m_pObject->GetRoot().NotifyChildren(ObservedEvent::PumpHit, d);
					}
					//Return
					return m_pHitState;
				}
			}
		}
		break;
	case ObservedEvent::InputPumpReleased:
		{
		data.SetData("Enemy", nullptr);
		return m_pInactiveState;
		}
		break;
	}
	return this;
}
FSMState* FSMStatePumpActive::UpdateFirst(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(data);
	UNREFERENCED_PARAMETER(sceneData);

	if (m_pObject)
	{
		auto* pSprite = m_pObject->GetComponent<SpriteComponent>();
		if (pSprite)
		{
			//Used for offset
			auto r = pSprite->GetCurrentSource();
			auto& t = m_pObject->GetTransform();

			switch (m_CurrentDirection)
			{
			case Direction::Right:
				t.SetLocalPosition(r.width + m_PlayerOffset, 0);
				break;
			case Direction::Left:
				t.SetLocalPosition(-(r.width + m_PlayerOffset), 0);
				break;
			case Direction::Down:
				t.SetLocalPosition(0, r.height + m_PlayerOffset);
				break;
			case Direction::Up:
				t.SetLocalPosition(0, - (r.height + m_PlayerOffset));
				break;
			}
		}

		m_Accu += sceneData.GetTime()->GetDeltaTime();
		if (m_Accu >= m_Duration)
		{
			ObservedData d{};
			m_pObject->GetRoot().Notify(ObservedEvent::InputPumpReleased, d);
			m_pObject->GetRoot().NotifyChildren(ObservedEvent::InputPumpReleased, d);
		}
	}
	return this;
}







FSMStatePumpPumping::FSMStatePumpPumping(GameObject* pObj, FSMState* pInactive)
	: m_pObject(pObj)
	, m_pInactiveState(pInactive)
{
}
void FSMStatePumpPumping::Enter(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(data);
	UNREFERENCED_PARAMETER(sceneData);
	if (m_pObject)
	{
		auto* pSprite = m_pObject->GetComponent<SpriteComponent>();
		if (pSprite) pSprite->SetFreezed(true);
	}
}
void FSMStatePumpPumping::Exit(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(data);
	UNREFERENCED_PARAMETER(sceneData);
	if (m_pObject)
	{
		auto* pSprite = m_pObject->GetComponent<SpriteComponent>();
		if (pSprite) pSprite->SetFreezed(false);
	}
}
FSMState* FSMStatePumpPumping::OnNotify(ObservedEvent oevent, const ObservedData& odata, FSMData& data)
{
	UNREFERENCED_PARAMETER(odata);

	switch (oevent)
	{
	case ObservedEvent::InputPumpReleased:
		//Notify enemy
	{
		GameObject* pEnemy = nullptr;
		if (data.GetData<GameObject*>("Enemy", pEnemy) && pEnemy)
		{
			pEnemy->GetRoot().Notify(ObservedEvent::PumpReleased, ObservedData{});
		}
	}
		return m_pInactiveState;
		break;
	}
	return this;
}












