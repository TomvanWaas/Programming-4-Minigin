#include "MiniginPCH.h"
#pragma comment(lib,"xinput.lib")

// ReSharper disable once CppUnusedIncludeDirective
#include <vld.h>
#include "SDL.h"
#include "EngineObject.h"
#include "DigDugApplication.h"
#include "ResourceManager.h"

#pragma warning( push )  
#pragma warning( disable : 4100 )  
int main(int argc, char* argv[]) {
#pragma warning( pop )

	Engine::Logger::GetInstance().SetPath("Log.txt");
	Engine::ResourceManager::GetInstance().SetPath("Minigin/Resources/");

	Engine::EngineObject engine{};

	engine.Run(new DigDugApplication(), Engine::WindowSettings{ 2.5f * 224 , 2.5f * 288, "DigDug Game :: Programming 4" });
    return 0;
}

