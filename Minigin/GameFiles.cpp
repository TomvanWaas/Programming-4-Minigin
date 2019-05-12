#include "MiniginPCH.h"
#include "GameFiles.h"
#include "Enumerations.h"
#include "GameObject.h"
#include "Scene.h"
#include "Components.h"
#include "InputManager.h"
#include "FSMCondition.h"
#include "FSMEvent.h"
#include "FSMState.h"
#include "DigDugGridComponent.h"
#include "GameFSMClasses.h"
#include "GameInputCommands.h"
#include "SpriteCollisionComponent.h"
#include "FSMConditions.h"
#include "FSMEvents.h"
#include "DigDugMovementComponent.h"
#include "DigDugPlayer.h"
#include "DigDugEnemy.h"
#include "DigDugEnemyHealth.h"
#include "ObstacleParts.h"
#include "GameFSMStates.h"
#include "EnemyManager.h"
#include "EnemyParts.h"
#include "PumpParts.h"

using namespace DigDug;
typedef std::shared_ptr<FSMCondition> Condition;
typedef std::shared_ptr<FSMEvent> Event;
typedef std::shared_ptr<FSMTransition> Transition;
typedef std::vector<std::shared_ptr<FSMCondition>> Conditions;
typedef std::vector<std::shared_ptr<FSMEvent>> Events;






GameObject* DigDug::MakePlayerObject(Scene& scene, const DigDugSettings& settings, char up, char down, char left,
	char right, char pump)
{
	auto* pPlayer = scene.CreateGameObject();

	//Components
	pPlayer->AddComponent(new RenderComponent());
	auto pMove = new MovementComponent();
	pPlayer->AddComponent(pMove);
	auto pCollider = new AABBCollisionComponent();
	pPlayer->AddComponent(pCollider);
	auto pFSM = new FiniteStateMachineComponent();
	pPlayer->AddComponent(pFSM);
	auto pMovement = new DigDugMovementComponent(*settings.pGrid, 100.0f);
	pPlayer->AddComponent(pMovement);
	auto pSprite = new SpriteComponent();
	pPlayer->AddComponent(pSprite);
	auto pData = new DataComponent();
	pPlayer->AddComponent(pData);
	pPlayer->AddComponent(new DigDugPlayer());

	//Set Values
	pCollider->SetTrigger(false);
	pCollider->SetTag("Player");
	pCollider->SetCollider(settings.spriteWidth, settings.spriteHeight, 0, 0);
	pSprite->SetSpriteSpeed(settings.spriteSpeed);
	pSprite->SetTexture(settings.spriteTexture);
	pData->AddData("Pump", PumpStatus::Deactivated);
	pData->AddData("Health", PlayerHealth::Alive);
	pData->AddData("Status", PlayerState::Idle);

	//Make
	MakePlayerSprites(pSprite, settings);
	MakePlayerInput(pPlayer, settings, up, down, left, right, pump);
	MakePlayerFSM(pPlayer, settings);
	//MakePlayerPump(scene, *pPlayer, settings);
	CreatePump(*pPlayer, settings);


	return pPlayer;
}

