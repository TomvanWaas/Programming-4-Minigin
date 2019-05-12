#pragma once
#include "InputCommand.h"
#include "ObservedData.h"
#include "ObserverEvents.h"
#include "GameObject.h"


namespace DigDug
{
	enum class Direction;
	class DigDugMovementComponent;
	class ButtonManager;

	class PlayerInput final : public InputCommand
	{
		DigDug::Direction m_Direction;
		DigDug::DigDugMovementComponent* m_pMovement;
	public:
		explicit PlayerInput(DigDug::Direction d, DigDug::DigDugMovementComponent* pMove);
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


}
















