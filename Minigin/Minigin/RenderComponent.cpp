#include "MiniginPCH.h"
#include "RenderComponent.h"
#include "ResourceManager.h"
#include "RenderManager.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Texture2D.h"
#include "SceneData.h"

RenderComponent::RenderComponent(const std::string& filename)
	: m_HasSrc(false)
	, m_SrcRect()
	, m_pTexture()
{
	SetTexture(filename);
}
RenderComponent::RenderComponent(const std::shared_ptr<Texture2D>& pTexture)
	: m_HasSrc(false)
	, m_SrcRect()
	, m_pTexture()
{
	SetTexture(pTexture);
}


void RenderComponent::Initialize(const SceneData& sceneData)
{
	RegisterRenderable(sceneData.pRenderManager);
}

void RenderComponent::Render(const RenderManager& renderer) const
{
	if (m_pGameObject != nullptr && m_pTexture != nullptr)
	{
		const auto transform = m_pGameObject->GetComponent<TransformComponent>();
		const auto position = transform->GetWorldPosition();
		const auto scale = transform->GetWorldScale();
		
		Rect dst;
		dst.x = position.x;
		dst.y = position.y;

		if (m_HasSrc)
		{
			dst.width = m_SrcRect.width * scale.x;
			dst.height = m_SrcRect.height * scale.y;
			dst.x -= dst.width *0.5f;
			dst.y -= dst.height*0.5f;
			renderer.RenderTexture(*m_pTexture, m_SrcRect, dst);
		}
		else
		{
			dst.width = m_pTexture->GetWidth() * scale.x;
			dst.height = m_pTexture->GetHeight() * scale.y;
			dst.x -= dst.width *0.5f;
			dst.y -= dst.height*0.5f;
			renderer.RenderTexture(*m_pTexture, dst);
		}

	}
}


void RenderComponent::SetTexture(const std::string& filename)
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
}
void RenderComponent::SetTexture(const std::shared_ptr<Texture2D>& pTexture)
{
	m_pTexture = pTexture;
}
const std::shared_ptr<Texture2D>& RenderComponent::GetTexture() const
{
	return m_pTexture;
}


void RenderComponent::SetSource(const Rect& r)
{
	m_SrcRect = r;
	m_HasSrc = true;
}
void RenderComponent::ClearSource()
{
	m_HasSrc = false;
}
const Rect& RenderComponent::GetSource() const
{
	return m_SrcRect;
}
bool RenderComponent::HasSource() const
{
	return m_HasSrc;
}
