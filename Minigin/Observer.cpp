#include "MiniginPCH.h"
#include "Observer.h"
#include "Subject.h"
#include <algorithm>

Observer::~Observer()
{
	DestroyObserver();
}

void Observer::DestroyObserver()
{
	for (Subject* pSubject : m_pSubjects)
	{
		if (pSubject) pSubject->RemoveObserver(this);
	}
	m_pSubjects.clear();
}

void Observer::FilterSubjects()
{
	m_pSubjects.erase(std::remove_if(m_pSubjects.begin(), m_pSubjects.end(), [](Subject* pSubject)
	{
		return pSubject == nullptr;
	}), m_pSubjects.end());
}


bool Observer::AddSubject(Subject* pSubject)
{
	if (!pSubject) return false;
	auto i = std::find(m_pSubjects.begin(), m_pSubjects.end(), pSubject);
	if (i == m_pSubjects.end())
	{
		m_pSubjects.push_back(pSubject);
		pSubject->AddObserver(this);
		return true;
	}
	return false;
}
bool Observer::RemoveSubject(Subject* pSubject)
{
	if (!pSubject) return false;
	auto i = std::find(m_pSubjects.begin(), m_pSubjects.end(), pSubject);
	if (i != m_pSubjects.end())
	{
		(*i) = nullptr;
		pSubject->RemoveObserver(this);
		return true;
	}
	return false;
}