void DigDug::MakePlayerSprites(SpriteComponent* pSpriteComp, const DigDugSettings& settings)
{
	if (pSpriteComp == nullptr) return;

	Rect s(0, settings.spriteHeight, settings.spriteWidth, settings.spriteHeight);

	//Moving
	pSpriteComp->AddSprite(unsigned int(PlayerSprite::MoveRight), new TickSource(s, 2, true));
	s.x += 2 * settings.spriteWidth;
	pSpriteComp->AddSprite(unsigned int(PlayerSprite::MoveUp), new TickSource(s, 2, true));
	s.x += 2 * settings.spriteWidth;
	pSpriteComp->AddSprite(unsigned int(PlayerSprite::MoveLeft), new TickSource(s, 2, true));
	s.x += 2 * settings.spriteWidth;
	pSpriteComp->AddSprite(unsigned int(PlayerSprite::MoveDown), new TickSource(s, 2, true));

	//Throwing
	s.x = 0.0f;
	s.y += settings.spriteHeight;
	pSpriteComp->AddSprite(unsigned int(PlayerSprite::ThrowRight), new TickSource(s, 1, false));
	s.x += settings.spriteWidth;
	pSpriteComp->AddSprite(unsigned int(PlayerSprite::ThrowUp), new TickSource(s, 1, false));
	s.x += settings.spriteWidth;
	pSpriteComp->AddSprite(unsigned int(PlayerSprite::ThrowLeft), new TickSource(s, 1, false));
	s.x += settings.spriteWidth;
	pSpriteComp->AddSprite(unsigned int(PlayerSprite::ThrowDown), new TickSource(s, 1, false));

	//Pumping
	s.x = 0.0f;
	s.y += settings.spriteHeight;
	pSpriteComp->AddSprite(unsigned int(PlayerSprite::PumpRight), new TickSource(s, 2, true));
	s.x += 2 * settings.spriteWidth;
	pSpriteComp->AddSprite(unsigned int(PlayerSprite::PumpUp), new TickSource(s, 2, true));
	s.x += 2 * settings.spriteWidth;
	pSpriteComp->AddSprite(unsigned int(PlayerSprite::PumpLeft), new TickSource(s, 2, true));
	s.x += 2 * settings.spriteWidth;
	pSpriteComp->AddSprite(unsigned int(PlayerSprite::PumpDown), new TickSource(s, 2, true));

	//Being Crushed
	s.x = 0.0f;
	s.y += settings.spriteHeight;
	pSpriteComp->AddSprite(unsigned int(PlayerSprite::CrushedRight), new TickSource(s, 2, false));
	s.x += 2 * settings.spriteWidth;
	pSpriteComp->AddSprite(unsigned int(PlayerSprite::CrushedUp), new TickSource(s, 2, false));
	s.x += 2 * settings.spriteWidth;
	pSpriteComp->AddSprite(unsigned int(PlayerSprite::CrushedLeft), new TickSource(s, 2, false));
	s.x += 2 * settings.spriteWidth;
	pSpriteComp->AddSprite(unsigned int(PlayerSprite::CrushedDown), new TickSource(s, 2, false));

	//Being Dead
	s.x = 0.0f;
	s.y += settings.spriteHeight;
	pSpriteComp->AddSprite(unsigned int(PlayerSprite::DiedRight), new TickSource(s, 4, false));
	s.x += 4 * settings.spriteWidth;
	pSpriteComp->AddSprite(unsigned int(PlayerSprite::DiedUp), new TickSource(s, 4, false));
	s.x = 0.0f;
	s.y += settings.spriteHeight;
	pSpriteComp->AddSprite(unsigned int(PlayerSprite::DiedLeft), new TickSource(s, 4, false));
	s.x += 4 * settings.spriteWidth;
	pSpriteComp->AddSprite(unsigned int(PlayerSprite::DiedDown), new TickSource(s, 4, false));

	//Being Idle
	s.x = 0.0f;
	s.y = settings.spriteHeight;
	pSpriteComp->AddSprite(unsigned int(PlayerSprite::IdleRight), new TickSource(s, 1, false));
	s.x += 2 * settings.spriteWidth;
	pSpriteComp->AddSprite(unsigned int(PlayerSprite::IdleUp), new TickSource(s, 1, false));
	s.x += 2 * settings.spriteWidth;
	pSpriteComp->AddSprite(unsigned int(PlayerSprite::IdleLeft), new TickSource(s, 1, false));
	s.x += 2 * settings.spriteWidth;
	pSpriteComp->AddSprite(unsigned int(PlayerSprite::IdleDown), new TickSource(s, 1, false));

}

