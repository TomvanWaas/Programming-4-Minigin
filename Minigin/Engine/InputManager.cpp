#include "stdafx.h"
#include "InputManager.h"
#include <SDL.h>
#include <minwinbase.h>
#include "InputCommand.h"
#include <algorithm>

using namespace Minigin;

BYTE* InputManager::m_pCurrentKeyboardState = nullptr;
BYTE* InputManager::m_pPreviousKeyboardState = nullptr;
XINPUT_STATE InputManager::m_PreviousControllerStates[XUSER_MAX_COUNT];
XINPUT_STATE InputManager::m_CurrentControllerStates[XUSER_MAX_COUNT];
bool InputManager::m_IsInitialized = false;
bool InputManager::m_IsControllerConnected[XUSER_MAX_COUNT];
bool InputManager::m_Continue = true;

//Static
void InputManager::StaticInitialize()
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
bool InputManager::StaticProcessInput()
{
	//UpdateFirstOverride Keyboard States
	BYTE* pBuffer = m_pPreviousKeyboardState;
	m_pPreviousKeyboardState = m_pCurrentKeyboardState;
	m_pCurrentKeyboardState = pBuffer;
	GetKeyboardState(m_pCurrentKeyboardState);

	//UpdateFirstOverride Controller States
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

	return m_Continue;
}
void InputManager::StaticDestroy()
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

void InputManager::StaticQuit()
{
	m_Continue = false;
}

//
void InputManager::Update(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);
	//Actions
	for (auto& action : m_InputActions)
	{
		action.second.isTriggered = IsTriggered(action.second);
		if (action.second.isTriggered && action.second.pCommand != nullptr)
		{
			action.second.pCommand->Execute();
		}
	}
}

unsigned int InputManager::AddInputAction(const InputAction& action, unsigned id)
{
	// It is not guaranteed to have it on 'id', that is why the new id gets returned
	while (m_InputActions.find(id) != m_InputActions.end())
	{
		++id;
	}
	m_InputActions[id] = action;
	return id;
}
void InputManager::RemoveInputAction(unsigned id)
{
	auto i = m_InputActions.find(id);
	if (i != m_InputActions.end())
	{
		m_InputActions.erase(i);
	}
}
const InputAction* InputManager::GetInputAction(unsigned id) const
{
	auto i = m_InputActions.find(id);
	if (i != m_InputActions.end())
	{
		return &(*i).second;
	}
	return nullptr;
}


bool InputManager::IsTriggered(const InputAction& action) const
{
	switch (action.triggerState)
	{
	case InputTriggerState::Down:
		//Keyboard
		if (action.ValidKeyboardCode() &&
			KeyboardKeyDown(action.keyboardCode, false))
		{
			return true;
		}
		//Mouse
		if (action.ValidMousebuttonCode() &&
			MouseButtonDown(action.mousebuttonCode, false))
		{
			return true;
		}
		//Gamepad
		if (action.ValidGamepadCode() && action.ValidPlayerID() &&
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
			!KeyboardKeyDown(action.keyboardCode, false))
		{
			return true;
		}
		//Mouse
		if (action.ValidMousebuttonCode() &&
			!MouseButtonDown(action.mousebuttonCode, false))
		{
			return true;
		}
		//Gamepad
		if (action.ValidGamepadCode() && action.ValidPlayerID() &&
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
		playerId < XUSER_MAX_COUNT && playerId >= 0 &&
		m_IsControllerConnected[playerId])
	{
		if (prev)
		{
			return ((m_PreviousControllerStates[playerId].Gamepad.wButtons&button) != 0);
		}
		return ((m_CurrentControllerStates[playerId].Gamepad.wButtons&button) != 0);
	}
	else if (playerId < XUSER_MAX_COUNT && playerId >= 0 &&
		m_IsControllerConnected[playerId])
	{
		float epsilon = 32767 * 0.5f;
		//Joysticks
		switch (button)
		{
		case (int(GamepadCode::GAMEPAD_LEFTSTICK_LEFT)):
		{
			if (prev) return m_PreviousControllerStates[playerId].Gamepad.sThumbLX < -epsilon;
			return m_CurrentControllerStates[playerId].Gamepad.sThumbLX < -epsilon;
		}
		break;
		case (int(GamepadCode::GAMEPAD_LEFTSTICK_RIGHT)):
		{
			if (prev) return m_PreviousControllerStates[playerId].Gamepad.sThumbLX > epsilon;
			return m_CurrentControllerStates[playerId].Gamepad.sThumbLX > epsilon;
		}
		break;
		case (int(GamepadCode::GAMEPAD_LEFTSTICK_UP)):
		{
			if (prev) return m_PreviousControllerStates[playerId].Gamepad.sThumbLY > epsilon;
			return m_CurrentControllerStates[playerId].Gamepad.sThumbLY > epsilon;
		}
		break;
		case (int(GamepadCode::GAMEPAD_LEFTSTICK_DOWN)):
		{
			if (prev) return m_PreviousControllerStates[playerId].Gamepad.sThumbLY < -epsilon;
			return m_CurrentControllerStates[playerId].Gamepad.sThumbLY < -epsilon;
		}
		break;
		case (int(GamepadCode::GAMEPAD_RIGHTSTICK_LEFT)):
		{
			if (prev) return m_PreviousControllerStates[playerId].Gamepad.sThumbRX < -epsilon;
			return m_CurrentControllerStates[playerId].Gamepad.sThumbRX < -epsilon;
		}
		break;
		case (int(GamepadCode::GAMEPAD_RIGHTSTICK_RIGHT)):
		{
			if (prev) return m_PreviousControllerStates[playerId].Gamepad.sThumbRX > epsilon;
			return m_CurrentControllerStates[playerId].Gamepad.sThumbRX > epsilon;
		}
		break;
		case (int(GamepadCode::GAMEPAD_RIGHTSTICK_UP)):
		{
			if (prev) return m_PreviousControllerStates[playerId].Gamepad.sThumbRY > epsilon;
			return m_CurrentControllerStates[playerId].Gamepad.sThumbRY > epsilon;
		}
		break;
		case (int(GamepadCode::GAMEPAD_RIGHTSTICK_DOWN)):
		{
			if (prev) return m_PreviousControllerStates[playerId].Gamepad.sThumbRY < -epsilon;
			return m_CurrentControllerStates[playerId].Gamepad.sThumbRY < -epsilon;
		}
		break;
		case (int(GamepadCode::GAMEPAD_LEFTTRIGGER)):
		{
			if (prev) return int(m_PreviousControllerStates[playerId].Gamepad.bLeftTrigger) > 0;
			return int(m_CurrentControllerStates[playerId].Gamepad.bLeftTrigger) > 0;
		}
		break;
		case (int(GamepadCode::GAMEPAD_RIGHTTRIGGER)):
		{
			if (prev) return int(m_PreviousControllerStates[playerId].Gamepad.bRightTrigger) > 0;
			return int(m_CurrentControllerStates[playerId].Gamepad.bRightTrigger) > 0;
		}
		break;
		}
	}
	return false;
}




