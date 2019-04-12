#include "MiniginPCH.h"
#include "Minigin.h"
#include <chrono>
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include <SDL.h>
#include "GameObject.h"
#include "Scene.h"
#include "TransformComponent.h"
#include "TextComponent.h"
#include "RenderComponent.h"
#include "FPSComponent.h"
#include "SpriteComponent.h"
#include "PositionGridComponent.h"
#include "Vector2.h"
#include "GridMovementComponent.h"
#include "InputCommand.h"
#include "MovementComponent.h"
#include "RenderManager.h"
#include "Texture2D.h"


//Test Func
void DaeBase(SceneManager& sceneManager);
void TestSpriteComponent(SceneManager& sceneManager);
void TestGridMovementComponent(SceneManager& sceneManager);
void TestGame(SceneManager& sceneManager);

//Game
void dae::Minigin::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_pWindow = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (m_pWindow == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	RenderManager::InitializeRenderer(m_pWindow);
}

/**
 * Code constructing the scene world starts here
 */
void dae::Minigin::LoadGame(SceneManager& sceneManager) const
{
	TestGame(sceneManager);
}

void dae::Minigin::Cleanup()
{
	SDL_DestroyWindow(m_pWindow);
	m_pWindow = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Resources/");
	
	SceneManager sceneManager{};
	LoadGame(sceneManager);
	{
		auto t = std::chrono::high_resolution_clock::now();
		
		sceneManager.Initialize();
		InputManager::Initialize();

		bool doContinue = true;
		auto lastTime = std::chrono::high_resolution_clock::now();
		float lag = 0.0f;
		const float msPerUpdate = 0.001f * m_MsPerFrame;
		while (doContinue)
		{		
			const auto currentTime = std::chrono::high_resolution_clock::now();
			float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
			lastTime = currentTime;
			lag += deltaTime;

			doContinue = InputManager::ProcessInput();
			while (lag >= msPerUpdate)
			{
				sceneManager.Update(msPerUpdate);
				lag -= msPerUpdate;
			}
			sceneManager.Render();
		}

		InputManager::Destroy();

	}

	Cleanup();
}



//Test Func
void DaeBase(SceneManager& sceneManager)
{
	auto scene = sceneManager.CreateScene("Demo");

	auto go = new GameObject();
	RenderComponent* pRenderComponent = new RenderComponent{ "background.jpg" };
	go->AddComponent(pRenderComponent);
	scene->AddChild(go);

	go = new GameObject();
	pRenderComponent = new RenderComponent{ "logo.png" };
	go->AddComponent(pRenderComponent);
	go->GetComponent<TransformComponent>()->SetWorldPosition(216, 180);
	scene->AddChild(go);

	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	GameObject* to = new GameObject{};
	to->AddComponent(new RenderComponent{});
	to->AddComponent(new TextComponent{ "Programming 4 Assignment", SDL_Color{255,255,255,255}, font });

	to->GetComponent<TransformComponent>()->SetWorldPosition(80, 20);
	scene->AddChild(to);

	//FPS Counter
	auto smallfont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 15);
	GameObject* fpsCounter = new GameObject{ };
	fpsCounter->AddComponent(new RenderComponent{});
	fpsCounter->AddComponent(new TextComponent{ "", SDL_Color{255,255,0,255}, smallfont });
	fpsCounter->GetComponent<TransformComponent>()->SetWorldPosition(5, 5);
	FPSComponent* fpsComp = new FPSComponent{};
	fpsComp->SetPrecision(0);
	fpsCounter->AddComponent(fpsComp);
	scene->AddChild(fpsCounter);
}

//Test movement
class MoveCommand final : public InputCommand
{
public:
	MoveCommand(GridMovementComponent* pGridMove, SpriteComponent* pSprite, Direction dir, Direction ifdir = Direction::None, bool hasifdir = false)
		: m_pMovement(pGridMove)
		, m_Direction(dir)
		, m_RequiredDirection(ifdir)
		, m_HasRequired(hasifdir)
		, InputCommand()
		, m_pSprite(pSprite)
	{}

	virtual void Execute() override
	{
		if (m_pMovement != nullptr)
		{
			if (m_HasRequired && m_pMovement->GetCurrentDirection() == m_RequiredDirection)
			{
				m_pMovement->Move(m_Direction);
				m_pSprite->SetCurrentSource(unsigned int(m_Direction));
			}
			else if (!m_HasRequired)
			{
				m_pMovement->Move(m_Direction);
				m_pSprite->SetCurrentSource(unsigned int(m_Direction));
			}
		}
	}


private:
	GridMovementComponent* m_pMovement;
	SpriteComponent* m_pSprite;
	Direction m_Direction;
	Direction m_RequiredDirection;
	bool m_HasRequired;

};

