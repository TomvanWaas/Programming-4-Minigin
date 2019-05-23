#include "MiniginPCH.h"
#include "GameFiles.h"
#include "Enumerations.h"
#include "GameObject.h"
#include "Scene.h"
#include "Components.h"
#include "InputManager.h"
#include "FSMState.h"
#include "DigDugGridComponent.h"
#include "GameInputCommands.h"
#include "SpriteCollisionComponent.h"
#include "DigDugMovementComponent.h"
#include "ObstacleParts.h"
#include "GameFSMStates.h"
#include "EnemyManager.h"
#include "EnemyParts.h"
#include "PumpParts.h"
#include "GameEvents.h"
#include "DigDugEnemyComponent.h"
#include "DigDugPlayerComponent.h"
#include "PlayerParts.h"
#include "ScreenRenderComponent.h"
#include "AutokillComponent.h"
#include "EnemyScoreObserver.h"
#include "ObstacleScoreObserver.h"
#include "ObserverManager.h"

using namespace DigDug;














//Background
GameObject* DigDug::MakeBackground(Scene& scene, const DigDugSettings& settings)
{
	auto pBackground = scene.CreateGameObject();

	//Texture
	auto pTextureObject = pBackground->CreateChild();
	{
		auto pRenderComp = new ScreenRenderComponent();
		pRenderComp->SetTexture(settings.pBackgroundTexture);
		if (pTextureObject->AddComponent(pRenderComp))
		{
			pTextureObject->GetTransform().SetLocalScale(3.25f, 2.25f);
			pTextureObject->GetTransform().SetLocalPosition(0, -20.0f);
			pRenderComp->SetRenderPriority(-50, scene.GetSceneData());
		}
		else
		{
			SAFE_DELETE(pRenderComp);
		}
	}
	//Grid
	auto pGridObject = pBackground->CreateChild();
	{
		auto pGridComp = new DigDugGridComponent(settings.gridWidth, settings.gridHeight);
		if (pGridObject->AddComponent(pGridComp))
		{

			pGridComp->SetOffset(settings.gridOffset);

			auto pGridRender = new MultiRenderComponent(settings.pOtherTexture);
			pGridRender->SetSource(Rect{ settings.spriteWidth * 6, 0, settings.spriteWidth, settings.spriteHeight });
			if (pGridObject->AddComponent(pGridRender))
			{
				pGridRender->SetRenderPriority(-25, scene.GetSceneData());
			}
			else
			{
				SAFE_DELETE(pGridRender);
			}
		}
		else
		{
			SAFE_DELETE(pGridComp);
		}
	}



	return pBackground;
}









GameObject* DigDug::CreateObstacle(Scene& scene, const DigDugSettings& settings, Observer* pScoreObserver)
{
	GameObject* pObject = scene.CreateGameObject();

	auto pObserver = new ObstacleScoreObserver();
	pObject->AddObserver(pObserver);
	pObserver->AddObserver(pScoreObserver);

	auto om = scene.GetSceneData().GetManager<ObserverManager>();
	if (om) om->AddUnownedObserver(pObserver);

	//Components
	pObject->AddComponent(new RenderComponent());
	auto* pColliderC = new AABBCollisionComponent(); pObject->AddComponent(pColliderC);
	auto* pSpriteC = new SpriteComponent(); pObject->AddComponent(pSpriteC);
	auto* pFsmC = new FiniteStateMachineComponent(); pObject->AddComponent(pFsmC);

	////Construct
	//Collider
	pColliderC->SetTrigger(false);
	pColliderC->SetTag("Obstacle");
	//pColliderC->SetOffset(0, 0, 0, 0);
	pColliderC->SetCollider(settings.spriteWidth, settings.spriteHeight, 0, 0);

	//Sprite
	pSpriteC->SetTexture(settings.pOtherTexture);
	pSpriteC->SetSpriteSpeed(settings.spriteSpeed);
	Rect src{ 0,0, settings.spriteWidth, settings.spriteHeight };
	pSpriteC->AddSprite(unsigned int(ObstacleSpriteID::Idle), new FixedSource(src));
	pSpriteC->AddSprite(unsigned int(ObstacleSpriteID::Moving), new TickSource(src, 2, true));
	src.x += 2 * src.width;
	pSpriteC->AddSprite(unsigned int(ObstacleSpriteID::Destroyed), new TickSource(src, 2, false));

	//Fsm
	FSMState* pDestrState = new FSMStateDestroyObject(pObject);
	FSMState* pDestrDelayState = new FSMStateDelay(1.5f, pDestrState);
	FSMState* pFallState = new FSMStateObstacleFall(pObject, settings.pGrid, 100.0f, pDestrDelayState);
	FSMState* pFallDelayState = new FSMStateDelay(1.5f, pFallState);
	FSMState* pIdleState = new FSMStateObstacleIdle(settings.pGrid, pObject, pFallDelayState);
	pFsmC->SaveState("DestoyedState",pDestrState);
	pFsmC->SaveState("DestroyedDelayState", pDestrDelayState);
	pFsmC->SaveState("FallState",pFallState);
	pFsmC->SaveState("FallDelayState",pFallDelayState);
	pFsmC->SaveState("IdleState",pIdleState);
					 
	pFsmC->SetState(pIdleState);
	return pObject;
}

