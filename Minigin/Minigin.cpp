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
#include "WindowSettings.h"
#include "Deletor.h"

WindowSettings dae::Minigin::m_WindowSettings = { 720.0f, 560.0f, "Pogramming 4 Assignment" };




void dae::Minigin::Initialize(const WindowSettings& settings)
{
	//InitializeOverride Window
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_pWindow = SDL_CreateWindow(
		settings.name.c_str(),
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		int(settings.width),
		int(settings.height),
		SDL_WINDOW_OPENGL
	);
	if (m_pWindow == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	//InitializeOverride RenderManager
	RenderManager::InitializeRenderer(m_pWindow);

	//Initialze ResourceManager
	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Initialize("../Resources/");
}
void dae::Minigin::Cleanup()
{
	InputManager::StaticDestroy();

	SDL_DestroyWindow(m_pWindow);
	m_pWindow = nullptr;
	SDL_Quit();
	SAFE_DELETE(m_pApplication);
}
void dae::Minigin::Run()
{
	//InitializeOverride
	Initialize(m_WindowSettings);
	SceneManager sceneManager{};
	if (m_pApplication != nullptr)
	{
		m_pApplication->Initialize(sceneManager, m_WindowSettings);
	}
	auto t = std::chrono::high_resolution_clock::now();
	sceneManager.Initialize();
	InputManager::StaticInitialize();

	try
	{
		//Gameloop
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

			doContinue = InputManager::StaticProcessInput();
			while (lag >= msPerUpdate)
			{
				sceneManager.Update(msPerUpdate);
				Deletor::GetInstance().DeleteStore();
				lag -= msPerUpdate;
			}
			sceneManager.Render();
		}
	}
	catch (std::exception e)
	{
		std::cout << "Exception!";
		std::cout << e.what();
	}


	//Cleanup
	Deletor::GetInstance().DeleteStore();
	Cleanup();
}



void dae::Minigin::SetApplication(Application* pApp)
{
	if (pApp != nullptr) m_pApplication = pApp;
}

