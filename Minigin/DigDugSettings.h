#pragma once
#include <memory>
#include "Vector2.h"
class Font;
class Texture2D;
class DigDugGridComponent;
namespace DigDug
{
	struct DigDugSettings
	{
		//Grid
		DigDugGridComponent* pGrid = nullptr;
		Vector2 gridOffset = Vector2(48, 48);
		unsigned int gridWidth = 15;
		unsigned int gridHeight = 12;

		//Sprites
		float spriteWidth = 16.0f;
		float spriteHeight = 16.0f;
		float spriteSpeed = 0.25f;

		//Resources
		std::shared_ptr<Texture2D> pDigDugTexture = nullptr;
		std::shared_ptr<Texture2D> pPookaTexture = nullptr;
		std::shared_ptr<Texture2D> pFygarTexture = nullptr;
		std::shared_ptr<Texture2D> pBackgroundTexture = nullptr;
		std::shared_ptr<Texture2D> pPumpTexture = nullptr;
		std::shared_ptr<Texture2D> pOtherTexture = nullptr;
		std::shared_ptr<Font> pScoreFont = nullptr;


	};
}
