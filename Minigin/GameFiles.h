#pragma once


struct Color4;
class BaseComponent;
class Observer;
struct InputAction;
class GameObject;
class Scene;
class SpriteComponent;
struct SDL_Color;
#include "DigDugSettings.h"

	class DigDugGridComponent;
namespace DigDug
{
	enum class Direction;


	   	 
	GameObject* MakeBackground(Scene& scene, const DigDugSettings& settings);

	








	GameObject* CreateObstacle(Scene& scene, const DigDugSettings& settings, Observer* pScoreObserver);
	GameObject* CreatePooka(Scene& scene, const DigDugSettings& settings, Observer* pScoreObserver);
	GameObject* CreatePump(GameObject &parent, const DigDugSettings& settings);
	GameObject* CreatePlayer(Scene& scene, const DigDugSettings& settings, InputAction up, InputAction down, InputAction left, InputAction right, InputAction pump, int id = 0);
	void HelpPlayerSprites(SpriteComponent& comp, const DigDugSettings& settings);

	GameObject* CreateFygar(Scene& scene, const DigDugSettings& settings, Observer* pScoreObserver);
	GameObject* CreateFire(GameObject& parent, const DigDugSettings& settings);

	GameObject* CreateScore(Scene& scene, const std::shared_ptr<Font>& pFont, const SDL_Color& color, int score);
	GameObject* CreateScoreManager(Scene& scene, const std::shared_ptr<Font>& pFont, const Color4& color, BaseComponent* pScoreObserver);
}