#pragma once
#include <unordered_map>

namespace Engine
{
	class ObservedData final
	{
		class BData abstract
		{
		public:
			//Just to have a abstract class
			BData() = default;
			virtual ~BData() = default;
		};
		template <class T>
		class TData final : public BData
		{
		public:
			explicit TData(const T& d) : m_Data(d) {  }
			const T& GetData() const { return m_Data; }
			T& GetData() { return m_Data; }
		private:
			T m_Data;
		};
		std::unordered_map<std::string, BData*> m_pData;
	public:
		explicit ObservedData() = default;
		~ObservedData()
		{
			for (auto data : m_pData)
			{
				SAFE_DELETE(data.second);
			}
			m_pData.clear();
		}

		ObservedData(const ObservedData& other) = delete;
		ObservedData(ObservedData&& other) noexcept = delete;
		ObservedData& operator=(const ObservedData& other) = delete;
		ObservedData& operator=(ObservedData&& other) noexcept = delete;

		template <class T>
		bool GetData(const std::string& name, T& dataHolder) const
		{
			auto i = m_pData.find(name);
			if (i != m_pData.end())
			{
				TData<T>* pData = dynamic_cast<TData<T>*>(i->second);
				if (pData != nullptr)
				{
					dataHolder = pData->GetData();
					return true;
				}
			}
			return false;
		}

		template <class T>
		bool AddData(const std::string& name, const T& data)
		{
			auto i = m_pData.find(name);
			if (i == m_pData.end())
			{
				m_pData[name] = new TData<T>(data);
				return true;
			}
			return false;
		}

		template <class T>
		bool GetFirstOf(T& dataHolder) const
		{
			const type_info& ti = typeid(T);
			for (auto& pData : m_pData)
			{
				if (pData.second && ti == typeid(*pData.second))
				{
					TData<T>* pTData = dynamic_cast<TData<T>*>(pData.second);
					if (pTData != nullptr)
					{
						dataHolder = pTData->GetData();
						return true;
					}
				}
			}
			return false;
		}

	};
}