GameObject* DigDug::CreatePooka(Scene& scene, const DigDugSettings& settings, Observer* pScoreObserver)
{

	GameObject* pObject = scene.CreateGameObject();

	auto* pObserver = new EnemyScoreObserver(EnemyScoreObserver::Type::Pooka);
	pObject->AddObserver(pObserver);
	pObserver->AddObserver(pScoreObserver);

	auto om = scene.GetSceneData().GetManager<ObserverManager>();
	if (om) om->AddUnownedObserver(pObserver);

	//Components
	AABBCollisionComponent* pCollider = new AABBCollisionComponent();
	pObject->AddComponent(pCollider);
	SpriteComponent* pSprite = new SpriteComponent();
	pObject->AddComponent(pSprite);
	pObject->AddComponent(new RenderComponent());
	pObject->AddComponent(new MovementComponent());
	FiniteStateMachineComponent* pFSM = new FiniteStateMachineComponent();
	pObject->AddComponent(pFSM);

	auto* em = scene.GetSceneData().GetManager<EnemyManager>();
	if (em)em->RegisterEnemy(pObject);
	pObject->AddComponent(new DigDugEnemyComponent());

	//Construct
	{
		//Collider
		pCollider->SetTrigger(true);
		pCollider->SetCollider(settings.spriteWidth, settings.spriteHeight, 0, 0);
		pCollider->SetTag("Enemy");
	}
	{
		//Sprite
		pSprite->SetSpriteSpeed(settings.spriteSpeed);
		pSprite->SetTexture(settings.pPookaTexture);

		Rect start = Rect{ 0, 0, settings.spriteWidth, settings.spriteHeight };
		pSprite->AddSprite(unsigned int(EnemySpriteID::IdleRight), new FixedSource(start));
		pSprite->AddSprite(unsigned int(EnemySpriteID::MoveRight), new TickSource(start, 2, true));
		start.x += 2 * start.width;
		pSprite->AddSprite(unsigned int(EnemySpriteID::CrushedRight), new FixedSource(start));
		start.x += start.width;
		pSprite->AddSprite(unsigned int(EnemySpriteID::Ghost), new TickSource(start, 2, true));
		start.x = 0;
		start.y += start.height;
		pSprite->AddSprite(unsigned int(EnemySpriteID::IdleLeft), new FixedSource(start));
		pSprite->AddSprite(unsigned int(EnemySpriteID::MoveLeft), new TickSource(start, 2, true));
		start.x += 2 * start.width;
		pSprite->AddSprite(unsigned int(EnemySpriteID::CrushedLeft), new FixedSource(start));
		start.x = 0;
		start.y += start.height;
		start.width = 2 * start.width;
		start.height = 2 * start.height;
		pSprite->AddSprite(unsigned int(EnemySpriteID::PumpedRightTier1), new FixedSource(start));
		start.x += start.width;
		pSprite->AddSprite(unsigned int(EnemySpriteID::PumpedRightTier2), new FixedSource(start));
		start.x += start.width;
		pSprite->AddSprite(unsigned int(EnemySpriteID::PumpedRightTier3), new FixedSource(start));
		start.x += start.width;
		pSprite->AddSprite(unsigned int(EnemySpriteID::PumpedRightTier4), new FixedSource(start));
		start.x = 0;
		start.y += start.height;
		pSprite->AddSprite(unsigned int(EnemySpriteID::PumpedLeftTier1), new FixedSource(start));
		start.x += start.width;
		pSprite->AddSprite(unsigned int(EnemySpriteID::PumpedLeftTier2), new FixedSource(start));
		start.x += start.width;
		pSprite->AddSprite(unsigned int(EnemySpriteID::PumpedLeftTier3), new FixedSource(start));
		start.x += start.width;
		pSprite->AddSprite(unsigned int(EnemySpriteID::PumpedLeftTier4), new FixedSource(start));
	}
	{

		FSMState* pDeadState = new FSMStateDestroyObject(pObject);
		FSMState* pDeadDelayState = new FSMStateDelay(1.5f, pDeadState);
		FSMState* pCrushedState = new FSMStateEnemyCrushed(settings.pGrid, pObject, 100.0f, pDeadDelayState);

		FSMStateEnemyPumping* pPumpedState = new FSMStateEnemyPumping(pObject, 2.0f, 0.5f, nullptr, pDeadDelayState, pCrushedState);
		FSMStateEnemyGhost* pGhostState = new FSMStateEnemyGhost(pObject, 60.0f, 5.0f, settings.pGrid, nullptr, pCrushedState, pPumpedState);
		FSMState* pMoveState = new FSMStatePookaMoving(60.0f, settings.pGrid, pObject, pPumpedState, pGhostState, pCrushedState, 10.0f);
		pPumpedState->SetMoveState(pMoveState);
		pGhostState->SetMoveState(pMoveState);

		pFSM->SaveState("DeadState",pDeadState);
		pFSM->SaveState("DeadDelayedState", pDeadDelayState);
		pFSM->SaveState("CrushedState",pCrushedState);
		pFSM->SaveState("PumpedState",pPumpedState);
		pFSM->SaveState("GhostState",pGhostState);
		pFSM->SaveState("MoveState",pMoveState);


		pFSM->SetState(pMoveState);


	}
	return pObject;
}

