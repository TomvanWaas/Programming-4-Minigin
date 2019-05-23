#pragma once
#include "InputCommand.h"
#include "ObservedEvent.h"
#include "GameObject.h"

class SceneManager;
namespace DigDug
{
	enum class Direction;
	class DigDugMovementComponent;
	class ButtonManager;

	class PlayerInput final : public InputCommand
	{
		DigDug::Direction m_Direction;
		int m_PlayerId;
		Scene* m_pScene;
	public:
		explicit PlayerInput(DigDug::Direction d, int playerId, Scene* pScene);
		virtual void Execute() override;
	};

	template <class T>
	class InputSetData final : public InputCommand
	{
		T* m_pData;
		T m_Target;
	public:
		explicit InputSetData(T& ref, const T& val)
			: m_Target(val)
			, m_pData(&ref)
		{}
		virtual void Execute() override
		{
			if (m_pData != nullptr) *m_pData = m_Target;
		}
	};


	class InputButtonSet final : public InputCommand
	{
	public:
		enum class Action
		{
			Next,
			Previous,
			Select
		};
		explicit InputButtonSet(ButtonManager* pManager, Action action);
		virtual ~InputButtonSet() = default;
		virtual void Execute() override;

	private:
		ButtonManager* m_pManager;
		Action m_Action;
	};


	class PlayerNotifier final : public InputCommand
	{
	public:
		explicit PlayerNotifier(int playerId, ObservedEvent event, Scene* pScene)
			: m_PlayerId(playerId), m_Event(event), m_pScene(pScene)
		{}
		virtual ~PlayerNotifier() = default;
		virtual void Execute() override;
	private:
		ObservedEvent m_Event;
		int m_PlayerId;
		Scene* m_pScene;
	};
	class InputNotifier final : public InputCommand
	{
	public:
		explicit InputNotifier(GameObject* pObject, ObservedEvent event)
			: m_pObject(pObject), m_Event(event)
		{}
		virtual ~InputNotifier() = default;
		virtual void Execute() override;
	private:
		ObservedEvent m_Event;
		GameObject* m_pObject;
	};

	class  SceneSetInput final : public InputCommand
	{
	public:
		explicit SceneSetInput(const std::string& name, SceneManager* pManager)
			: m_SceneName(name)
			, m_pSceneManager(pManager)
		{}
		virtual ~SceneSetInput() = default;
		virtual void Execute() override;
	private:
		std::string m_SceneName;
		SceneManager* m_pSceneManager;
	};

}
















