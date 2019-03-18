#include "MiniginPCH.h"
#include "RenderComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Texture2D.h"


RenderComponent::RenderComponent(const std::string& filename)
{
	SetTexture(filename);
}
RenderComponent::RenderComponent(const std::shared_ptr<dae::Texture2D>& pTexture)
{
	SetTexture(pTexture);
}

void RenderComponent::Render() const
{
	if (m_pOwner != nullptr && m_pTexture != nullptr)
	{
		const auto transform = m_pOwner->GetTransform();
		const auto position = transform->GetPosition();
		dae::Renderer::GetInstance().RenderTexture(*m_pTexture, position.x, position.y);
	}
}

void RenderComponent::SetTexture(const std::string& filename)
{
	m_pTexture = dae::ResourceManager::GetInstance().LoadTexture(filename);
}
void RenderComponent::SetTexture(const std::shared_ptr<dae::Texture2D>& pTexture)
{
	m_pTexture = pTexture;
}

const std::shared_ptr<dae::Texture2D>& RenderComponent::GetTexture() const
{
	return m_pTexture;
}
