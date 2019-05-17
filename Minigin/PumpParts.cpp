#include "MiniginPCH.h"
#include "PumpParts.h"
#include "ObservedEvent.h"
#include "GameObject.h"
#include "Components.h"
#include "Enumerations.h"
#include "ObservedData.h"
#include "Time.h"
#include "GameEvents.h"
#include "DigDugMovementComponent.h"


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
		if (pSprite)
		{
			pSprite->SetCurrentSprite(unsigned int(PumpSpriteID::None));
			
		}

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
	if (oevent == GameEvent::InputPumpPressed)
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
	, m_Deactivated(false)
{
}
void FSMStatePumpActive::Enter(const SceneData& sceneData, FSMData& data)
{
	UNREFERENCED_PARAMETER(sceneData);
	if (m_pObject)
	{
		m_Accu = 0;
		m_Deactivated = false;

		//Set data
		auto parent = GetGameObject()->GetParent();
		if (parent)
		{
			auto move = parent->GetComponent<DigDugMovementComponent>();
			if (move)
			{
				if (!data.SetData("Direction", move->GetLookDirection()))
				{
					data.AddData("Direction", move->GetLookDirection());
				}				
			}
		}

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
				pSprite->SetCurrentSprite(unsigned int(PumpSpriteID::Right));
				break;
			case Direction::Left:
				pSprite->SetCurrentSprite(unsigned int(PumpSpriteID::Left));
				break;
			case Direction::Down:
				pSprite->SetCurrentSprite(unsigned int(PumpSpriteID::Down));
				break;
			case Direction::Up:
				pSprite->SetCurrentSprite(unsigned int(PumpSpriteID::Up));
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
					if (!m_Deactivated)
					{
						//Notify player
						if (m_pObject)
						{
							ObservedData d{};
							m_pObject->GetRoot().Notify(GameEvent::PumpHit, d);
							m_pObject->GetRoot().NotifyChildren(GameEvent::PumpHit, d);
						}
						//Return
						return m_pHitState;
					}
					else
					{
						//Notify player
						if (m_pObject )
						{
							ObservedData d{};
							m_pObject->GetRoot().Notify(GameEvent::PumpMissed, d);
							m_pObject->GetRoot().NotifyChildren(GameEvent::PumpMissed, d);
						}
						//Return
						return m_pInactiveState;
					}
				}
			}
		}
		break;
	case GameEvent::InputPumpReleased:
		{
		m_Deactivated = true;
		}
		break;
	case GameEvent::InputPumpPressed:
		{
		m_Deactivated = false;
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
			const Vector2& scale = m_pObject->GetTransform().GetWorldScale();

			Vector2 offset = Vector2::Zero;
			if (m_pObject->GetParent())
			{
				AABBCollisionComponent* pC = m_pObject->GetParent()->GetComponent<AABBCollisionComponent>();
				if (pC)
				{
					offset.x = pC->GetCollider().width * 0.5f;
					offset.y = pC->GetCollider().height * 0.5f;
				}
			}
			switch (m_CurrentDirection)
			{
			case Direction::Right:
				t.SetLocalPosition(r.width*0.5f * scale.x + m_PlayerOffset + offset.x, 0);
				break;
			case Direction::Left:
				t.SetLocalPosition(-(r.width*0.5f * scale.x + m_PlayerOffset + offset.x), 0);
				break;
			case Direction::Down:
				t.SetLocalPosition(0, r.height*0.5f * scale.y + m_PlayerOffset + offset.y);
				break;
			case Direction::Up:
				t.SetLocalPosition(0, - (r.height*0.5f * scale.y + m_PlayerOffset + offset.y));
				break;
			}
		}

		m_Accu += sceneData.GetTime()->GetDeltaTime();
		if (m_Accu >= m_Duration)
		{
			ObservedData d{};
			m_pObject->GetRoot().Notify(GameEvent::PumpMissed, d);
			m_pObject->GetRoot().NotifyChildren(GameEvent::PumpMissed, d);
			return m_pInactiveState;
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
	UNREFERENCED_PARAMETER(data);
	UNREFERENCED_PARAMETER(odata);

	switch (oevent)
	{
	case GameEvent::InputMovePressed:
	case GameEvent::InputPumpReleased:
		if (GetGameObject()) GetGameObject()->GetRoot().Notify(GameEvent::PumpMissed, ObservedData{});
		return m_pInactiveState;
		break;
	case ObservedEvent::ColliderExited:
	{
		AABBCollisionComponent* pOther = nullptr;
		if (odata.GetData<AABBCollisionComponent*>("Collider", pOther) && pOther)
		{
			if (pOther->GetTag() == "Enemy")
			{
				//Notify player
				if (m_pObject && m_pObject->GetParent())
				{
					ObservedData d{};
					m_pObject->GetRoot().Notify(GameEvent::PumpMissed, d);
				}
				//Return
				return m_pInactiveState;
			}
		}
	}
	}
	return this;
}












