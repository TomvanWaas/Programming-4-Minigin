#pragma once
#include "ColliderCommand.h"


class GameObject;



class LassoColliderCommand final : public ColliderCommand
{
public:
	explicit LassoColliderCommand(bool& hashit, bool target)
		: m_pHasHit(&hashit)
		, m_Target(target)
	{}
	virtual void Execute(AABBCollisionComponent* pOther) override;
private:
	bool* m_pHasHit;
	bool m_Target;
};





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









