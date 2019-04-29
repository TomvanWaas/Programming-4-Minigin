#pragma once
struct SDL_Window;
class SceneManager;
class Application;
namespace dae
{
	class Minigin
	{
		const int m_MsPerFrame = 16; //16 for 60 fps, 33 for 30 fps
		SDL_Window* m_pWindow{};
		Application* m_pApplication;

	public:
		void Initialize();
		void Cleanup();
		void Run();

		void SetApplication(Application* pApp);

	};
}