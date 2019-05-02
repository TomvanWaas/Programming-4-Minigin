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
	RegisterRenderable(sceneData.pRenderManager);
}

void MultiRenderComponent::DestroyOverride(const SceneData& sceneData)
{
	UnRegisterRenderable(sceneData.pRenderManager);
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

			//If no dst nor src => take whole texture as dst
			Rect dst = Rect(0, 0, float(m_pTexture->GetWidth()), float(m_pTexture->GetHeight()));
			if (rinfo.hasDst)
			{
				//Else if dst => take dstrect
				dst = rinfo.dst;
			}
			else if (rinfo.hasSrc)
			{
				//Else if src => take src measurements
				dst = Rect(0, 0, rinfo.src.width, rinfo.src.height);
			}
			//Apply Transform
			dst.x *= scale.x;
			dst.y *= scale.y;
			dst.x += position.x;
			dst.y += position.y;
			dst.width *= scale.x;
			dst.height *= scale.y;
			//Recenter
			dst.x -= dst.width *0.5f;
			dst.y -= dst.height*0.5f;

			//Render
			if (rinfo.hasSrc)
			{
				renderer.RenderTexture(*m_pTexture, rinfo.src, dst);
			}
			else
			{
				renderer.RenderTexture(*m_pTexture, dst);
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



