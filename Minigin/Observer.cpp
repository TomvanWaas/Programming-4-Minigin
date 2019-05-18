#include "MiniginPCH.h"
#include "Observer.h"
#include "Subject.h"

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
}


bool Observer::AddSubject(Subject* pSubject)
{
	auto i = std::find(m_pSubjects.begin(), m_pSubjects.end(), pSubject);
	if (i == m_pSubjects.end())
	{
		m_pSubjects.push_back(pSubject);
		return true;
	}
	return false;
}
bool Observer::RemoveSubject(Subject* pSubject)
{
	auto i = std::find(m_pSubjects.begin(), m_pSubjects.end(), pSubject);
	if (i != m_pSubjects.end())
	{
		m_pSubjects.erase(i);
		return true;
	}
	return false;
}
