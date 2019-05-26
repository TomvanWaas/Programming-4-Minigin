#pragma once
#include <memory>
#include "Vector2.h"
namespace Engine
{
	class Font;
	class Texture2D;
}
namespace DigDug
{
	class DigDugGridComponent;
	struct DigDugSettings
	{
		//Grid
		DigDugGridComponent* pGrid = nullptr;
		Engine::Vector2 gridOffset = Engine::Vector2(48, 48);
		unsigned int gridWidth = 15;
		unsigned int gridHeight = 12;

		//Sprites
		float spriteWidth = 16.0f;
		float spriteHeight = 16.0f;
		float spriteSpeed = 0.25f;

		//Resources
		std::shared_ptr<Engine::Texture2D> pDigDugTexture = nullptr;
		std::shared_ptr<Engine::Texture2D> pPookaTexture = nullptr;
		std::shared_ptr<Engine::Texture2D> pFygarTexture = nullptr;
		std::shared_ptr<Engine::Texture2D> pBackgroundTexture = nullptr;
		std::shared_ptr<Engine::Texture2D> pPumpTexture = nullptr;
		std::shared_ptr<Engine::Texture2D> pOtherTexture = nullptr;
		std::shared_ptr<Engine::Font> pScoreFont = nullptr;

		//Values
		float fygarFireDelay = 7.5f;
		float fygarChargeTime = 1.5f;
		float fygarGhostTime = 7.5f;
		float fygarGhostDelay = 10.0f;
		float fygarSpeed = 70.0f;
		float fygarGhostSpeed = 60.0f;
		float fygarFireSpeed = 1.5f;

		float pookaGhostTime = 10.0f;
		float pookaGhostDelay = 10.0f;
		float pookaSpeed = 70.0f;
		float pookaGhostSpeed = 60.0f;

		float obstacleSpeed = 100.0f;
		float obstacleDelay = 2.0f;

		float playerSpeed = 80.0f;

		float deadDelay = 2.0f;

		float pumpTime = 2.0f;
		float pumpSpeed = 0.65f;
		float deflateSpeed = 0.5f;
	};
}
