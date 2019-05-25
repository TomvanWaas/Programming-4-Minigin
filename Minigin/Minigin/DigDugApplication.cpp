#include "MiniginPCH.h"
#include "DigDugApplication.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "InputManager.h"
#include "WindowSettings.h"
#include "DigDugSingleplayerScene.h"
#include "DigDugMenuScene.h"
#include "DigDugMultiplayerScene.h"
#include "DigDugVersusScene.h"
#include "DigDugPauseScene.h"

using namespace DigDug;
using namespace Minigin;

#include "ObservedData.h"
void DigDugApplication::Initialize(SceneManager& sceneManager, const WindowSettings& windowSettings)
{
	UNREFERENCED_PARAMETER(windowSettings);

	//MenuScene
	auto pMenuScene = new DigDugMenuScene("Menu");
	sceneManager.AddScene(pMenuScene);

	//PauseScene
	auto pPauseScene = new DigDugPauseScene("Pause");
	sceneManager.AddScene(pPauseScene);


	int nrSingleplayer = 2;
	int nrVersus = 2;
	int nrCoop = 2;


	///////////////////////////////////////////////////////////
	//Singleplayer
	//Add all singleplayer levels here so SetNextScene can be used
	//...
	for (int i = 0; i < nrSingleplayer; ++i)
	{
		std::stringstream level{};
		level << "Level" << i << ".txt";
		std::stringstream name{};
		name << "SingleplayerScene" << i;
		auto* pScene = new DigDugSingleplayerScene(name.str(), level.str(), 3, i+1);
		sceneManager.AddScene(pScene);
	}
	

	///////////////////////////////////////////////////////////
	//Co-op
	//Add all Co-op levels here so SetNextScene can be used
	//...
	for (int i = 0; i < nrCoop; ++i)
	{
		std::stringstream level{};
		level << "Level" << i << ".txt";
		std::stringstream name{};
		name << "CoopScene" << i;
		auto* pScene = new DigDugMultiplayerScene(name.str(), level.str(), 3, 3, i+1);
		sceneManager.AddScene(pScene);
	}
	

	///////////////////////////////////////////////////////////
	//Versus
	//Add all Versus levels here so SetNextScene can be used
	//...
	for (int i = 0; i < nrVersus; ++i)
	{
		std::stringstream level{};
		level << "Level" << i << ".txt";
		std::stringstream name{};
		name << "VersusScene" << i;
		auto* pScene = new DigDugVersusScene(name.str(), level.str(), 3, i+1);
		sceneManager.AddScene(pScene);
	}

	
	sceneManager.SetActiveScene("Menu");
}




