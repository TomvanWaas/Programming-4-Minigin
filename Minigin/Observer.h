#pragma once
#include "ObservedEvent.h"
class GameObject;
class ObservedData;
class Observer abstract
{
public:
	explicit Observer() = default;
	virtual ~Observer() = default;

	virtual void Notify(ObservedEvent event, const ObservedData& data)
	{
		UNREFERENCED_PARAMETER(event);
		UNREFERENCED_PARAMETER(data);
	}

	Observer(const Observer& other) = delete;
	Observer(Observer&& other) noexcept = delete;
	Observer& operator=(const Observer& other) = delete;
	Observer& operator=(Observer&& other) noexcept = delete;

};

