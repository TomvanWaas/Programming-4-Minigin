#include "stdafx.h"
#include "Subject.h"
#include "Observer.h"
#include <algorithm>

using namespace Minigin;

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
		(*i) = nullptr;
		pObserver->RemoveSubject(this);
		return true;
	}
	return false;
}

void Subject::NotifyObservers(ObservedEvent event, const ObservedData& data)
{
	FilterObservers();
	for (size_t i = 0; i < m_pObservers.size(); ++i)
	{
		auto* pObserver = m_pObservers[i];
		if (pObserver) pObserver->Notify(event, data);
	}
}

void Subject::FilterObservers()
{
	m_pObservers.erase(std::remove_if(m_pObservers.begin(), m_pObservers.end(), [](Observer* pObserver)
	{
		return pObserver == nullptr;
	}), m_pObservers.end());
}
