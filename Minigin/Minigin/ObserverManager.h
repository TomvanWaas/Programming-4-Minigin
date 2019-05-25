#pragma once
#include "Manager.h"

namespace Minigin
{
	class Observer;
}
namespace DigDug
{
	class ObserverManager final : public Minigin::Manager
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