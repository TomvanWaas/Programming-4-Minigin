#include "MiniginPCH.h"
#include "GameColliderCommands.h"
#include "GameObject.h"
#include "DataComponent.h"
#include "AABBCollisionComponent.h"
#include "MovementComponent.h"
#include "Enumerations.h"
#include "DigDugMovementComponent.h"

void StoneColliderCommand::Execute(AABBCollisionComponent* pOther)
{
	if (pOther != nullptr && (pOther->GetTag() == "Player" || pOther->GetTag() == "Enemy"))
	{
		if (pOther->GetGameObject() != nullptr)
		{
			//Disable Movement
			auto pMove = pOther->GetGameObject()->GetRoot().GetComponent<DigDug::DigDugMovementComponent>();
			if (pMove) pMove->SetEnabled(false);

			//Set Crushed
			auto pData = pOther->GetGameObject()->GetRoot().GetComponent<DataComponent>();
			if (pData)
			{
				pData->SetData("Health", DigDug::PlayerHealth::Crushed);
			}
		}
	}
}
