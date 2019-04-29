#pragma once

template <class T>
class Grid
{
public:
	explicit Grid()
		: m_Width(1)
		, m_Height(1)
	{
	}
	virtual ~Grid()
	{
		if (m_pValues) delete[] m_pValues;
		m_pValues = nullptr;
	}

	Grid(const Grid& other) = delete;
	Grid(Grid&& other) noexcept = delete;
	Grid& operator=(const Grid& other) = delete;
	Grid& operator=(Grid&& other) noexcept = delete;

	T& operator[](unsigned int i)
	{
		return m_pValues[i];
	}
	const T& operator[](unsigned int i) const
	{
		return m_pValues[i];
	}

	T* GetPtr(unsigned int w, unsigned int h) const
	{
		if (IsValid(w, h))
		{
			return &m_pValues[h*m_Width + w];
		}
		return nullptr;
	}
	T* GetPtr(unsigned int i) const
	{
		if (IsValid(i))
		{
			return &m_pValues[i];
		}
		return nullptr;
	}
	T GetVal(unsigned int w, unsigned int h, const T& base = T{})
	{
		auto* p = GetPtr(w, h);
		if (p == nullptr) return base;
		return *p;
	}
	T GetVal(unsigned int i, const T& base = T{})
	{
		auto* p = GetPtr(i);
		if (p == nullptr) return base;
		return *p;
	}

	bool IsValid(unsigned int w, unsigned int h) const
	{
		return (m_pValues != nullptr && w < m_Width && h < m_Height);
	}
	bool IsValid(unsigned int i) const
	{
		return (m_pValues != nullptr && i < m_Width*m_Height);
	}

	bool Find(const T& v, bool comp(const T&, const T&)) const
	{
		for (unsigned int h{ 0 }; h < m_Height; ++h)
		{
			for (unsigned int w{ 0 }; w < m_Width; ++w)
			{
				if (comp(v, *GetPtr(w, h)))
				{
					return true;
				}
			}
		}
		return false;
	}
	bool Find(const T& v) const
	{
		for (unsigned int h{ 0 }; h < m_Height; ++h)
		{
			for (unsigned int w{ 0 }; w < m_Width; ++w)
			{
				if (v == *GetPtr(w, h))
				{
					return true;
				}
			}
		}
		return false;
	}
	
	unsigned int FindIndex(const T& v) const
	{
		if (m_pValues == nullptr) return (m_Width*m_Height); //Invalid

		unsigned int i{ 0 };
		for (i; i < m_Width*m_Height; ++i)
		{
			if (*GetPtr(i) == v) return i;
		}
		return i;
	}
	unsigned int FindIndex(const T& v, bool comp(const T&, const T&)) const
	{
		if (m_pValues == nullptr) return (m_Width*m_Height); //Invalid

		unsigned int i{ 0 };
		for (i; i < m_Width*m_Height; ++i)
		{
			if (comp(*GetPtr(i), v)) return i;
		}
		return i;
	}

	unsigned int GetWidth() const { return m_Width; }
	unsigned int GetHeight() const { return m_Height; }
	void SetWidth(unsigned int w) { m_Width = w; }
	void SetHeight(unsigned int h) { m_Height = h; }
	bool IsMade() const { return m_pValues != nullptr; }

	void Make()
	{
		if (m_pValues == nullptr)
		{
			m_pValues = new T[m_Width*m_Height];
		}
	}

	
private:
	unsigned int m_Width;
	unsigned int m_Height;
	T* m_pValues;
};