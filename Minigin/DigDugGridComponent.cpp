#include "MiniginPCH.h"
#include "DigDugGridComponent.h"
#include "Transform.h"
#include "RenderComponent.h"
#include "RenderManager.h"
#include "MultiRenderComponent.h"
#include "GameObject.h"
#include "SceneData.h"
#include "Scene.h"
#include "ObservedData.h"

DigDugGridComponent::DigDugGridComponent(unsigned w, unsigned h)
#ifdef Debug
	: Renderable(DebugPriority)
#endif
{
	m_GridMarks.SetWidth(w*2-1);
	m_GridMarks.SetHeight(h*2-1);
	m_GridPositions.SetWidth(w*2-1);
	m_GridPositions.SetHeight(h*2-1);
}


#ifdef Debug
void DigDugGridComponent::Render(const RenderManager& renderer) const
{
	if (m_GridPositions.IsMade() == false || GetGameObject() == nullptr) return;

	const auto scale = GetGameObject()->GetTransform().GetWorldScale();
	const auto pos = GetGameObject()->GetTransform().GetWorldPosition();
	const auto width = m_GridPositions.GetWidth();
	const auto height = m_GridPositions.GetHeight();

	//Walkable: White
	renderer.SetRenderColor(255, 255, 255);;
	for (unsigned int i = 0; i < width; i += 2)
	{		
		renderer.RenderLine(m_GridPositions.Get(i, 0) * scale + pos, m_GridPositions.Get(i, height - 1) * scale + pos);
	}
	for (unsigned int i = 0; i < height; i += 2)
	{
		renderer.RenderLine(m_GridPositions.Get(0, i) * scale + pos, m_GridPositions.Get(width - 1, i) * scale + pos);
	}

	//Inbetween: Red
	renderer.SetRenderColor(255, 0, 0);
	for (unsigned int i = 1; i < width; i += 2)
	{
		renderer.RenderLine(m_GridPositions.Get(i, 0) * scale + pos, m_GridPositions.Get(i, height - 1) * scale + pos);
	}
	for (unsigned int i = 1; i < height; i += 2)
	{
		renderer.RenderLine(m_GridPositions.Get(0, i) * scale + pos, m_GridPositions.Get(width - 1, i) * scale + pos);
	}

	//Center
	renderer.SetRenderColor(0, 0, 255);
	renderer.RenderPoint(pos, 3);
	renderer.ClearRenderColor();
}
#endif
void DigDugGridComponent::InitializeOverride(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);
	m_GridPositions.Make();
	m_GridMarks.Make();

	//Init values
	for (unsigned int h = 0; h < m_GridMarks.GetHeight(); ++h)
	{
		for (unsigned int w = 0; w < m_GridMarks.GetWidth(); ++w)
		{
			*m_GridMarks.GetPtr(w, h) = false;
		}
	}

	//
	m_pMultiRenderer = GetGameObject()->GetComponent<MultiRenderComponent>();

#ifdef Debug
	RegisterRenderable(sceneData.GetRender());
#endif
}






bool DigDugGridComponent::IsOnPoint(const Vector2& p) const
{
	if (m_GridPositions.IsMade() == false) return false;
	if (GetGameObject() != nullptr)
	{
		Vector2 rp = WorldToGrid(p);
		return m_GridPositions.IsOnPoint(rp, [](const Vector2& a, const Vector2& b)
		{
			return (a.x - b.x)*(a.x - b.y) < 0.1f && (a.y - b.y)*(a.y - b.y) < 0.1f;
		});
	}
	return m_GridPositions.IsOnPoint(p);

}
bool DigDugGridComponent::IsOnLine(const Vector2& p) const
{
	if (m_GridPositions.IsMade() == false) return false;
	if (GetGameObject() != nullptr)
	{
		Vector2 rp = WorldToGrid(p);
		return m_GridPositions.IsOnLine(rp, [](const Vector2& a, const Vector2& b)
		{
			return (a.x - b.x)*(a.x - b.y) < 0.1f && (a.y - b.y)*(a.y - b.y) < 0.1f;
		});
	}
	return m_GridPositions.IsOnLine(p);
}
bool DigDugGridComponent::IsOnGrid(const Vector2& p) const
{
	if (m_GridPositions.IsMade() == false) return false;
	if (GetGameObject() != nullptr)
	{
		Vector2 rp = WorldToGrid(p);
		return m_GridPositions.IsOnGrid(rp, [](const Vector2& a, const Vector2& b)
		{
			return (a.x - b.x)*(a.x - b.y) < 0.1f && (a.y - b.y)*(a.y - b.y) < 0.1f;
		});
	}
	return m_GridPositions.IsOnGrid(p);
}
bool DigDugGridComponent::IsOnWalkablePoint(const Vector2& p) const
{
	if (m_GridPositions.IsMade())
	{
		Vector2 rp = WorldToGrid(p);
		Vector2 cp = m_GridPositions.ClosestPoint(rp);
		if ((cp.x - rp.x)*(cp.x - rp.x) < 0.1f &&
			(cp.y - rp.y)*(cp.y - rp.y) < 0.1f)
		{
			auto i = m_GridPositions.FindIndex(cp);
			if (m_GridPositions.IsValid(i))
			{
				const auto w = m_GridPositions.GetWidth();
				return ((i / w) % 2 == 0 && (i % w) % 2 == 0);
			}
		}
	}
	return false;
}
bool DigDugGridComponent::IsOnWalkableLine(const Vector2& p) const
{
	if (m_GridPositions.IsMade())
	{
		Vector2 rp = WorldToGrid(p);
		Vector2 cp = m_GridPositions.ClosestPoint(rp);
		if ((cp.x - rp.x)*(cp.x - rp.x) < 0.1f &&
			(cp.y - rp.y)*(cp.y - rp.y) < 0.1f)
		{
			auto i = m_GridPositions.FindIndex(cp);
			if (m_GridPositions.IsValid(i))
			{
				const auto w = m_GridPositions.GetWidth();
				return ((i / w) % 2 == 0 || (i % w) % 2 == 0);
			}
		}
	}
	return false;
}
bool DigDugGridComponent::IsOnWalkableGrid(const Vector2& p) const
{
	//Grid density has no effect
	return IsOnGrid(p);
}

