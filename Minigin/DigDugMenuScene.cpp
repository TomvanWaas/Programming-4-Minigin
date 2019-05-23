#include "MiniginPCH.h"
#include "DigDugMenuScene.h"
#include "Button.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "GameObject.h"
#include "GameButtons.h"
#include "RenderComponent.h"
#include "Minigin.h"
#include "InputManager.h"
#include "GameInputCommands.h"
#include "ScreenRenderComponent.h"

using namespace DigDug;

DigDugMenuScene::DigDugMenuScene(const std::string& name)
	: Scene(name)
{
}

void DigDugMenuScene::SceneInitialize()
{
	auto* pButtonManager = new DigDug::ButtonManager();
	GetSceneData().AddManager(pButtonManager);

	auto pFontNone = ResourceManager::GetInstance().LoadFont("DIGDUG.ttf", 30);
	auto pFontHover = ResourceManager::GetInstance().LoadFont("DIGDUG.ttf", 40);
	SDL_Color colorNone = { 150,150,150,255 };
	SDL_Color colorHover = { 255,255,255,255 };

	//Make Buttons
	//
	auto* pPlayAloneButton = CreateGameObject();
	{
		auto* pButton = new ButtonComponent();
		pPlayAloneButton->AddComponent(pButton);
		pButton->SetStateNone(pFontNone, colorNone);
		pButton->SetStateHover(pFontHover, colorHover);
		auto* pText = new TextComponent();
		pText->SetText("Play - One player");
		pPlayAloneButton->AddComponent(pText);
		pPlayAloneButton->AddComponent(new RenderComponent());
		pButton->SetAction(std::make_shared<SceneSwapButton>(*GetSceneManager(), "SingleplayerScene0", true));
	}
	//
	auto* pPlayTogetherButton = CreateGameObject();
	{
		auto* pButton = new ButtonComponent();
		pPlayTogetherButton->AddComponent(pButton);
		pButton->SetStateNone(pFontNone, colorNone);
		pButton->SetStateHover(pFontHover, colorHover);
		auto* pText = new TextComponent();
		pText->SetText("Play - Two players");
		pPlayTogetherButton->AddComponent(pText);
		pPlayTogetherButton->AddComponent(new RenderComponent());
		pButton->SetAction(std::make_shared<SceneSwapButton>(*GetSceneManager(), "CoopScene0", true));
	}
	//
	auto* pVersusButton = CreateGameObject();
	{
		auto* pButton = new ButtonComponent();
		pVersusButton->AddComponent(pButton);
		pButton->SetStateNone(pFontNone, colorNone);
		pButton->SetStateHover(pFontHover, colorHover);
		auto* pText = new TextComponent();
		pText->SetText("Play - Versus");
		pVersusButton->AddComponent(pText);
		pVersusButton->AddComponent(new RenderComponent());
		pButton->SetAction(std::make_shared<SceneSwapButton>(*GetSceneManager(), "VersusScene0", true));
	}
	//
	auto* pQuitButton = CreateGameObject();
	{
		auto* pButton = new ButtonComponent();
		pQuitButton->AddComponent(pButton);
		pButton->SetStateNone(pFontNone, colorNone);
		pButton->SetStateHover(pFontHover, colorHover);
		auto* pText = new TextComponent();
		pText->SetText("Quit");
		pQuitButton->AddComponent(pText);
		pQuitButton->AddComponent(new RenderComponent());
		pButton->SetAction(std::make_shared<QuitButton>());
	}


	//Position
	const auto& windowSettings = dae::Minigin::GetWindowSettings();
	pPlayAloneButton->GetTransform().SetWorldPosition(windowSettings.width*0.5f, windowSettings.height*0.35f);
	pPlayTogetherButton->GetTransform().SetWorldPosition(windowSettings.width*0.5f, windowSettings.height*0.45f);
	pVersusButton->GetTransform().SetWorldPosition(windowSettings.width*0.5f, windowSettings.height*0.55f);
	pQuitButton->GetTransform().SetWorldPosition(windowSettings.width*0.5f, windowSettings.height*0.65f);


	//Make Input
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

Scene* DigDugMenuScene::OnReload() const
{
	return new DigDugMenuScene(GetName());
}


