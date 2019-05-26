#pragma once
#include "Manager.h"

namespace Engine
{
	class Observer;
}
namespace DigDug
{
	class ObserverManager final : public Engine::Manager
	{
	public:
		explicit ObserverManager() = default;
		virtual ~ObserverManager();
		bool AddUnownedObserver(Observer* pObserver);
		bool RemoveUnownedObserver(Observer* pObserver);
	private:
		std::vector<Observer*> m_pObservers;
	};

}