void DigDug::MakePlayerInput(GameObject* pPlayer, const DigDugSettings& settings, char up, char down, char left, char right, char pump)
{
	UNREFERENCED_PARAMETER(settings);
	auto* pMove = pPlayer->GetComponent<DigDugMovementComponent>();
	auto* pInput = pPlayer->GetScene()->GetSceneData().GetInput();
	auto* pData = pPlayer->GetComponent<DataComponent>();
	PumpStatus* pPump = nullptr;
	pData->GetData("Pump", pPump);

	auto pLeftDown = std::make_shared<PlayerInput>(Direction::Left, pMove);
	auto pRightDown = std::make_shared<PlayerInput>(Direction::Right, pMove);
	auto pUpDown = std::make_shared<PlayerInput>(Direction::Up, pMove);
	auto pDownDown = std::make_shared<PlayerInput>(Direction::Down, pMove);
	//auto pPumpPressed = std::make_shared<InputSetData<PumpStatus>>(*pPump, PumpStatus::Activated);
	//auto pPumpReleased = std::make_shared<InputSetData<PumpStatus>>(*pPump, PumpStatus::Deactivated);
	auto pNone = std::make_shared<PlayerInput>(Direction::None, pMove);

	auto pPmpPressed = std::make_shared<InputNotifier>(pPlayer, ObservedEvent::InputPumpPressed);
	auto pPmpReleased = std::make_shared<InputNotifier>(pPlayer, ObservedEvent::InputPumpReleased);

	InputAction leftA{ InputTriggerState::Down, pLeftDown, -1, left, -1 };
	InputAction rightA{ InputTriggerState::Down, pRightDown, -1, right, -1 };
	InputAction upA{ InputTriggerState::Down, pUpDown, -1, up, -1 };
	InputAction downA{ InputTriggerState::Down, pDownDown, -1, down, -1 };
	InputAction pumpPressedA{ InputTriggerState::Pressed, pPmpPressed, -1, pump, -1 };
	InputAction pumpReleasedA{ InputTriggerState::Released, pPmpReleased, -1, pump, -1 };

	InputAction leftB{ InputTriggerState::Up, pNone, -1, left, -1 };
	InputAction rightB{ InputTriggerState::Up, pNone, -1, left, -1 };
	InputAction upB{ InputTriggerState::Up, pNone, -1, left, -1 };
	InputAction downB{ InputTriggerState::Up, pNone, -1, left, -1 };

	pInput->AddInputAction(leftA);
	pInput->AddInputAction(rightA);
	pInput->AddInputAction(upA);
	pInput->AddInputAction(downA);
	pInput->AddInputAction(pumpPressedA);
	pInput->AddInputAction(pumpReleasedA);
	pInput->AddInputAction(leftB);
	pInput->AddInputAction(rightB);
	pInput->AddInputAction(upB);
	pInput->AddInputAction(downB);

}

