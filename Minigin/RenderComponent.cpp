#include "MiniginPCH.h"
#include "RenderComponent.h"
#include "ResourceManager.h"
#include "RenderManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "Texture2D.h"
#include "SceneData.h"
#include <SDL.h>

RenderComponent::RenderComponent(const std::string& texturePath)
	: m_pTexture(ResourceManager::GetInstance().LoadTexture(texturePath))
	, m_HasSource(false)
	, m_Source()
{
}
RenderComponent::RenderComponent(const std::shared_ptr<Texture2D>& texture)
	: m_pTexture(texture)
	, m_HasSource(false)
	, m_Source()
{}

void RenderComponent::InitializeOverride(const SceneData& sceneData)
{
	RegisterRenderable(sceneData.GetRender());
}
void RenderComponent::DestroyOverride(const SceneData& sceneData)
{
	UnRegisterRenderable(sceneData.GetRender());
}

void RenderComponent::Render(const RenderManager& renderer) const
{
	if ( GetGameObject() != nullptr && m_pTexture != nullptr && IsEnabled())
	{
		const auto& transform = GetGameObject()->GetTransform();
		const auto position = transform.GetWorldPosition();
		const auto scale = transform.GetWorldScale();
		const auto rot = transform.GetWorldRotation(true);

		
		
		
		
		//Without angle nor flipmode
		if ((rot*rot) < 0.01f && m_FlipMode == FlipMode::None)
		{
			//Has source
			if (m_HasSource)
			{
				renderer.RenderTexture(*m_pTexture, position, scale, m_Source);
			}
			//No source
			else
			{
				renderer.RenderTexture(*m_pTexture, position, scale);
			}
		}
		//With angle and\or flipmode
		else
		{
			renderer.RenderTexture(*m_pTexture, position, scale, m_Source, rot, position, m_FlipMode);
		}
	}
}





void RenderComponent::SetTexture(const std::string& filename)
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
	if (!m_HasSource)
	{
		m_Source.x = 0;
		m_Source.y = 0;
		m_Source.width = float(m_pTexture->GetWidth());
		m_Source.height = float(m_pTexture->GetHeight());
	}
}
void RenderComponent::SetTexture(const std::shared_ptr<Texture2D>& pTexture)
{
	m_pTexture = pTexture;
	if (!m_HasSource)
	{
		m_Source.x = 0;
		m_Source.y = 0;
		m_Source.width = float(m_pTexture->GetWidth());
		m_Source.height = float(m_pTexture->GetHeight());
	}
}
const std::shared_ptr<Texture2D>& RenderComponent::GetTexture() const
{
	return m_pTexture;
}


void RenderComponent::SetSource(const Rect& r)
{
	m_Source = r;
	m_HasSource = true;
}
void RenderComponent::ClearSource()
{
	m_HasSource = false;
	m_Source.x = 0;
	m_Source.y = 0;
	if (m_pTexture != nullptr)
	{
		m_Source.width = float(m_pTexture->GetWidth());
		m_Source.height = float(m_pTexture->GetHeight());
	}
}
const Rect& RenderComponent::GetSource() const
{
	return m_Source;
}
bool RenderComponent::HasSource() const
{
	return m_HasSource;
}



void RenderComponent::SetFlipMode(FlipMode flip)
{
	m_FlipMode = flip;
}
FlipMode RenderComponent::GetFlipMode() const
{
	return m_FlipMode;
}

