#include "stdafx.h"
#include "Transform.h"
#include "GameObject.h"
#include "Logger.h"
#include "MiniginMath.h"
#include "ObservedData.h"

using namespace Minigin;

Transform::Transform(GameObject& gameObject)
	: m_LocalPosition(0, 0)
	, m_LocalRotationDegrees(0)
	, m_LocalScale(1, 1)
	, m_pGameObject(&gameObject)
	, m_WorldPosition(0,0)
	, m_WorldRotationDegrees(0)
	, m_WorldScale(1,1)
{
}

const GameObject* Transform::GetGameObject() const
{
	return m_pGameObject;
}

void Transform::OnNotify(ObservedEvent event, const ObservedData& data)
{
	UNREFERENCED_PARAMETER(data);
	switch (event)
	{
	case ObservedEvent::ScaleChanged:
		m_WorldScale = GetLocalScale();
		if (m_pGameObject != nullptr)
		{
			const GameObject* pParent = GetGameObject()->GetParent();
			while (pParent != nullptr)
			{
				m_WorldScale *= pParent->GetTransform().GetLocalScale();
				pParent = pParent->GetParent();
			}
		}
		break;
	case ObservedEvent::RotationChanged:
		m_WorldRotationDegrees = GetLocalRotation(true);
		if (m_pGameObject != nullptr)
		{
			const GameObject* pParent = GetGameObject()->GetParent();
			while (pParent != nullptr)
			{
				m_WorldRotationDegrees += pParent->GetTransform().GetLocalRotation(true);
				pParent = pParent->GetParent();
			}
		}
		break;
	case ObservedEvent::PositionChanged:
		m_WorldPosition = GetLocalPosition();
		if (m_pGameObject != nullptr)
		{
			const GameObject* pParent = GetGameObject()->GetParent();
			while (pParent != nullptr)
			{
				m_WorldPosition += pParent->GetTransform().GetLocalPosition();
				pParent = pParent->GetParent();
			}
		}
		break;
	}
}


void Transform::SetWorldPosition(float x, float y)
{
	SetWorldPosition(Vector2(x, y));
}
void Transform::SetWorldPosition(const Vector2& p)
{
	SetLocalPosition(GetLocalPosition() + (p - m_WorldPosition));
}
const Vector2& Transform::GetWorldPosition() const
{
	return m_WorldPosition;
}

void Transform::SetLocalPosition(float x, float y)
{
	SetLocalPosition(Vector2(x, y));
}
void Transform::SetLocalPosition(const Vector2& p)
{
	m_LocalPosition = p;
	if (m_pGameObject)
	{
		ObservedData d{};
		m_pGameObject->Notify(ObservedEvent::PositionChanged, d);
		m_pGameObject->NotifyChildren(ObservedEvent::PositionChanged, d);
	}
}
const Vector2& Transform::GetLocalPosition() const
{
	return m_LocalPosition;
}



void Transform::SetWorldRotation(float rot, bool isdegrees)
{
	if (isdegrees)
	{
		SetLocalRotation(GetLocalRotation(true) + rot - m_WorldRotationDegrees, true);
	}
	else
	{
		SetLocalRotation(GetLocalRotation(true) + (rot*RADTODEG) - m_WorldRotationDegrees, true);
	}
}
float Transform::GetWorldRotation(bool isdegrees) const
{
	if (isdegrees)
	{
		return m_WorldRotationDegrees;
	}
	else
	{
		return m_WorldRotationDegrees * DEGTORAD;
	}
}

void Transform::SetLocalRotation(float rot, bool isdegrees)
{
	if (isdegrees) m_LocalRotationDegrees = rot;
	else
	{
		m_LocalRotationDegrees = rot * RADTODEG;
	}

	if (m_pGameObject)
	{
		ObservedData d{};
		m_pGameObject->Notify(ObservedEvent::RotationChanged, d);
		m_pGameObject->NotifyChildren(ObservedEvent::RotationChanged, d);
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
	SetLocalScale(GetLocalScale() * (s / m_WorldScale));
}
void Transform::SetWorldScale(float x, float y)
{
	SetWorldScale(Vector2(x, y));
}
const Vector2& Transform::GetWorldScale() const
{
	return m_WorldScale;
}

void Transform::SetLocalScale(const Vector2& s)
{
	m_LocalScale = s;
	if (m_pGameObject)
	{
		ObservedData d{};
		m_pGameObject->Notify(ObservedEvent::ScaleChanged, d);
		m_pGameObject->NotifyChildren(ObservedEvent::ScaleChanged, d);
	}
}
void Transform::SetLocalScale(float x, float y)
{
	SetLocalScale(Vector2(x, y));
}
const Vector2& Transform::GetLocalScale() const
{
	return m_LocalScale;
}

void Transform::SetLocalTransform(const Vector2& pos, const Vector2& scale, float rot, bool isdegrees)
{
	SetLocalPosition(pos);
	SetLocalScale(scale);
	SetLocalRotation(rot, isdegrees);
}

void Transform::SetWorldTransform(const Vector2& pos, const Vector2& scale, float rot, bool isdegrees)
{
	SetWorldPosition(pos);
	SetWorldScale(scale);
	SetWorldRotation(rot, isdegrees);
}

