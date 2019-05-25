#pragma once
#include "InputCommand.h"
#include "ObservedEvent.h"
#include "GameObject.h"

namespace Minigin
{
	class SceneManager;
}
namespace DigDug
{
	enum class Direction;
	class DigDugMovementComponent;
	class ButtonManager;

	class PlayerInput final : public Minigin::InputCommand
	{
		DigDug::Direction m_Direction;
		int m_PlayerId;
		Minigin::Scene* m_pScene;
	public:
		explicit PlayerInput(DigDug::Direction d, int playerId, Minigin::Scene* pScene);
		virtual void Execute() override;
	};

	template <class T>
	class InputSetData final : public Minigin::InputCommand
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


	class InputButtonSet final : public Minigin::InputCommand
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


	class PlayerNotifier final : public Minigin::InputCommand
	{
	public:
		explicit PlayerNotifier(int playerId, Minigin::ObservedEvent event, Minigin::Scene* pScene)
			: m_PlayerId(playerId), m_Event(event), m_pScene(pScene)
		{}
		virtual ~PlayerNotifier() = default;
		virtual void Execute() override;
	private:
		Minigin::ObservedEvent m_Event;
		int m_PlayerId;
		Minigin::Scene* m_pScene;
	};
	class InputNotifier final : public Minigin::InputCommand
	{
	public:
		explicit InputNotifier(Minigin::GameObject* pObject, Minigin::ObservedEvent event)
			: m_pObject(pObject), m_Event(event)
		{}
		virtual ~InputNotifier() = default;
		virtual void Execute() override;
	private:
		Minigin::ObservedEvent m_Event;
		Minigin::GameObject* m_pObject;
	};

	class  SceneSetInput final : public Minigin::InputCommand
	{
	public:
		explicit SceneSetInput(const std::string& name, Minigin::SceneManager* pManager)
			: m_SceneName(name)
			, m_pSceneManager(pManager)
		{}
		virtual ~SceneSetInput() = default;
		virtual void Execute() override;
	private:
		std::string m_SceneName;
		Minigin::SceneManager* m_pSceneManager;
	};





	class EnemyNotifier final : public Minigin::InputCommand
	{
	public:
		explicit EnemyNotifier(int enemyId, Minigin::ObservedEvent event, Minigin::Scene* pScene)
			: m_EnemyId(enemyId), m_Event(event), m_pScene(pScene)
		{}
		virtual ~EnemyNotifier() = default;
		virtual void Execute() override;
	private:
		Minigin::ObservedEvent m_Event;
		int m_EnemyId;
		Minigin::Scene* m_pScene;
	};


}
















