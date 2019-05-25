#pragma once
struct SDL_Window;

namespace Minigin
{
	struct WindowSettings;
	class SceneManager;
	class Application;

	class Engine
	{
		const int m_MsPerFrame = 16; //16 for 60 fps, 33 for 30 fps
		SDL_Window* m_pWindow{};
		Application* m_pApplication;


		static WindowSettings m_WindowSettings;
		static bool m_Continue ;
		static bool m_Run ;


		void InitializeEngine();
		void CleanupEngine();

		void Initialize(SceneManager& sceneManager, Application* pApplication);
		void Cleanup(Application* pApplication);
	public:
		
		void Run(Application* pApplication);


		static void Quit();
		static void Reload();
		static const WindowSettings& GetWindowSettings() { return m_WindowSettings; }
	};
}