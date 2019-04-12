#pragma once
#include "ObserverEvents.h"

class GameObject;
class Observer abstract
{
public:
	explicit Observer() = default;
	virtual ~Observer() = default;

	virtual void Notify(GameObject* pGameObject, ObservedEvent event) = 0;

	Observer(const Observer& other) = delete;
	Observer(Observer&& other) noexcept = delete;
	Observer& operator=(const Observer& other) = delete;
	Observer& operator=(Observer&& other) noexcept = delete;

};

