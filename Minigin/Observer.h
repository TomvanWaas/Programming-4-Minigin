#pragma once
#include "ObserverEvents.h"
class GameObject;
class Observer abstract
{
public:
	explicit Observer() = default;
	virtual ~Observer() = default;

	virtual void Notify(GameObject* pObject, ObservedEvent event)
	{
		UNREFERENCED_PARAMETER(event);
		UNREFERENCED_PARAMETER(pObject);
	}

	Observer(const Observer& other) = delete;
	Observer(Observer&& other) noexcept = delete;
	Observer& operator=(const Observer& other) = delete;
	Observer& operator=(Observer&& other) noexcept = delete;

};

