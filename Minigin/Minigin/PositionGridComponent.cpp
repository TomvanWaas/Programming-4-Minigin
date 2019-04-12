#include "MiniginPCH.h"
#include "PositionGridComponent.h"
#include "Logger.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "SceneData.h"

PositionGridComponent::PositionGridComponent(unsigned int width, unsigned int height, const Vector2& offset)
	: m_Width(width)
	, m_Height(height)
	, m_Offset(offset)
	, Renderable(1)
{
	
}


void PositionGridComponent::Initialize(const SceneData& sceneData)
{
	RegisterRenderable(sceneData.pRenderManager);
}

bool PositionGridComponent::IsValid(unsigned w, unsigned h) const
{
	return (w < m_Width && h < m_Height);
}
Vector2 PositionGridComponent::Get(unsigned int w, unsigned int h) const
{
	if (m_pGameObject == nullptr)
	{
		Logger::GetInstance().LogError("PositionGridComponent::Get > no gameObject attached");
		return Vector2{};
	}
	if (IsValid(w, h))
	{
		return GetUnsafe(w, h);
	}
	Logger::GetInstance().LogError("PositionGridComponent::Get > Index out of bounds");
	return Vector2{};
}


bool PositionGridComponent::IsOnPoint(const Vector2& pos, float off) const
{
	Vector2 v = ClampToPoint(pos);
	return ((pos.x - v.x)*(pos.x - v.x) < off && (pos.y - v.y)*(pos.y - v.y) < off);
}
Vector2 PositionGridComponent::ClampToPoint(const Vector2& pos) const
{
	//Clamp to lines between gridpoints
	if (m_pGameObject == nullptr)
	{
		Logger::GetInstance().LogError("PositionGridComponent::ClampToPoint > No gameObject attached");
		return pos;
	}

	const auto* pTransform = m_pGameObject->GetComponent<TransformComponent>();
	const auto world = pTransform->GetWorldPosition();
	const auto scale = pTransform->GetWorldScale();

	Vector2 result = pos - world;
	Vector2 b = result;
	b.x = float(int(b.x / (m_Offset.x*scale.x)) * m_Offset.x*scale.x);
	b.y = float(int(b.y / (m_Offset.y*scale.y)) * m_Offset.y*scale.y);

	//X
	if ((result.x - b.x)*(result.x - b.x) > (m_Offset.x *0.5f*scale.x)*(m_Offset.x*0.5f*scale.x))
	{
		if (result.x > 0)
		{
			result.x = b.x + m_Offset.x*scale.x;
		}
		else if (result.x < 0)
		{
			result.x = b.x - m_Offset.x*scale.x;
		}
		else
		{
			result.x = b.x;
		}

	}
	else
	{
		result.x = b.x;
	}

	//Y
	if ((result.y - b.y)*(result.y - b.y) > (m_Offset.y*0.5f*scale.y)*(m_Offset.y*0.5f*scale.y))
	{
		if (result.y > 0)
		{
			result.y = b.y + m_Offset.y*scale.y;
		}
		else if (result.y < 0)
		{
			result.y = b.y - m_Offset.y*scale.y;
		}
		else
		{
			result.y = b.y;
		}
	}
	else
	{
		result.y = b.y;
	}

	return result + world;
}


bool PositionGridComponent::IsOnLine(const Vector2& pos) const
{
	Vector2 v = ClampToPoint(pos);
	return (((v.x - pos.x)*(v.x - pos.x) < FLT_EPSILON)
		|| ((v.y - pos.y)*(v.y - pos.y) < FLT_EPSILON));
}
Vector2 PositionGridComponent::ClampToLine(const Vector2& pos) const
{
	if (m_pGameObject == nullptr)
	{
		Logger::GetInstance().LogError("PositionGridComponent::ClampToLine > No gameObject attached");
		return pos;
	}

	Vector2 v = ClampToPoint(pos);
	Vector2 result = pos;

	if ((v.x - result.x)*(v.x - result.x) < (v.y - result.y)*(v.y - result.y))
	{
		result.x = v.x;
	}
	else
	{
		result.y = v.y;
	}
	return result;
}


bool PositionGridComponent::IsInGrid(const Vector2& pos) const
{
	if (m_pGameObject == nullptr)
	{
		Logger::GetInstance().LogError("PositionGridComponent::IsInGrid > No gameObject attached");
		return false;
	}

	Vector2 topLeft = GetUnsafe(0, 0);
	Vector2 bottomRight = GetUnsafe(m_Width - 1, m_Height - 1);

	return ((pos.x >= topLeft.x && pos.x <= bottomRight.x
		&& pos.y >= topLeft.y && pos.y <= bottomRight.y));
}
Vector2 PositionGridComponent::ClampToGrid(const Vector2& pos) const
{
	if (m_pGameObject == nullptr)
	{
		Logger::GetInstance().LogError("PositionGridComponent::ClampToGrid > No gameObject attached");
		return pos;
	}

	const auto tl = GetUnsafe(0, 0);
	const auto br = GetUnsafe(m_Width - 1, m_Height - 1);

	Vector2 r = pos;
	r.x = (r.x <= tl.x) ? tl.x : ((r.x >= br.x) ? br.x : r.x);
	r.y = (r.y <= tl.y) ? tl.y : ((r.y >= br.y) ? br.y : r.y);
	return r;
}


Vector2 PositionGridComponent::GetUnsafe(unsigned int w, unsigned int h) const
{
	auto* pTransform = m_pGameObject->GetComponent<TransformComponent>();

	Vector2 p(pTransform->GetWorldPosition());
	Vector2 s(pTransform->GetWorldScale());

	p.x += w * m_Offset.x*s.x;
	p.y += h * m_Offset.y*s.y;
	return p;
}





//TEST
#include <SDL.h>
#include "RenderManager.h"
void PositionGridComponent::Render(const RenderManager& renderer) const
{
	if (m_pGameObject == nullptr) return;

	auto* rend = renderer.GetSDLRenderer();

	for (unsigned int w{0}; w < m_Width; ++w)
	{
		const Vector2 s = GetUnsafe(w, 0);
		const Vector2 e = GetUnsafe(w, m_Height - 1);

		SDL_RenderDrawLine(rend, int(s.x), int(s.y), int(e.x), int(e.y));
	}

	for (unsigned int h{0}; h < m_Height; ++h)
	{
		const Vector2 s = GetUnsafe(0, h);
		const Vector2 e = GetUnsafe(m_Width - 1, h);

		SDL_RenderDrawLine(rend, int(s.x), int(s.y), int(e.x), int(e.y));
	}
}


