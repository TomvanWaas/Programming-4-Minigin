#include "stdafx.h"
#include "Engine.h"
#include <chrono>
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include <SDL.h>
#include "GameObject.h"
#include "RenderManager.h"
#include "Application.h"
#include "Deletor.h"

using namespace Minigin;

WindowSettings* Minigin::Engine::m_pWindowSettings = nullptr;
bool Minigin::Engine::m_Continue = true;
bool Minigin::Engine::m_Run = true;


void Minigin::Engine::InitializeEngine(SDL_Window* pWindow)
{
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		Logger::GetInstance().LogError("Engine::Initialize > Failed to Initialize SDL");
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}
	//Initialize Window
	pWindow = SDL_CreateWindow(GetWindowSettings().name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, int(GetWindowSettings().width), int(GetWindowSettings().height), SDL_WINDOW_OPENGL);
	if (pWindow == nullptr)
	{
		Logger::GetInstance().LogError("Engine::Initialize > Failed to Initialize Window");
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	//Initialize RenderManager
	RenderManager::InitializeRenderer(pWindow);

	//Initialize ResourceManager
	ResourceManager::GetInstance().Initialize("../Resources/");

	//Initialize Input
	InputManager::StaticInitialize();
}
void Minigin::Engine::CleanupEngine(SDL_Window*& pWindow)
{
	//Destroy Renderer
	RenderManager::DestroyRenderer();

	//Destroy Window
	if (pWindow) SDL_DestroyWindow(pWindow);
	pWindow = nullptr;

	//StaticQuit SDL
	SDL_Quit();

	//Destroy Input
	InputManager::StaticDestroy();
}



void Minigin::Engine::Initialize(SceneManager& sceneManager, Application* pApplication)
{
	//Initialize Application
	if (pApplication != nullptr)pApplication->Initialize(sceneManager, GetWindowSettings());
	else Logger::GetInstance().LogError("Engine::Initialize > No Application Selected");
	
	//Initialize SceneManager
	sceneManager.Initialize();
}
void Minigin::Engine::Cleanup(Application* pApplication)
{
	//Destroy Application
	SAFE_DELETE(pApplication);

	//Destroy DeleteStore
	Deletor::GetInstance().DeleteStore();
}


void Minigin::Engine::Run(Application* pApplication, const WindowSettings& wsettings, int msPerFrame)
{
	SDL_Window* pWindow = nullptr;
	WindowSettings settings = wsettings;
	m_pWindowSettings = &settings;


	InitializeEngine(pWindow);

	//Loop to enable Reloading
	while (m_Run)
	{
		m_Run = false;

		//SceneManager
		SceneManager sceneManager{};

		//Initialize
		Initialize(sceneManager, pApplication);

		//Run
		try
		{
			m_Continue = true;

			auto t = std::chrono::high_resolution_clock::now();
			auto lastTime = std::chrono::high_resolution_clock::now();

			float lag = 0.0f;
			const float msPerUpdate = 0.001f * msPerFrame;

			while (m_Continue)
			{
				const auto currentTime = std::chrono::high_resolution_clock::now();
				float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
				lastTime = currentTime;

				m_Continue &= InputManager::StaticProcessInput();
				sceneManager.Update(deltaTime);

				lag += deltaTime;
				while (lag >= msPerUpdate)
				{
					//Fixed Update
					lag -= msPerUpdate;
					sceneManager.FixedUpdate(msPerUpdate);
				}
				Deletor::GetInstance().DeleteStore();
				sceneManager.Render();
			}
		}
		catch (std::exception e)
		{
			std::stringstream msg{};
			msg << "Exception > " << e.what();
			Logger::GetInstance().LogError(msg.str());
		}


		//Cleanup
		Cleanup(pApplication);
	}
	CleanupEngine(pWindow);

	m_pWindowSettings = nullptr;
}




void Minigin::Engine::Quit()
{
	m_Continue = false;
	m_Run = false;
}
void Minigin::Engine::Reload()
{
	m_Run = true;
	m_Continue = false;
}

const WindowSettings& Engine::GetWindowSettings()
{
	if (m_pWindowSettings) return *m_pWindowSettings;
	throw std::exception("Engine::GetWindowSettings > Is Called while no Engine is running");	
}




