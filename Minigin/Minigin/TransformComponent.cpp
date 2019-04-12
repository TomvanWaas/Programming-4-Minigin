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


TransformComponent::TransformComponent()
	: m_LocalPosition(0, 0)
	, m_LocalRotationDegrees(0)
	, m_LocalScale(1, 1)
	, m_pParent(nullptr)
{
}

void TransformComponent::Initialize(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);

	if (m_pGameObject != nullptr && m_pGameObject->GetParent() != nullptr)
	{
		m_pParent = m_pGameObject->GetParent()->GetComponent<TransformComponent>();
	}
}
void TransformComponent::ReInitialize(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);

	m_pParent = nullptr;
	if (m_pGameObject != nullptr && m_pGameObject->GetParent() != nullptr)
	{
		m_pParent = m_pGameObject->GetParent()->GetComponent<TransformComponent>();
	}
}



void TransformComponent::SetWorldPosition(float x, float y)
{
	SetWorldPosition(Vector2(x, y));
}
void TransformComponent::SetWorldPosition(const Vector2& p)
{
	Vector2 wp = GetWorldPosition();
	m_LocalPosition += (p - wp);
}
Vector2 TransformComponent::GetWorldPosition() const
{
	Vector2 p(0, 0);
	const TransformComponent* pParent = this;
	while (pParent != nullptr)
	{
		p += pParent->GetLocalPosition();
		pParent = pParent->m_pParent;
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
		m_LocalRotationDegrees += rot - wr;
	}
	else
	{
		m_LocalRotationDegrees += (rot*RADTODEG) - wr;
	}
}
float TransformComponent::GetWorldRotation(bool isdegrees) const
{
	float r = 0.0f;
	const TransformComponent* pParent = this;
	while (pParent != nullptr)
	{
		r += pParent->GetLocalRotation(isdegrees);
		pParent = pParent->m_pParent;
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
	m_LocalScale *= s / ws;
}
void TransformComponent::SetWorldScale(float x, float y)
{
	SetWorldScale(Vector2(x, y));
}
Vector2 TransformComponent::GetWorldScale() const
{
	Vector2 s(1, 1);
	const TransformComponent* pParent = this;
	while (pParent != nullptr)
	{
		s *= pParent->m_LocalScale;
		pParent = pParent->m_pParent;
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

