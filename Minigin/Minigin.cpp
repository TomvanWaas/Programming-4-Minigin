#include "MiniginPCH.h"
#include "Minigin.h"
#include <chrono>
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include <SDL.h>
#include "GameObject.h"
#include "RenderManager.h"
#include "Application.h"


//Application
void dae::Minigin::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_pWindow = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (m_pWindow == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	RenderManager::InitializeRenderer(m_pWindow);
}


void dae::Minigin::Cleanup()
{
	SDL_DestroyWindow(m_pWindow);
	m_pWindow = nullptr;
	SDL_Quit();
	SAFE_DELETE(m_pApplication);
}

void dae::Minigin::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Resources/");
	
	SceneManager sceneManager{};
	if (m_pApplication != nullptr)
	{
		m_pApplication->Initialize(sceneManager);
	}
	//Run
	{
		auto t = std::chrono::high_resolution_clock::now();
		
		sceneManager.Initialize();
		InputManager::Initialize();

		bool doContinue = true;
		auto lastTime = std::chrono::high_resolution_clock::now();
		float lag = 0.0f;
		const float msPerUpdate = 0.001f * m_MsPerFrame;
		while (doContinue)
		{		
			const auto currentTime = std::chrono::high_resolution_clock::now();
			float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
			lastTime = currentTime;
			lag += deltaTime;

			doContinue = InputManager::ProcessInput();
			while (lag >= msPerUpdate)
			{
				sceneManager.Update(msPerUpdate);
				lag -= msPerUpdate;
			}
			sceneManager.Render();
		}

		InputManager::Destroy();

	}

	Cleanup();
}

void dae::Minigin::SetApplication(Application* pApp)
{
	if (pApp != nullptr) m_pApplication = pApp;
}

