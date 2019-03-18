#include "MiniginPCH.h"
#include "Minigin.h"
#include <chrono>
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <SDL.h>
#include "GameObject.h"
#include "Scene.h"
#include "TransformComponent.h"
#include "TextComponent.h"
#include "RenderComponent.h"
#include "FPSComponent.h"
#include "Time.h"


void dae::Minigin::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(window);
}

/**
 * Code constructing the scene world starts here
 */
void dae::Minigin::LoadGame() const
{
	auto scene = SceneManager::GetInstance().CreateScene("Demo");

	auto go = new GameObject();
	RenderComponent* pRenderComponent = new RenderComponent{ "background.jpg" };
	go->AddComponent(pRenderComponent);
	scene->Add(go);

	go = new GameObject();
	pRenderComponent = new RenderComponent{ "logo.png" };
	go->AddComponent(pRenderComponent);
	go->GetTransform()->SetPosition(216, 180, 0);
	scene->Add(go);

	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	GameObject* to = new GameObject{};
	to->AddComponent(new RenderComponent{});
	to->AddComponent(new TextComponent{ "Programming 4 Assignment", SDL_Color{255,255,255,255}, font });

	to->GetTransform()->SetPosition(80, 20, 0);
	scene->Add(to);

	//FPS Counter
	auto smallfont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 15);
	GameObject* fpsCounter = new GameObject{};
	fpsCounter->AddComponent(new RenderComponent{});
	fpsCounter->AddComponent(new TextComponent{ "", SDL_Color{255,255,0,255}, smallfont });
	fpsCounter->GetTransform()->SetPosition(5, 5, 0);
	FPSComponent* fpsComp = new FPSComponent{};
	fpsComp->SetPrecision(0);
	fpsCounter->AddComponent(fpsComp);
	scene->Add(fpsCounter);
}

void dae::Minigin::Cleanup()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(window);
	window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");

	
	LoadGame();
	{
		auto t = std::chrono::high_resolution_clock::now();
		auto& renderer = Renderer::GetInstance();
		auto& sceneManager = SceneManager::GetInstance();
		sceneManager.Initialize();
		auto& input = InputManager::GetInstance();

		bool doContinue = true;
		auto lastTime = std::chrono::high_resolution_clock::now();
		float lag = 0.0f;
		const float msPerUpdate = 0.001f * msPerFrame;
		while (doContinue)
		{		
			const auto currentTime = std::chrono::high_resolution_clock::now();
			float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
			lastTime = currentTime;
			lag += deltaTime;

			Time::GetInstance().Update(deltaTime);

			doContinue = input.ProcessInput();
			while (lag >= msPerUpdate)
			{
				sceneManager.Update();
				lag -= msPerUpdate;
			}
			renderer.Render();
		}
	
	}

	Cleanup();
}