void DigDug::MakePlayerFSM(GameObject* pPlayer, const DigDugSettings& settings)
{
	UNREFERENCED_PARAMETER(settings);
	auto* pFSM = pPlayer->GetComponent<FiniteStateMachineComponent>();
	auto* pData = pPlayer->GetComponent<DataComponent>();
	auto* pMove = pPlayer->GetComponent<DigDugMovementComponent>();
	auto* pSprite = pPlayer->GetComponent<SpriteComponent>();
	PlayerHealth* pHealth = nullptr;
	PlayerState* pState = nullptr;
	PumpStatus* pPump = nullptr;
	Direction* pLookDirection = &pMove->GetLookDirection();
	Direction* pMoveDirection = &pMove->GetMoveDirection();
	pData->GetData("Health", pHealth);
	pData->GetData("Status", pState);
	pData->GetData("Pump", pPump);

	//States
	FSMStateDefault* pIdleState = new FSMStateDefault();
	FSMStateDefault* pMovingState = new FSMStateDefault();
	FSMStateDefault* pThrowingState = new FSMStateDefault();
	FSMStateDefault* pPumpingState = new FSMStateDefault();
	FSMStateDefault* pKilledState = new FSMStateDefault();
	FSMStateDefault* pCrushedStartState = new FSMStateDefault();
	FSMStateDefault* pCrushedEndState = new FSMStateDefault();
	pFSM->SaveState(pIdleState);
	pFSM->SaveState(pMovingState);
	pFSM->SaveState(pThrowingState);
	pFSM->SaveState(pPumpingState);
	pFSM->SaveState(pKilledState);
	pFSM->SaveState(pCrushedStartState);
	pFSM->SaveState(pCrushedEndState);

	//Events
	Event pUnlockMovementEvent = std::make_shared<FSMComponentEnableEvent>(pMove, FSMComponentEnableEvent::Operation::SetTrue);
	Event pLockMovementEvent = std::make_shared<FSMComponentEnableEvent>(pMove, FSMComponentEnableEvent::Operation::SetFalse);
	Event pSpriteEvent = std::make_shared<FSMEventPlayerSprite>(*pLookDirection, pSprite, *pState);
	Event pMarkEvent = std::make_shared<FSMGridMarkerEvent>(*pPlayer, *settings.pGrid);

	Event pStateSetIdleEvent = std::make_shared<FSMSetDataEvent<PlayerState>>(*pState, PlayerState::Idle);
	Event pStateSetMovingEvent = std::make_shared<FSMSetDataEvent<PlayerState>>(*pState, PlayerState::Moving);
	Event pStateSetThrownEvent = std::make_shared<FSMSetDataEvent<PlayerState>>(*pState, PlayerState::Thrown);
	Event pStateSetKilledEvent = std::make_shared<FSMSetDataEvent<PlayerState>>(*pState, PlayerState::Killed);
	Event pStateSetCrushedEvent = std::make_shared<FSMSetDataEvent<PlayerState>>(*pState, PlayerState::Crushed);
	Event pStateSetPumpingEvent = std::make_shared<FSMSetDataEvent<PlayerState>>(*pState, PlayerState::Pumping);
	Event pMoveDownOnCrushedEvent = std::make_shared<FSMMoveEvent>(Vector2(0, 100), *pPlayer);

	//Conditions
	Condition pPumpStateActivatedCondition = std::make_shared<FSMDataCondition<PumpStatus>>(*pPump, PumpStatus::Activated);
	Condition pPumpStateHitCondition = std::make_shared<FSMDataCondition<PumpStatus>>(*pPump, PumpStatus::Hit);
	Condition pPumpStateMissedCondition = std::make_shared<FSMDataCondition<PumpStatus>>(*pPump, PumpStatus::Missed);
	Condition pPumpStateDeactivatedCondition = std::make_shared<FSMDataCondition<PumpStatus>>(*pPump, PumpStatus::Deactivated);
	Condition pIsDeadCondition = std::make_shared<FSMDataCondition<PlayerHealth>>(*pHealth, PlayerHealth::Killed);
	Condition pIsCrushedCondition = std::make_shared<FSMDataCondition<PlayerHealth>>(*pHealth, PlayerHealth::Crushed);
	Condition pIdleCondition = std::make_shared<FSMDataCondition<Direction>>(*pMoveDirection, Direction::None);
	Condition pMoveCondition = std::make_shared<FSMNotCondition>(pIdleCondition);
	Condition pBelowNotMarkedCondition = std::make_shared<FSMGridMarkCondition>(*settings.pGrid, *pPlayer, Vector2{ 0, settings.gridOffset.y*0.25f }, false);
	Condition pTimerCondition = std::make_shared<FSMTimeCondition>(1.5f);
	Condition pOnPointCondition = std::make_shared<FSMGridPositionPointCondition>(*settings.pGrid, pPlayer->GetTransform());

	//Transitions
	Transition pToDead = std::make_shared<FSMTransition>(pKilledState, pIsDeadCondition);
	Transition pToCrushedStart = std::make_shared<FSMTransition>(pCrushedStartState, pIsCrushedCondition);
	Transition pToIdle = std::make_shared<FSMTransition>(pIdleState, pIdleCondition);
	Transition pToMove = std::make_shared<FSMTransition>(pMovingState, pMoveCondition);
	Transition pToThrow = std::make_shared<FSMTransition>(pThrowingState, pPumpStateActivatedCondition);
	Transition pToPump = std::make_shared<FSMTransition>(pPumpingState, pPumpStateHitCondition);
	Transition pFromPump = std::make_shared<FSMTransition>(pIdleState, pPumpStateDeactivatedCondition);
	Transition pFromThrow = std::make_shared<FSMTransition>(pIdleState, pPumpStateMissedCondition);
	Transition pToCrushedEnd = std::make_shared<FSMTransition>(pCrushedEndState, std::make_shared<FSMMultiAndCondition>(Conditions{ pBelowNotMarkedCondition, pOnPointCondition }));
	Transition pToCrushedDead = std::make_shared<FSMTransition>(pKilledState, pTimerCondition);

	//Make
	pIdleState->SetEnterEvent(std::make_shared<FSMMultiEvent>(Events{ pUnlockMovementEvent,pStateSetIdleEvent, pSpriteEvent }));
	pIdleState->AddTransition(pToThrow);
	pIdleState->AddTransition(pToMove);
	pIdleState->AddTransition(pToDead);
	pIdleState->AddTransition(pToCrushedStart);

	pMovingState->SetEnterEvent(std::make_shared<FSMMultiEvent>(Events{ pUnlockMovementEvent, pStateSetMovingEvent }));
	pMovingState->AddTransition(pToIdle);
	pMovingState->AddTransition(pToDead);
	pMovingState->AddTransition(pToCrushedStart);
	pMovingState->AddTransition(pToThrow);
	pMovingState->SetUpdateFirstEvent(std::make_shared<FSMMultiEvent>(Events{ pSpriteEvent, pMarkEvent }));

	pThrowingState->SetEnterEvent(std::make_shared<FSMMultiEvent>(Events{ pLockMovementEvent, pStateSetThrownEvent ,pSpriteEvent }));
	pThrowingState->AddTransition(pFromThrow);
	pThrowingState->AddTransition(pFromPump);
	pThrowingState->AddTransition(pToPump);

	pPumpingState->SetEnterEvent(std::make_shared<FSMMultiEvent>(Events{ pStateSetPumpingEvent,pSpriteEvent }));
	pPumpingState->AddTransition(pFromPump);
	pPumpingState->AddTransition(pFromThrow);
	pPumpingState->AddTransition(pToDead);
	pPumpingState->AddTransition(pToCrushedStart);

	pCrushedStartState->SetEnterEvent(std::make_shared<FSMMultiEvent>(Events{ pLockMovementEvent, pStateSetIdleEvent,pSpriteEvent }));
	pCrushedStartState->AddTransition(pToCrushedEnd);
	pCrushedStartState->SetUpdateFirstEvent(pMoveDownOnCrushedEvent);

	pCrushedEndState->SetEnterEvent(std::make_shared<FSMMultiEvent>(Events{ pStateSetCrushedEvent,pSpriteEvent }));
	pCrushedEndState->AddTransition(pToCrushedDead);

	pKilledState->SetEnterEvent(std::make_shared<FSMMultiEvent>(Events{ pLockMovementEvent , pStateSetKilledEvent,pSpriteEvent }));




	
	pFSM->SetState(pIdleState);
}

