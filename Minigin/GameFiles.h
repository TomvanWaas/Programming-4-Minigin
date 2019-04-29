#pragma once
#include "Direction.h"
//Includes



//Forward decl
struct InputAction;
class GameObject;
class Scene;
class DigDugGridComponent;
class SpriteComponent;
class Texture2D;

namespace DigDug
{
	struct DigDugSettings
	{
		float spriteWidth = 16.0f;
		float spriteHeight = 16.0f;
		std::shared_ptr<Texture2D> spriteTexture = nullptr;
		float spriteSpeed = 0.25f;
		DigDugGridComponent* pGrid = nullptr;
	};

	//Functions
	GameObject* MakePlayer(Scene& scene,
		InputAction& moveLeftIAction,
		InputAction& moveRightIAction,
		InputAction& moveUpIAction,
		InputAction& moveDownIAction,
		InputAction& throwIAction, 
		const DigDugSettings& settings
	);

	void MakePlayerSprites(SpriteComponent* pSpriteComp, const DigDugSettings& settings);

	void MakePlayerInput(Scene& scene,
		unsigned int& moveleftUp, unsigned int& moveleftDown,
		unsigned int& moverightUp, unsigned int& moverightDown,
		unsigned int& movedownUp, unsigned int& movedownDown,
		unsigned int& moveupUp, unsigned int& moveupDown,
		unsigned int& throwUp, unsigned int& throwDown,
		InputAction& moveLeftIAction,
		InputAction& moveRightIAction,
		InputAction& moveUpIAction,
		InputAction& moveDownIAction,
		InputAction& throwIAction);

	void MakePlayerFSM(Scene& scene, GameObject* pPlayer, DigDugGridComponent& grid, const float moveSpeed,
		unsigned int& moveleftUp, unsigned int& moveleftDown,
		unsigned int& moverightUp, unsigned int& moverightDown,
		unsigned int& movedownUp, unsigned int& movedownDown,
		unsigned int& moveupUp, unsigned int& moveupDown,
		unsigned int& throwUp, unsigned int& throwDown);



	GameObject* MakeBackground(Scene& scene);

	GameObject* MakeStone(Scene& scene, DigDugGridComponent& grid);

	GameObject* MakeLasso(GameObject& parent, const DigDugSettings& settings, Direction dir);
	
}