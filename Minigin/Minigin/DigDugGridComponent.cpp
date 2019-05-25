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
#include "GameEvents.h"

using namespace Minigin;
using namespace DigDug;


DigDugGridComponent::DigDugGridComponent(unsigned w, unsigned h)
#ifdef Debug
	: Renderable(DebugPriority)
#endif
{
	m_GridMarks.SetWidth(w*2-1);
	m_GridMarks.SetHeight(h*2-1);
}


#ifdef Debug
void DigDugGridComponent::Render(const RenderManager& renderer) const
{
	if (GetGameObject() == nullptr) return;

	const auto pos = GetGameObject()->GetTransform().GetWorldPosition();
	const auto width = m_GridMarks.GetWidth();
	const auto height = m_GridMarks.GetHeight();


	//Walkable: White
	renderer.SetRenderColor(255, 255, 255);;
	for (unsigned int i = 0; i < width; i += 2)
	{		
		renderer.RenderLine(GetWorld(i, 0), GetWorld(i, height - 1));
	}
	for (unsigned int i = 0; i < height; i += 2)
	{
		renderer.RenderLine(GetWorld(0, i), GetWorld(width - 1, i));
	}

	//Inbetween: Red
	renderer.SetRenderColor(255, 0, 0);
	for (unsigned int i = 1; i < width; i += 2)
	{
		renderer.RenderLine(GetWorld(i, 0), GetWorld(i, height - 1));
	}
	for (unsigned int i = 1; i < height; i += 2)
	{
		renderer.RenderLine(GetWorld(0, i), GetWorld(width - 1, i));
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
	//m_GridPositions.Make();
	m_GridMarks.Make();

	//Initialize values
	for (unsigned int h = 0; h < m_GridMarks.GetHeight(); ++h)
	{
		for (unsigned int w = 0; w < m_GridMarks.GetWidth(); ++w)
		{
			*m_GridMarks.GetPtr(w, h) = (h == 0);
		}
	}

	//
	m_pMultiRenderer = GetGameObject()->GetComponent<MultiRenderComponent>();

#ifdef Debug
	RegisterRenderable(sceneData.GetRender());
#endif
}






bool DigDugGridComponent::IsOnPoint(const Vector2& p, float epsilon) const
{
	auto l = ClosestPoint(p);
	return ((l.x - p.x)*(l.x - p.x) < epsilon*epsilon && (l.y - p.y)*(l.y - p.y) < epsilon*epsilon);
}
bool DigDugGridComponent::IsOnLine(const Vector2& p, float epsilon) const
{
	auto l = ClosestLine(p);
	return ((l.x - p.x)*(l.x - p.x) < epsilon*epsilon && (l.y - p.y)*(l.y - p.y) < epsilon*epsilon);
}
bool DigDugGridComponent::IsOnGrid(const Vector2& p, float epsilon) const
{
	auto l = ClosestGrid(p);
	return ((l.x - p.x)*(l.x - p.x) < epsilon*epsilon && (l.y - p.y)*(l.y - p.y) < epsilon*epsilon);
}
bool DigDugGridComponent::IsOnWalkablePoint(const Vector2& p, float epsilon) const
{
	auto l = ClosestWalkablePoint(p);
	return ((l.x - p.x)*(l.x - p.x) < epsilon*epsilon && (l.y - p.y)*(l.y - p.y) < epsilon*epsilon);
}
bool DigDugGridComponent::IsOnWalkableLine(const Vector2& p, float epsilon) const
{
	auto l = ClosestWalkableLine(p);
	return ((l.x - p.x)*(l.x - p.x) < epsilon*epsilon && (l.y - p.y)*(l.y - p.y) < epsilon*epsilon);
}
bool DigDugGridComponent::IsOnWalkableGrid(const Vector2& p, float epsilon) const
{
	//Grid density has no effect
	return IsOnGrid(p, epsilon);
}

Vector2 DigDugGridComponent::ClosestPoint(const Vector2& p) const
{
	//.//
	auto g = WorldToGrid(p);
	g /= m_Offset;
	if (g.x != 0) g.x = float(int(g.x + 0.5f*abs(g.x) / g.x));
	if (g.y != 0) g.y = float(int(g.y + 0.5f*abs(g.y) / g.y));
	g *= m_Offset;
	return GridToWorld(g);
}
Vector2 DigDugGridComponent::ClosestLine(const Vector2& p) const
{
	//./
	auto c = ClosestPoint(p);
	if ((c.x - p.x)*(c.x - p.x) < (c.y - p.y)*(c.y - p.y))
	{
		return Vector2(c.x, p.y);
	}
	return Vector2(p.x, c.y);
}
Vector2 DigDugGridComponent::ClosestGrid(const Vector2& p) const
{
	//./
	auto tl = GetWorld(0, 0);
	auto br = GetWorld(m_GridMarks.GetWidth() - 1, m_GridMarks.GetHeight() - 1);
	return Vector2((p.x < tl.x) ? tl.x : ((p.x > br.x) ? br.x : p.x),
		(p.y < tl.y) ? tl.y : ((p.y > br.y) ? br.y : p.y));
}
Vector2 DigDugGridComponent::ClosestWalkablePoint(const Vector2& p) const
{
	//./
	auto g = WorldToGrid(p);
	g /= m_Offset*2;
	if (g.x != 0) g.x = float(int(g.x + 0.5f*abs(g.x) / g.x));
	if (g.y != 0) g.y = float(int(g.y + 0.5f*abs(g.y) / g.y) );
	g *= m_Offset*2;
	return GridToWorld(g);
}
Vector2 DigDugGridComponent::ClosestWalkableLine(const Vector2& p) const
{
	//./
	auto c = ClosestWalkablePoint(p);
	if ((c.x - p.x)*(c.x - p.x) < (c.y - p.y)*(c.y - p.y))
	{
		return Vector2(c.x, p.y);
	}
	return Vector2(p.x, c.y);
}
Vector2 DigDugGridComponent::ClosestWalkableGrid(const Vector2& p) const
{
	return ClosestGrid(p);
}




bool DigDugGridComponent::IsMarked(const Vector2& p, float epsilon) const
{
	if (m_GridMarks.IsMade())
	{
		int idx = GetIdx(p, epsilon);
		if (m_GridMarks.IsValid(idx))
		{
			return m_GridMarks[idx];
		}
	}
	return false;
}
void DigDugGridComponent::Mark(const Vector2& p, float epsilon)
{
	if (m_GridMarks.IsMade())
	{
		auto i = GetIdx(p, epsilon);
		if (m_GridMarks.IsValid(i) && !m_GridMarks[i])
		{
			m_GridMarks[i] = true;
			UpdateSurroundings(i);
			UpdateRender(i);

			//Notify Scene
			if (GetGameObject() && GetGameObject()->GetScene())
			{
				GetGameObject()->GetScene()->NotifyAll(GameEvent::GridMarked, ObservedData{});
			}
		}

	}
}


Vector2 DigDugGridComponent::GetPosition(unsigned w, unsigned h) const
{
	return ClosestGrid(GetWorld(w, h));
}
Vector2 DigDugGridComponent::GetWalkablePosition(unsigned w, unsigned h) const
{
	return ClosestGrid(GetWorld(w * 2, h * 2));
}


void DigDugGridComponent::SetOffset(const Vector2& o)
{
	m_Offset = o * 0.5f;
	//m_GridPositions.SetOffset(o*0.5f);
}
void DigDugGridComponent::SetWidth(unsigned w)
{
	if (m_GridMarks.IsMade() == false /*&& m_GridPositions.IsMade() == false*/)
	{
		m_GridMarks.SetWidth(w * 2 - 1);
		//m_GridPositions.SetWidth(2*w-1);
	}
}
void DigDugGridComponent::SetHeight(unsigned h)
{
	if (m_GridMarks.IsMade() == false /*&& m_GridPositions.IsMade() == false*/)
	{
		m_GridMarks.SetHeight(h*2-1);
		//m_GridPositions.SetHeight(2*h-1);
	}
}

Vector2 DigDugGridComponent::GetWalkableOffset() const
{
	return m_Offset * 2;

	/*Vector2 offset = m_GridPositions.GetWalkableOffset() * 2;
	if (GetGameObject())
	{
		offset *= GetGameObject()->GetTransform().GetWorldScale();
	}
	return offset;*/
}
unsigned DigDugGridComponent::GetWidth() const
{
	return (m_GridMarks.GetWidth() + 1) / 2;
	//return (m_GridPositions.GetWidth()+1)/2;
}
unsigned DigDugGridComponent::GetHeight() const
{
	return (m_GridMarks.GetHeight() + 1) / 2;

	//return (m_GridPositions.GetHeight()+1)/2;
}




void DigDugGridComponent::UpdateRender(unsigned idx)
{
	if (m_pMultiRenderer != nullptr)
	{
		//Update on RenderComp
		
		Rect d{};
		Vector2 dst = GetLocal(idx % m_GridMarks.GetWidth(), idx / m_GridMarks.GetWidth());
		d.width = m_Offset.x * 1.5f;
		d.height = m_Offset.y * 1.5f;
		d.x = dst.x - d.width*0.5f;
		d.y = dst.y - d.height*0.5f;
		m_pMultiRenderer->AddRenderDst(idx, d); 

	}
}
void DigDugGridComponent::UpdateSurroundings(unsigned int w, unsigned int h)
{
	//Update the booleans based on their surroundings (If all eight surrounded are true, the middle becomes true)
	if (m_GridMarks.IsMade())
	{
		const auto width = m_GridMarks.GetWidth();

		for (int i = int(h) - 1; i <= int(h) + 1; ++i)
		{
			for (int j = int(w) - 1; j <= int(w) + 1; ++j)
			{
				if (m_GridMarks.GetVal(j, i, true) == false
					&& m_GridMarks.GetVal(j - 1, i, false)
					&& m_GridMarks.GetVal(j + 1, i, false)
					&& m_GridMarks.GetVal(j - 1, i - 1, false)
					&& m_GridMarks.GetVal(j - 1, i + 1, false)
					&& m_GridMarks.GetVal(j + 1, i - 1, false)
					&& m_GridMarks.GetVal(j + 1, i + 1, false)
					&& m_GridMarks.GetVal(j, i - 1, false)
					&& m_GridMarks.GetVal(j, i + 1, false))
				{
					int idx = i * width + j;
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






Vector2 DigDugGridComponent::GetWorld(unsigned w, unsigned h)const
{
	return GridToWorld(GetLocal(w, h));
}
Vector2 DigDugGridComponent::GetLocal(unsigned w, unsigned h)const
{
	Vector2 r{};

	auto width = m_GridMarks.GetWidth();
	auto height = m_GridMarks.GetHeight();

	r.x = (w - (width-1) * 0.5f) * m_Offset.x;
	r.y = (h - (height-1) * 0.5f) * m_Offset.y;

	return r;
}




int DigDugGridComponent::GetIdx(const Vector2& world, float epsilon) const
{
	//./
	
	auto lp = WorldToGrid(world);
	lp /= m_Offset;
	int x = (lp.x != 0) ? int(lp.x + 0.5f*abs(lp.x) / lp.x) : int(lp.x);
	int y = (lp.y != 0) ? int(lp.y + 0.5f*abs(lp.y) / lp.y) : int(lp.y);
	if ((x - lp.x)*(x - lp.x) < epsilon && (y - lp.y)*(y - lp.y) < epsilon)
	{
		auto width = m_GridMarks.GetWidth();
		auto height = m_GridMarks.GetHeight();
		x = x + int(width*0.5f);
		y = y + int(height*0.5f);
		return (x + y * width);
	}
	return -1;
}

int DigDugGridComponent::CloseIdx(const Vector2& world) const
{
	//./
	auto lp = WorldToGrid(world);
	lp /= m_Offset;
	int x = (lp.x != 0) ? int(lp.x + 0.5f*abs(lp.x) / lp.x) : int(lp.x);
	int y = (lp.y != 0) ? int(lp.y + 0.5f*abs(lp.y) / lp.y) : int(lp.y);
	auto width = m_GridMarks.GetWidth();
	auto height = m_GridMarks.GetHeight();
	x = x + int(width*0.5f);
	y = y + int(height*0.5f);
	return (x + y * width);
}



