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
#include "GameColliderCommands.h"
#include "FSMConditions.h"
#include "FSMEvents.h"
#include "DigDugMovementComponent.h"


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
	pPlayer->AddComponent(new MovementComponent());
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
	MakePlayerPump(scene, *pPlayer, settings);



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
	pSpriteComp->AddSprite(unsigned int(PlayerSprite::CrushedRight), new TickSource(s, 2, true));
	s.x += 2 * settings.spriteWidth;
	pSpriteComp->AddSprite(unsigned int(PlayerSprite::CrushedUp), new TickSource(s, 2, true));
	s.x += 2 * settings.spriteWidth;
	pSpriteComp->AddSprite(unsigned int(PlayerSprite::CrushedLeft), new TickSource(s, 2, true));
	s.x += 2 * settings.spriteWidth;
	pSpriteComp->AddSprite(unsigned int(PlayerSprite::CrushedDown), new TickSource(s, 2, true));

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
	pSpriteComp->AddSprite(unsigned int(PlayerSprite::IdleRight), new TickSource(s, 1, true));
	s.x += 2 * settings.spriteWidth;
	pSpriteComp->AddSprite(unsigned int(PlayerSprite::IdleUp), new TickSource(s, 1, true));
	s.x += 2 * settings.spriteWidth;
	pSpriteComp->AddSprite(unsigned int(PlayerSprite::IdleLeft), new TickSource(s, 1, true));
	s.x += 2 * settings.spriteWidth;
	pSpriteComp->AddSprite(unsigned int(PlayerSprite::IdleDown), new TickSource(s, 1, true));

}