GameObject* DigDug::CreatePump(GameObject& parent, const DigDugSettings& settings)
{
	UNREFERENCED_PARAMETER(settings);
	GameObject* pPump = parent.CreateChild();
	float speed = 0.65f * settings.spriteSpeed;

	//Components
	auto* pCollider = new SpriteCollisionComponent();
	pPump->AddComponent(pCollider);
	auto* pSprite = new SpriteComponent();
	pPump->AddComponent(pSprite);
	auto* pFSM = new FiniteStateMachineComponent();
	pPump->AddComponent(pFSM);
	auto* pRender = new RenderComponent();
	pPump->AddComponent(pRender);

	//Make
	{
		pRender->SetRenderPriority(200, parent.GetScene()->GetSceneData());
	}
	{
		pCollider->SetTrigger(true);
		pCollider->SetTag("Pump");
		pCollider->SetOffset(0, 0, 0, 0);
	}
	{
		pSprite->SetTexture(settings.pPumpTexture);
		pSprite->SetSpriteSpeed(speed);
		pSprite->AddSprite(unsigned int(PumpSpriteID::None), new FixedSource(Rect{ 0,0,0,0 }));
		Rect src{ 0,0, settings.spriteWidth, settings.spriteHeight };
		pSprite->AddSprite(unsigned int(PumpSpriteID::Up), new SweepSource(Rect{ 0,0, src.width,0 }, Rect{ 0,0, src.width, src.height * 2 }, 2, false));
		pSprite->AddSprite(unsigned int(PumpSpriteID::Right), new SweepSource(Rect{ 3 * src.width, 0, 0, src.height }, Rect{ src.width, 0, 2 * src.width, src.height }, 2, false));
		pSprite->AddSprite(unsigned int(PumpSpriteID::Down), new SweepSource{ Rect{3 * src.width, 2 * src.height, src.width, 0}, Rect{3 * src.width, 0, src.width, 2 * src.height}, 2, false });
		pSprite->AddSprite(unsigned int(PumpSpriteID::Left), new SweepSource{ Rect{4 * src.width, 0, 0, src.height}, Rect{4 * src.width, 0, 2 * src.width, src.height}, 2, false });
	}
	{
		FSMStatePumpInactive* pInactiveState = new FSMStatePumpInactive(nullptr, pPump);
		FSMStatePumpActive* pActiveState = new FSMStatePumpActive(pPump, 0, 2 * speed, pInactiveState, nullptr);
		FSMState* pHitState = new FSMStatePumpPumping(pPump, pInactiveState);
		pInactiveState->SetActiveState(pActiveState);
		pActiveState->SetPumpState(pHitState);

		pFSM->SaveState("InactiveState", pInactiveState);
		pFSM->SaveState("ActiveState",pActiveState);
		pFSM->SaveState("HitState",pHitState);


		pFSM->SetState(pInactiveState);
	}

	return pPump;
}



