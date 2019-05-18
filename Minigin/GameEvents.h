#pragma once
#include "ObservedEvent.h"

class GameEvent final : public ObservedEvent
{
public:
	static const int GridMarked = BASE_END + 1;
	static const int InputPumpPressed = BASE_END + 2;
	static const int InputPumpReleased = BASE_END + 3;
	static const int InputMovePressed = BASE_END + 4;

	static const int HitByObstacle = BASE_END + 5;
	static const int HitByEnemy = BASE_END + 6;
	static const int HitByPump = BASE_END + 7;

	static const int ObstacleHit = BASE_END + 8;
	static const int EnemyHit = BASE_END + 9;
	static const int PumpHit = BASE_END + 10;
	static const int PumpMissed = BASE_END + 11;

	static const int PlayerDied = BASE_END + 12;
	static const int EnemyDied = BASE_END + 13;
	static const int ObstacleDied = BASE_END + 14;

	static const int FygarFire = BASE_END + 15;

	static const int GainedScore = BASE_END + 16;
};
