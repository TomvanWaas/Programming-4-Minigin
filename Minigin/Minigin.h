#pragma once
struct WindowSettings;
struct SDL_Window;
class SceneManager;
class Application;
#include "WindowSettings.h"
namespace dae
{
	class Minigin
	{
		const int m_MsPerFrame = 16; //16 for 60 fps, 33 for 30 fps
		SDL_Window* m_pWindow{};
		Application* m_pApplication;


		static WindowSettings m_WindowSettings;
		static bool m_Continue ;
		static bool m_Run ;


		void InitializeEngine();
		void CleanupEngine();

		void Initialize(SceneManager& sceneManager);
		void Cleanup();
	public:
		
		void Run();


		static void Quit();
		static void Reload();
		static const WindowSettings& GetWindowSettings() { return m_WindowSettings; }
	};
}