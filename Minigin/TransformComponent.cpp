#include "MiniginPCH.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "Logger.h"

#ifndef PI
#define PI 3.14159265359f
#endif
#ifndef RADTODEG
#define RADTODEG 57.2957795f
#endif
#ifndef DEGTORAD
#define DEGTORAD 0.0174532925f 
#endif

TransformComponent::TransformComponent(GameObject& gameObject)
	: BaseComponent(gameObject)
	, m_LocalPosition(0, 0)
	, m_LocalRotationDegrees(0)
	, m_LocalScale(1, 1)
{
}




void TransformComponent::SetWorldPosition(float x, float y)
{
	SetWorldPosition(Vector2(x, y));
}
void TransformComponent::SetWorldPosition(const Vector2& p)
{
	Vector2 wp = GetWorldPosition();
	SetLocalPosition(GetLocalPosition() + (p - wp));
}
Vector2 TransformComponent::GetWorldPosition() const
{
	Vector2 p = GetLocalPosition();
	const GameObject* pParent = GetGameObject().GetParent();
	while (pParent != nullptr)
	{
		p += pParent->GetComponent<TransformComponent>()->GetLocalPosition();
		pParent = pParent->GetParent();
	}
	return p;
}

void TransformComponent::SetLocalPosition(float x, float y)
{
	SetLocalPosition(Vector2(x, y));
}
void TransformComponent::SetLocalPosition(const Vector2& p)
{
	m_LocalPosition = p;
}
const Vector2& TransformComponent::GetLocalPosition() const
{
	return m_LocalPosition;
}



void TransformComponent::SetWorldRotation(float rot, bool isdegrees)
{
	float wr = GetWorldRotation(true);
	if (isdegrees)
	{
		SetLocalRotation(GetLocalRotation(true) + rot - wr, true);
	}
	else
	{
		SetLocalRotation(GetLocalRotation(true) + (rot*RADTODEG) - wr, true);
	}
}
float TransformComponent::GetWorldRotation(bool isdegrees) const
{
	float r = GetLocalRotation(isdegrees);
	const GameObject* pParent = GetGameObject().GetParent();
	while (pParent != nullptr)
	{
		r += pParent->GetComponent<TransformComponent>()->GetLocalRotation(isdegrees);
		pParent = pParent->GetParent();
	}
	return r;
}

void TransformComponent::SetLocalRotation(float rot, bool isdegrees)
{
	if (isdegrees) m_LocalRotationDegrees = rot;
	else
	{
		m_LocalRotationDegrees = rot * RADTODEG;
	}
}
float TransformComponent::GetLocalRotation(bool isdegrees) const
{
	return (isdegrees) ? m_LocalRotationDegrees : (m_LocalRotationDegrees*DEGTORAD);
}



void TransformComponent::SetWorldScale(Vector2 s)
{
	if (s.x == 0.0f || s.y == 0.0f)
	{
		Logger::GetInstance().LogWarning("TransformComponent::SetWorldScale > Scale x or y was zero");
		if (s.x == 0.0f) s.x = FLT_EPSILON;
		if (s.y == 0.0f) s.y = FLT_EPSILON;
	}
	Vector2 ws = GetWorldScale();
	if (ws.x == 0.0f || ws.y == 0.0f)
	{
		Logger::GetInstance().LogWarning("TransformComponent::SetWorldScale > WorldScale x or y was zero");
		if (ws.x == 0.0f) ws.x = FLT_EPSILON;
		if (ws.y == 0.0f) ws.y = FLT_EPSILON;
	}
	SetLocalScale(GetLocalScale() * (s / ws));
}
void TransformComponent::SetWorldScale(float x, float y)
{
	SetWorldScale(Vector2(x, y));
}
Vector2 TransformComponent::GetWorldScale() const
{
	Vector2 s = GetLocalScale();
	const GameObject* pParent = GetGameObject().GetParent();
	while (pParent != nullptr)
	{
		s *= pParent->GetComponent<TransformComponent>()->GetLocalScale();
		pParent = pParent->GetParent();
	}
	return s;
}

void TransformComponent::SetLocalScale(const Vector2& s)
{
	m_LocalScale = s;
}
void TransformComponent::SetLocalScale(float x, float y)
{
	SetLocalScale(Vector2(x, y));
}
const Vector2& TransformComponent::GetLocalScale() const
{
	return m_LocalScale;
}

