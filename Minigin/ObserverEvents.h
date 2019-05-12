#pragma once

enum class ObservedEvent
{
	//Engine Related
	ColliderEntered,
	ColliderExited,
	PositionChanged,
	ScaleChanged,
	RotationChanged,
	//...



	//Game Related
	GridMarked,
	HitByObstacle,
	Killed,
	PumpHit,
	PumpReleased,
	InputPumpPressed,
	InputPumpReleased,

	//...
	End
};