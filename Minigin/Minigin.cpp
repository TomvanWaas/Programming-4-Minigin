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
#include "ApplicationSelector.h"

WindowSettings dae::Minigin::m_WindowSettings = { 224*2.5f, 288*2.5f, "Programming 4 Assignment: DigDug" };
bool dae::Minigin::m_Continue = true;
bool dae::Minigin::m_Run = true;



void dae::Minigin::InitializeEngine()
{
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		Logger::GetInstance().LogError("Minigin::Initialize > Failed to Initialize SDL");
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}
	//Initialize Window
	m_pWindow = SDL_CreateWindow(m_WindowSettings.name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, int(m_WindowSettings.width), int(m_WindowSettings.height), SDL_WINDOW_OPENGL);
	if (m_pWindow == nullptr)
	{
		Logger::GetInstance().LogError("Minigin::Initialize > Failed to Initialize Window");
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	//Initialize RenderManager
	RenderManager::InitializeRenderer(m_pWindow);

	//Initialize ResourceManager
	ResourceManager::GetInstance().Initialize("../Resources/");

	//Initialize Input
	InputManager::StaticInitialize();
}
void dae::Minigin::CleanupEngine()
{
	//Destroy Renderer
	RenderManager::DestroyRenderer();

	//Destroy Window
	SDL_DestroyWindow(m_pWindow);
	m_pWindow = nullptr;

	//Quit SDL
	SDL_Quit();

	//Destroy Input
	InputManager::StaticDestroy();
}



void dae::Minigin::Initialize(SceneManager& sceneManager)
{
	m_pApplication = Selector::GetApplication();

	//Initialize Application
	if (m_pApplication != nullptr)m_pApplication->Initialize(sceneManager, m_WindowSettings);
	else Logger::GetInstance().LogError("Minigin::Initialize > No Application Selected");
	
	//Initialize SceneManager
	sceneManager.Initialize();
}
void dae::Minigin::Cleanup()
{
	//Destroy Application
	SAFE_DELETE(m_pApplication);

	//Destroy DeleteStore
	Deletor::GetInstance().DeleteStore();
}


void dae::Minigin::Run()
{
	InitializeEngine();

	//Loop to enable Reloading
	while (m_Run)
	{
		m_Run = false;

		//SceneManager
		SceneManager sceneManager{};

		//Initialize
		Initialize(sceneManager);

		//Run
		try
		{
			m_Continue = true;

			auto t = std::chrono::high_resolution_clock::now();
			auto lastTime = std::chrono::high_resolution_clock::now();

			float lag = 0.0f;
			const float msPerUpdate = 0.001f * m_MsPerFrame;

			while (m_Continue)
			{
				const auto currentTime = std::chrono::high_resolution_clock::now();
				float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
				lastTime = currentTime;
				lag += deltaTime;

				m_Continue &= InputManager::StaticProcessInput();
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
		Cleanup();
	}

	CleanupEngine();
}




void dae::Minigin::Quit()
{
	m_Continue = false;
	m_Run = false;
}
void dae::Minigin::Reload()
{
	m_Run = true;
	m_Continue = false;
}




