#include "MiniginPCH.h"
#include "Subject.h"
#include "Observer.h"

Subject::~Subject()
{
	DestroySubject();
}

void Subject::DestroySubject()
{
	for (Observer* pObserver : m_pObservers)
	{
		if (pObserver) pObserver->RemoveSubject(this);
	}
	m_pObservers.clear();
}

bool Subject::AddObserver(Observer* pObserver)
{
	if (!pObserver) return false;
	auto i = std::find(m_pObservers.begin(), m_pObservers.end(), pObserver);
	if (i == m_pObservers.end())
	{
		m_pObservers.push_back(pObserver);
		pObserver->AddSubject(this);
		return true;
	}
	return false;
}
bool Subject::RemoveObserver(Observer* pObserver)
{
	if (!pObserver) return false;
	auto i = std::find(m_pObservers.begin(), m_pObservers.end(), pObserver);
	if (i != m_pObservers.end())
	{
		m_pObservers.erase(i);
		pObserver->RemoveSubject(this);
		return true;
	}
	return false;
}

void Subject::NotifyObservers(ObservedEvent event, const ObservedData& data)
{
	for (Observer* pObserver : m_pObservers)
	{
		if (pObserver) pObserver->Notify(event, data);
	}
}