GameObject* DigDug::CreatePlayer(Scene& scene, const DigDugSettings& settings, InputAction up, InputAction down,
	InputAction left, InputAction right, InputAction pump, int id)
{
	GameObject* pObject = scene.CreateGameObject();

	pObject->AddComponent(new DigDugPlayerComponent(id));
	auto* pFSM = new FiniteStateMachineComponent();
	pObject->AddComponent(pFSM);
	auto* pCollider = new AABBCollisionComponent();
	pObject->AddComponent(pCollider);
	pObject->AddComponent(new RenderComponent());
	auto pMove = new MovementComponent();
	pObject->AddComponent(pMove);
	auto pMovement = new DigDugMovementComponent(*settings.pGrid, 75.0f);
	pObject->AddComponent(pMovement);
	auto pSprite = new SpriteComponent();
	pObject->AddComponent(pSprite);

	//Construct
	//Set Values
	pCollider->SetTrigger(false);
	pCollider->SetTag("Player");
	pCollider->SetCollider(settings.spriteWidth, settings.spriteHeight, 0, 0);
	pSprite->SetSpriteSpeed(settings.spriteSpeed);
	pSprite->SetTexture(settings.pDigDugTexture);
	pMovement->SetEnabled(false);
	HelpPlayerSprites(*pSprite, settings);

	//Input
	auto pInput = scene.GetSceneData().GetInput();
	auto pLeftDown = std::make_shared<PlayerInput>(Direction::Left, id, &scene);
	auto pRightDown = std::make_shared<PlayerInput>(Direction::Right, id, &scene);
	auto pUpDown = std::make_shared<PlayerInput>(Direction::Up, id, &scene);
	auto pDownDown = std::make_shared<PlayerInput>(Direction::Down, id, &scene);
	auto pNone = std::make_shared<PlayerInput>(Direction::None, id, &scene);

	auto pPmpPressed = std::make_shared<PlayerNotifier>(id, GameEvent::InputPumpPressed, &scene);
	auto pPmpReleased = std::make_shared<PlayerNotifier>(id, GameEvent::InputPumpReleased, &scene);

	left.triggerState = InputTriggerState::Down;
	left.pCommand = pLeftDown;
	right.triggerState = InputTriggerState::Down;
	right.pCommand = pRightDown;
	up.triggerState = InputTriggerState::Down;
	up.pCommand = pUpDown;
	down.triggerState = InputTriggerState::Down;
	down.pCommand = pDownDown;

	InputAction leftUp{ left };
	InputAction rightUp{ right };
	InputAction upUp{ up };
	InputAction downUp{ down };
	leftUp.triggerState = InputTriggerState::Up;
	rightUp.triggerState = InputTriggerState::Up;
	upUp.triggerState = InputTriggerState::Up;
	downUp.triggerState = InputTriggerState::Up;
	leftUp.pCommand = pNone;
	rightUp.pCommand = pNone;
	upUp.pCommand = pNone;
	downUp.pCommand = pNone;

	pump.triggerState = InputTriggerState::Pressed;
	pump.pCommand = pPmpPressed;
	InputAction pumpUp = pump;
	pumpUp.triggerState = InputTriggerState::Released;
	pumpUp.pCommand = pPmpReleased;


	pInput->AddInputAction(left);
	pInput->AddInputAction(right);
	pInput->AddInputAction(up);
	pInput->AddInputAction(down);
	pInput->AddInputAction(pump);
	pInput->AddInputAction(pumpUp);
	pInput->AddInputAction(leftUp);
	pInput->AddInputAction(rightUp);
	pInput->AddInputAction(upUp);
	pInput->AddInputAction(downUp);


	//FSM
	FSMState* pIdleState = new FSMStatePlayerIdle("DeadState", "CrushedState", "ThrowState", "MoveState");
	FSMState* pMoveState = new FSMStatePlayerMoving("DeadState", "CrushedState", "ThrowState", "IdleState", settings.pGrid);
	FSMState* pCrushedState = new FSMStatePlayerCrushed("CrushDelay", settings.pGrid, 100.0f);
	FSMState* pDeadState = new FSMStatePlayerDead();
	FSMState* pThrowState = new FSMStatePlayerThrowing("DeadState", "CrushedState", "IdleState", "PumpState");
	FSMState* pPumpState = new FSMStatePlayerPumping("DeadState", "CrushedState", "IdleState", "MoveState");
	FSMState* pCrushDelay = new FSMStateDelay(1.0f, pDeadState);
	pFSM->SaveState("IdleState", pIdleState);
	pFSM->SaveState("MoveState", pMoveState);
	pFSM->SaveState("CrushedState", pCrushedState);
	pFSM->SaveState("DeadState", pDeadState);
	pFSM->SaveState("ThrowState", pThrowState);
	pFSM->SaveState("CrushDelay", pCrushDelay);
	pFSM->SaveState("PumpState", pPumpState);

	pFSM->SetState(pIdleState);

	CreatePump(*pObject, settings);
	return pObject;
}



