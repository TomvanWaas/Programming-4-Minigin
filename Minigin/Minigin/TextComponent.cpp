#include "MiniginPCH.h"
#include "TextComponent.h"
#include "Font.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Texture2D.h"
#include "RenderManager.h"
#include "RenderComponent.h"
#include "Logger.h"
#include "SceneData.h"

TextComponent::TextComponent(const std::string& text, const SDL_Color& color, const std::string& fontpath, unsigned fontsize)
	: m_pFont(ResourceManager::GetInstance().LoadFont(fontpath, fontsize))
	, m_Color(color)
	, m_Text(text)
	, m_bNeedsUpdate(true)
{
}
TextComponent::TextComponent(const std::string& text, const SDL_Color& color, const std::shared_ptr<Font>& pFont)
	: m_pFont(pFont)
	, m_Color(color)
	, m_Text(text)
	, m_bNeedsUpdate(true)
{
}



void TextComponent::Initialize(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);

	if (m_pGameObject != nullptr)
	{
		m_pRenderComponent = m_pGameObject->GetComponent<RenderComponent>();
		if (m_pRenderComponent == nullptr)
		{
			Logger::GetInstance().LogWarning("TextComponent::Initialize > Cannot find RenderComponent");
		}
	}
}
void TextComponent::Update(const SceneData& sceneData)
{
	//Make texture
	if (m_bNeedsUpdate && m_Text != "" && m_pFont != nullptr)
	{
		const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), m_Color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(sceneData.pRenderManager->GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		auto pTexture = std::make_shared<Texture2D>(texture);
		m_bNeedsUpdate = false;

		if (m_pRenderComponent != nullptr)
		{
			m_pRenderComponent->SetTexture(pTexture);
		}
	}
}
void TextComponent::ReInitialize(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);

	if (m_pGameObject != nullptr)
	{
		m_pRenderComponent = m_pGameObject->GetComponent<RenderComponent>();
		if (m_pRenderComponent == nullptr)
		{
			Logger::GetInstance().LogWarning("TextComponent::ReInitialize > Cannot find RenderComponent");
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
	m_pFont = ResourceManager::GetInstance().LoadFont(fontpath, fontsize);
	m_bNeedsUpdate = true;
}
void TextComponent::SetFont(const std::shared_ptr<Font>& pFont)
{
	m_pFont = pFont;
	m_bNeedsUpdate = true;
}
void TextComponent::SetColor(const SDL_Color& color)
{
	m_Color = color;
	m_bNeedsUpdate = true;
}