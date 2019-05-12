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
		//Sprites
		float spriteWidth = 16.0f;
		float spriteHeight = 16.0f;
		std::shared_ptr<Texture2D> spriteTexture = nullptr;
		float spriteSpeed = 0.25f;
		//Grid
		DigDugGridComponent* pGrid = nullptr;
		Vector2 gridOffset = Vector2(48, 48);
		unsigned int gridWidth = 15;
		unsigned int gridHeight = 12;
		std::shared_ptr<Texture2D> backgroundTexture = nullptr;
		std::shared_ptr<Texture2D> cavesTexture;
	};



	GameObject* MakePlayerObject(Scene& scene, const DigDugSettings& settings, char up, char down, char left, char right, char pump);

	void MakePlayerSprites(SpriteComponent* pSpriteComp, const DigDugSettings& settings);
	void MakePlayerInput(GameObject* pPlayer, const DigDugSettings& settings, char up, char down, char left, char right, char pump);
	void MakePlayerFSM(GameObject* pPlayer, const DigDugSettings& settings);
	GameObject* MakePlayerPump(Scene& scene, GameObject& parent, const DigDugSettings& settings);

	   	 
	GameObject* MakeBackground(Scene& scene, const DigDugSettings& settings);

	








	GameObject* CreateObstacle(Scene& scene, const DigDugSettings& settings);
	GameObject* CreatePooka(Scene& scene, const DigDugSettings& settings);
	GameObject* CreatePump(GameObject &parent, const DigDugSettings& settings);

}