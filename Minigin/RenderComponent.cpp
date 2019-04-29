#include "MiniginPCH.h"
#include "RenderComponent.h"
#include "ResourceManager.h"
#include "RenderManager.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Texture2D.h"
#include "SceneData.h"

RenderComponent::RenderComponent(GameObject& gameObject)
	: BaseComponent(gameObject)
	, m_SrcRect()
	, m_HasSrc(false)
	, m_pTexture()
{
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

		//If no dst nor src => take whole texture as dst
		Rect dst = Rect(0, 0, float(m_pTexture->GetWidth()), float(m_pTexture->GetHeight()));
		if (m_HasDst)
		{
			//Else if dst => take dstrect
			dst = m_DstRect;
		}
		else if (m_HasSrc)
		{
			//Else if src => take src measurements
			dst = Rect(0, 0, m_SrcRect.width, m_SrcRect.height);
		}
		//Apply Transform
		dst.x += position.x;
		dst.y += position.y;
		dst.width *= scale.x;
		dst.height *= scale.y;
		//Recenter
		dst.x -= dst.width *0.5f;
		dst.y -= dst.height*0.5f;

		//Render
		if (m_HasSrc)
		{		
			renderer.RenderTexture(*m_pTexture, m_SrcRect, dst);
		}
		else
		{
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

void RenderComponent::SetDestination(const Rect& r)
{
	m_DstRect = r;
	m_HasSrc = true;
}
void RenderComponent::ClearDestination()
{
	m_HasDst = false;
}
const Rect& RenderComponent::GetDestination() const
{
	return m_DstRect;
}
bool RenderComponent::HasDestination() const
{
	return m_HasDst;
}
