#pragma once
#include "ObservedEvent.h"
#include "Subject.h"
class ObservedData;
class Observer abstract
{
	friend Subject;
public:
	Observer() = default;
	virtual ~Observer();

	Observer(const Observer& other) = delete;
	Observer(Observer&& other) noexcept = delete;
	Observer& operator=(const Observer& other) = delete;
	Observer& operator=(Observer&& other) noexcept = delete;

	void DestroyObserver();
	virtual void Notify(ObservedEvent event, const ObservedData& data)
	{
		UNREFERENCED_PARAMETER(event);
		UNREFERENCED_PARAMETER(data);
	}
private:
	//Used to unregister from subjects if self gets destroyed
	std::vector<Subject*> m_pSubjects;

	//Friend func (Used to remove subject when a subject gets destroyed)
	bool AddSubject(Subject* pSubject);
	bool RemoveSubject(Subject* pSubject);
};


