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
#include "WindowSettings.h"
#include "Deletor.h"
#include "WindowSettings.h"

using namespace Minigin;

WindowSettings Minigin::Engine::m_WindowSettings = { 224*2.5f, 288*2.5f, "Programming 4 Assignment: DigDug" };
bool Minigin::Engine::m_Continue = true;
bool Minigin::Engine::m_Run = true;



void Minigin::Engine::InitializeEngine()
{
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		Logger::GetInstance().LogError("Engine::Initialize > Failed to Initialize SDL");
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}
	//Initialize Window
	m_pWindow = SDL_CreateWindow(m_WindowSettings.name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, int(m_WindowSettings.width), int(m_WindowSettings.height), SDL_WINDOW_OPENGL);
	if (m_pWindow == nullptr)
	{
		Logger::GetInstance().LogError("Engine::Initialize > Failed to Initialize Window");
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	//Initialize RenderManager
	RenderManager::InitializeRenderer(m_pWindow);

	//Initialize ResourceManager
	ResourceManager::GetInstance().Initialize("../Resources/");

	//Initialize Input
	InputManager::StaticInitialize();
}
void Minigin::Engine::CleanupEngine()
{
	//Destroy Renderer
	RenderManager::DestroyRenderer();

	//Destroy Window
	SDL_DestroyWindow(m_pWindow);
	m_pWindow = nullptr;

	//StaticQuit SDL
	SDL_Quit();

	//Destroy Input
	InputManager::StaticDestroy();
}



void Minigin::Engine::Initialize(SceneManager& sceneManager, Application* pApplication)
{
	//Initialize Application
	if (pApplication != nullptr)pApplication->Initialize(sceneManager, m_WindowSettings);
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


void Minigin::Engine::Run(Application* pApplication)
{
	InitializeEngine();

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
			std::stringstream msg{};
			msg << "Exception > " << e.what();
			Logger::GetInstance().LogError(msg.str());
		}


		//Cleanup
		Cleanup(pApplication);
	}

	CleanupEngine();
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




