#pragma once

namespace Minigin
{
	class ObservedEvent;
	class ObservedData;
	class Manager;
	class Time;
	class RenderManager;
	class InputManager;
	class CollisionManager;
	class Scene;
	class SceneData final
	{
	public:
		explicit SceneData(Scene* pScene)
			: m_pTimeManager{ nullptr }
			, m_pRenderManager{ nullptr }
			, m_pInputManager(nullptr)
			, m_pCollisionManager(nullptr)
			, m_Initialized(false)
			, m_pManagers()
			, m_pScene(pScene)
		{}
		~SceneData();

		SceneData(const SceneData& other) = delete;
		SceneData(SceneData&& other) noexcept = delete;
		SceneData& operator=(const SceneData& other) = delete;
		SceneData& operator=(SceneData&& other) noexcept = delete;

		void Initialize();
		void Update(float elapsed);
		void Notify(ObservedEvent event, const ObservedData& data);
		void LateInitialize();

		Time* GetTime() const { return m_pTimeManager; }
		RenderManager* GetRender() const { return m_pRenderManager; }
		InputManager* GetInput() const { return m_pInputManager; }
		CollisionManager* GetCollision() const { return m_pCollisionManager; }

		bool AddManager(Manager* pManager)
		{
			auto i = std::find(m_pManagers.begin(), m_pManagers.end(), pManager);
			if (i == m_pManagers.end())
			{
				m_pManagers.push_back(pManager);
				return true;
			}
			return false;
		}

		template<class T>
		T* GetManager() const
		{
			const type_info& info = typeid(T);
			for (Manager* pManager : m_pManagers)
			{
				if (pManager && info == typeid(*pManager))
				{
					return static_cast<T*>(pManager);
				}
			}
			return nullptr;
		}

		Scene* GetScene() const { return m_pScene; }

	private:
		bool m_Initialized = false;
		Time* m_pTimeManager;
		RenderManager* m_pRenderManager;
		InputManager* m_pInputManager;
		CollisionManager* m_pCollisionManager;
		std::vector<Manager*> m_pManagers;
		Scene* m_pScene;
	};

}