#pragma once
#include <map>

namespace Engine
{
	class FSMData final
	{
	private:
		struct FSMBData abstract
		{
		};
		template <class T>
		struct FSMTData final : public FSMBData
		{
			explicit FSMTData(const T& data) : data(data) {}
			T data;
		};

		std::map<std::string, FSMBData*> m_pData;

	public:
		explicit FSMData() = default;
		~FSMData()
		{
			for (auto& data : m_pData)
			{
				SAFE_DELETE(data.second);
			}
			m_pData.clear();
		}

		FSMData(const FSMData& other) = delete;
		FSMData(FSMData&& other) noexcept = delete;
		FSMData& operator=(const FSMData& other) = delete;
		FSMData& operator=(FSMData&& other) noexcept = delete;


		template <class T>
		bool AddData(const std::string& dataName, const T& data)
		{
			if (m_pData.find(dataName) == m_pData.end())
			{
				m_pData[dataName] = new FSMTData<T>(data);
				return true;
			}
			return false;
		}
		template <class T>
		bool SetData(const std::string& dataName, const T& data)
		{
			auto i = m_pData.find(dataName);
			if (i != m_pData.end() && (*i).second)
			{
				FSMTData<T>* pData = static_cast<FSMTData<T>*>(i->second);
				if (pData)
				{
					pData->data = data;
					return true;
				}
			}
			return false;
		}
		template <class T>
		bool GetData(const std::string& dataName, T& dataHolder) const
		{
			auto i = m_pData.find(dataName);
			if (i != m_pData.end() && (*i).second)
			{
				FSMTData<T>* pData = static_cast<FSMTData<T>*>(i->second);
				if (pData)
				{
					dataHolder = pData->data;
					return true;
				}
			}
			return false;
		}
		template <class T>
		bool GetData(const std::string& dataName, T*& pDataHolder) const
		{
			auto i = m_pData.find(dataName);
			if (i != m_pData.end() && (*i).second)
			{
				FSMTData<T>* pData = static_cast<FSMTData<T>*>(i->second);
				if (pData)
				{
					pDataHolder = &pData->data;
					return true;
				}
			}
			return false;
		}

	};
}
