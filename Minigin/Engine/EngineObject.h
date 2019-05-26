#pragma once
struct SDL_Window;
#include "WindowSettings.h"
namespace Engine
{
	class SceneManager;
	class Application;

	class EngineObject final
	{		
		static WindowSettings* m_pWindowSettings;
		static bool m_Continue ;
		static bool m_Run ;


		void InitializeEngine(SDL_Window* pWindow);
		void CleanupEngine(SDL_Window*& pWindow);

		void Initialize(SceneManager& sceneManager, Application* pApplication);
		void Cleanup(Application* pApplication);
	public:
		explicit EngineObject() = default;
		~EngineObject() = default;

		void Run(Application* pApplication, const WindowSettings& wsettings, unsigned int msPerFrame = 16, unsigned int maxFixedPerFrame = 3);//16 for 60 fps, 33 for 30 fps


		static void Quit();
		static void Reload();
		static const WindowSettings& GetWindowSettings();
	};
}