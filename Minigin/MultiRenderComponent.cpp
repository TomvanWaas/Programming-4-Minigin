#include "MiniginPCH.h"
#include "MultiRenderComponent.h"
#include "ResourceManager.h"
#include "Transform.h"
#include "RenderManager.h"
#include "Texture2D.h"
#include "Logger.h"
#include "SceneData.h"
#include "GameObject.h"

MultiRenderComponent::MultiRenderComponent(const std::string& texturePath)
	: m_pTexture(ResourceManager::GetInstance().LoadTexture(texturePath))
	, m_RenderInfos()
{
}
MultiRenderComponent::MultiRenderComponent(const std::shared_ptr<Texture2D>& texture)
	: m_pTexture(texture)
	, m_RenderInfos()
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

		for (const std::pair<unsigned int, RenderInfo>& info : m_RenderInfos)
		{
			auto& rinfo = info.second;			

			Vector2 center{};
			center.x = (rinfo.dst.x + rinfo.dst.width * scale.x * 0.5f) * scale.x;
			center.y = (rinfo.dst.y + rinfo.dst.height * scale.y * 0.5f) * scale.y;
			center += position;
			Vector2 scl{};
			scl.x = scale.x * rinfo.dst.width / m_pTexture->GetWidth();
			scl.y = scale.y * rinfo.dst.height / m_pTexture->GetHeight();
			if (rinfo.hasSrc)
			{
				renderer.RenderTexture(*m_pTexture, center, scl, rinfo.src);
			}
			else
			{
				renderer.RenderTexture(*m_pTexture, center, scl);
			}
		}
	}
}



void MultiRenderComponent::AddRenderInfo(unsigned id, const RenderInfo& info)
{
	auto i = m_RenderInfos.find(id);
	if (i == m_RenderInfos.end())
	{
		m_RenderInfos[id] = info;
	}
	else
	{
		Logger::GetInstance().LogWarning("MultiRenderComponent::AddRenderInfo > Id already used");
	}
}
void MultiRenderComponent::RemoveRenderInfo(unsigned id)
{
	auto i = m_RenderInfos.find(id);
	if (i != m_RenderInfos.end())
	{
		m_RenderInfos.erase(i);
	}
	else
	{
		Logger::GetInstance().LogWarning("MultiRenderComponent::RemoveRenderInfo > Id not found");
	}
}
const MultiRenderComponent::RenderInfo* MultiRenderComponent::GetRenderInfo(unsigned id) const
{
	auto i = m_RenderInfos.find(id);
	if (i != m_RenderInfos.end())
	{
		return &(*i).second;
	}
	return nullptr;
}
void MultiRenderComponent::ClearRenderInfos()
{
	m_RenderInfos.clear();
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



