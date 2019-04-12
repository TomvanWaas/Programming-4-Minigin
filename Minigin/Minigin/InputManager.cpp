#include "MiniginPCH.h"
#include "InputManager.h"
#include <SDL.h>
#include <minwinbase.h>
#include "InputCommand.h"
#include <algorithm>

BYTE* InputManager::m_pCurrentKeyboardState = nullptr;
BYTE* InputManager::m_pPreviousKeyboardState = nullptr;
XINPUT_STATE InputManager::m_PreviousControllerStates[XUSER_MAX_COUNT];
XINPUT_STATE InputManager::m_CurrentControllerStates[XUSER_MAX_COUNT];
bool InputManager::m_IsInitialized = false;
bool InputManager::m_IsControllerConnected[XUSER_MAX_COUNT];

//Static
void InputManager::Initialize()
{
	if (m_IsInitialized == false)
	{
		//Keyboard
		m_pCurrentKeyboardState = new BYTE[256];
		m_pPreviousKeyboardState = new BYTE[256];
		GetKeyboardState(m_pCurrentKeyboardState);
		GetKeyboardState(m_pPreviousKeyboardState);

		//Gamepad
		for (unsigned int i = 0; i < XUSER_MAX_COUNT; ++i)
		{
			XINPUT_STATE state{};
			ZeroMemory(&state, sizeof(XINPUT_STATE)); //Clear space
			DWORD res = XInputGetState(i, &state);	//Is connected
			m_IsControllerConnected[i] = (res == ERROR_SUCCESS);
		}

		m_IsInitialized = true;
	}
}
bool InputManager::ProcessInput()
{
	//Update Keyboard States
	BYTE* pBuffer = m_pPreviousKeyboardState;
	m_pPreviousKeyboardState = m_pCurrentKeyboardState;
	m_pCurrentKeyboardState = pBuffer;
	GetKeyboardState(m_pCurrentKeyboardState);

	//Update Controller States
	for (unsigned int i = 0; i < XUSER_MAX_COUNT; ++i)
	{
		if (m_IsControllerConnected[i] == false)
		{
			break; //Not continue because lower numbers are priority over higher
		}

		m_PreviousControllerStates[i] = m_CurrentControllerStates[i];
		DWORD res = XInputGetState(i, &m_CurrentControllerStates[i]);
		m_IsControllerConnected[i] = (res == ERROR_SUCCESS);
	}

	//Check continuation
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
		{
			return false;
		}
		/*if (e.type == SDL_KEYDOWN) {

		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {

		}*/
	}

	return true;
}
void InputManager::Destroy()
{
	if (m_IsInitialized)
	{
		if (m_pCurrentKeyboardState != nullptr) delete[] m_pCurrentKeyboardState;
		m_pCurrentKeyboardState = nullptr;
		if (m_pPreviousKeyboardState != nullptr) delete[] m_pPreviousKeyboardState;
		m_pPreviousKeyboardState = nullptr;
		m_IsInitialized = false;
	}
}

//
void InputManager::UpdateActions()
{
	//Actions
	for (InputAction& action : m_InputActions)
	{
		action.isTriggered = IsTriggered(action);
		if (action.isTriggered && action.pCommand != nullptr)
		{
			action.pCommand->Execute();
		}
	}
}
void InputManager::AddInputAction(InputAction action)
{
	auto i = std::find_if(m_InputActions.begin(), m_InputActions.end(), 
		[&action](const InputAction& a)
	{
		return (action.gamepadCode == a.gamepadCode &&
			action.keyboardCode == a.keyboardCode &&
			action.mousebuttonCode == a.mousebuttonCode &&
			action.playerID == a.playerID &&
			action.triggerState == a.triggerState &&
			action.pCommand == a.pCommand);
	});

	if (i == m_InputActions.end())
	{
		m_InputActions.push_back(action);
	}
}




