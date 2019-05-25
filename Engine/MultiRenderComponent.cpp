#include "stdafx.h"
#include "MultiRenderComponent.h"
#include "ResourceManager.h"
#include "Transform.h"
#include "RenderManager.h"
#include "Texture2D.h"
#include "Logger.h"
#include "SceneData.h"
#include "GameObject.h"

using namespace Minigin;

MultiRenderComponent::MultiRenderComponent(const std::string& texturePath)
	: m_pTexture(ResourceManager::GetInstance().LoadTexture(texturePath))
	, m_RenderDestinations()
	, m_Source()
	, m_HasSource(false)
{
}
MultiRenderComponent::MultiRenderComponent(const std::shared_ptr<Texture2D>& texture)
	: m_pTexture(texture)
	, m_RenderDestinations()
	, m_Source()
	, m_HasSource(false)
{
}


void MultiRenderComponent::InitializeOverride(const SceneData& sceneData)
{
	RegisterRenderable(sceneData.GetRender());
}

void MultiRenderComponent::DestroyOverride(const SceneData& sceneData)
{
	UnRegisterRenderable(sceneData.GetRender());
}

void MultiRenderComponent::Render(const RenderManager& renderer) const
{
	if (GetGameObject() != nullptr && m_pTexture != nullptr&& IsEnabled())
	{
		const auto& transform = GetGameObject()->GetTransform();
		const auto position = transform.GetWorldPosition();
		const auto scale = transform.GetWorldScale();

		for (const std::pair<unsigned int, Rect>& dstpair : m_RenderDestinations)
		{
			auto& dst = dstpair.second;			

			Vector2 center{};
			center.x = (dst.x + dst.width * scale.x * 0.5f) * scale.x;
			center.y = (dst.y + dst.height * scale.y * 0.5f) * scale.y;
			center += position;
			
			if (m_HasSource)
			{
				Vector2 scl{};
				scl.x = scale.x * dst.width / m_Source.width;
				scl.y = scale.y * dst.height / m_Source.height;
				renderer.RenderTexture(*m_pTexture, center, scl, m_Source);
			}
			else
			{
				Vector2 scl{};
				scl.x = scale.x * dst.width / m_pTexture->GetWidth();
				scl.y = scale.y * dst.height / m_pTexture->GetHeight();
				renderer.RenderTexture(*m_pTexture, center, scl);
			}
		}
	}
}



void MultiRenderComponent::AddRenderDst(unsigned id, const Rect& dst)
{
	auto i = m_RenderDestinations.find(id);
	if (i == m_RenderDestinations.end())
	{
		m_RenderDestinations[id] = dst;
	}
	else
	{
		Logger::GetInstance().LogWarning("MultiRenderComponent::AddRenderInfo > Id already used");
	}
}
void MultiRenderComponent::RemoveRenderDst(unsigned id)
{
	auto i = m_RenderDestinations.find(id);
	if (i != m_RenderDestinations.end())
	{
		m_RenderDestinations.erase(i);
	}
	else
	{
		Logger::GetInstance().LogWarning("MultiRenderComponent::RemoveRenderInfo > Id not found");
	}
}
const Rect* MultiRenderComponent::GetRenderDst(unsigned id) const
{
	auto i = m_RenderDestinations.find(id);
	if (i != m_RenderDestinations.end())
	{
		return &(*i).second;
	}
	return nullptr;
}
void MultiRenderComponent::ClearRenderDst()
{
	m_RenderDestinations.clear();
}




void MultiRenderComponent::SetTexture(const std::string& filename)
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
}
void MultiRenderComponent::SetTexture(const std::shared_ptr<Texture2D>& pTexture)
{
	m_pTexture = pTexture;
}
const std::shared_ptr<Texture2D>& MultiRenderComponent::GetTexture() const
{
	return m_pTexture;
}



