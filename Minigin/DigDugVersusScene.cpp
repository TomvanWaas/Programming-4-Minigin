#include "MiniginPCH.h"
#include "DigDugVersusScene.h"

using namespace DigDug;

DigDugVersusScene::DigDugVersusScene(const std::string& name, const std::string& levelFile, int numLives, int levelNumber)
	: Scene(name)
{
	UNREFERENCED_PARAMETER(levelFile);
	UNREFERENCED_PARAMETER(numLives);
	UNREFERENCED_PARAMETER(levelNumber);
}


DigDugVersusScene::~DigDugVersusScene()
{
}

Scene* DigDugVersusScene::OnReload() const
{
	return nullptr;
}