GameObject* DigDug::MakePlayerPump(Scene& scene, GameObject& parent, const DigDugSettings& settings)
{
	UNREFERENCED_PARAMETER(scene);
	UNREFERENCED_PARAMETER(parent);
	UNREFERENCED_PARAMETER(settings);

	auto pObject = parent.CreateChild();
	auto* pData = parent.GetComponent<DataComponent>();
	auto* pPlayerMove = parent.GetComponent<DigDugMovementComponent>();
	//PlayerHealth* pHealth = nullptr;
	PlayerState* pState = nullptr;
	PumpStatus* pPump = nullptr;
	Direction* pDirection = &pPlayerMove->GetLookDirection();
	//pData->GetData("Health", pHealth);
	pData->GetData("Status", pState);
	pData->GetData("Pump", pPump);


	//Components
	pObject->AddComponent(new RenderComponent());
	auto pCollider = new SpriteCollisionComponent();
	pObject->AddComponent(pCollider);
	auto pFSM = new FiniteStateMachineComponent();
	pObject->AddComponent(pFSM);
	auto pSprite = new SpriteComponent();
	pObject->AddComponent(pSprite);
	auto pMove = new MovementComponent();
	pObject->AddComponent(pMove);

	//Set Values
	{
		//Collider
		pCollider->SetTrigger(true);
		pCollider->SetOffset(0, 0, 0, 0);
		pCollider->SetTag("Pump");
		//pCollider->SetEnterCommand(std::make_shared<PumpColliderCommand>(true, *pPump));
		//pCollider->SetExitCommand(std::make_shared<PumpColliderCommand>(false, *pPump));
	}
	{
		//Sprite
		pSprite->SetSpriteSpeed(0.05f);
		pSprite->SetTexture(settings.spriteTexture);
		Rect start{ 0, 7 * settings.spriteHeight, settings.spriteWidth,settings.spriteHeight };
		pSprite->AddSprite(unsigned int(Direction::Up), new SweepSource(Rect(start.x, start.y, start.width, 0), Rect(start.x, start.y, start.width, 2 * start.height), 2, false));
		pSprite->AddSprite(unsigned int(Direction::Right), new SweepSource(Rect(start.x + 3 * start.width, start.y, 0, start.height), Rect(start.x + start.width, start.y, 2 * start.width, start.height), 2, false));
		pSprite->AddSprite(unsigned int(Direction::Down), new SweepSource(Rect(start.x + 3 * start.width, start.y + 2 * start.height, start.width, 0), Rect(start.x + 3 * start.width, start.y, start.width, 2 * start.height), 2, false));
		pSprite->AddSprite(unsigned int(Direction::Left), new SweepSource(Rect(start.x + 4 * start.width, start.y, 0, start.height), Rect(start.x + 4 * start.width, start.y, 2 * start.width, start.height), 2, false));
		pSprite->AddSprite(unsigned int(Direction::None), new FixedSource(Rect(0, 0, -1, -1)));
	}
	{
		//FSM

		//States
		FSMStateDefault* pNoneState = new FSMStateDefault();
		FSMStateDefault* pThrownState = new FSMStateDefault();
		FSMStateDefault* pMissedState = new FSMStateDefault();
		FSMStateDefault* pHitState = new FSMStateDefault();
		pFSM->SaveState(pNoneState);
		pFSM->SaveState(pThrownState);
		pFSM->SaveState(pMissedState);
		pFSM->SaveState(pHitState);

		//Events
		Event pSpriteEvent = std::make_shared<FSMEventPumpSprite>(*pDirection, pSprite);
		Event pSpriteRemoveEvent = std::make_shared<FSMSpriteSourceEvent>(unsigned int(Direction::None), pSprite);
		Event pMoveEvent = std::make_shared<FSMEventGridMove>(*pDirection, pMove, 300.0f);
		Event pResetLocalEvent = std::make_shared<FSMSetLocalPosition>(Vector2{}, pObject->GetTransform());
		Event pLockSpriteEvent = std::make_shared<FSMComponentEnableEvent>(pSprite, FSMComponentEnableEvent::Operation::SetFalse);
		Event pUnlockSpriteEvent = std::make_shared<FSMComponentEnableEvent>(pSprite, FSMComponentEnableEvent::Operation::SetTrue);
		Event pSetHitStatusEvent = std::make_shared<FSMSetDataEvent<PumpStatus>>(*pPump, PumpStatus::Hit);
		Event pSetMissStatusEvent = std::make_shared<FSMSetDataEvent<PumpStatus>>(*pPump, PumpStatus::Missed);


		//Conditions
		Condition pInputPressedCondition = std::make_shared<FSMDataCondition<PumpStatus>>(*pPump, PumpStatus::Activated);
		Condition pTimerCondition = std::make_shared<FSMTimeCondition>(0.2f);
		Condition pInputReleasedConition = std::make_shared<FSMDataCondition<PumpStatus>>(*pPump, PumpStatus::Deactivated);

		//Transitions
		Transition pActivateTransition = std::make_shared<FSMTransition>(pThrownState, pInputPressedCondition);
		//Transition pHitTransition = std::make_shared<FSMTransition>(pHitState, //TODO: Hashit);
		Transition pMissedTransition = std::make_shared<FSMTransition>(pMissedState, pTimerCondition);
		Transition pDeactivateTransition = std::make_shared<FSMTransition>(pNoneState, pInputReleasedConition);





		//Make
		pNoneState->AddTransition(pActivateTransition);
		pNoneState->SetEnterEvent(std::make_shared<FSMMultiEvent>(Events{ pSpriteRemoveEvent,pResetLocalEvent }));

		pThrownState->SetEnterEvent(pSpriteEvent);
		//pThrownState->AddTransition(pHitTransition);	//Todo
		pThrownState->AddTransition(pMissedTransition);
		pThrownState->AddTransition(pDeactivateTransition);
		pThrownState->SetUpdateFirstEvent(pMoveEvent);

		pMissedState->AddTransition(pDeactivateTransition);
		pMissedState->SetEnterEvent(std::make_shared<FSMMultiEvent>(Events{ pSpriteRemoveEvent,pResetLocalEvent, pSetMissStatusEvent }));

		pHitState->AddTransition(pDeactivateTransition);
		pHitState->SetEnterEvent(std::make_shared<FSMMultiEvent>(Events{ pLockSpriteEvent ,pSetHitStatusEvent}));
		pHitState->SetExitEvent(pUnlockSpriteEvent);


		//Set
		pFSM->SetState(pNoneState);
	}
	return pObject;
}














