#pragma once
struct SDL_Window;
#include "WindowSettings.h"
namespace Minigin
{
	class SceneManager;
	class Application;

	class Engine final
	{		
		static WindowSettings* m_pWindowSettings;
		static bool m_Continue ;
		static bool m_Run ;


		void InitializeEngine(SDL_Window* pWindow);
		void CleanupEngine(SDL_Window*& pWindow);

		void Initialize(SceneManager& sceneManager, Application* pApplication);
		void Cleanup(Application* pApplication);
	public:
		explicit Engine() = default;
		~Engine() = default;

		void Run(Application* pApplication, const WindowSettings& wsettings, int msPerFrame = 16);//16 for 60 fps, 33 for 30 fps


		static void Quit();
		static void Reload();
		static const WindowSettings& GetWindowSettings();
	};
}