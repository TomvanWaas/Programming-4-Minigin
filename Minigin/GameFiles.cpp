#include "MiniginPCH.h"
#include "GameFiles.h"
#include "SpriteIds.h"
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

using namespace DigDug;
typedef std::shared_ptr<FSMCondition> Condition;
typedef std::shared_ptr<FSMEvent> Event;
typedef std::shared_ptr<FSMTransition> Transition;
typedef std::vector<std::shared_ptr<FSMCondition>> Conditions;
typedef std::vector<std::shared_ptr<FSMEvent>> Events;

GameObject* DigDug::MakePlayer(Scene& scene,
	InputAction& moveLeftIAction,
	InputAction& moveRightIAction,
	InputAction& moveUpIAction,
	InputAction& moveDownIAction,
	InputAction& throwIAction, 
	const DigDugSettings& settings)
{
	UNREFERENCED_PARAMETER(moveLeftIAction);
	UNREFERENCED_PARAMETER(moveRightIAction);
	UNREFERENCED_PARAMETER(moveDownIAction);
	UNREFERENCED_PARAMETER(moveUpIAction);
	UNREFERENCED_PARAMETER(throwIAction);
	UNREFERENCED_PARAMETER(scene);
	UNREFERENCED_PARAMETER(settings);

	float moveSpeed = 50.0f;

	//Make
	GameObject* pPlayer = scene.CreateGameObject();

	//Add Components
	pPlayer->CreateComponent<RenderComponent>()->SetRenderPriority(100,scene.GetSceneData());
	auto pCollisionComp = pPlayer->CreateComponent<AABBCollisionComponent>();
	auto pMovementComp = pPlayer->CreateComponent<MovementComponent>();
	pPlayer->CreateComponent<FiniteStateMachineComponent>();
	auto pSpriteComp = pPlayer->CreateComponent<SpriteComponent>();
	auto pDataComp = pPlayer->CreateComponent<DataComponent>();

	//Set Component Values
	pCollisionComp->SetCollider(settings.spriteWidth, settings.spriteHeight, 0, 0);
	pMovementComp->SetInterpolation(0);
	pDataComp->AddData("IsKilled", false);
	pDataComp->AddData("IsCrushed", false);

	//Sprites
	MakePlayerSprites(pSpriteComp, settings);

	///////////////////////////////////////////////////////////////////////
	//Input
	///////////////////////////////////////////////////////////////////////

	unsigned int iactionIDMoveLeftDown = 0;
	unsigned int iactionIDMoveLeftUp = 0;
	unsigned int iactionIDMoveRightDown = 0;
	unsigned int iactionIDMoveRightUp = 0;
	unsigned int iactionIDMoveDownDown = 0;
	unsigned int iactionIDMoveDownUp = 0;
	unsigned int iactionIDMoveUpDown = 0;
	unsigned int iactionIDMoveUpUp = 0;
	unsigned int iactionIDThrowDown = 0;
	unsigned int iactionIDThrowUp = 0;

	MakePlayerInput(scene, 
		iactionIDMoveLeftUp, iactionIDMoveLeftDown, 
		iactionIDMoveRightUp, iactionIDMoveRightDown, 
		iactionIDMoveDownUp, iactionIDMoveDownDown, 
		iactionIDMoveUpUp, iactionIDMoveUpDown, 
		iactionIDThrowUp, iactionIDThrowDown, 
		moveLeftIAction, moveRightIAction, 
		moveUpIAction, moveDownIAction, throwIAction);

	MakePlayerFSM(scene, pPlayer, *settings.pGrid, moveSpeed,
		iactionIDMoveLeftUp, iactionIDMoveLeftDown, 
		iactionIDMoveRightUp, iactionIDMoveRightDown, 
		iactionIDMoveDownUp, iactionIDMoveDownDown,
		iactionIDMoveUpUp, iactionIDMoveUpDown, 
		iactionIDThrowUp, iactionIDThrowDown);
	
	///////////////////////////////////////////////////////////////////////
	//End
	///////////////////////////////////////////////////////////////////////

	return pPlayer;
}



