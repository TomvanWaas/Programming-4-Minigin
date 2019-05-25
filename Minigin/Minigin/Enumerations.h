#pragma once
namespace DigDug
{
	enum class Direction
	{
		Up,
		Down,
		Left,
		Right,
		None
	};



	enum class PlayerSpriteID
	{
		IdleUp,
		IdleDown,
		IdleLeft,
		IdleRight,

		DigUp,
		DigDown,
		DigLeft,
		DigRight,

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
	enum class ObstacleSpriteID
	{
		Idle,
		Moving,
		Destroyed
	};
	enum class PumpSpriteID
	{
		Up,
		Right,
		Down,
		Left,
		None
	};
	enum class EnemySpriteID
	{
		IdleRight,
		MoveRight,
		CrushedRight,
		Ghost,
		PumpedRightTier1,
		PumpedRightTier2,
		PumpedRightTier3,
		PumpedRightTier4,
		IdleLeft,
		MoveLeft,
		CrushedLeft,
		PumpedLeftTier1,
		PumpedLeftTier2,
		PumpedLeftTier3,
		PumpedLeftTier4,
		FireLeft,
		FireRight
	};
	enum class FireSpriteID
	{
		None,
		Right,
		Left
	};




}