Vector2 DigDugGridComponent::ClosestPoint(const Vector2& p) const
{
	return GridToWorld(m_GridPositions.ClosestPoint(WorldToGrid(p)));
}

Vector2 DigDugGridComponent::ClosestLine(const Vector2& p) const
{
	return GridToWorld(m_GridPositions.ClosestLine(WorldToGrid(p)));
}

Vector2 DigDugGridComponent::ClosestGrid(const Vector2& p) const
{
	return GridToWorld(m_GridPositions.ClosestGrid(WorldToGrid(p)));
}
Vector2 DigDugGridComponent::ClosestWalkablePoint(const Vector2& p) const
{
	Vector2 pos = WorldToGrid(p);
	auto i = m_GridPositions.FindIndex(m_GridPositions.ClosestPoint(pos));
	int ix = int(int(float((i % m_GridPositions.GetWidth())) + 0.5f)* 0.5f) * 2;
	int iy = int(int(float((i / m_GridPositions.GetWidth())) + 0.5f)* 0.5f) * 2;
	return GridToWorld(m_GridPositions.Get(ix, iy));
}
Vector2 DigDugGridComponent::ClosestWalkableLine(const Vector2& p) const
{
	Vector2 pos = WorldToGrid(p);
	auto i = m_GridPositions.FindIndex(m_GridPositions.ClosestPoint(pos));
	int x = i % m_GridPositions.GetWidth();
	int y = i / m_GridPositions.GetWidth();
	int ix = int((float(x) + 0.5f)* 0.5f) * 2;
	int iy = int((float(y) + 0.5f)* 0.5f) * 2;
	if ((ix - x)*(ix - x) < (iy - y)*(iy - y))
	{
		return GridToWorld(m_GridPositions.Get(ix, y));
	}
	else
	{
		return GridToWorld(m_GridPositions.Get(x, iy));
	}
}
Vector2 DigDugGridComponent::ClosestWalkableGrid(const Vector2& p) const
{
	return ClosestGrid(p);
}




bool DigDugGridComponent::IsMarked(const Vector2& p) const
{
	if (m_GridPositions.IsMade() && m_GridMarks.IsMade())
	{
		Vector2 rp = WorldToGrid(p);
		auto i = m_GridPositions.FindIndex(rp, [](const Vector2& a, const Vector2& b)
		{
			return ((a.x - b.x)*(a.x - b.x) < 0.1f && (a.y - b.y)*(a.y - b.y) < 0.1f);
		});
		if (m_GridMarks.IsValid(i))
		{
			return m_GridMarks[i];
		}
	}
	return false;
}
void DigDugGridComponent::Mark(const Vector2& p)
{
	if (m_GridPositions.IsMade() && m_GridMarks.IsMade())
	{
		Vector2 rp = WorldToGrid(p);
		auto i = m_GridPositions.FindIndex(rp, [](const Vector2& a, const Vector2& b)
		{
			return ((a.x - b.x)*(a.x - b.x) < 1 && (a.y - b.y)*(a.y - b.y) < 1);
		});
		if (m_GridPositions.IsValid(i)
			&& !m_GridMarks[i])
		{

			m_GridMarks[i] = true;
			UpdateSurroundings(i);
			UpdateRender(i);

			//Notify Scene
			if (GetGameObject() && GetGameObject()->GetScene())
			{
				GetGameObject()->GetScene()->Notify(ObservedEvent::GridMarked, ObservedData{});
			}
		}

	}
}


