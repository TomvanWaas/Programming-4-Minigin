#pragma once
#include "ColliderCommand.h"


class GameObject;



class StoneColliderCommand final : public ColliderCommand
{
public:
	StoneColliderCommand(GameObject& obj)
		: m_pObject(&obj)
	{}
	virtual void Execute(AABBCollisionComponent* pOther) override;
private:
	GameObject* m_pObject;
};









