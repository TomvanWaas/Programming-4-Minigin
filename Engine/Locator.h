#pragma once
namespace Minigin
{
	template <class T, class N>
	class Locator abstract
	{
	public:
		virtual ~Locator() = default;

		static void Initialize()
		{
			m_pService = &m_NullService;
		}
		static T& Get()
		{
			return *m_pService;
		}
		static void Register(T* service)
		{
			m_pService = (service == nullptr) ? &m_NullService : service;
		}
		static void Delete()
		{
			if (m_pService != nullptr && m_pService != &m_NullService)
			{
				delete m_pService;
				m_pService = &m_NullService;
			}
		}

	private:
		Locator() = default;

		static N m_NullService;
		static T* m_pService;

	};

	template <class T, class N>
	N Locator<T, N>::m_NullService{};
	template <class T, class N>
	T* Locator<T, N>::m_pService = nullptr;

}