Vector2 DigDugGridComponent::GetPosition(unsigned w, unsigned h) const
{
	Vector2 r{};
	if (m_GridPositions.IsValid(w, h))
	{
		r = m_GridPositions.Get(w, h);
	}
	return GridToWorld(r);
}


void DigDugGridComponent::SetOffset(const Vector2& o)
{
	m_GridPositions.SetOffset(o*0.5f);
}
void DigDugGridComponent::SetWidth(unsigned w)
{
	if (m_GridMarks.IsMade() == false && m_GridPositions.IsMade() == false)
	{
		m_GridMarks.SetWidth(2 * w - 1);
		m_GridPositions.SetWidth(2*w-1);
	}
}
void DigDugGridComponent::SetHeight(unsigned h)
{
	if (m_GridMarks.IsMade() == false && m_GridPositions.IsMade() == false)
	{
		m_GridMarks.SetHeight(2 * h - 1);
		m_GridPositions.SetHeight(2*h-1);
	}
}

Vector2 DigDugGridComponent::GetOffset() const
{
	Vector2 offset = m_GridPositions.GetOffset() * 2;
	if (GetGameObject())
	{
		offset *= GetGameObject()->GetTransform().GetWorldScale();
	}
	return offset;
}
unsigned DigDugGridComponent::GetWidth() const
{
	return (m_GridPositions.GetWidth()+1)/2;
}
unsigned DigDugGridComponent::GetHeight() const
{
	return (m_GridPositions.GetHeight()+1)/2;
}




void DigDugGridComponent::UpdateRender(unsigned idx)
{
	if (m_pMultiRenderer != nullptr)
	{
		//Update on RenderComp
		MultiRenderComponent::RenderInfo info{};
		Vector2 p = m_GridPositions.Get(idx);
		info.hasSrc = false;
		info.dst.width = m_GridPositions.GetOffset().x * 1.5f;
		info.dst.height = m_GridPositions.GetOffset().y * 1.5f;
		info.dst.x = p.x - info.dst.width*0.5f;
		info.dst.y = p.y - info.dst.height*0.5f;
		m_pMultiRenderer->AddRenderInfo(idx, info);
	}
}
void DigDugGridComponent::UpdateSurroundings(unsigned int w, unsigned int h)
{
	//Update the booleans based on their surroundings (If all eight surrounded are true, the middle becomes true)
	if (m_GridMarks.IsMade())
	{
		const auto width = m_GridMarks.GetWidth();

		for (unsigned int i = h - 1; i <= h + 1; ++i)
		{
			for (unsigned int j = w - 1; j <= w + 1; ++j)
			{
				int idx = i * width + j;
				if (m_GridMarks[idx] == false
					&& m_GridMarks.GetVal(idx - 1, false)
					&& m_GridMarks.GetVal(idx + 1, false)
					&& m_GridMarks.GetVal(idx - width - 1, false)
					&& m_GridMarks.GetVal(idx - width + 1, false)
					&& m_GridMarks.GetVal(idx + width - 1, false)
					&& m_GridMarks.GetVal(idx + width + 1, false)
					&& m_GridMarks.GetVal(idx + width, false)
					&& m_GridMarks.GetVal(idx - width, false))
				{
					m_GridMarks[idx] = true;
					UpdateRender(idx);
				}
				
			}
		}
	}
}
void DigDugGridComponent::UpdateSurroundings(unsigned i)
{
	if (m_GridMarks.IsMade())
	{
		auto w = m_GridMarks.GetWidth();
		auto r = i / w;
		auto c = i % w;
		UpdateSurroundings(c, r);
	}
}



Vector2 DigDugGridComponent::WorldToGrid(const Vector2& p) const
{
	if (GetGameObject() != nullptr)
	{
		Vector2 rp = p;
		rp -= GetGameObject()->GetTransform().GetWorldPosition();
		rp /= GetGameObject()->GetTransform().GetWorldScale();
		return rp;
	}
	return p;
}
Vector2 DigDugGridComponent::GridToWorld(const Vector2& p) const
{
	if (GetGameObject() != nullptr)
	{
		Vector2 rp = p;
		rp *= GetGameObject()->GetTransform().GetWorldScale();
		rp += GetGameObject()->GetTransform().GetWorldPosition();
		return rp;
	}
	return p;
}