void DigDug::HelpPlayerSprites(SpriteComponent& comp, const DigDugSettings& settings)
{
	Rect s(0, 0, settings.spriteWidth, settings.spriteHeight);

	//Move
	comp.AddSprite(unsigned int(PlayerSpriteID::MoveRight), new TickSource(s, 2, true));
	s.x += 2 * s.width;
	comp.AddSprite(unsigned int(PlayerSpriteID::MoveUp), new TickSource(s, 2, true));
	s.x += 2 * s.width;
	comp.AddSprite(unsigned int(PlayerSpriteID::MoveLeft), new TickSource(s, 2, true));
	s.x += 2 * s.width;
	comp.AddSprite(unsigned int(PlayerSpriteID::MoveDown), new TickSource(s, 2, true));

	//Dig
	s.x = 0;
	s.y += s.height;
	comp.AddSprite(unsigned int(PlayerSpriteID::DigRight), new TickSource(s, 2, true));
	s.x += 2 * s.width;
	comp.AddSprite(unsigned int(PlayerSpriteID::DigUp), new TickSource(s, 2, true));
	s.x += 2 * s.width;
	comp.AddSprite(unsigned int(PlayerSpriteID::DigLeft), new TickSource(s, 2, true));
	s.x += 2 * s.width;
	comp.AddSprite(unsigned int(PlayerSpriteID::DigDown), new TickSource(s, 2, true));

	//Throw
	s.x = 0;
	s.y += s.height;
	comp.AddSprite(unsigned int(PlayerSpriteID::ThrowRight), new FixedSource(s));
	s.x += s.width;
	comp.AddSprite(unsigned int(PlayerSpriteID::ThrowUp), new FixedSource(s));
	s.x += s.width;
	comp.AddSprite(unsigned int(PlayerSpriteID::ThrowLeft), new FixedSource(s));
	s.x += s.width;
	comp.AddSprite(unsigned int(PlayerSpriteID::ThrowDown), new FixedSource(s));

	//Pumping
	s.x = 0;
	s.y += s.height;
	comp.AddSprite(unsigned int(PlayerSpriteID::PumpRight), new TickSource(s, 2, true));
	s.x += 2 * s.width;
	comp.AddSprite(unsigned int(PlayerSpriteID::PumpUp), new TickSource(s, 2, true));
	s.x += 2 * s.width;
	comp.AddSprite(unsigned int(PlayerSpriteID::PumpLeft), new TickSource(s, 2, true));
	s.x += 2 * s.width;
	comp.AddSprite(unsigned int(PlayerSpriteID::PumpDown), new TickSource(s, 2, true));

	//Crushed
	s.x = 0;
	s.y += s.height;
	comp.AddSprite(unsigned int(PlayerSpriteID::CrushedRight), new TickSource(s, 2, false));
	s.x += 2 * s.width;
	comp.AddSprite(unsigned int(PlayerSpriteID::CrushedUp), new TickSource(s, 2, false));
	s.x += 2 * s.width;
	comp.AddSprite(unsigned int(PlayerSpriteID::CrushedLeft), new TickSource(s, 2, false));
	s.x += 2 * s.width;
	comp.AddSprite(unsigned int(PlayerSpriteID::CrushedDown), new TickSource(s, 2, false));

	//Dead
	s.x = 0;
	s.y += s.height;
	comp.AddSprite(unsigned int(PlayerSpriteID::DiedRight), new TickSource(s, 4, false));
	s.x += 4 * s.width;
	comp.AddSprite(unsigned int(PlayerSpriteID::DiedUp), new TickSource(s, 4, false));
	s.x = 0;
	s.y += s.height;
	comp.AddSprite(unsigned int(PlayerSpriteID::DiedLeft), new TickSource(s, 4, false));
	s.x += 4 * s.width;
	comp.AddSprite(unsigned int(PlayerSpriteID::DiedDown), new TickSource(s, 4, false));


	//Idle
	s.x = s.width;
	s.y = 0;
	comp.AddSprite(unsigned int(PlayerSpriteID::IdleRight), new FixedSource(s));
	s.x += 2 * s.width;
	comp.AddSprite(unsigned int(PlayerSpriteID::IdleUp), new FixedSource(s));
	s.x += 2 * s.width;
	comp.AddSprite(unsigned int(PlayerSpriteID::IdleLeft), new FixedSource(s));
	s.x += 2 * s.width;
	comp.AddSprite(unsigned int(PlayerSpriteID::IdleDown), new FixedSource(s));


}







