#include "MiniginPCH.h"
#include "DigDugPauseScene.h"
#include "GameButtons.h"
#include "TextComponent.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Components.h"
#include "Minigin.h"
#include "InputManager.h"
#include "GameInputCommands.h"
#include "ScreenRenderComponent.h"

using namespace DigDug;

DigDugPauseScene::DigDugPauseScene(const std::string& name)
	: Scene(name)
	, m_pResumeButton(nullptr)
{
}

void DigDugPauseScene::SceneInitialize()
{
	m_pResumeButton = std::make_shared<SceneSwapButton>(*GetSceneManager(), "", false);

	auto* pButtonManager = new DigDug::ButtonManager();
	GetSceneData().AddManager(pButtonManager);

	//Data
	auto pFontNone = ResourceManager::GetInstance().LoadFont("DIGDUG.ttf", 30);
	auto pFontHover = ResourceManager::GetInstance().LoadFont("DIGDUG.ttf", 40);
	SDL_Color colorNone = { 150,150,150,255 };
	SDL_Color colorHover = { 255,255,255,255 };


	//Buttons
	auto* pResumeButton = CreateGameObject();
	{
		auto* pButton = new DigDug::ButtonComponent();
		pResumeButton->AddComponent(pButton);
		pButton->SetStateNone(pFontNone, colorNone);
		pButton->SetStateHover(pFontHover, colorHover);
		auto* pText = new TextComponent();
		pText->SetText("Resume");
		pResumeButton->AddComponent(pText);
		pResumeButton->AddComponent(new RenderComponent());
		pButton->SetAction(m_pResumeButton);
	}
	auto* pMenuButton = CreateGameObject();
	{
		auto* pButton = new DigDug::ButtonComponent();
		pMenuButton->AddComponent(pButton);
		pButton->SetStateNone(pFontNone, colorNone);
		pButton->SetStateHover(pFontHover, colorHover);
		auto* pText = new TextComponent();
		pText->SetText("Menu");
		pMenuButton->AddComponent(pText);
		pMenuButton->AddComponent(new RenderComponent());
		pButton->SetAction(std::make_shared<DigDug::SceneSwapButton>(*GetSceneManager(), "Menu"));
	}


	//Positions
	const auto& windowSettings = dae::Minigin::GetWindowSettings();
	pResumeButton->GetTransform().SetWorldPosition(windowSettings.width*0.5f, windowSettings.height*0.4f);
	pMenuButton->GetTransform().SetWorldPosition(windowSettings.width*0.5f, windowSettings.height*0.6f);

	//Input
	auto* input = GetSceneData().GetInput();
	InputAction next{};
	next.triggerState = InputTriggerState::Pressed;
	next.pCommand = std::make_shared<InputButtonSet>(pButtonManager, InputButtonSet::Action::Next);
	next.keyboardCode = VK_DOWN;
	next.playerID = 0;
	next.gamepadCode = int(GamepadCode::GAMEPAD_RIGHTSTICK_DOWN);
	InputAction prev{};
	prev.triggerState = InputTriggerState::Pressed;
	prev.pCommand = std::make_shared<InputButtonSet>(pButtonManager, InputButtonSet::Action::Previous);
	prev.keyboardCode = VK_UP;
	prev.playerID = 0;
	prev.gamepadCode = int(GamepadCode::GAMEPAD_RIGHTSTICK_UP);
	InputAction select{};
	select.triggerState = InputTriggerState::Pressed;
	select.pCommand = std::make_shared<InputButtonSet>(pButtonManager, InputButtonSet::Action::Select);
	select.keyboardCode = VK_SPACE;
	select.playerID = 0;
	select.gamepadCode = int(GamepadCode::GAMEPAD_A);

	input->AddInputAction(next);
	input->AddInputAction(prev);
	input->AddInputAction(select);

	//Background
	auto* pBackground = CreateGameObject();
	{
		auto* pRender = new ScreenRenderComponent();
		pRender->SetTexture("MenuBackground.png");
		pBackground->AddComponent(pRender);
		pRender->SetRenderPriority(-10, GetSceneData());
	}
}

Scene* DigDugPauseScene::OnReload() const
{
	return new DigDugPauseScene(GetName());
}

void DigDugPauseScene::Enter(Scene* pPreviousScene)
{
	if (pPreviousScene && m_pResumeButton)
	{
		m_pResumeButton->SetSceneName(pPreviousScene->GetName());
	}
	else if (m_pResumeButton)
	{
		m_pResumeButton->SetSceneName("");
	}
}
