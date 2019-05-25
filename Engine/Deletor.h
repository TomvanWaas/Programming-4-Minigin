#pragma once
namespace Minigin
{
	class Deletor final : public Singleton<Deletor>
	{
		class BDelete abstract
		{
		public:
			BDelete() = default;
			virtual ~BDelete() = default;
			virtual void DeleteStore() = 0;
		};
		template <class T>
		class TDelete final : public BDelete
		{
		public:
			virtual ~TDelete() = default;
			static TDelete<T>& GetInstance()
			{
				static TDelete<T> instance;
				return instance;
			}
			void StoreDelete(T* pObject)
			{
				if (pObject && std::find(m_pObjects.begin(), m_pObjects.end(), pObject) == m_pObjects.end())
				{
					m_pObjects.push_back(pObject);
				}
			}
			virtual void DeleteStore() override
			{
				for (T* pObject : m_pObjects)
				{
					SAFE_DELETE(pObject);
				}
				m_pObjects.clear();
			}
		private:
			TDelete() = default;
			std::vector<T*> m_pObjects;
		};

	public:
		~Deletor() = default;
		Deletor()
		{
			for (BDelete* pDeletor : m_pDeletors)
			{
				pDeletor->DeleteStore();
			}
			m_pDeletors.clear();
		}

		Deletor(const Deletor& other) = delete;
		Deletor(Deletor&& other) noexcept = delete;
		Deletor& operator=(const Deletor& other) = delete;
		Deletor& operator=(Deletor&& other) noexcept = delete;

		template <class T>
		void StoreDelete(T* pObject)
		{
			TDelete<T>::GetInstance().StoreDelete(pObject);
			if (std::find(m_pDeletors.begin(), m_pDeletors.end(), &TDelete<T>::GetInstance()) == m_pDeletors.end())
			{
				m_pDeletors.push_back(&TDelete<T>::GetInstance());
			}
		}

		void DeleteStore()
		{
			for (BDelete* pDeletor : m_pDeletors)
			{
				pDeletor->DeleteStore();
			}
			m_pDeletors.clear();
		}

	private:
		std::vector<BDelete*> m_pDeletors;
	};


}