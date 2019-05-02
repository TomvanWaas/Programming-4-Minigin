#pragma once
namespace DigDug
{
	enum class PlayerSprite
	{
		//Order: Up -> Down -> Left -> Right
		IdleUp,
		IdleDown,
		IdleLeft,
		IdleRight,

		MoveUp,
		MoveDown,
		MoveLeft,
		MoveRight,

		ThrowUp,
		ThrowDown,
		ThrowLeft,
		ThrowRight,

		PumpUp,
		PumpDown,
		PumpLeft,
		PumpRight,

		CrushedUp,
		CrushedDown,
		CrushedLeft,
		CrushedRight,

		DiedUp,
		DiedDown,
		DiedLeft,
		DiedRight
	};

	enum class PlayerState
	{
		Idle,
		Moving,
		Thrown,
		Pumping,
		Crushed,
		Killed
	};

	enum class StoneSprite
	{
		Idle,
		Moving,
		Destroyed
	};

	enum class LassoSprite
	{
		Up,
		Right,
		Down,
		Left,
		None
	};


	enum class PlayerHealth
	{
		Alive,
		Killed,
		Crushed
	};
	enum class PumpStatus
	{
		Deactivated,
		Hit,
		Missed,
		Activated
	};
	enum class Direction
	{
		Up,
		Down,
		Left,
		Right,
		None
	};
}