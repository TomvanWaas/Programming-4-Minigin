///////////////////////////////////////////////////////////////////////////////
// Largely Based on InputManager from Overlord Engine (Graphics Programming 2)
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include <map>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Xinput.h>
#include "Manager.h"
#pragma comment(lib, "Xinput.lib")

namespace Engine
{
	enum class GamepadCode
	{
		GAMEPAD_DPAD_UP = 0x0001,
		GAMEPAD_DPAD_DOWN = 0x0002,
		GAMEPAD_DPAD_LEFT = 0x0004,
		GAMEPAD_DPAD_RIGHT = 0x0008,
		GAMEPAD_START = 0x0010,
		GAMEPAD_BACK = 0x0020,
		GAMEPAD_LEFT_THUMB = 0x0040,
		GAMEPAD_RIGHT_THUMB = 0x0080,
		GAMEPAD_LEFT_SHOULDER = 0x0100,
		GAMEPAD_RIGHT_SHOULDER = 0x0200,
		GAMEPAD_A = 0x1000,
		GAMEPAD_B = 0x2000,
		GAMEPAD_X = 0x4000,
		GAMEPAD_Y = 0x8000,
		GAMEPAD_LEFTSTICK_UP = 0x8000 * 2,
		GAMEPAD_LEFTSTICK_DOWN = 0x8000 * 4,
		GAMEPAD_LEFTSTICK_RIGHT = 0x8000 * 8,
		GAMEPAD_LEFTSTICK_LEFT = 0x8000 * 16,
		GAMEPAD_RIGHTSTICK_UP = 0x8000 * 32,
		GAMEPAD_RIGHTSTICK_DOWN = 0x8000 * 64,
		GAMEPAD_RIGHTSTICK_RIGHT = 0x8000 * 128,
		GAMEPAD_RIGHTSTICK_LEFT = 0x8000 * 256,
		GAMEPAD_LEFTTRIGGER = 0x8000 * 512,
		GAMEPAD_RIGHTTRIGGER = 0x8000 * 1024,

		GAMEPAD_MAX = 0x8000 * 2048
	};


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
			, playerID(0)
		{}
		InputAction(InputTriggerState triggerstate
			, const std::shared_ptr<InputCommand>& command
			, int mousecode = -1
			, int keyboardcode = -1
			, int gamepadcode = -1
			, int playerID = -1)
			: triggerState(triggerstate)
			, mousebuttonCode(mousecode)
			, keyboardCode(keyboardcode)
			, gamepadCode(gamepadcode)
			, pCommand(command)
			, isTriggered(false)
			, playerID(playerID)
		{}

		InputAction(const InputAction& other)
			:triggerState(other.triggerState)
			, mousebuttonCode(other.mousebuttonCode)
			, keyboardCode(other.keyboardCode)
			, gamepadCode(other.gamepadCode)
			, pCommand(other.pCommand)
			, isTriggered(false)
			, playerID(other.playerID)
		{}
		InputAction(InputAction&& other)
			:triggerState(other.triggerState)
			, mousebuttonCode(other.mousebuttonCode)
			, keyboardCode(other.keyboardCode)
			, gamepadCode(other.gamepadCode)
			, pCommand(other.pCommand)
			, isTriggered(false)
			, playerID(other.playerID)
		{}
		InputAction& operator=(const InputAction& other)
		{
			triggerState = other.triggerState;
			mousebuttonCode = other.mousebuttonCode;
			keyboardCode = other.keyboardCode;
			gamepadCode = other.gamepadCode;
			pCommand = other.pCommand;
			isTriggered = false;
			playerID = other.playerID;
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
			playerID = other.playerID;
			return *this;
		}


		bool ValidKeyboardCode() const { return keyboardCode > 0x07 && keyboardCode <= 0xFE; }
		bool ValidMousebuttonCode() const { return mousebuttonCode > 0x0 && mousebuttonCode <= 0x06; }
		bool ValidGamepadCode() const { return gamepadCode > 0x0000 && gamepadCode < int(GamepadCode::GAMEPAD_MAX); }
		bool ValidPlayerID() const { return playerID < XUSER_MAX_COUNT && playerID >= 0; }

		InputTriggerState triggerState;
		int mousebuttonCode;	//VK_...
		int keyboardCode;		//VK_...
		int gamepadCode;		//XINPUT_GAMEPAD_...
		unsigned int playerID;
		bool isTriggered;
		std::shared_ptr<InputCommand> pCommand;
	};

	class InputManager final : public Manager
	{
	public:
		InputManager() = default;
		~InputManager() = default;

		static void StaticInitialize();
		static bool StaticProcessInput();
		static void StaticDestroy();
		static void StaticQuit();

		virtual void Update(const SceneData& sceneData) override;
		unsigned int AddInputAction(const InputAction& action, unsigned int id = 0);
		void RemoveInputAction(unsigned int id);
		const InputAction* GetInputAction(unsigned int id) const;

	private:
		std::map<unsigned int, InputAction> m_InputActions;



		static bool m_IsInitialized;
		static bool m_Continue;

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


}