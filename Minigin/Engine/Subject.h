#pragma once
#include "ObservedEvent.h"
namespace Minigin
{
	class ObservedData;
	class Observer;

	class Subject abstract
	{
	public:
		Subject() = default;
		virtual ~Subject();

		Subject(const Subject& other) = delete;
		Subject(Subject&& other) noexcept = delete;
		Subject& operator=(const Subject& other) = delete;
		Subject& operator=(Subject&& other) noexcept = delete;

		void DestroySubject();
		bool AddObserver(Observer* pObserver);
		bool RemoveObserver(Observer* pObserver);
	protected:
		void NotifyObservers(ObservedEvent event, const ObservedData& data);
		void FilterObservers();
	private:
		std::vector<Observer*> m_pObservers;

	};
}
