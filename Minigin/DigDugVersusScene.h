#pragma once
#include "Scene.h"

namespace DigDug
{
	class DigDugVersusScene final : public Scene
	{
	public:
		DigDugVersusScene(const std::string& name, const std::string& levelFile, int numLives, int levelNumber);
		~DigDugVersusScene();

		virtual Scene* OnReload() const override;
	};

}