void DigDug::MakePlayerInput(GameObject* pPlayer, const DigDugSettings& settings, char up, char down, char left, char right, char pump)
{
	UNREFERENCED_PARAMETER(settings);
	auto* pMove = pPlayer->GetComponent<DigDugMovementComponent>();
	auto* pInput = pPlayer->GetScene()->GetSceneData().pInput;
	auto* pData = pPlayer->GetComponent<DataComponent>();
	PumpStatus* pPump = nullptr;
	pData->GetData("Pump", pPump);

	auto pLeftDown = std::make_shared<PlayerInput>(Direction::Left, pMove);
	auto pRightDown = std::make_shared<PlayerInput>(Direction::Right, pMove);
	auto pUpDown = std::make_shared<PlayerInput>(Direction::Up, pMove);
	auto pDownDown = std::make_shared<PlayerInput>(Direction::Down, pMove);
	auto pPumpPressed = std::make_shared<InputSetData<PumpStatus>>(*pPump, PumpStatus::Activated);
	auto pPumpReleased = std::make_shared<InputSetData<PumpStatus>>(*pPump, PumpStatus::Deactivated);
	auto pNone = std::make_shared<PlayerInput>(Direction::None, pMove);

	InputAction leftA{ InputTriggerState::Down, pLeftDown, -1, left, -1 };
	InputAction rightA{ InputTriggerState::Down, pRightDown, -1, right, -1 };
	InputAction upA{ InputTriggerState::Down, pUpDown, -1, up, -1 };
	InputAction downA{ InputTriggerState::Down, pDownDown, -1, down, -1 };
	InputAction pumpPressedA{ InputTriggerState::Pressed, pPumpPressed, -1, pump, -1 };
	InputAction pumpReleasedA{ InputTriggerState::Released, pPumpReleased, -1, pump, -1 };

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
	FSMState* pIdleState = new FSMState();
	FSMState* pMovingState = new FSMState();
	FSMState* pThrowingState = new FSMState();
	FSMState* pPumpingState = new FSMState();
	FSMState* pKilledState = new FSMState();
	FSMState* pCrushedState = new FSMState();
	pFSM->SaveState(pIdleState);
	pFSM->SaveState(pMovingState);
	pFSM->SaveState(pThrowingState);
	pFSM->SaveState(pPumpingState);
	pFSM->SaveState(pKilledState);
	pFSM->SaveState(pCrushedState);

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

	//Conditions
	Condition pPumpStateActivatedCondition = std::make_shared<FSMDataCondition<PumpStatus>>(*pPump, PumpStatus::Activated);
	Condition pPumpStateHitCondition = std::make_shared<FSMDataCondition<PumpStatus>>(*pPump, PumpStatus::Hit);
	Condition pPumpStateMissedCondition = std::make_shared<FSMDataCondition<PumpStatus>>(*pPump, PumpStatus::Missed);
	Condition pPumpStateDeactivatedCondition = std::make_shared<FSMDataCondition<PumpStatus>>(*pPump, PumpStatus::Deactivated);
	Condition pIsDeadCondition = std::make_shared<FSMDataCondition<PlayerHealth>>(*pHealth, PlayerHealth::Killed);
	Condition pIsCrushedCondition = std::make_shared<FSMDataCondition<PlayerHealth>>(*pHealth, PlayerHealth::Crushed);
	Condition pIdleCondition = std::make_shared<FSMDataCondition<Direction>>(*pMoveDirection, Direction::None);
	Condition pMoveCondition = std::make_shared<FSMNotCondition>(pIdleCondition);


	//Transitions
	Transition pToDead = std::make_shared<FSMTransition>(pKilledState, pIsDeadCondition);
	Transition pToCrushed = std::make_shared<FSMTransition>(pKilledState, pIsCrushedCondition);
	Transition pToIdle = std::make_shared<FSMTransition>(pIdleState, pIdleCondition);
	Transition pToMove = std::make_shared<FSMTransition>(pMovingState, pMoveCondition);
	Transition pToThrow = std::make_shared<FSMTransition>(pThrowingState, pPumpStateActivatedCondition);
	Transition pToPump = std::make_shared<FSMTransition>(pPumpingState, pPumpStateHitCondition);
	Transition pFromPump = std::make_shared<FSMTransition>(pIdleState, pPumpStateDeactivatedCondition);
	Transition pFromThrow = std::make_shared<FSMTransition>(pIdleState, pPumpStateMissedCondition);

	//Make
	pIdleState->SetEnterEvent(std::make_shared<FSMMultiEvent>(Events{ pUnlockMovementEvent,pStateSetIdleEvent, pSpriteEvent }));
	pIdleState->AddTransition(pToThrow);
	pIdleState->AddTransition(pToMove);
	pIdleState->AddTransition(pToDead);
	pIdleState->AddTransition(pToCrushed);

	pMovingState->SetEnterEvent(std::make_shared<FSMMultiEvent>(Events{ pUnlockMovementEvent, pStateSetMovingEvent }));
	pMovingState->AddTransition(pToIdle);
	pMovingState->AddTransition(pToDead);
	pMovingState->AddTransition(pToCrushed);
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
	pPumpingState->AddTransition(pToCrushed);

	pCrushedState->SetEnterEvent(std::make_shared<FSMMultiEvent>(Events{ pLockMovementEvent, pStateSetCrushedEvent,pSpriteEvent }));

	pKilledState->SetEnterEvent(std::make_shared<FSMMultiEvent>(Events{ pLockMovementEvent , pStateSetKilledEvent,pSpriteEvent }));
	
	
	pFSM->SetState(pIdleState, pPlayer->GetScene()->GetSceneData());
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
		FSMState* pNoneState = new FSMState();
		FSMState* pThrownState = new FSMState();
		FSMState* pMissedState = new FSMState();
		FSMState* pHitState = new FSMState();
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
		pFSM->SetState(pNoneState, scene.GetSceneData());
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

GameObject* DigDug::MakeStone(Scene& scene, const DigDugSettings& settings)
{
	auto pStone = scene.CreateGameObject();

	//Add Components
	pStone->AddComponent(new RenderComponent());
	auto pCollider = new SpriteCollisionComponent();
	pStone->AddComponent(pCollider);
	auto pSprite = new SpriteComponent();
	pStone->AddComponent(pSprite);
	auto pFSM = new FiniteStateMachineComponent();
	pStone->AddComponent(pFSM);
	pStone->AddComponent(new MovementComponent());


	//Set Values
	{
		//Collider
		pCollider->SetTrigger(false);
		pCollider->SetTag("Stone");
		pCollider->SetOffset(0, 0, 0, 0);
	}
	{
		//Sprite
		pSprite->SetSpriteSpeed(settings.spriteSpeed);
		pSprite->SetTexture(settings.spriteTexture);
		Rect src{ 0, 23 * settings.spriteHeight , settings.spriteWidth, settings.spriteHeight };
		pSprite->AddSprite(unsigned int(StoneSprite::Idle), new FixedSource(src));
		pSprite->AddSprite(unsigned int(StoneSprite::Moving), new TickSource(src, 2, true));
		src.x += 2 * settings.spriteWidth;
		pSprite->AddSprite(unsigned int(StoneSprite::Destroyed), new FixedSource(src));
	}
	{
		Vector2 offset = Vector2(0, settings.gridOffset.y*0.5f);

		//FSM
		
		//States
		FSMState* pIdleState = new FSMState();
		FSMState* pTriggeredState = new FSMState();
		FSMState* pFallState = new FSMState();
		FSMState* pHitState = new FSMState();
		pFSM->SaveState(pIdleState);
		pFSM->SaveState(pTriggeredState);
		pFSM->SaveState(pFallState);
		pFSM->SaveState(pHitState);

		//Events
		Event pSpriteIdleEvent = std::make_shared<FSMSpriteSourceEvent>(unsigned int(StoneSprite::Idle), pSprite);
		Event pSpriteFallEvent = std::make_shared<FSMSpriteSourceEvent>(unsigned int(StoneSprite::Moving), pSprite);
		Event pSpriteHitEvent = std::make_shared<FSMSpriteSourceEvent>(unsigned int(StoneSprite::Destroyed), pSprite);

		Event pMoveEvent = std::make_shared<FSMForceMoveEvent>(Vector2(0, 100), pStone->GetTransform());
		Event pTriggerEvent = std::make_shared<FSMTriggerEvent>(*pCollider, true);
		Event pDeleteEvent = std::make_shared<FSMDeleteGameObjectEvent>(pStone);
		Event pMarkSelfEvent = std::make_shared<FSMGridMarkerEvent>(*pStone, *settings.pGrid);
		Event pMarkbelowEvent = std::make_shared<FSMGridMarkerEvent>(*pStone, *settings.pGrid, offset);

		//Conditions
		Condition pIsMarkedCondition = std::make_shared<FSMGridMarkCondition>(*settings.pGrid, *pStone, offset, true);
		Condition pIsNotMarkedCondition = std::make_shared<FSMGridMarkCondition>(*settings.pGrid, *pStone, offset*0.5f, false);
		Condition pIsOnPointCondition = std::make_shared<FSMGridPositionPointCondition>(*settings.pGrid, pStone->GetTransform(), true);
		Condition pTimeCondition = std::make_shared<FSMTimeCondition>(2.0f);

		//Transitions
		Transition pIdleToTriggerTransition = std::make_shared<FSMTransition>(pTriggeredState, pIsMarkedCondition);
		Transition pTriggerToFallTransition = std::make_shared<FSMTransition>(pFallState, pTimeCondition);
		Transition pFallToHitTransition = std::make_shared<FSMTransition>(pHitState, std::make_shared<FSMMultiAndCondition>(Conditions{ pIsNotMarkedCondition, pIsOnPointCondition }));
		Transition pHitToEndTransition = std::make_shared<FSMTransition>(nullptr, pTimeCondition);

		//Make
		pIdleState->SetEnterEvent(pSpriteIdleEvent);
		pIdleState->AddTransition(pIdleToTriggerTransition);

		pTriggeredState->SetEnterEvent(pSpriteFallEvent);
		pTriggeredState->AddTransition(pTriggerToFallTransition);
		pTriggeredState->SetExitEvent(std::make_shared<FSMMultiEvent>(Events{ pMarkSelfEvent, pMarkbelowEvent }));

		pFallState->AddTransition(pFallToHitTransition);
		pFallState->SetUpdateFirstEvent(pMoveEvent);
		pFallState->SetEnterEvent(pTriggerEvent);

		pHitState->SetEnterEvent(pSpriteHitEvent);
		pHitState->AddTransition(pHitToEndTransition);
		pHitState->SetExitEvent(pDeleteEvent); //Delete on exit


		pFSM->SetState(pIdleState, scene.GetSceneData());
	}
	return pStone;
}
