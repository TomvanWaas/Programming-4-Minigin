#pragma once
#include "BaseComponent.h"

template <class T>
class ValueGridComponent final : public BaseComponent
{
public:
	explicit ValueGridComponent(unsigned int w, unsigned int h, const T& base)
		: BaseComponent()
		, m_Width(w)
		, m_Height(h)
		, m_Base(base)
	{}
	~ValueGridComponent()
	{
		if (m_pValues != nullptr)
		{
			delete[] m_pValues;
			m_pValues = nullptr;
		}
	}

	ValueGridComponent(const ValueGridComponent& other) = delete;
	ValueGridComponent(ValueGridComponent&& other) noexcept = delete;
	ValueGridComponent& operator=(const ValueGridComponent& other) = delete;
	ValueGridComponent& operator=(ValueGridComponent&& other) noexcept = delete;

	void Initialize(const SceneData& sceneData) override
	{
		UNREFERENCED_PARAMETER(sceneData);

		if (m_pValues == nullptr)
		{
			m_pValues = new T[w*h];

			for (unsigned int i{ 0 }; i < w*h; ++i)
			{
				m_pValues[i] = base;
			}
		}
	}

	bool IsValid(unsigned int w, unsigned int h) const
	{
		return (w < m_Width && h < m_Height);
	}
	T Get(unsigned int w, unsigned int h) const
	{
		if (IsValid(w, h))
		{
			return m_pValues[h*m_Width + w];
		}
		return m_Base;
	}
	T& Get(unsigned int w, unsigned int h)
	{
		return Get(w, h);
	}

private:
	unsigned int m_Width;
	unsigned int m_Height;

	T* m_pValues;
	T m_Base;

};

