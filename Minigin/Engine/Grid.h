#pragma once
namespace Minigin
{
	template <class T>
	class Grid
	{
	public:
		explicit Grid(unsigned int w, unsigned int h)
			: m_Width(w)
			, m_Height(h)
		{}
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



#include "Vector2.h"
	template<>
	class Grid<Vector2>
	{
	public:
		explicit Grid(unsigned int w, unsigned int h, const Vector2& offset = { 1,1 })
			: m_Width(w)
			, m_Height(h)
			, m_Offset(offset)
		{}
		explicit Grid()
			: m_Width(1)
			, m_Height(1)
			, m_Offset(1, 1)
			, m_IsMade(false)
		{}
		virtual ~Grid() = default;

		Grid(const Grid& other) = delete;
		Grid(Grid&& other) noexcept = delete;
		Grid& operator=(const Grid& other) = delete;
		Grid& operator=(Grid&& other) noexcept = delete;

		Vector2 operator[](unsigned int i)
		{
			return Get(i);
		}
		Vector2 Get(unsigned int w, unsigned int h) const
		{
			Vector2 r;
			r.x = float(w - (m_Width - 1) * 0.5f) * m_Offset.x;
			r.y = float(h - (m_Height - 1) * 0.5f) * m_Offset.y;
			return r;
		}
		Vector2 Get(unsigned int i) const
		{
			return Get(i%m_Width, i / m_Width);
		}

		bool IsValid(unsigned int w, unsigned int h) const
		{
			return (w < m_Width && h < m_Height);
		}
		bool IsValid(unsigned int i) const
		{
			return (i < m_Width*m_Height);
		}
		bool IsValid(const Vector2& v) const
		{
			return (v.x >= -1 * int(m_Width) * 0.5f * m_Offset.x &&
				v.x <= m_Width * 0.5f * m_Offset.x &&
				v.y >= -1 * int(m_Height) * 0.5f * m_Offset.y &&
				v.y <= m_Height * 0.5f * m_Offset.y);
		}

		bool Find(const Vector2& v, bool comp(const Vector2&, const Vector2&)) const
		{
			Vector2 p = v / m_Offset;
			return (IsValid(v) && comp(p, Vector2(std::round(p.x), std::round(p.y))));
		}
		bool Find(const Vector2& v) const
		{
			Vector2 p = v / m_Offset;
			return (IsValid(v) && p == Vector2(std::round(p.x), std::round(p.y)));
		}

		unsigned int FindIndex(const Vector2& v) const
		{
			if (IsValid(v) && Find(v))
			{
				Vector2 p = v / m_Offset;
				const unsigned int w = unsigned int(int(std::round(p.x)) + int(m_Width*0.5f));
				const unsigned int h = unsigned int(int(std::round(p.y)) + int(m_Height*0.5f));
				return (h*m_Width + w);
			}
			return (m_Width*m_Height);
		}
		unsigned int FindIndex(const Vector2& v, bool comp(const Vector2&, const Vector2&)) const
		{
			if (IsValid(v) && Find(v, comp))
			{
				Vector2 p = v / m_Offset;
				const unsigned int w = unsigned int(int(std::round(p.x) + int(m_Width*0.5f)));
				const unsigned int h = unsigned int(int(std::round(p.y) + int(m_Height*0.5f)));
				return (h*m_Width + w);
			}
			return (m_Width*m_Height);
		}


		bool IsOnPoint(const Vector2& p) const
		{
			return (ClosestPoint(p) == p);
		}
		bool IsOnLine(const Vector2& p) const
		{
			return (ClosestLine(p) == p);
		}
		bool IsOnGrid(const Vector2& p) const
		{
			return (ClosestGrid(p) == p);
		}
		bool IsOnPoint(const Vector2& p, bool comp(const Vector2&, const Vector2&)) const
		{
			return (comp(ClosestPoint(p), p));
		}
		bool IsOnLine(const Vector2& p, bool comp(const Vector2&, const Vector2&)) const
		{
			return (comp(ClosestLine(p), p));
		}
		bool IsOnGrid(const Vector2& p, bool comp(const Vector2&, const Vector2&)) const
		{
			return (comp(ClosestGrid(p), p));
		}
		Vector2 ClosestPoint(const Vector2& p) const
		{
			Vector2 rp = p;
			rp /= m_Offset;
			if (rp.x != 0.0f) rp.x += 0.5f*abs(rp.x) / rp.x;
			if (rp.y != 0.0f) rp.y += 0.5f*abs(rp.y) / rp.y;
			rp = Vector2(float(int(rp.x)), float(int(rp.y)));
			return (rp * m_Offset);
		}
		Vector2 ClosestLine(const Vector2& p) const
		{
			Vector2 cp = ClosestPoint(p);
			return ((p.x - cp.x)*(p.x - cp.x) >= (p.y - cp.y)*(p.y - cp.y)) ? Vector2(p.x, cp.y) : Vector2(cp.x, p.y);
		}
		Vector2 ClosestGrid(const Vector2& p) const
		{
			Vector2 topLeft = Get(0, 0);
			Vector2 bottomRight = Get(m_Width - 1, m_Height - 1);
			return Vector2((p.x < topLeft.x) ?
				topLeft.x : ((p.x > bottomRight.x) ?
					bottomRight.x : p.x),
					(p.y < topLeft.y) ?
				topLeft.y : ((p.y > bottomRight.y) ?
					bottomRight.y : p.y));
		}




		void SetOffset(const Vector2& o) { m_Offset = o; }
		const Vector2& GetOffset() const { return m_Offset; }
		unsigned int GetWidth() const { return m_Width; }
		unsigned int GetHeight() const { return m_Height; }
		void SetWidth(unsigned int w) { m_Width = w; }
		void SetHeight(unsigned int h) { m_Height = h; }
		bool IsMade() const { return m_IsMade; }
		void Make() { m_IsMade = true; }


	private:
		unsigned int m_Width;
		unsigned int m_Height;
		Vector2 m_Offset;
		bool m_IsMade = false;
	};
}

