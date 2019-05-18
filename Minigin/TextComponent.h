#pragma once
#include "BaseComponent.h"
#include <SDL.h>
class RenderComponent;
class Font;
class TextObject;
class Texture2D;

class TextComponent final : public BaseComponent
{
public:
	explicit TextComponent() = default;
	virtual ~TextComponent() override = default;

	TextComponent(const TextComponent& other) = delete;
	TextComponent(TextComponent&& other) noexcept = delete;
	TextComponent& operator=(const TextComponent& other) = delete;
	TextComponent& operator=(TextComponent&& other) noexcept = delete;

	virtual void UpdateFirstOverride(const SceneData& sceneData) override;

	void SetText(const std::string& text);
	void SetColor(const SDL_Color& color);
	void SetFont(const std::string& fontpath, unsigned int fontsize);
	void SetFont(const std::shared_ptr<Font>& pFont);

private:
	std::string m_Text = "";
	SDL_Color m_Color;
	std::shared_ptr<Font> m_pFont;
	bool m_bNeedsUpdate = true;
};

