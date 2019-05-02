#include "MiniginPCH.h"
#include "GameColliderCommands.h"
#include "GameObject.h"
#include "DataComponent.h"
#include "AABBCollisionComponent.h"
#include "MovementComponent.h"


void LassoColliderCommand::Execute(AABBCollisionComponent* pOther)
{
	if (pOther != nullptr && pOther->GetTag() == "Enemy" && pOther->GetGameObject() != nullptr)
	{
		DataComponent* pData = pOther->GetGameObject()->GetRoot().GetComponent<DataComponent>();
		if (pData != nullptr)
		{
			pData->SetData("IsHit", m_Target);
			if (m_pHasHit != nullptr)
			{
				*m_pHasHit = m_Target;
			}
		}
	}
}

void StoneColliderCommand::Execute(AABBCollisionComponent* pOther)
{
	if (pOther != nullptr && (pOther->GetTag() == "Player" || pOther->GetTag() == "Enemy"))
	{
		if (pOther->GetGameObject() != nullptr)
		{
			//Disable Movement
			auto pMove = pOther->GetGameObject()->GetRoot().GetComponent<MovementComponent>();
			if (pMove) pMove->SetEnabled(false);

			//Set Crushed
			auto pData = pOther->GetGameObject()->GetRoot().GetComponent<DataComponent>();
			if (pData)
			{
				pData->SetData("IsCrushed", true);
			}

			//Make Child of object
			pOther->GetGameObject()->GetRoot().SetParent(m_pObject);
		}
	}
}
