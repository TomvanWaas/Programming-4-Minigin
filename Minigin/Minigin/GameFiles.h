#pragma once


namespace Minigin
{
	struct Color4;
	class BaseComponent;
	class Observer;
	struct InputAction;
	class GameObject;
	class Scene;
	class SpriteComponent;
}
struct SDL_Color;
#include "DigDugSettings.h"

	class DigDugGridComponent;
namespace DigDug
{
	enum class Direction;

	//Background
	Minigin::GameObject* MakeBackground(Minigin::Scene& scene, const DigDugSettings& settings);

	//Obstacle
	Minigin::GameObject* CreateObstacle(Minigin::Scene& scene, const DigDugSettings& settings, Minigin::Observer* pScoreObserver);
	
	//Player
	Minigin::GameObject* CreatePlayer(Minigin::Scene& scene, const DigDugSettings& settings, Minigin::InputAction up, Minigin::InputAction down, Minigin::InputAction left, Minigin::InputAction right, Minigin::InputAction pump, int id = 0);
	void HelpPlayerSprites(Minigin::SpriteComponent& comp, const DigDugSettings& settings);
	Minigin::GameObject* CreatePump(Minigin::GameObject &parent, const DigDugSettings& settings);


	//Score
	Minigin::GameObject* CreateScore(Minigin::Scene& scene, const std::shared_ptr<Minigin::Font>& pFont, const SDL_Color& color, int score);
	Minigin::GameObject* CreateScoreManager(Minigin::Scene& scene, const std::shared_ptr<Minigin::Font>& pFont, const Minigin::Color4& color, Minigin::BaseComponent* pScoreObserver);


	//Pooka
	Minigin::GameObject* CreatePooka(Minigin::Scene& scene, const DigDugSettings& settings, int id, Minigin::Observer* pScoreObserver);
	Minigin::GameObject* CreateAIPooka(Minigin::Scene& scene, const DigDugSettings& settings, int id, Minigin::Observer* pScoreObserver);


	//Fygar
	Minigin::GameObject* CreateFygar(Minigin::Scene& scene, const DigDugSettings& settings, int id, Minigin::Observer* pScoreObserver);
	Minigin::GameObject* CreateAIFygar(Minigin::Scene& scene, const DigDugSettings& settings, int id, Minigin::Observer* pScoreObserver);
	Minigin::GameObject* CreatePlayerFygar(Minigin::Scene& scene, const DigDugSettings& settings, int id, Minigin::Observer* pScoreObserver, Minigin::InputAction up, Minigin::InputAction down, Minigin::InputAction left, Minigin::InputAction right, Minigin::InputAction fire, Minigin::InputAction ghost);
	Minigin::GameObject* CreateFire(Minigin::GameObject& parent, const DigDugSettings& settings);


}