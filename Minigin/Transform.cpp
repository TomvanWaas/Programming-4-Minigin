#include "MiniginPCH.h"
#include "Transform.h"
#include "GameObject.h"
#include "Logger.h"
#include "MiniginMath.h"


Transform::Transform(GameObject& gameObject)
	: m_LocalPosition(0, 0)
	, m_LocalRotationDegrees(0)
	, m_LocalScale(1, 1)
	, m_pGameObject(&gameObject)
{
}

const GameObject* Transform::GetGameObject() const
{
	return m_pGameObject;
}



void Transform::SetWorldPosition(float x, float y)
{
	SetWorldPosition(Vector2(x, y));
}
void Transform::SetWorldPosition(const Vector2& p)
{
	Vector2 wp = GetWorldPosition();
	SetLocalPosition(GetLocalPosition() + (p - wp));
}
Vector2 Transform::GetWorldPosition() const
{
	Vector2 p = GetLocalPosition();
	if (m_pGameObject != nullptr)
	{
		const GameObject* pParent = GetGameObject()->GetParent();
		while (pParent != nullptr)
		{
			p += pParent->GetTransform().GetLocalPosition();
			pParent = pParent->GetParent();
		}
	}
	return p;
}

void Transform::SetLocalPosition(float x, float y)
{
	SetLocalPosition(Vector2(x, y));
}
void Transform::SetLocalPosition(const Vector2& p)
{
	m_LocalPosition = p;
}
const Vector2& Transform::GetLocalPosition() const
{
	return m_LocalPosition;
}



void Transform::SetWorldRotation(float rot, bool isdegrees)
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
float Transform::GetWorldRotation(bool isdegrees) const
{
	float r = GetLocalRotation(isdegrees);
	if (m_pGameObject != nullptr)
	{
		const GameObject* pParent = GetGameObject()->GetParent();
		while (pParent != nullptr)
		{
			r += pParent->GetTransform().GetLocalRotation(isdegrees);
			pParent = pParent->GetParent();
		}
	}
	return r;
}

void Transform::SetLocalRotation(float rot, bool isdegrees)
{
	if (isdegrees) m_LocalRotationDegrees = rot;
	else
	{
		m_LocalRotationDegrees = rot * RADTODEG;
	}
}
float Transform::GetLocalRotation(bool isdegrees) const
{
	return (isdegrees) ? m_LocalRotationDegrees : (m_LocalRotationDegrees*DEGTORAD);
}



void Transform::SetWorldScale(Vector2 s)
{
	if (s.x == 0.0f || s.y == 0.0f)
	{
		Logger::GetInstance().LogWarning("Transform::SetWorldScale > Scale x or y was zero");
		if (s.x == 0.0f) s.x = FLT_EPSILON;
		if (s.y == 0.0f) s.y = FLT_EPSILON;
	}
	Vector2 ws = GetWorldScale();
	if (ws.x == 0.0f || ws.y == 0.0f)
	{
		Logger::GetInstance().LogWarning("Transform::SetWorldScale > WorldScale x or y was zero");
		if (ws.x == 0.0f) ws.x = FLT_EPSILON;
		if (ws.y == 0.0f) ws.y = FLT_EPSILON;
	}
	SetLocalScale(GetLocalScale() * (s / ws));
}
void Transform::SetWorldScale(float x, float y)
{
	SetWorldScale(Vector2(x, y));
}
Vector2 Transform::GetWorldScale() const
{
	Vector2 s = GetLocalScale();
	if (m_pGameObject != nullptr)
	{
		const GameObject* pParent = GetGameObject()->GetParent();
		while (pParent != nullptr)
		{
			s *= pParent->GetTransform().GetLocalScale();
			pParent = pParent->GetParent();
		}
	}
	return s;
}

void Transform::SetLocalScale(const Vector2& s)
{
	m_LocalScale = s;
}
void Transform::SetLocalScale(float x, float y)
{
	SetLocalScale(Vector2(x, y));
}
const Vector2& Transform::GetLocalScale() const
{
	return m_LocalScale;
}

