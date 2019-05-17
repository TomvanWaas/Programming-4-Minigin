#pragma once



struct InputAction;
class GameObject;
class Scene;
class DigDugGridComponent;
class SpriteComponent;
class Texture2D;
#include "Vector2.h"

namespace DigDug
{
	enum class PlayerState;
	enum class PlayerHealth;
	enum class PumpStatus;
	enum class Direction;

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

		//Textures
		std::shared_ptr<Texture2D> pDigDugTexture = nullptr;
		std::shared_ptr<Texture2D> pPookaTexture = nullptr;
		std::shared_ptr<Texture2D> pFygarTexture = nullptr;
		std::shared_ptr<Texture2D> pBackgroundTexture = nullptr;
		std::shared_ptr<Texture2D> pCaveTexture = nullptr;
		std::shared_ptr<Texture2D> pPumpTexture = nullptr;
		std::shared_ptr<Texture2D> pOtherTexture = nullptr;



	};




	   	 
	GameObject* MakeBackground(Scene& scene, const DigDugSettings& settings);

	








	GameObject* CreateObstacle(Scene& scene, const DigDugSettings& settings);
	GameObject* CreatePooka(Scene& scene, const DigDugSettings& settings);
	GameObject* CreatePump(GameObject &parent, const DigDugSettings& settings);
	GameObject* CreatePlayer(Scene& scene, const DigDugSettings& settings, char up, char down, char left, char right, char pump);
	void HelpPlayerSprites(SpriteComponent& comp, const DigDugSettings& settings);

	GameObject* CreateFygar(Scene& scene, const DigDugSettings& settings);
	GameObject* CreateFire(GameObject& parent, const DigDugSettings& settings);

}