//Background
GameObject* DigDug::MakeBackground(Scene& scene, const DigDugSettings& settings)
{
	auto pBackground = scene.CreateGameObject();

	//Texture
	auto pTextureObject = pBackground->CreateChild();
	{
		auto pRenderComp = new RenderComponent(settings.backgroundTexture);
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

			auto pGridRender = new MultiRenderComponent(settings.cavesTexture);
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









GameObject* DigDug::CreateObstacle(Scene& scene, const DigDugSettings& settings)
{
	GameObject* pObject = scene.CreateGameObject();

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
	pSpriteC->SetTexture("Sprites_Other.png");
	pSpriteC->SetSpriteSpeed(settings.spriteSpeed);
	Rect src{ 0,0, settings.spriteWidth, settings.spriteHeight };
	pSpriteC->AddSprite(unsigned int(ObstacleSprite::Idle), new FixedSource(src));
	pSpriteC->AddSprite(unsigned int(ObstacleSprite::Moving), new TickSource(src, 2, true));
	src.x += 2 * src.width;
	pSpriteC->AddSprite(unsigned int(ObstacleSprite::Destroyed), new FixedSource(src));

	//Fsm
	FSMState* pDestrState = new FSMStateDestroyObject(pObject);
	FSMState* pDestrDelayState = new FSMStateDelay(1.5f, pDestrState);
	FSMState* pFallState = new FSMStateObstacleFall(pObject, settings.pGrid, 100.0f, pDestrDelayState);
	FSMState* pFallDelayState = new FSMStateDelay(1.5f, pFallState);
	FSMState* pIdleState = new FSMStateObstacleIdle(settings.pGrid, pObject, pFallDelayState);
	pFsmC->SaveState(pDestrState);
	pFsmC->SaveState(pDestrDelayState);
	pFsmC->SaveState(pFallState);
	pFsmC->SaveState(pFallDelayState);
	pFsmC->SaveState(pIdleState);

	pFsmC->SetState(pIdleState);
	return pObject;
}

GameObject* DigDug::CreatePooka(Scene& scene, const DigDugSettings& settings)
{

	GameObject* pObject = scene.CreateGameObject();

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
		pSprite->SetTexture("Sprites_Pooka.png");

		Rect start = Rect{ 0, 0, settings.spriteWidth, settings.spriteHeight };
		pSprite->AddSprite(unsigned int(EnemySprite::IdleRight), new FixedSource(start));
		pSprite->AddSprite(unsigned int(EnemySprite::MoveRight), new TickSource(start, 2, true));
		start.x += 2 * start.width;
		pSprite->AddSprite(unsigned int(EnemySprite::CrushedRight), new FixedSource(start));
		start.x += start.width;
		pSprite->AddSprite(unsigned int(EnemySprite::Ghost), new TickSource(start, 2, true));
		start.x = 0;
		start.y += start.height;
		pSprite->AddSprite(unsigned int(EnemySprite::IdleLeft), new FixedSource(start));
		pSprite->AddSprite(unsigned int(EnemySprite::MoveLeft), new TickSource(start, 2, true));
		start.x += 2 * start.width;
		pSprite->AddSprite(unsigned int(EnemySprite::CrushedLeft), new FixedSource(start));
		start.x = 0;
		start.y += start.height;
		start.width = 2 * start.width;
		start.height = 2 * start.height;
		pSprite->AddSprite(unsigned int(EnemySprite::PumpedRightTier1), new FixedSource(start));
		start.x += start.width;
		pSprite->AddSprite(unsigned int(EnemySprite::PumpedRightTier2), new FixedSource(start));
		start.x += start.width;
		pSprite->AddSprite(unsigned int(EnemySprite::PumpedRightTier3), new FixedSource(start));
		start.x += start.width;
		pSprite->AddSprite(unsigned int(EnemySprite::PumpedRightTier4), new FixedSource(start));
		start.x = 0;
		start.y += start.height;
		pSprite->AddSprite(unsigned int(EnemySprite::PumpedLeftTier1), new FixedSource(start));
		start.x += start.width;
		pSprite->AddSprite(unsigned int(EnemySprite::PumpedLeftTier2), new FixedSource(start));
		start.x += start.width;
		pSprite->AddSprite(unsigned int(EnemySprite::PumpedLeftTier3), new FixedSource(start));
		start.x += start.width;
		pSprite->AddSprite(unsigned int(EnemySprite::PumpedLeftTier4), new FixedSource(start));
	}
	{

		FSMState* pDeadState = new FSMStateDestroyObject(pObject);
		FSMState* pDeadDelayState = new FSMStateDelay(1.5f, pDeadState);
		FSMState* pCrushedState = new FSMStateEnemyCrushed(settings.pGrid, pObject, 100.0f, pDeadDelayState);

		FSMStateEnemyPumping* pPumpedState = new FSMStateEnemyPumping(pObject, 2.0f, 0.5f, nullptr, pDeadDelayState, pCrushedState);
		FSMStateEnemyGhost* pGhostState = new FSMStateEnemyGhost(pObject, 60.0f, 5.0f, settings.pGrid, nullptr, pCrushedState, pPumpedState);
		FSMState* pMoveState = new FSMStateEnemyMoving(60.0f, settings.pGrid, pObject, pPumpedState, pGhostState, pCrushedState, 10.0f);
		pPumpedState->SetMoveState(pMoveState);
		pGhostState->SetMoveState(pMoveState);

		pFSM->SaveState(pDeadState);
		pFSM->SaveState(pDeadDelayState);
		pFSM->SaveState(pCrushedState);
		pFSM->SaveState(pPumpedState);
		pFSM->SaveState(pGhostState);
		pFSM->SaveState(pMoveState);


		pFSM->SetState(pMoveState);


	}
	return pObject;
}

GameObject* DigDug::CreatePump(GameObject& parent, const DigDugSettings& settings)
{
	UNREFERENCED_PARAMETER(settings);
	GameObject* pPump = parent.CreateChild();

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
		pSprite->SetTexture("Sprites_Pump.png");
		pSprite->SetSpriteSpeed(settings.spriteSpeed);
		pSprite->AddSprite(unsigned int(PumpSpriteID::None), new FixedSource(Rect{ 0,0,0,0 }));
		Rect src{ 0,0, settings.spriteWidth, settings.spriteHeight };
		pSprite->AddSprite(unsigned int(PumpSpriteID::Up), new SweepSource(Rect{ 0,0, src.width,0 }, Rect{ 0,0, src.width, src.height * 2 }, 2, false));
		pSprite->AddSprite(unsigned int(PumpSpriteID::Right), new SweepSource(Rect{ 3 * src.width, 0, 0, src.height }, Rect{ src.width, 0, 2 * src.width, src.height }, 2, false));
		pSprite->AddSprite(unsigned int(PumpSpriteID::Down), new SweepSource{ Rect{3 * src.width, 2 * src.height, src.width, 0}, Rect{3 * src.width, 0, src.width, 2 * src.height}, 2, false });
		pSprite->AddSprite(unsigned int(PumpSpriteID::Left), new SweepSource{ Rect{4 * src.width, 0, 0, src.height}, Rect{4 * src.width, 0, 2 * src.width, src.height}, 2, false });
	}
	{
		FSMStatePumpInactive* pInactiveState = new FSMStatePumpInactive(nullptr, pPump);
		FSMStatePumpActive* pActiveState = new FSMStatePumpActive(pPump, 0, 2 * settings.spriteSpeed, pInactiveState, nullptr);
		FSMState* pHitState = new FSMStatePumpPumping(pPump, pInactiveState);
		pInactiveState->SetActiveState(pActiveState);
		pActiveState->SetPumpState(pHitState);

		pFSM->SaveState(pInactiveState);
		pFSM->SaveState(pActiveState);
		pFSM->SaveState(pHitState);


		pFSM->SetState(pInactiveState);
	}

	return pPump;
}