void DigDug::MakePlayerSprites(SpriteComponent* pSpriteComp, const DigDugSettings& settings)
{
	//UNREFERENCED_PARAMETER(settings);
	//UNREFERENCED_PARAMETER(pSpriteComp);

	if (pSpriteComp == nullptr) return;

	pSpriteComp->SetSpriteSpeed(settings.spriteSpeed);
	pSpriteComp->SetTexture(settings.spriteTexture);

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

void DigDug::MakePlayerInput(Scene& scene, 
	unsigned& moveleftUp, unsigned& moveleftDown, 
	unsigned& moverightUp, unsigned& moverightDown, 
	unsigned& movedownUp, unsigned& movedownDown, 
	unsigned& moveupUp, unsigned& moveupDown,
	unsigned& throwUp, unsigned& throwDown, 
	InputAction& moveLeftIAction,
	InputAction& moveRightIAction,
	InputAction& moveUpIAction,
	InputAction& moveDownIAction,
	InputAction& throwIAction)
{
	InputManager* input = scene.GetSceneData().pInput;

	//Left
	InputAction moveLeftIActionReleased(moveLeftIAction);
	moveLeftIAction.triggerState = InputTriggerState::Down;
	moveLeftIActionReleased.triggerState = InputTriggerState::Up;
	moveLeftIAction.pCommand = nullptr;
	moveLeftIActionReleased.pCommand = nullptr;

	//Right
	InputAction moveRightIActionReleased(moveRightIAction);
	moveRightIAction.triggerState = InputTriggerState::Down;
	moveRightIActionReleased.triggerState = InputTriggerState::Up;
	moveRightIAction.pCommand = nullptr;
	moveRightIActionReleased.pCommand = nullptr;

	//Down
	InputAction moveDownIActionReleased(moveDownIAction);
	moveDownIAction.triggerState = InputTriggerState::Down;
	moveDownIActionReleased.triggerState = InputTriggerState::Up;
	moveDownIAction.pCommand = nullptr;
	moveDownIActionReleased.pCommand = nullptr;

	//Up
	InputAction moveUpIActionReleased(moveUpIAction);
	moveUpIAction.triggerState = InputTriggerState::Down;
	moveUpIActionReleased.triggerState = InputTriggerState::Up;
	moveUpIAction.pCommand = nullptr;
	moveUpIActionReleased.pCommand = nullptr;

	//Throw
	InputAction throwIActionReleased(throwIAction);
	throwIAction.triggerState = InputTriggerState::Down;
	throwIActionReleased.triggerState = InputTriggerState::Up;
	throwIAction.pCommand = nullptr;
	throwIActionReleased.pCommand = nullptr;

	//Add InputActions
	moveleftDown = input->AddInputAction(moveLeftIAction);
	moveleftUp = input->AddInputAction(moveLeftIActionReleased);

	moverightDown = input->AddInputAction(moveRightIAction);
	moverightUp = input->AddInputAction(moveRightIActionReleased);

	movedownDown = input->AddInputAction(moveDownIAction);
	movedownUp = input->AddInputAction(moveDownIActionReleased);

	moveupDown = input->AddInputAction(moveUpIAction);
	moveupUp = input->AddInputAction(moveUpIActionReleased);

	throwDown = input->AddInputAction(throwIAction);
	throwUp = input->AddInputAction(throwIActionReleased);
}




void DigDug::MakePlayerFSM(Scene& scene, GameObject* pPlayer, DigDugGridComponent& grid, const float moveSpeed,
	unsigned& moveleftUp, unsigned& moveleftDown, 
	unsigned& moverightUp, unsigned& moverightDown, 
	unsigned& movedownUp, unsigned& movedownDown,
	unsigned& moveupUp, unsigned& moveupDown,
	unsigned& throwUp, unsigned& throwDown)
{
	auto pDataComp = pPlayer->GetComponent<DataComponent>();

	////////////////////////////////////////////////////////////////////////
	//FSM
	////////////////////////////////////////////////////////////////////////
	auto pFSMComp = pPlayer->GetComponent<FiniteStateMachineComponent>();

	//////////////
	//States
	//////////////

	FSMState* pIdleUpState = new FSMState();
	FSMState* pIdleDownState = new FSMState();
	FSMState* pIdleLeftState = new FSMState();
	FSMState* pIdleRightState = new FSMState();

	FSMState* pMoveLeftState = new FSMState();
	FSMState* pMoveRightState = new FSMState();
	FSMState* pMoveUpState = new FSMState();
	FSMState* pMoveDownState = new FSMState();

	FSMState* pThrowUpState = new FSMState();
	FSMState* pThrowDownState = new FSMState();
	FSMState* pThrowRightState = new FSMState();
	FSMState* pThrowLeftState = new FSMState();

	FSMState* pPumpingUpState = new FSMState();
	FSMState* pPumpingDownState = new FSMState();
	FSMState* pPumpingLeftState = new FSMState();
	FSMState* pPumpingRightState = new FSMState();

	FSMState* pDeadUpState = new FSMState();
	FSMState* pDeadDownState = new FSMState();
	FSMState* pDeadLeftState = new FSMState();
	FSMState* pDeadRightState = new FSMState();

	FSMState* pCrushedLeftState = new FSMState();
	FSMState* pCrushedRightState = new FSMState();
	FSMState* pCrushedUpState = new FSMState();
	FSMState* pCrushedDownState = new FSMState();

	/////////////////
	//Conditions
	/////////////////

	//Input Conditions
	Condition pInputLeftUpCondition = nullptr;
	Condition pInputLeftDownCondition = nullptr;
	Condition pInputRightUpCondition = nullptr;
	Condition pInputRightDownCondition = nullptr;
	Condition pInputDownUpCondition = nullptr;
	Condition pInputDownDownCondition = nullptr;
	Condition pInputUpUpCondition = nullptr;
	Condition pInputUpDownCondition = nullptr;
	Condition pInputThrowUpCondition = nullptr;
	Condition pInputThrowDownCondition = nullptr;

	auto input = scene.GetSceneData().pInput;

	const InputAction* ref = input->GetInputAction(moveleftUp);
	if (ref) pInputLeftUpCondition = std::make_shared<FSMDataRefCondition<bool>>(ref->isTriggered, true);
	ref = input->GetInputAction(moveleftDown);
	if (ref) pInputLeftDownCondition = std::make_shared<FSMDataRefCondition<bool>>(ref->isTriggered, true);

	ref = input->GetInputAction(moverightUp);
	if (ref) pInputRightUpCondition = std::make_shared<FSMDataRefCondition<bool>>(ref->isTriggered, true);
	ref = input->GetInputAction(moverightDown);
	if (ref) pInputRightDownCondition = std::make_shared<FSMDataRefCondition<bool>>(ref->isTriggered, true);

	ref = input->GetInputAction(movedownUp);
	if (ref) pInputDownUpCondition = std::make_shared<FSMDataRefCondition<bool>>(ref->isTriggered, true);
	ref = input->GetInputAction(movedownDown);
	if (ref) pInputDownDownCondition = std::make_shared<FSMDataRefCondition<bool>>(ref->isTriggered, true);

	ref = input->GetInputAction(moveupUp);
	if (ref) pInputUpUpCondition = std::make_shared<FSMDataRefCondition<bool>>(ref->isTriggered, true);
	ref = input->GetInputAction(moveupDown);
	if (ref) pInputUpDownCondition = std::make_shared<FSMDataRefCondition<bool>>(ref->isTriggered, true);

	ref = input->GetInputAction(throwUp);
	if (ref) pInputThrowUpCondition = std::make_shared<FSMDataRefCondition<bool>>(ref->isTriggered, true);
	ref = input->GetInputAction(throwDown);
	if (ref) pInputThrowDownCondition = std::make_shared<FSMDataRefCondition<bool>>(ref->isTriggered, true);

	//Other Conditions
	Condition pPositionOnGridPointCondition = std::make_shared<FSMGridCondition>(grid, *pPlayer);
	bool* pBool = nullptr;
	pDataComp->GetData("IsKilled", pBool);
	Condition pIsDeadCondition = std::make_shared<FSMDataRefCondition<bool>>(*pBool, true);
	pDataComp->GetData("IsCrushed", pBool);
	Condition pIsCrushedCondition = std::make_shared<FSMDataRefCondition<bool>>(*pBool, true);



	////////////
	//Events
	////////////

	//Movement Events
	Event pMoveLeftEvent = std::make_shared<FSMMoveEvent>(Vector2(-moveSpeed, 0), *pPlayer);
	Event pMoveRightEvent = std::make_shared<FSMMoveEvent>(Vector2(moveSpeed, 0), *pPlayer);
	Event pMoveDownEvent = std::make_shared<FSMMoveEvent>(Vector2(0, moveSpeed), *pPlayer);
	Event pMoveUpEvent = std::make_shared<FSMMoveEvent>(Vector2(0, -moveSpeed), *pPlayer);

	//Sprite Events
	Event pSpriteMoveLeftEvent = std::make_shared<FSMSpriteEvent>(unsigned int(PlayerSprite::MoveLeft), *pPlayer);
	Event pSpriteMoveRightEvent = std::make_shared<FSMSpriteEvent>(unsigned int(PlayerSprite::MoveRight), *pPlayer);
	Event pSpriteMoveDownEvent = std::make_shared<FSMSpriteEvent>(unsigned int(PlayerSprite::MoveDown), *pPlayer);
	Event pSpriteMoveUpEvent = std::make_shared<FSMSpriteEvent>(unsigned int(PlayerSprite::MoveUp), *pPlayer);
	
	Event pSpriteIdleUpEvent = std::make_shared<FSMSpriteEvent>(unsigned int(PlayerSprite::IdleUp), *pPlayer);
	Event pSpriteIdleDownEvent = std::make_shared<FSMSpriteEvent>(unsigned int(PlayerSprite::IdleDown), *pPlayer);
	Event pSpriteIdleRightEvent = std::make_shared<FSMSpriteEvent>(unsigned int(PlayerSprite::IdleRight), *pPlayer);
	Event pSpriteIdleLeftEvent = std::make_shared<FSMSpriteEvent>(unsigned int(PlayerSprite::IdleLeft), *pPlayer);
	
	Event pSpriteThrowLeftEvent = std::make_shared<FSMSpriteEvent>(unsigned int(PlayerSprite::ThrowLeft), *pPlayer);
	Event pSpriteThrowRightEvent = std::make_shared<FSMSpriteEvent>(unsigned int(PlayerSprite::ThrowRight), *pPlayer);
	Event pSpriteThrowDownEvent = std::make_shared<FSMSpriteEvent>(unsigned int(PlayerSprite::ThrowDown), *pPlayer);
	Event pSpriteThrowUpEvent = std::make_shared<FSMSpriteEvent>(unsigned int(PlayerSprite::ThrowUp), *pPlayer);
	
	Event pSpriteCrushedDownEvent = std::make_shared<FSMSpriteEvent>(unsigned int(PlayerSprite::CrushedDown), *pPlayer);
	Event pSpriteCrushedRightEvent = std::make_shared<FSMSpriteEvent>(unsigned int(PlayerSprite::CrushedRight), *pPlayer);
	Event pSpriteCrushedLeftEvent = std::make_shared<FSMSpriteEvent>(unsigned int(PlayerSprite::CrushedLeft), *pPlayer);
	Event pSpriteCrushedUpEvent = std::make_shared<FSMSpriteEvent>(unsigned int(PlayerSprite::CrushedUp), *pPlayer);
	
	Event pSpritePumpDownEvent = std::make_shared<FSMSpriteEvent>(unsigned int(PlayerSprite::PumpDown), *pPlayer);
	Event pSpritePumpRightEvent = std::make_shared<FSMSpriteEvent>(unsigned int(PlayerSprite::PumpRight), *pPlayer);
	Event pSpritePumpLeftEvent = std::make_shared<FSMSpriteEvent>(unsigned int(PlayerSprite::PumpLeft), *pPlayer);
	Event pSpritePumpUpEvent = std::make_shared<FSMSpriteEvent>(unsigned int(PlayerSprite::PumpUp), *pPlayer);
	
	Event pSpriteDiedDownEvent = std::make_shared<FSMSpriteEvent>(unsigned int(PlayerSprite::DiedDown), *pPlayer);
	Event pSpriteDiedRightEvent = std::make_shared<FSMSpriteEvent>(unsigned int(PlayerSprite::DiedRight), *pPlayer);
	Event pSpriteDiedLeftEvent = std::make_shared<FSMSpriteEvent>(unsigned int(PlayerSprite::DiedLeft), *pPlayer);
	Event pSpriteDiedUpEvent = std::make_shared<FSMSpriteEvent>(unsigned int(PlayerSprite::DiedUp), *pPlayer);

	Event pMarkGridEvent = std::make_shared<FSMGridMarkerEvent>(*pPlayer, grid);

	///////////////
	//Transitions
	///////////////

	//Move
	Transition pMoveLeftTransition = std::make_shared<FSMTransition>(pMoveLeftState, std::make_shared<FSMMultiAndCondition>(Conditions{ pInputLeftDownCondition , pInputRightUpCondition, pInputUpUpCondition ,pInputDownUpCondition }));
	Transition pMoveLeftTransitionContinue = std::make_shared<FSMTransition>(pMoveLeftState, std::make_shared<FSMMultiOrCondition>(Conditions{ pInputLeftDownCondition , pInputUpDownCondition, pInputDownDownCondition }));
	Transition pMoveLeftTransitionOnPoint = std::make_shared<FSMTransition>(pMoveLeftState, std::make_shared<FSMMultiAndCondition>(Conditions{ pPositionOnGridPointCondition, pInputLeftDownCondition, pInputRightUpCondition, pInputUpUpCondition, pInputDownUpCondition }));

	Transition pMoveRightTransition = std::make_shared<FSMTransition>(pMoveRightState, std::make_shared<FSMMultiAndCondition>(Conditions{ pInputRightDownCondition , pInputLeftUpCondition, pInputUpUpCondition ,pInputDownUpCondition }));
	Transition pMoveRightTransitionContinue = std::make_shared<FSMTransition>(pMoveRightState, std::make_shared<FSMMultiOrCondition>(Conditions{ pInputRightDownCondition , pInputUpDownCondition, pInputDownDownCondition }));
	Transition pMoveRightTransitionOnPoint = std::make_shared<FSMTransition>(pMoveRightState, std::make_shared<FSMMultiAndCondition>(Conditions{ pPositionOnGridPointCondition, pInputRightDownCondition, pInputLeftUpCondition, pInputUpUpCondition, pInputDownUpCondition }));

	Transition pMoveUpTransition = std::make_shared<FSMTransition>(pMoveUpState, std::make_shared<FSMMultiAndCondition>(Conditions{ pInputUpDownCondition , pInputDownUpCondition, pInputLeftUpCondition ,pInputRightUpCondition }));
	Transition pMoveUpTransitionContinue = std::make_shared<FSMTransition>(pMoveUpState, std::make_shared<FSMMultiOrCondition>(Conditions{ pInputUpDownCondition , pInputRightDownCondition, pInputLeftDownCondition }));
	Transition pMoveUpTransitionOnPoint = std::make_shared<FSMTransition>(pMoveUpState, std::make_shared<FSMMultiAndCondition>(Conditions{ pPositionOnGridPointCondition, pInputUpDownCondition, pInputDownUpCondition, pInputLeftUpCondition, pInputRightUpCondition }));

	Transition pMoveDownTransition = std::make_shared<FSMTransition>(pMoveDownState, std::make_shared<FSMMultiAndCondition>(Conditions{ pInputDownDownCondition , pInputUpUpCondition, pInputRightUpCondition ,pInputLeftUpCondition }));
	Transition pMoveDownTransitionContinue = std::make_shared<FSMTransition>(pMoveDownState, std::make_shared<FSMMultiOrCondition>(Conditions{ pInputDownDownCondition , pInputRightDownCondition, pInputLeftDownCondition }));
	Transition pMoveDownTransitionOnPoint = std::make_shared<FSMTransition>(pMoveDownState, std::make_shared<FSMMultiAndCondition>(Conditions{ pPositionOnGridPointCondition, pInputDownDownCondition, pInputUpUpCondition, pInputRightUpCondition, pInputLeftUpCondition }));

	//Idle 
	auto pIdleConditions = std::make_shared<FSMMultiAndCondition>(Conditions{ pInputDownUpCondition, pInputLeftUpCondition, pInputRightUpCondition, pInputUpUpCondition, pInputThrowUpCondition });
	Transition pIdleUpTransition = std::make_shared<FSMTransition>(pIdleUpState, pIdleConditions);
	Transition pIdleDownTransition = std::make_shared<FSMTransition>(pIdleDownState, pIdleConditions);
	Transition pIdleLeftTransition = std::make_shared<FSMTransition>(pIdleLeftState, pIdleConditions);
	Transition pIdleRightTransition = std::make_shared<FSMTransition>(pIdleRightState, pIdleConditions);

	//Throw
	Transition pThrowUpTransition = std::make_shared<FSMTransition>(pThrowUpState, pInputThrowDownCondition);
	Transition pThrowDownTransition = std::make_shared<FSMTransition>(pThrowDownState, pInputThrowDownCondition);
	Transition pThrowLeftTransition = std::make_shared<FSMTransition>(pThrowLeftState, pInputThrowDownCondition);
	Transition pThrowRightTransition = std::make_shared<FSMTransition>(pThrowRightState, pInputThrowDownCondition);

	//Died
	Transition pDiedUpTransition = std::make_shared<FSMTransition>(pDeadUpState, pIsDeadCondition);
	Transition pDiedDownTransition = std::make_shared<FSMTransition>(pDeadDownState, pIsDeadCondition);
	Transition pDiedLeftTransition = std::make_shared<FSMTransition>(pDeadLeftState, pIsDeadCondition);
	Transition pDiedRightTransition = std::make_shared<FSMTransition>(pDeadRightState, pIsDeadCondition);

	//Crushed
	Transition pCrushedUpTransition = std::make_shared<FSMTransition>(pCrushedUpState, pIsCrushedCondition);
	Transition pCrushedDownTransition = std::make_shared<FSMTransition>(pCrushedDownState, pIsCrushedCondition);
	Transition pCrushedLeftTransition = std::make_shared<FSMTransition>(pCrushedLeftState, pIsCrushedCondition);
	Transition pCrushedRightTransition = std::make_shared<FSMTransition>(pCrushedRightState, pIsCrushedCondition);


	//Pumpt
	Transition pPumpUpTransition = std::make_shared<FSMTransition>(pPumpingUpState, nullptr);	//CONDITION : Hit something with 'lasso?'
	//...
	//Else, after time => idle



	//////////////////////////////////////////////////////////////////////
	//Make States
	//////////////////////////////////////////////////////////////////////

	//Idle
	{
		//Possibilities
		//Idle => Move
		//Idle => Throw
		//Idle => Dead
		//Idle => Crushed

		//Up
		pIdleUpState->SetEnterEvent(pSpriteIdleUpEvent);

		pIdleUpState->AddTransition(pMoveRightTransitionOnPoint);
		pIdleUpState->AddTransition(pMoveLeftTransitionOnPoint);
		pIdleUpState->AddTransition(pMoveDownTransition);
		pIdleUpState->AddTransition(pMoveUpTransitionContinue);

		pIdleUpState->AddTransition(pThrowUpTransition);
		pIdleUpState->AddTransition(pDiedUpTransition);
		pIdleUpState->AddTransition(pCrushedUpTransition);

		//Down
		pIdleDownState->SetEnterEvent(pSpriteIdleDownEvent);

		pIdleDownState->AddTransition(pMoveLeftTransitionOnPoint);
		pIdleDownState->AddTransition(pMoveRightTransitionOnPoint);
		pIdleDownState->AddTransition(pMoveUpTransition);
		pIdleDownState->AddTransition(pMoveDownTransitionContinue);

		pIdleDownState->AddTransition(pThrowDownTransition);
		pIdleDownState->AddTransition(pDiedDownTransition);
		pIdleDownState->AddTransition(pCrushedDownTransition);

		//Left
		pIdleLeftState->SetEnterEvent(pSpriteIdleLeftEvent);

		pIdleLeftState->AddTransition(pMoveUpTransitionOnPoint);
		pIdleLeftState->AddTransition(pMoveDownTransitionOnPoint);
		pIdleLeftState->AddTransition(pMoveRightTransition);
		pIdleLeftState->AddTransition(pMoveLeftTransitionContinue);

		pIdleLeftState->AddTransition(pThrowLeftTransition);
		pIdleLeftState->AddTransition(pDiedLeftTransition);
		pIdleLeftState->AddTransition(pCrushedLeftTransition);

		//Down
		pIdleRightState->SetEnterEvent(pSpriteIdleRightEvent);

		pIdleRightState->AddTransition(pMoveUpTransitionOnPoint);
		pIdleRightState->AddTransition(pMoveDownTransitionOnPoint);
		pIdleRightState->AddTransition(pMoveLeftTransition);
		pIdleRightState->AddTransition(pMoveRightTransitionContinue);

		pIdleRightState->AddTransition(pThrowRightTransition);
		pIdleRightState->AddTransition(pDiedRightTransition);
		pIdleRightState->AddTransition(pCrushedRightTransition);
	}
	//Movement
	{
		//Possibilities
		//Move => Idle
		//Move => Dead
		//Move => Throw
		//Move = > Crushed

		//Up
		pMoveUpState->SetEnterEvent(pSpriteMoveUpEvent);
		pMoveUpState->SetUpdateFirstEvent(pMoveUpEvent);
		pMoveUpState->SetUpdateSecondEvent(pMarkGridEvent);

		pMoveUpState->AddTransition(pIdleUpTransition);
		pMoveUpState->AddTransition(pMoveDownTransition);
		pMoveUpState->AddTransition(pMoveLeftTransitionOnPoint);
		pMoveUpState->AddTransition(pMoveRightTransitionOnPoint);

		pMoveUpState->AddTransition(pThrowUpTransition);
		pMoveUpState->AddTransition(pDiedUpTransition);
		pMoveUpState->AddTransition(pCrushedUpTransition);

		//Down
		pMoveDownState->SetEnterEvent(pSpriteMoveDownEvent);
		pMoveDownState->SetUpdateFirstEvent(pMoveDownEvent);
		pMoveDownState->SetUpdateSecondEvent(pMarkGridEvent);

		pMoveDownState->AddTransition(pIdleDownTransition);
		pMoveDownState->AddTransition(pMoveUpTransition);
		pMoveDownState->AddTransition(pMoveLeftTransitionOnPoint);
		pMoveDownState->AddTransition(pMoveRightTransitionOnPoint);

		pMoveDownState->AddTransition(pThrowDownTransition);
		pMoveDownState->AddTransition(pDiedDownTransition);
		pMoveDownState->AddTransition(pCrushedDownTransition);

		//Left
		pMoveLeftState->SetEnterEvent(pSpriteMoveLeftEvent);
		pMoveLeftState->SetUpdateFirstEvent(pMoveLeftEvent);
		pMoveLeftState->SetUpdateSecondEvent(pMarkGridEvent);

		pMoveLeftState->AddTransition(pIdleLeftTransition);
		pMoveLeftState->AddTransition(pMoveRightTransition);
		pMoveLeftState->AddTransition(pMoveUpTransitionOnPoint);
		pMoveLeftState->AddTransition(pMoveDownTransitionOnPoint);

		pMoveLeftState->AddTransition(pThrowLeftTransition);
		pMoveLeftState->AddTransition(pDiedLeftTransition);
		pMoveLeftState->AddTransition(pCrushedLeftTransition);

		//Right
		pMoveRightState->SetEnterEvent(pSpriteMoveRightEvent);
		pMoveRightState->SetUpdateFirstEvent(pMoveRightEvent);
		pMoveRightState->SetUpdateSecondEvent(pMarkGridEvent);

		pMoveRightState->AddTransition(pIdleRightTransition);
		pMoveRightState->AddTransition(pMoveLeftTransition);
		pMoveRightState->AddTransition(pMoveUpTransitionOnPoint);
		pMoveRightState->AddTransition(pMoveDownTransitionOnPoint);

		pMoveRightState->AddTransition(pThrowRightTransition);
		pMoveRightState->AddTransition(pDiedRightTransition);
		pMoveRightState->AddTransition(pCrushedRightTransition);
	}
	//Throw (& inhale (return or idk)?)
	{
		//Possibilities
		//Throw -> Pump
		//Throw -> Idle
		//Throw -> Crushed
		//Throw -> Dead

		//Up
		pThrowUpState->SetEnterEvent(pSpriteThrowUpEvent);
		//Down
		pThrowDownState->SetEnterEvent(pSpriteThrowDownEvent);
		//Left
		pThrowLeftState->SetEnterEvent(pSpriteThrowLeftEvent);
		//Right
		pThrowRightState->SetEnterEvent(pSpriteThrowRightEvent);

	}
	//Pump
	{
		//Possibilities
		//Pump -> Pump
		//Pump -> Idle
		//Pump -> Crushed
		//Pump -> Dead

		//Up
		pPumpingUpState->SetEnterEvent(pSpritePumpUpEvent);
		//Down
		pPumpingDownState->SetEnterEvent(pSpritePumpDownEvent);
		//Left
		pPumpingLeftState->SetEnterEvent(pSpritePumpLeftEvent);
		//Right
		pPumpingRightState->SetEnterEvent(pSpritePumpRightEvent);
	}
	//Dead
	{
		//Posibilities
		// - None

		//Up
		pDeadUpState->SetEnterEvent(pSpriteDiedUpEvent);
		//Down
		pDeadDownState->SetEnterEvent(pSpriteDiedDownEvent);
		//Right
		pDeadRightState->SetEnterEvent(pSpriteDiedRightEvent);
		//Left
		pDeadLeftState->SetEnterEvent(pSpriteDiedLeftEvent);

	}
	//Crushed
	{
		//Posibilities
		// - None

		//Up
		pDeadUpState->SetEnterEvent(pSpriteCrushedUpEvent);
		//Down
		pDeadDownState->SetEnterEvent(pSpriteCrushedDownEvent);
		//Right
		pDeadRightState->SetEnterEvent(pSpriteCrushedRightEvent);
		//Left
		pDeadLeftState->SetEnterEvent(pSpriteCrushedLeftEvent);

	}






	//Set
	pFSMComp->SetState(pIdleRightState, scene.GetSceneData());

	//FSM - Saves
	//States
	pFSMComp->SaveState(pIdleUpState);
	pFSMComp->SaveState(pIdleDownState);
	pFSMComp->SaveState(pIdleLeftState);
	pFSMComp->SaveState(pIdleRightState);

	pFSMComp->SaveState(pMoveLeftState);
	pFSMComp->SaveState(pMoveRightState);
	pFSMComp->SaveState(pMoveUpState);
	pFSMComp->SaveState(pMoveDownState);

	pFSMComp->SaveState(pThrowUpState);
	pFSMComp->SaveState(pThrowDownState);
	pFSMComp->SaveState(pThrowLeftState);
	pFSMComp->SaveState(pThrowRightState);

	pFSMComp->SaveState(pPumpingUpState);
	pFSMComp->SaveState(pPumpingDownState);
	pFSMComp->SaveState(pPumpingLeftState);
	pFSMComp->SaveState(pPumpingRightState);

	pFSMComp->SaveState(pDeadUpState);
	pFSMComp->SaveState(pDeadDownState);
	pFSMComp->SaveState(pDeadLeftState);
	pFSMComp->SaveState(pDeadRightState);

	pFSMComp->SaveState(pCrushedLeftState);
	pFSMComp->SaveState(pCrushedRightState);
	pFSMComp->SaveState(pCrushedUpState);
	pFSMComp->SaveState(pCrushedDownState);

	//Conditions

	//Events
}















GameObject* DigDug::MakeBackground(Scene& scene)
{
	auto pBackground = scene.CreateGameObject();
	pBackground->GetComponent<TransformComponent>()->SetWorldPosition(250, 250);

	//Texture
	auto pTextureObject = pBackground->CreateChild();
	{
		auto pRenderComp = pTextureObject->CreateComponent<RenderComponent>();
		pRenderComp->SetTexture("background.png");
		pTextureObject->GetComponent<TransformComponent>()->SetLocalScale(1.6f, 1.6f);
	}
	//Grid
	auto pGridObject = pBackground->CreateChild();
	{
		auto pGridComp = pGridObject->CreateComponent<DigDugGridComponent>();
		pGridComp->SetHeight(17);
		pGridComp->SetWidth(15);
		pGridComp->SetOffset(Vector2(24, 24));

		auto pGridRender = pGridObject->CreateComponent<MultiRenderComponent>();
		pGridRender->SetTexture("Cave.png");
		pGridRender->SetRenderPriority(10, scene.GetSceneData());
	}



	return pBackground;
}

GameObject* DigDug::MakeStone(Scene& scene, DigDugGridComponent& grid)
{
	UNREFERENCED_PARAMETER(scene);
	UNREFERENCED_PARAMETER(grid);

	//auto pStoneObject = scene.CreateGameObject();

	//auto pCollider = pStoneObject->CreateComponent<AABBCollisionComponent>();
	//auto pMovement = pStoneObject->CreateComponent<MovementComponent>();
	//auto pRender = pStoneObject->CreateComponent<RenderComponent>();
	//auto pSprite = pStoneObject->CreateComponent<SpriteComponent>();
	//auto pFSM = pStoneObject->CreateComponent<FiniteStateMachineComponent>();

	////Sprite
	//Rect src{};
	//pSprite->AddSprite(unsigned int(StoneSprite::Idle), SpriteSource{ src, 1, SpriteSource::Type::Tick, false });
	//pSprite->AddSprite(unsigned int(StoneSprite::Moving), SpriteSource{ src, 2, SpriteSource::Type::Tick, true, char(SpriteSource::Flag::X) });
	//src.x += 2 * src.width;
	//pSprite->AddSprite(unsigned int(StoneSprite::Destroyed), SpriteSource{ src, 1, SpriteSource::Type::Tick, false });


	return nullptr;
	
}

GameObject* DigDug::MakeLasso(GameObject& parent, const DigDugSettings& settings, Direction dir)
{
	UNREFERENCED_PARAMETER(parent);
	UNREFERENCED_PARAMETER(settings);
	UNREFERENCED_PARAMETER(dir);

	//auto pLasso = parent.CreateChild();

	//pLasso->CreateComponent<RenderComponent>();
	//
	////Collider
	//auto pColComp = pLasso->CreateComponent<AABBCollisionComponent>();
	//pColComp->SetTrigger(true);
	//pColComp->SetCollider(settings.spriteWidth, settings.spriteHeight, 0, 0);


	////Sprites
	//auto pSpriteComp = pLasso->CreateComponent<SpriteComponent>();
	//pSpriteComp->SetSpriteSpeed(settings.spriteSpeed);
	//pSpriteComp->SetTexture(settings.spriteTexture);
	//float duration = 2 * settings.spriteSpeed;

	//{
	//	Rect s{ 0, 7 * settings.spriteHeight , settings.spriteWidth, settings.spriteHeight };
	//	switch (dir)
	//	{
	//	case Direction::Up:
	//	{
	//		pSpriteComp->AddSprite(unsigned int(LassoSprite::Forth), new SweepSource(Rect{ s.x, s.y, s.width, 0 }, Rect{ s.x, s.y, s.width, 2 * s.height }, 2, true));
	//		pSpriteComp->AddSprite(unsigned int(LassoSprite::Back), new SweepSource(Rect{ s.x, s.y, s.width, s.height * 2 }, Rect{ s.x, s.y, s.width, 0 }, 2, true));
	//	}
	//	break;
	//	case Direction::Right:
	//	{
	//		pSpriteComp->AddSprite(unsigned int(LassoSprite::Forth), new SweepSource(Rect{ s.x + s.width * 3, s.y, 0, s.height }, Rect{ s.x + s.width, s.y, s.width * 2, s.height }, 2, true));
	//		pSpriteComp->AddSprite(unsigned int(LassoSprite::Back), new SweepSource(Rect{ s.x + s.width, s.y, s.width * 2, s.height }, Rect{ s.x + s.width * 3, s.y, 0, s.height }, 2, true));
	//	}
	//	break;
	//	case Direction::Down:
	//	{
	//		pSpriteComp->AddSprite(unsigned int(LassoSprite::Forth), new SweepSource(Rect{ s.x + 3 * s.width, s.y + 2 * s.height, s.width, 0 }, Rect{ s.x + 3 * s.width, s.y, s.width, 2 * s.height }, 2, true));
	//		pSpriteComp->AddSprite(unsigned int(LassoSprite::Back), new SweepSource(Rect{ s.x + 3 * s.width, s.y, s.width, 2 * s.height }, Rect{ s.x + 3 * s.width, s.y + 2 * s.height, s.width, 0 }, 2, true));
	//	}
	//	break;
	//	case Direction::Left:
	//	{
	//		pSpriteComp->AddSprite(unsigned int(LassoSprite::Forth), new SweepSource(Rect{ s.x + 4 * s.width, s.y, 0, s.height }, Rect{ s.x + 4 * s.width, s.y, s.width * 2,s.height }, 2, true));
	//		pSpriteComp->AddSprite(unsigned int(LassoSprite::Back), new SweepSource(Rect{ s.x + 4 * s.width, s.y, s.width * 2,s.height }, Rect{ s.x + 4 * s.width, s.y, 0, s.height }, 2, true));
	//	}
	//	break;

	//	}
	//}


	////FSM
	//auto pFSMComp = pLasso->CreateComponent<FiniteStateMachineComponent>();

	//





	return nullptr;
}

