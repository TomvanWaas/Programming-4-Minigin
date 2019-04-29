#include "MiniginPCH.h"
#pragma comment(lib,"xinput.lib")

// ReSharper disable once CppUnusedIncludeDirective
#include <vld.h>
#include "SDL.h"
#include "Minigin.h"


//Define the application
#define DigDug



//Include based on Define
#ifdef DigDug
	#include "DigDugApplication.h"
#endif


#pragma warning( push )  
#pragma warning( disable : 4100 )  
int main(int argc, char* argv[]) {
#pragma warning( pop )

	dae::Minigin engine;

	//Set Application Based on Define
#ifdef DigDug
	engine.SetApplication(new DigDugApplication());
#endif

	engine.Run();
    return 0;
}

