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
	public:
		void Initialize(const WindowSettings& settings);
		void Cleanup();
		void Run();

		void SetApplication(Application* pApp);

		static const WindowSettings& GetWindowSettings() { return m_WindowSettings; }
	};
}