bool InputManager::IsTriggered(const InputAction& action) const
{
	switch (action.triggerState)
	{
	case InputTriggerState::Down:
		//Keyboard
		if (action.ValidKeyboardCode() &&
			KeyboardKeyDown(action.keyboardCode, true) &&
			KeyboardKeyDown(action.keyboardCode, false))
		{
			return true;
		}
		//Mouse
		if (action.ValidMousebuttonCode() &&
			MouseButtonDown(action.mousebuttonCode, true) &&
			MouseButtonDown(action.mousebuttonCode, false))
		{
			return true;
		}
		//Gamepad
		if (action.ValidGamepadCode() && action.ValidPlayerID() &&
			GamepadButtonDown(action.gamepadCode, action.playerID, true) &&
			GamepadButtonDown(action.gamepadCode, action.playerID, false))
		{
			return true;
		}
		break;

	case InputTriggerState::Released:
		//Keyboard
		if (action.ValidKeyboardCode() &&
			KeyboardKeyDown(action.keyboardCode, true) &&
			!KeyboardKeyDown(action.keyboardCode, false))
		{
			return true;
		}
		//Mouse
		if (action.ValidMousebuttonCode() &&
			MouseButtonDown(action.mousebuttonCode, true) &&
			!MouseButtonDown(action.mousebuttonCode, false))
		{
			return true;
		}
		//Gamepad
		if (action.ValidGamepadCode() && action.ValidPlayerID() &&
			GamepadButtonDown(action.gamepadCode, action.playerID, true) &&
			!GamepadButtonDown(action.gamepadCode, action.playerID, false))
		{
			return true;
		}
		break;

	case InputTriggerState::Pressed:
		//Keyboard
		if (action.ValidKeyboardCode() && 
			!KeyboardKeyDown(action.keyboardCode, true) &&
			KeyboardKeyDown(action.keyboardCode, false))
		{
			return true;
		}
		//Mouse
		if (action.ValidMousebuttonCode() &&
			!MouseButtonDown(action.mousebuttonCode, true) &&
			MouseButtonDown(action.mousebuttonCode, false))
		{
			return true;
		}
		//Gamepad
		if (action.ValidGamepadCode() && action.ValidPlayerID() &&
			!GamepadButtonDown(action.gamepadCode, action.playerID, true) && 
			GamepadButtonDown(action.gamepadCode, action.playerID, false))
		{
			return true;
		}
		break;
	case InputTriggerState::Up:
		//Keyboard
		if (action.ValidKeyboardCode() &&
			!KeyboardKeyDown(action.keyboardCode, true) &&
			!KeyboardKeyDown(action.keyboardCode, false))
		{
			return true;
		}
		//Mouse
		if (action.ValidMousebuttonCode() &&
			!MouseButtonDown(action.mousebuttonCode, true) &&
			!MouseButtonDown(action.mousebuttonCode, false))
		{
			return true;
		}
		//Gamepad
		if (action.ValidGamepadCode() && action.ValidPlayerID() &&
			!GamepadButtonDown(action.gamepadCode, action.playerID, true) &&
			!GamepadButtonDown(action.gamepadCode, action.playerID, false))
		{
			return true;
		}
		break;
	}
	return false;
}
bool InputManager::KeyboardKeyDown(int key, bool prev) const
{
	if (m_pCurrentKeyboardState != nullptr &&	//Valid state
		m_pPreviousKeyboardState != nullptr &&	//Valid state
		key > 0x07 && key <= 0xFE &&			//Valid key
		((prev && ((m_pPreviousKeyboardState[key] & 0xF0) != 0))			//Previous
			|| (!prev && ((m_pCurrentKeyboardState[key] & 0xF0) != 0))))	//Current
	{
		return true;
	}
	return false;
}
bool InputManager::MouseButtonDown(int button, bool prev) const
{
	if (button > 0x00 && button <= 0x06)
	{
		if (prev)
		{
			return (m_pPreviousKeyboardState != nullptr && 
				(m_pPreviousKeyboardState[button] & 0xF0) != 0);
		}
		return (m_pCurrentKeyboardState != nullptr &&
			(m_pCurrentKeyboardState[button] & 0xF0) != 0);
	}
	return false;
}
bool InputManager::GamepadButtonDown(int button, int playerId, bool prev) const
{
	if (button > 0x0000 && button <= 0x8000 &&
		playerId < XUSER_MAX_COUNT && 
		m_IsControllerConnected[playerId])
	{
		if (prev)
		{
			return ((m_PreviousControllerStates[playerId].Gamepad.wButtons&button) != 0);
		}
		return ((m_CurrentControllerStates[playerId].Gamepad.wButtons&button) != 0);
	}
	return false;
}