GameObject* DigDug::CreateFygar(Scene& scene, const DigDugSettings& settings, Observer* pScoreObserver)
{
	auto pObject = scene.CreateGameObject();

	auto* pObserver = new EnemyScoreObserver(EnemyScoreObserver::Type::Fygar);
	pObject->AddObserver(pObserver);
	pObserver->AddObserver(pScoreObserver);

	auto om = scene.GetSceneData().GetManager<ObserverManager>();
	if (om) om->AddUnownedObserver(pObserver);

	//Components
	auto pSprite = new SpriteComponent();
	auto pCol = new AABBCollisionComponent();
	auto pFsm = new FiniteStateMachineComponent();
	pObject->AddComponent(pSprite);
	pObject->AddComponent(pCol);
	pObject->AddComponent(pFsm);
	pObject->AddComponent(new RenderComponent());
	pObject->AddComponent(new MovementComponent());

	auto* em = scene.GetSceneData().GetManager<EnemyManager>();
	if (em)em->RegisterEnemy(pObject);
	pObject->AddComponent(new DigDugEnemyComponent());

	//Constr
	{
		pCol->SetTrigger(true);
		pCol->SetTag("Enemy");
		pCol->SetCollider(settings.spriteWidth, settings.spriteHeight, 0, 0);
	}
	{
		//Sprite
		pSprite->SetSpriteSpeed(settings.spriteSpeed);
		pSprite->SetTexture(settings.pFygarTexture);

		Rect start = Rect{ 0, 0, settings.spriteWidth, settings.spriteHeight };
		pSprite->AddSprite(unsigned int(EnemySpriteID::IdleRight), new FixedSource(start));
		pSprite->AddSprite(unsigned int(EnemySpriteID::MoveRight), new TickSource(start, 2, true));
		start.x += 2 * start.width;
		pSprite->AddSprite(unsigned int(EnemySpriteID::CrushedRight), new FixedSource(start));
		start.x += start.width;
		pSprite->AddSprite(unsigned int(EnemySpriteID::FireRight), new TickSource(start, 2, true));
		start.x += 3 * start.width;
		pSprite->AddSprite(unsigned int(EnemySpriteID::Ghost), new TickSource(start, 2, true));
		start.x = 0;
		start.y += start.height;
		pSprite->AddSprite(unsigned int(EnemySpriteID::IdleLeft), new FixedSource(start));
		pSprite->AddSprite(unsigned int(EnemySpriteID::MoveLeft), new TickSource(start, 2, true));
		start.x += 2 * start.width;
		pSprite->AddSprite(unsigned int(EnemySpriteID::CrushedLeft), new FixedSource(start));
		start.x += start.width;
		pSprite->AddSprite(unsigned int(EnemySpriteID::FireLeft), new TickSource(start, 2, true));
		start.x = 0;
		start.y += 3*start.height;
		start.width = 2 * start.width;
		start.height = 2 * start.height;
		pSprite->AddSprite(unsigned int(EnemySpriteID::PumpedRightTier1), new FixedSource(start));
		start.x += start.width;
		pSprite->AddSprite(unsigned int(EnemySpriteID::PumpedRightTier2), new FixedSource(start));
		start.x += start.width;
		pSprite->AddSprite(unsigned int(EnemySpriteID::PumpedRightTier3), new FixedSource(start));
		start.x += start.width;
		pSprite->AddSprite(unsigned int(EnemySpriteID::PumpedRightTier4), new FixedSource(start));
		start.x = 0;
		start.y += start.height;
		pSprite->AddSprite(unsigned int(EnemySpriteID::PumpedLeftTier1), new FixedSource(start));
		start.x += start.width;
		pSprite->AddSprite(unsigned int(EnemySpriteID::PumpedLeftTier2), new FixedSource(start));
		start.x += start.width;
		pSprite->AddSprite(unsigned int(EnemySpriteID::PumpedLeftTier3), new FixedSource(start));
		start.x += start.width;
		pSprite->AddSprite(unsigned int(EnemySpriteID::PumpedLeftTier4), new FixedSource(start));
	}
	{
		//FSM
		FSMState* pDeadState = new FSMStateDestroyObject(pObject);
		FSMState* pDeadDelayState = new FSMStateDelay(1.5f, pDeadState);
		FSMState* pCrushedState = new FSMStateEnemyCrushed(settings.pGrid, pObject, 100.0f, pDeadDelayState);

		FSMState* pMoveState = new FSMStateFygarMoving(60.0f, 3.0f, 10.0f, settings.pGrid, "PumpedState", "GhostState", "CrushedState", "FireState");
		FSMState* pFireState = new FSMStateFygarFire(3 * settings.spriteSpeed, "PumpedState", "CrushedState", "MoveState");
		FSMStateEnemyPumping* pPumpedState = new FSMStateEnemyPumping(pObject, 2.0f, 0.5f, pMoveState, pDeadDelayState, pCrushedState);
		FSMStateEnemyGhost* pGhostState = new FSMStateEnemyGhost(pObject, 60.0f, 5.0f, settings.pGrid, pMoveState, pCrushedState, pPumpedState);

		pFsm->SaveState("DeadState", pDeadState);
		pFsm->SaveState("DeadDelayedState", pDeadDelayState);
		pFsm->SaveState("CrushedState", pCrushedState);
		pFsm->SaveState("PumpedState", pPumpedState);
		pFsm->SaveState("GhostState", pGhostState);
		pFsm->SaveState("MoveState", pMoveState);
		pFsm->SaveState("FireState", pFireState);

		pFsm->SetState(pMoveState);


	}
	CreateFire(*pObject, settings);

	return pObject;
}

