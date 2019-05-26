#pragma once
#include "BaseComponent.h"
#include "Manager.h"
#include "Font.h"

namespace DigDug
{
	class ButtonComponent;
	class ButtonManager final : public Engine::Manager
	{
	public:
		void Next();
		void Previous();
		void Select();

		void RegisterButton(ButtonComponent* pButton);
		void UnregisterButton(ButtonComponent* pButton);
	private:
		std::vector<ButtonComponent*> m_pButtons;
		unsigned int m_CurrentID = 0;
	};


	class ButtonAction abstract
	{
	public:
		ButtonAction() = default;
		virtual ~ButtonAction() = default;
		virtual void Execute() = 0;
	};

	class ButtonComponent final : public Engine::BaseComponent
	{
	public:
		enum class ButtonState
		{
			None,
			Hovered,
			Selected
		};

		explicit ButtonComponent() = default;
		~ButtonComponent() = default;
		virtual void InitializeOverride(const Engine::SceneData& sceneData) override;
		virtual void DestroyOverride(const Engine::SceneData& sceneData) override;

		void SetStateNone(const std::shared_ptr<Engine::Font>& pFont, SDL_Color color) { m_pNoneFont = pFont; m_NoneColor = color; }
		void SetStateHover(const std::shared_ptr<Engine::Font>& pFont, SDL_Color color) { m_pHoverFont = pFont; m_HoverColor = color; }
		void SetAction(const std::shared_ptr<ButtonAction>& pAction) { m_pAction = pAction; }

		ButtonState GetState() const;
		void SetState(ButtonState state);
	private:
		ButtonState m_State = ButtonState::None;
		std::shared_ptr<ButtonAction> m_pAction;
		std::shared_ptr<Engine::Font> m_pNoneFont;
		SDL_Color m_NoneColor;
		std::shared_ptr<Engine::Font> m_pHoverFont;
		SDL_Color m_HoverColor;
	};

}