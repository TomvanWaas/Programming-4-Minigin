#pragma once


namespace Engine
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
	Engine::GameObject* MakeBackground(Engine::Scene& scene, const DigDugSettings& settings);

	//Obstacle
	Engine::GameObject* CreateObstacle(Engine::Scene& scene, const DigDugSettings& settings, Engine::Observer* pScoreObserver);
	
	//Player
	Engine::GameObject* CreatePlayer(Engine::Scene& scene, const DigDugSettings& settings, Engine::InputAction up, Engine::InputAction down, Engine::InputAction left, Engine::InputAction right, Engine::InputAction pump, int id = 0);
	void HelpPlayerSprites(Engine::SpriteComponent& comp, const DigDugSettings& settings);
	Engine::GameObject* CreatePump(Engine::GameObject &parent, const DigDugSettings& settings);


	//Score
	Engine::GameObject* CreateScore(Engine::Scene& scene, const std::shared_ptr<Engine::Font>& pFont, const SDL_Color& color, int score);
	Engine::GameObject* CreateScoreManager(Engine::Scene& scene, const std::shared_ptr<Engine::Font>& pFont, const Engine::Color4& color, Engine::BaseComponent* pScoreObserver);


	//Pooka
	Engine::GameObject* CreatePooka(Engine::Scene& scene, const DigDugSettings& settings, int id, Engine::Observer* pScoreObserver);
	Engine::GameObject* CreateAIPooka(Engine::Scene& scene, const DigDugSettings& settings, int id, Engine::Observer* pScoreObserver);


	//Fygar
	Engine::GameObject* CreateFygar(Engine::Scene& scene, const DigDugSettings& settings, int id, Engine::Observer* pScoreObserver);
	Engine::GameObject* CreateAIFygar(Engine::Scene& scene, const DigDugSettings& settings, int id, Engine::Observer* pScoreObserver);
	Engine::GameObject* CreatePlayerFygar(Engine::Scene& scene, const DigDugSettings& settings, int id, Engine::Observer* pScoreObserver, Engine::InputAction up, Engine::InputAction down, Engine::InputAction left, Engine::InputAction right, Engine::InputAction fire, Engine::InputAction ghost);
	Engine::GameObject* CreateFire(Engine::GameObject& parent, const DigDugSettings& settings);


}