GameObject* DigDug::CreateFire(GameObject& parent, const DigDugSettings& settings)
{
	auto pObject = parent.CreateChild();

	//Components
	auto pCol = new SpriteCollisionComponent();
	auto pSprite = new SpriteComponent();
	auto pFsm = new FiniteStateMachineComponent();
	pObject->AddComponent(new RenderComponent());
	pObject->AddComponent(pCol);
	pObject->AddComponent(pSprite);
	pObject->AddComponent(pFsm);

	//Constr
	{
		//Collider
		pCol->SetTrigger(true);
		pCol->SetTag("Fire");
	}
	{
		//Sprite
		pSprite->SetTexture(settings.pFygarTexture);
		pSprite->SetSpriteSpeed(settings.spriteSpeed);
		
		Rect s{ 0, 2 * settings.spriteHeight, settings.spriteWidth, settings.spriteHeight };
		pSprite->AddSprite(unsigned int(FireSpriteID::None), new FixedSource(Rect{}));
		GrowSource* pS = new GrowSource(false);
		pS->sources.push_back(s);
		pS->sources.push_back(Rect{ s.x + s.width, s.y, s.width * 2, s.height });
		pS->sources.push_back(Rect{ s.x + s.width * 3, s.y, s.width * 3, s.height });
		pSprite->AddSprite(unsigned int(FireSpriteID::Right), pS);
		pS = new GrowSource(false);
		s.y += s.height;
		pS->sources.push_back(s);
		pS->sources.push_back(Rect{ s.x + s.width, s.y, s.width * 2, s.height });
		pS->sources.push_back(Rect{ s.x + s.width * 3, s.y, s.width * 3, s.height });
		pSprite->AddSprite(unsigned int(FireSpriteID::Left), pS);
	}
	{
		//FSM
		FSMState* pNoneState = new FSMStateFireIdle("ActiveState");
		FSMState* pActiveState = new FSMStateFireActive("InactiveState", 3 * settings.spriteSpeed);
		pFsm->SaveState("ActiveState", pActiveState);
		pFsm->SaveState("InactiveState", pNoneState);
		pFsm->SetState(pNoneState);
	}

	return pObject;
}

