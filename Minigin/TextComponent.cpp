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

TextComponent::TextComponent(GameObject& gameObject)
	: BaseComponent(gameObject)
	, m_pFont(nullptr)
	, m_Color(SDL_Color())
	, m_Text("")
	, m_bNeedsUpdate(false)
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
void TextComponent::UpdateFirst(const SceneData& sceneData)
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
