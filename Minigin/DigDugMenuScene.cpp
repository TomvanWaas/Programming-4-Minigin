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

	auto pFontNone = ResourceManager::GetInstance().LoadFont("DIGDUG.ttf", 50);
	auto pFontHover = ResourceManager::GetInstance().LoadFont("DIGDUG.ttf", 75);
	SDL_Color colorNone = { 255,0,0,255 };
	SDL_Color colorHover = { 255,127,0,0 };

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
		pButton->SetAction(std::make_shared<SceneSwapButton>(*GetSceneManager(), "SingleplayerScene"));
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
		pButton->SetAction(std::make_shared<SceneSwapButton>(*GetSceneManager(), "MultiplayerScene"));
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
	pPlayAloneButton->GetTransform().SetWorldPosition(windowSettings.width*0.5f, windowSettings.height*0.25f);
	pPlayTogetherButton->GetTransform().SetWorldPosition(windowSettings.width*0.5f, windowSettings.height*0.5f);
	pQuitButton->GetTransform().SetWorldPosition(windowSettings.width*0.5f, windowSettings.height*0.75f);

	//Make Input
	auto* input = GetSceneData().GetInput();

	InputAction next{};
	next.pCommand = std::make_shared<InputButtonSet>(pButtonManager, InputButtonSet::Action::Next);
	next.keyboardCode = VK_DOWN;
	InputAction prev{};
	prev.pCommand = std::make_shared<InputButtonSet>(pButtonManager, InputButtonSet::Action::Previous);
	prev.keyboardCode = VK_UP;
	InputAction select{};
	select.pCommand = std::make_shared<InputButtonSet>(pButtonManager, InputButtonSet::Action::Select);
	select.keyboardCode = VK_SPACE;

	input->AddInputAction(next);
	input->AddInputAction(prev);
	input->AddInputAction(select);

	//Background
	auto* pBackground = CreateGameObject();
	{
		auto* pRender = new ScreenRenderComponent();
		pRender->SetTexture("background.png");
		pBackground->AddComponent(pRender);
		pRender->SetRenderPriority(-10, GetSceneData());
	}
}

Scene* DigDugMenuScene::GetNew() const
{
	return new DigDugMenuScene(GetName());
}


