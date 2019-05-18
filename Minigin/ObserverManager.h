#pragma once
#include "Manager.h"
class Observer;
class ObserverManager final : public Manager
{
public:
	explicit ObserverManager() = default;
	virtual ~ObserverManager();
	bool AddUnownedObserver(Observer* pObserver);
	bool RemoveUnownedObserver(Observer* pObserver);
private:
	std::vector<Observer*> m_pObservers;
};

