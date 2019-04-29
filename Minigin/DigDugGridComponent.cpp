#include "MiniginPCH.h"
#include "DigDugGridComponent.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "RenderManager.h"
#include "MultiRenderComponent.h"
#include "Texture2D.h"

DigDugGridComponent::DigDugGridComponent(GameObject& gameObject)
	: BaseComponent(gameObject)
#ifdef Debug
	, Renderable(500)
#endif
	, m_GridPositions()
	, m_GridMarks()
	, m_PositionOffset(Vector2(1,1))
{
}

#ifdef Debug
void DigDugGridComponent::Render(const RenderManager& renderer) const
{
	if (m_GridPositions.IsMade() == false) return;

	const auto width = m_GridPositions.GetWidth();
	const auto height = m_GridPositions.GetHeight();
	const Vector2 start = m_pGameObject->GetComponent<TransformComponent>()->GetWorldPosition();
	const Vector2 scale = m_pGameObject->GetComponent<TransformComponent>()->GetWorldScale();	
	const Vector2 diff = start - Vector2((width-1)*0.5f * scale.x * m_PositionOffset.x*0.5f, (height-1)* 0.5f* scale.y * m_PositionOffset.y * 0.5f);

	//Walkable: Red
	renderer.SetRenderColor(255, 0, 0);
	for (unsigned int h = 0; h < height; h += 2)
	{
		renderer.RenderLine(*m_GridPositions.GetPtr(0, h) + diff, *m_GridPositions.GetPtr(width - 1, h) + diff);
	}
	for (unsigned int w = 0; w < width; w += 2)
	{
		renderer.RenderLine(*m_GridPositions.GetPtr(w, 0) + diff, *m_GridPositions.GetPtr(w, height - 1) + diff);
	}

	//Inbetween: White
	renderer.SetRenderColor(255, 255, 255);;
	for (unsigned int h = 1; h < height; h += 2)
	{
		renderer.RenderLine(*m_GridPositions.GetPtr(0, h) + diff, *m_GridPositions.GetPtr(width - 1, h) + diff);
	}
	for (unsigned int w = 1; w < width; w += 2)
	{
		renderer.RenderLine(*m_GridPositions.GetPtr(w, 0) + diff, *m_GridPositions.GetPtr(w, height - 1) + diff);
	}

	//Center
	renderer.SetRenderColor(0, 0, 255);
	renderer.RenderPoint(start, 3);

	renderer.ClearRenderColor();
}
#endif

void DigDugGridComponent::Initialize(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);
	m_GridPositions.Make();
	m_GridMarks.Make();
	Vector2 offset = m_PositionOffset * 0.5f;

	//Init values
	for (unsigned int i = 0; i < m_GridPositions.GetHeight(); ++i)
	{
		for (unsigned int j = 0; j < m_GridPositions.GetWidth(); ++j)
		{
			*m_GridPositions.GetPtr(j, i) = Vector2(offset.x * j, offset.y * i);
			*m_GridMarks.GetPtr(j, i) = false;
		}
	}

	m_pMultiRenderer = m_pGameObject->GetComponent<MultiRenderComponent>();

#ifdef Debug
	RegisterRenderable(sceneData.pRenderManager);
#endif
}



bool DigDugGridComponent::IsOnPoint(const Vector2& p) const
{
	if (m_GridPositions.IsMade() && m_GridMarks.IsMade())
	{
		Vector2 diff = Vector2::Zero;
		if (m_pGameObject != nullptr)
		{
			const auto width = m_GridPositions.GetWidth();
			const auto height = m_GridPositions.GetHeight();
			const Vector2 start = m_pGameObject->GetComponent<TransformComponent>()->GetWorldPosition();
			const Vector2 scale = m_pGameObject->GetComponent<TransformComponent>()->GetWorldScale();
			diff = start - Vector2((width - 1)*0.5f * scale.x * m_PositionOffset.x*0.5f, (height - 1)* 0.5f* scale.y * m_PositionOffset.y * 0.5f);
		}
		auto idx = m_GridPositions.FindIndex(p-diff, [](const Vector2& a, const Vector2& b)
		{
			return ((a.x - b.x)*(a.x - b.x) < 0.1f && (a.y - b.y)*(a.y - b.y) < 0.1f);
		});
		if (m_GridPositions.IsValid(idx))
		{
			auto w = m_GridPositions.GetWidth();
			auto r = idx / w;
			auto c = idx % w;
			return (r % 2 == 0 && c % 2 == 0);
		}
	}
	return false;
}
bool DigDugGridComponent::IsMarked(const Vector2& p) const
{
	if (m_GridPositions.IsMade() && m_GridMarks.IsMade())
	{
		Vector2 diff = Vector2::Zero;
		if (m_pGameObject != nullptr)
		{
			const auto width = m_GridPositions.GetWidth();
			const auto height = m_GridPositions.GetHeight();
			const Vector2 start = m_pGameObject->GetComponent<TransformComponent>()->GetWorldPosition();
			const Vector2 scale = m_pGameObject->GetComponent<TransformComponent>()->GetWorldScale();
			diff = start - Vector2((width - 1)*0.5f * scale.x * m_PositionOffset.x*0.5f, (height - 1)* 0.5f* scale.y * m_PositionOffset.y * 0.5f);
		}
		auto idx = m_GridPositions.FindIndex(p - diff, [](const Vector2& a, const Vector2& b)
		{
			return ((a.x - b.x)*(a.x - b.x) < 0.1f && (a.y - b.y)*(a.y - b.y) < 0.1f);
		});
		if (m_GridPositions.IsValid(idx))
		{
			return m_GridMarks[idx];
		}
	}
	return false;
}
void DigDugGridComponent::Mark(const Vector2& p)
{
	if (m_GridPositions.IsMade() && m_GridMarks.IsMade())
	{
		Vector2 diff = Vector2::Zero;
		if (m_pGameObject != nullptr)
		{
			const auto width = m_GridPositions.GetWidth();
			const auto height = m_GridPositions.GetHeight();
			const Vector2 start = m_pGameObject->GetComponent<TransformComponent>()->GetWorldPosition();
			const Vector2 scale = m_pGameObject->GetComponent<TransformComponent>()->GetWorldScale();
			diff = start - Vector2((width - 1)*0.5f * scale.x * m_PositionOffset.x*0.5f, (height - 1)* 0.5f* scale.y * m_PositionOffset.y * 0.5f);
		}
		auto idx = m_GridPositions.FindIndex(p  - diff, [](const Vector2& a, const Vector2& b)
		{
			return ((a.x - b.x)*(a.x - b.x) < 5 && (a.y - b.y)*(a.y - b.y) < 5);
		});
		//Mark
		if (m_GridPositions.IsValid(idx)
			&& !m_GridMarks[idx])
		{

			m_GridMarks[idx] = true;
			UpdateSurroundings();
			UpdateRender(idx);
		}
	}
}
Vector2 DigDugGridComponent::GetPosition(unsigned w, unsigned h) const
{
	if (m_GridPositions.IsMade() && m_GridPositions.IsValid(w, h))
	{
		Vector2 diff = Vector2::Zero;
		if (m_pGameObject != nullptr)
		{
			const auto width = m_GridPositions.GetWidth();
			const auto height = m_GridPositions.GetHeight();
			const Vector2 start = m_pGameObject->GetComponent<TransformComponent>()->GetWorldPosition();
			const Vector2 scale = m_pGameObject->GetComponent<TransformComponent>()->GetWorldScale();
			diff = start - Vector2((width - 1)*0.5f * scale.x * m_PositionOffset.x*0.5f, (height - 1)* 0.5f* scale.y * m_PositionOffset.y * 0.5f);
		}
		return (*m_GridPositions.GetPtr(w, h) + diff);
	}
	return Vector2::Zero;
}



