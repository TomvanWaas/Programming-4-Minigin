#pragma once
#include <map>
class Scene;

template <class T>
class Manager abstract
{
public:
	explicit Manager() = default;
	virtual ~Manager() = default;

	Manager(const Manager& other) = delete;
	Manager(Manager&& other) noexcept = delete;
	Manager& operator=(const Manager& other) = delete;
	Manager& operator=(Manager&& other) noexcept = delete;

	static bool AddInstance(Scene* pScene, const T& v)
	{
		if (pScene != nullptr && m_Instances.find(pScene) == m_Instances.end())
		{
			m_Instances[pScene] = v;
			return true;
		}
		return false;
	}
	
	static T* GetInstance(Scene* pScene)
	{
		if (pScene != nullptr && m_Instances.find(pScene) != m_Instances.end())
		{
			return m_Instances[pScene];
		}
		return nullptr;
	}

private:
	static std::map<Scene*, T> m_Instances;

};