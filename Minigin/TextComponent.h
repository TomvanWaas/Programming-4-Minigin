#pragma once
#include "BaseComponent.h"
#include <SDL.h>
class RenderComponent;
namespace dae
{
	class Font;
	class TextObject;
	class Texture2D;
}

class TextComponent final : public BaseComponent
{
public:
	explicit TextComponent(const std::string& text, const SDL_Color& color, const std::string& fontpath, unsigned int fontsize);
	explicit TextComponent(const std::string& text, const SDL_Color& color, const std::shared_ptr<dae::Font>& pFont);
	virtual ~TextComponent() override = default;

	TextComponent(const TextComponent& other) = delete;
	TextComponent(TextComponent&& other) noexcept = delete;
	TextComponent& operator=(const TextComponent& other) = delete;
	TextComponent& operator=(TextComponent&& other) noexcept = delete;

	virtual void Initialize() override;
	virtual void Update() override;

	void SetText(const std::string& text);
	void SetColor(const SDL_Color& color);
	void SetFont(const std::string& fontpath, unsigned int fontsize);
	void SetFont(const std::shared_ptr<dae::Font>& pFont);

private:
	std::string m_Text;
	SDL_Color m_Color;
	std::shared_ptr<dae::Font> m_pFont;
	RenderComponent* m_pRenderComponent;
	bool m_bNeedsUpdate;
};

