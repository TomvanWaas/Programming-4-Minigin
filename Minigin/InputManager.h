///////////////////////////////////////////////////////////////////////////////
// Largely Based on InputManager from Overlord Engine (Graphics Programming 2)
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include <map>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Xinput.h>
#pragma comment(lib, "Xinput.lib")


enum class InputTriggerState
{
	Pressed,
	Down,
	Released,
	Up
};
class InputCommand;
struct InputAction
{
	InputAction()
		: pCommand(nullptr)
		, triggerState(InputTriggerState::Pressed)
		, mousebuttonCode(-1)
		, keyboardCode(-1)
		, gamepadCode(-1)
		, isTriggered(false)
	{}
	InputAction(InputTriggerState triggerstate
		, const std::shared_ptr<InputCommand>& command
		, int mousecode = -1
		, int keyboardcode = -1
		, int gamepadcode = -1)
		: triggerState(triggerstate)
		, mousebuttonCode(mousecode)
		, keyboardCode(keyboardcode)
		, gamepadCode(gamepadcode)
		, pCommand(command)
		, isTriggered(false)
	{}

	InputAction(const InputAction& other)
		:triggerState(other.triggerState)
		,mousebuttonCode(other.mousebuttonCode)
		,keyboardCode(other.keyboardCode)
		,gamepadCode(other.gamepadCode)
		,pCommand(other.pCommand)
		,isTriggered(false)
	{}
	InputAction(InputAction&& other)
		:triggerState(other.triggerState)
		, mousebuttonCode(other.mousebuttonCode)
		, keyboardCode(other.keyboardCode)
		, gamepadCode(other.gamepadCode)
		, pCommand(other.pCommand)
		, isTriggered(false)
	{}
	InputAction& operator=(const InputAction& other)
	{
		triggerState = other.triggerState;
		mousebuttonCode = other.mousebuttonCode;
		keyboardCode = other.keyboardCode;
		gamepadCode = other.gamepadCode;
		pCommand = other.pCommand;
		isTriggered = false;
		return *this;
	}
	InputAction& operator=(InputAction&& other)
	{
		triggerState = other.triggerState;
		mousebuttonCode = other.mousebuttonCode;
		keyboardCode = other.keyboardCode;
		gamepadCode = other.gamepadCode;
		pCommand = other.pCommand;
		isTriggered = false;
		return *this;
	}


	bool ValidKeyboardCode() const { return keyboardCode > 0x07 && keyboardCode <= 0xFE; }
	bool ValidMousebuttonCode() const { return mousebuttonCode > 0x0 && mousebuttonCode <= 0x06; }
	bool ValidGamepadCode() const { return gamepadCode > 0x0000 && gamepadCode <= 0x8000; }
	bool ValidPlayerID() const { return playerID < XUSER_MAX_COUNT; }

	InputTriggerState triggerState;
	int mousebuttonCode;	//VK_...
	int keyboardCode;		//VK_...
	int gamepadCode;		//XINPUT_GAMEPAD_...
	unsigned int playerID;
	bool isTriggered;
	std::shared_ptr<InputCommand> pCommand;
};

class InputManager final
{
public:
	InputManager() = default;
	~InputManager() = default;

	static void Initialize();
	static bool ProcessInput();
	static void Destroy();

	void UpdateActions();
	unsigned int AddInputAction(const InputAction& action, unsigned int id = 0);
	void RemoveInputAction(unsigned int id);
	const InputAction* GetInputAction(unsigned int id)const;
	
private:
	std::map<unsigned int,InputAction> m_InputActions;



	static bool m_IsInitialized;

	//GameController
	static XINPUT_STATE m_CurrentControllerStates[XUSER_MAX_COUNT];
	static XINPUT_STATE m_PreviousControllerStates[XUSER_MAX_COUNT];
	static bool m_IsControllerConnected[XUSER_MAX_COUNT];

	//Keyboard
	static BYTE* m_pCurrentKeyboardState;
	static BYTE* m_pPreviousKeyboardState;
	

	//Action
	bool IsTriggered(const InputAction& action) const;
	bool KeyboardKeyDown(int key, bool prev) const;
	bool MouseButtonDown(int button, bool prev) const;
	bool GamepadButtonDown(int button, int playerId, bool prev) const;
};


