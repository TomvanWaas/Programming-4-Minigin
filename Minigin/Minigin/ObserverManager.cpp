#include "MiniginPCH.h"
#include "ObserverManager.h"

using namespace Engine;
using namespace DigDug;

ObserverManager::~ObserverManager()
{
	for (Observer*& pObserver : m_pObservers)
	{
		SAFE_DELETE(pObserver);
	}
	m_pObservers.clear();
}

bool ObserverManager::AddUnownedObserver(Observer* pObserver)
{
	auto i = std::find(m_pObservers.begin(), m_pObservers.end(), pObserver);
	if (i == m_pObservers.end())
	{
		m_pObservers.push_back(pObserver);
		return true;
	}
	return false;
}

bool ObserverManager::RemoveUnownedObserver(Observer* pObserver)
{
	auto i = std::find(m_pObservers.begin(), m_pObservers.end(), pObserver);
	if (i != m_pObservers.end())
	{
		m_pObservers.erase(i);
		return true;
	}
	return false;
}