void TestGame(SceneManager& sceneManager)
{
	auto pScene = sceneManager.CreateScene("TestGame");



	GameObject* pBackground = new GameObject{};
	pBackground->GetComponent<TransformComponent>()->SetWorldPosition(200, 250);
	pBackground->GetComponent<TransformComponent>()->SetWorldScale(1.5f, 1.5f);

	RenderComponent* pBackgroundRenderComponent = new RenderComponent{ "background.png" };
	pBackground->AddComponent(pBackgroundRenderComponent);
	
	GameObject* pGrid = new GameObject{};
	pBackground->AddChild(pGrid);
	pGrid->GetComponent<TransformComponent>()->SetLocalPosition(
		-pBackgroundRenderComponent->GetTexture()->GetWidth()*0.5f*1.5f + 7.5f
		, -pBackgroundRenderComponent->GetTexture()->GetHeight()*0.5f*1.5f + 7.5f + 35.f);
	pGrid->GetComponent<TransformComponent>()->SetLocalScale(1, 1);

	PositionGridComponent* pGridComponent = new PositionGridComponent{ 14 ,17, Vector2(16.25f, 15) };
	pGrid->AddComponent(pGridComponent);

	pScene->AddChild(pBackground);



	GameObject* pPlayer = new GameObject{};
	pPlayer->GetComponent<TransformComponent>()->SetWorldPosition(pGridComponent->Get(0, 0));
	pPlayer->GetComponent<TransformComponent>()->SetWorldScale(1.5f, 1.5f);

	RenderComponent* pPlayerRenderComponent = new RenderComponent{};
	pPlayer->AddComponent(pPlayerRenderComponent);

	SpriteComponent* pPlayerSpriteComponent = new SpriteComponent{};
	pPlayerSpriteComponent->SetSpriteSpeed(0.25f);
	pPlayer->AddComponent(pPlayerSpriteComponent);
	pPlayerSpriteComponent->SetTexture("DigDugSprites.png");

	float w = 15;
	float h = 15;
	SpriteSource rightSource{ Rect(0,0, w,h), 2 };
	SpriteSource upSource{ Rect(2*w,0, w,h), 2 };
	SpriteSource leftSource{ Rect(4*w,0, w,h), 2 };
	SpriteSource downSource{ Rect(6*w,0, w,h), 2 };

	pPlayerSpriteComponent->AddSprite(unsigned int(Direction::Right), rightSource);
	pPlayerSpriteComponent->AddSprite(unsigned int(Direction::Up), upSource);
	pPlayerSpriteComponent->AddSprite(unsigned int(Direction::Left), leftSource);
	pPlayerSpriteComponent->AddSprite(unsigned int(Direction::Down), downSource);

	GridMovementComponent* pPlayerGridMovementComponent = new GridMovementComponent(pGridComponent, 50.0f);
	pPlayer->AddComponent(pPlayerGridMovementComponent);

	MovementComponent* pPlayerMovementComponent = new MovementComponent{ 1 };
	pPlayer->AddComponent(pPlayerMovementComponent);

	pScene->AddChild(pPlayer);


	//Apply commands
	std::shared_ptr<InputCommand> pUp = std::make_shared<MoveCommand>(pPlayerGridMovementComponent, pPlayerSpriteComponent, Direction::Up);
	std::shared_ptr<InputCommand> pDown = std::make_shared<MoveCommand>(pPlayerGridMovementComponent, pPlayerSpriteComponent, Direction::Down);
	std::shared_ptr<InputCommand> pLeft = std::make_shared<MoveCommand>(pPlayerGridMovementComponent, pPlayerSpriteComponent, Direction::Left);
	std::shared_ptr<InputCommand> pRight = std::make_shared<MoveCommand>(pPlayerGridMovementComponent, pPlayerSpriteComponent, Direction::Right);
	std::shared_ptr<InputCommand> pNoneUp = std::make_shared<MoveCommand>(pPlayerGridMovementComponent, pPlayerSpriteComponent, Direction::None, Direction::Up, true);
	std::shared_ptr<InputCommand> pNoneDown = std::make_shared<MoveCommand>(pPlayerGridMovementComponent, pPlayerSpriteComponent, Direction::None, Direction::Down, true);
	std::shared_ptr<InputCommand> pNoneLeft = std::make_shared<MoveCommand>(pPlayerGridMovementComponent, pPlayerSpriteComponent, Direction::None, Direction::Left, true);
	std::shared_ptr<InputCommand> pNoneRight = std::make_shared<MoveCommand>(pPlayerGridMovementComponent, pPlayerSpriteComponent, Direction::None, Direction::Right, true);

	pScene->GetSceneData().pInput->AddInputAction(InputAction(InputTriggerState::Pressed, pUp, -1, 'W', -1));
	pScene->GetSceneData().pInput->AddInputAction(InputAction(InputTriggerState::Pressed, pDown, -1, 'S', -1));
	pScene->GetSceneData().pInput->AddInputAction(InputAction(InputTriggerState::Pressed, pLeft, -1, 'A', -1));
	pScene->GetSceneData().pInput->AddInputAction(InputAction(InputTriggerState::Pressed, pRight, -1, 'D', -1));

	pScene->GetSceneData().pInput->AddInputAction(InputAction(InputTriggerState::Released, pNoneUp, -1, 'W', -1));
	pScene->GetSceneData().pInput->AddInputAction(InputAction(InputTriggerState::Released, pNoneDown, -1, 'S', -1));
	pScene->GetSceneData().pInput->AddInputAction(InputAction(InputTriggerState::Released, pNoneLeft, -1, 'A', -1));
	pScene->GetSceneData().pInput->AddInputAction(InputAction(InputTriggerState::Released, pNoneRight, -1, 'D', -1));

}
