#pragma once

class ObservedEvent
{
	int m_ID;
public:
	ObservedEvent(int id) : m_ID(id) {}
	virtual ~ObservedEvent() = default;

	//Overload int casting
	operator int() const { return m_ID; }

	static const int ColliderEntered = 0;
	static const int ColliderExited = 1;
	static const int PositionChanged = 2;
	static const int ScaleChanged = 3;
	static const int RotationChanged = 4;
	static const int Destroyed = 5;

	static const int BASE_END = 6;

	


};

