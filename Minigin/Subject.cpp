#include "MiniginPCH.h"
#include "Subject.h"
#include <algorithm>
#include "Observer.h"
#include "Logger.h"

bool Subject::AddObserver(const std::string& name, const std::shared_ptr<Observer>& pObserver)
{
	if (std::find_if(m_pObservers.begin(), m_pObservers.end(),
		[&name](const std::pair<std::string, std::shared_ptr<Observer>>& pObserver)
	{
		return (pObserver.first == name);
	}) == m_pObservers.end())
	{
		m_pObservers[name] = pObserver;
		return true;
	}

	Logger::GetInstance().LogWarning("Subject::AddObserver > Failed to add observer " + name);
	return false;
}

std::shared_ptr<Observer> Subject::RemoveObserver(const std::string& name)
{
	auto i = std::find_if(m_pObservers.begin(), m_pObservers.end(),
		[&name](const std::pair<std::string, std::shared_ptr<Observer>>& pObserver)
	{
		return pObserver.first == name;
	});

	if (i == m_pObservers.end())
	{
		Logger::GetInstance().LogWarning("Subject::RemoveObserver > No observer " + name + " found");
		return nullptr;
	}

	std::shared_ptr<Observer> pObserver = (*i).second;
	m_pObservers.erase(i);
	return pObserver;
}

void Subject::Notify(GameObject* pObject, const std::string& observername, ObservedEvent event)
{
	if (std::find_if(m_pObservers.begin(), m_pObservers.end(), 
		[&observername]
	(const std::pair<std::string, std::shared_ptr<Observer>>& p)
	{
		return p.first == observername;
	}) != m_pObservers.end())
	{
		m_pObservers[observername]->Notify(pObject, event);
	}
	else
	{
		Logger::GetInstance().LogWarning("Subject::Notify > No observer " + observername + " found");
	}
}

void Subject::Notify(GameObject* pObject, ObservedEvent event)
{
	std::for_each(m_pObservers.begin(), m_pObservers.end(), 
		[pObject, event]
	(const std::pair<std::string, std::shared_ptr<Observer>>& p)
	{
		p.second->Notify(pObject,event);
	});
}