GameObject* DigDug::CreateScore(Scene& scene, const std::shared_ptr<Font>& pFont, const SDL_Color& color, int score)
{
	auto* pObject = new GameObject();

	//Comps
	auto* pRenderComponent = new RenderComponent();
	pRenderComponent->SetRenderPriority(10, scene.GetSceneData());
	pObject->AddComponent(pRenderComponent);
	auto* pTextComponent = new TextComponent();
	pObject->AddComponent(pTextComponent);
	pObject->AddComponent(new AutokillComponent(2.0f));

	//Make
	pTextComponent->SetFont(pFont);
	pTextComponent->SetColor(color);
	pTextComponent->SetText(std::to_string(score));

	//Result
	scene.AddGameObject(pObject);
	return pObject;
}

GameObject* DigDug::CreateScoreManager(Scene& scene, const std::shared_ptr<Font>& pFont, const Color4& color,
	BaseComponent* pScoreObserver)
{
	auto pObject = scene.CreateGameObject();

	//Comp
	auto pRender = new RenderComponent();
	auto pText = new TextComponent();
	pObject->AddComponent(pScoreObserver);
	pObject->AddComponent(pRender);
	pObject->AddComponent(pText);

	pRender->SetPivot(Vector2(1, 1));
	pRender->SetRenderPriority(10, scene.GetSceneData());

	pText->SetColor(color.SDLColor());
	pText->SetFont(pFont);
	pText->SetText("0");

	return pObject;
}





