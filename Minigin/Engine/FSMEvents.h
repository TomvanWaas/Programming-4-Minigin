#pragma once
#include "FSMEvent.h"

namespace Engine
{
	//Class Forward
	class GameObject;
	class BaseComponent;
	class SpriteComponent;

	//Classes
	class FSMDeleteGameObjectEvent final : public FSMEvent
	{
		GameObject* m_pObject;
	public:
		explicit FSMDeleteGameObjectEvent(GameObject* pObject);
		virtual ~FSMDeleteGameObjectEvent() = default;
		virtual void Execute(const SceneData& sceneData, FSMData& data) override;
	};

	class FSMComponentEnableEvent final : public FSMEvent
	{
	public:
		enum class Operation
		{
			SetTrue,
			SetFalse,
			Toggle
		};
		explicit FSMComponentEnableEvent(BaseComponent* pComponent, Operation operation);
		virtual ~FSMComponentEnableEvent() = default;
		virtual void Execute(const SceneData& sceneData, FSMData& data) override;
	private:
		BaseComponent* m_pComponent;
		Operation m_Operation;
	};

	class FSMSpriteSourceEvent final : public FSMEvent
	{
		unsigned int m_ID;
		SpriteComponent* m_pSpriteComponent;
	public:
		explicit FSMSpriteSourceEvent(unsigned int id, SpriteComponent* pSpriteComponent);
		virtual ~FSMSpriteSourceEvent() = default;
		virtual void Execute(const SceneData& sceneData, FSMData& data) override;
	};

	class FSMDoOnceEvent final : public FSMEvent
	{
		bool m_Done;
		std::shared_ptr<FSMEvent> m_pEvent;
	public:
		explicit FSMDoOnceEvent(const std::shared_ptr<FSMEvent>& pEvent);
		virtual ~FSMDoOnceEvent() = default;
		virtual void Initialize(const SceneData& sceneData, FSMData& data) override;
		virtual void Execute(const SceneData& sceneData, FSMData& data) override;
	};

	template <class T>
	class FSMSetDataEvent final : public FSMEvent
	{
		T* m_pData;
		T m_Value;
	public:
		explicit FSMSetDataEvent(T& reference, const T& target)
			: m_Value(target)
			, m_pData(&reference)
		{}
		virtual ~FSMSetDataEvent() = default;
		virtual void Execute(const SceneData& sceneData, FSMData& data) override
		{
			UNREFERENCED_PARAMETER(data);

			UNREFERENCED_PARAMETER(sceneData);
			if (m_pData) *m_pData = m_Value;
		}
	};
}