void DigDugGridComponent::SetOffset(const Vector2& o)
{
	m_PositionOffset = o;
}
void DigDugGridComponent::SetWidth(unsigned w)
{
	m_GridMarks.SetWidth(2 * w - 1);
	m_GridPositions.SetWidth(2 * w - 1);
}
void DigDugGridComponent::SetHeight(unsigned h)
{
	m_GridMarks.SetHeight(2 * h - 1);
	m_GridPositions.SetHeight(2 * h - 1);
}



const Vector2& DigDugGridComponent::GetOffset() const
{
	return m_PositionOffset;
}
unsigned DigDugGridComponent::GetWidth() const
{
	return (m_GridPositions.GetWidth() / 2 + 1);
}
unsigned DigDugGridComponent::GetHeight() const
{
	return (m_GridPositions.GetHeight() / 2 + 1);
}







void DigDugGridComponent::UpdateRender(unsigned idx)
{
	if (m_pMultiRenderer != nullptr)
	{
		//Update on RenderComp
		MultiRenderComponent::RenderInfo info{};
		Vector2 diff = GetOffsetFromTransform(idx);

		info.hasDst = true;
		info.hasSrc = false;
		info.dst.width = float(m_pMultiRenderer->GetTexture()->GetWidth());
		info.dst.height = float(m_pMultiRenderer->GetTexture()->GetHeight());
		info.dst.x = diff.x;
		info.dst.y = diff.y;
		m_pMultiRenderer->AddRenderInfo(idx, info);
	}
}



void DigDugGridComponent::UpdateSurroundings()
{
	//Update the booleans based on their surroundings (If all eight surrounded are true, the middle becomes true)
	if (m_GridMarks.IsMade())
	{
		const auto w = m_GridMarks.GetWidth();
		const auto h = m_GridMarks.GetHeight();

		for (unsigned int i = 0; i < w*h; ++i)
		{
			if (m_GridMarks[i] == false
				&& m_GridMarks.GetVal(i - 1, false)
				&& m_GridMarks.GetVal(i + 1, false)
				&& m_GridMarks.GetVal(i - w - 1, false)
				&& m_GridMarks.GetVal(i - w + 1, false)
				&& m_GridMarks.GetVal(i + w - 1, false)
				&& m_GridMarks.GetVal(i + w + 1, false)
				&& m_GridMarks.GetVal(i + w, false)
				&& m_GridMarks.GetVal(i - w, false))
			{
				m_GridMarks[i] = true;
				UpdateRender(i);
			}
		}

	}
}

Vector2 DigDugGridComponent::GetOffsetFromTransform(unsigned int i) const
{
	Vector2 res = Vector2::Zero;
	if (m_pGameObject != nullptr && m_GridPositions.IsMade() && m_GridPositions.IsValid(i))
	{
		const auto p = m_GridPositions[i];
		const auto width = m_GridPositions.GetWidth();
		const auto height = m_GridPositions.GetHeight();
		const Vector2 scale = m_pGameObject->GetComponent<TransformComponent>()->GetWorldScale();

		res = Vector2(p.x - ((width - 1)*0.5f*scale.x *m_PositionOffset.x*0.5f), p.y - ((height - 1)*0.5f*scale.y*m_PositionOffset.y*0.5f));
	}
	return res;
}
