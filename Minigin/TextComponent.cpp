#include "MiniginPCH.h"
#include "TextComponent.h"
#include "Font.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Texture2D.h"
#include "Renderer.h"
#include "RenderComponent.h"
#include "Logger.h"

TextComponent::TextComponent(const std::string& text, const SDL_Color& color, const std::string& fontpath, unsigned fontsize)
	: m_pFont(dae::ResourceManager::GetInstance().LoadFont(fontpath, fontsize))
	, m_Color(color)
	, m_Text(text)
	, m_bNeedsUpdate(true)
	, m_pRenderComponent(nullptr)
{
}
TextComponent::TextComponent(const std::string& text, const SDL_Color& color, const std::shared_ptr<dae::Font>& pFont)
	: m_pFont(pFont)
	, m_Color(color)
	, m_Text(text)
	, m_bNeedsUpdate(true)
	, m_pRenderComponent(nullptr)
{
}

void TextComponent::Initialize()
{
	if (m_pOwner != nullptr)
	{
		m_pRenderComponent = m_pOwner->GetComponent<RenderComponent>(false);
	}
	if (m_pRenderComponent == nullptr)
	{
		Logger::GetInstance().LogWarning("TextComponent cannot find RenderComponent");
	}
}

void TextComponent::Update()
{
	//Make texture
	if (m_bNeedsUpdate && m_Text != "" && m_pFont != nullptr)
	{
		const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), m_Color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(dae::Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		auto pTexture = std::make_shared<dae::Texture2D>(texture);
		m_bNeedsUpdate = false;

		if (m_pRenderComponent != nullptr)
		{
			m_pRenderComponent->SetTexture(pTexture);
		}
	}
}

void TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_bNeedsUpdate = true;
}
void TextComponent::SetFont(const std::string& fontpath, unsigned int fontsize)
{
	m_pFont = dae::ResourceManager::GetInstance().LoadFont(fontpath, fontsize);
	m_bNeedsUpdate = true;
}
void TextComponent::SetFont(const std::shared_ptr<dae::Font>& pFont)
{
	m_pFont = pFont;
	m_bNeedsUpdate = true;
}
void TextComponent::SetColor(const SDL_Color& color)
{
	m_Color = color;
	m_bNeedsUpdate = true;
}
