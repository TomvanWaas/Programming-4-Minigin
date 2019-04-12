#pragma once
#include <map>
#include <memory>
#include "ObserverEvents.h"
class GameObject;
class Observer;
class Subject abstract
{
public:
	explicit Subject() = default;
	virtual ~Subject() = default;

	Subject(const Subject& other) = delete;
	Subject(Subject&& other) noexcept = delete;
	Subject& operator=(const Subject& other) = delete;
	Subject& operator=(Subject&& other) noexcept = delete;

	bool AddObserver(const std::string& name, const std::shared_ptr<Observer>& pObserver);
	std::shared_ptr<Observer> RemoveObserver(const std::string& name);

	void Notify(const std::string& observername, GameObject* pGameObject, ObservedEvent event);
	void Notify(GameObject* pGameObject, ObservedEvent event);

private:
	std::map<std::string, std::shared_ptr<Observer>> m_pObservers;

};

