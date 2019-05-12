#pragma once
#include "BaseComponent.h"
#include <map>


class DataComponent final : public BaseComponent
{
	class BaseData abstract
	{
	public:
		explicit BaseData() = default;
		virtual ~BaseData() = default;
	};

	template <class T>
	class Data final : public BaseData
	{
		T m_Data;
	public:
		explicit Data(const T& data)
			: m_Data(data)
			, BaseData()
		{}
		virtual ~Data() override = default;

		const T& GetData() const
		{
			return m_Data;
		}
		T& GetData()
		{
			return m_Data;
		}
		void SetData(const T& data)
		{
			m_Data = data;
		}
	};
	

public:
	explicit DataComponent() = default;
	virtual ~DataComponent() override
	{
		for (auto& data : m_pData)
		{
			SAFE_DELETE(data.second);
		}
	}

	DataComponent(const DataComponent& other) = delete;
	DataComponent(DataComponent&& other) noexcept = delete;
	DataComponent& operator=(const DataComponent& other) = delete;
	DataComponent& operator=(DataComponent&& other) noexcept = delete;

	template <class T>
	bool AddData(const std::string& dataname, const T& data)
	{
		auto i = m_pData.find(dataname);
		if (i == m_pData.end())
		{
			m_pData[dataname] = new Data<T>(data);
			return true;
		}
		return false;
	}

	//Makes the pointer point to data
	template <class T>
	bool GetData(const std::string& dataname, T*& pDataHolder) const
	{
		auto i = m_pData.find(dataname);
		if (i != m_pData.end())
		{
			Data<T>* pData = dynamic_cast<Data<T>*>((*i).second);
			if (pData != nullptr)
			{
				pDataHolder = &pData->GetData();
				return true;
			}
		}
		return false;
	}

	//Fills the value in with data
	template <class T>
	bool GetData(const std::string& dataname, T& dataHolder) const
	{
		auto i = m_pData.find(dataname);
		if (i != m_pData.end())
		{
			Data<T>* pData = dynamic_cast<Data<T>*>((*i).second);
			if (pData != nullptr)
			{
				dataHolder = pData->GetData();
				return true;
			}
		}
		return false;
	}

	template <class T>
	bool SetData(const std::string& dataname, const T& data)
	{
		T* pData = nullptr;
		GetData<T>(dataname, pData);
		if (pData != nullptr)
		{
			*pData = data;
			return true;
		}
		return false;
	}

	template <class T>
	T* FindFirstOf()
	{
		const type_info& ti = typeid(T);

		for (const auto& pData : m_pData)
		{
			if (pData.second != nullptr && typeid(*pData.second) == ti) return static_cast<T*>(pData.second);
		}
		return nullptr;	
	}
	


private:
	std::map<std::string, BaseData*> m_pData;
};

