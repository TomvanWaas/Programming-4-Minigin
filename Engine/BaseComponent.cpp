#include "stdafx.h"
#include "BaseComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "ObservedData.h"

using namespace Minigin;

void BaseComponent::Notify(ObservedEvent event, const ObservedData& data)
{
	if (IsConstructed() && IsEnabled() && !IsDestroyed())
	{
		OnNotify(event, data);
	}
}
void BaseComponent::Initialize(const SceneData& sceneData)
{
	if (IsConstructed() == false)
	{
		SetConstructed(true);
		InitializeOverride(sceneData);
	}
}
void BaseComponent::UpdateFirst(const SceneData& sceneData)
{
	if (IsConstructed() && IsEnabled() && !IsDestroyed())
	{
		UpdateFirstOverride(sceneData);
	}
}
void BaseComponent::UpdateSecond(const SceneData& sceneData)
{
	if (IsConstructed() && IsEnabled() && !IsDestroyed())
	{
		UpdateSecondOverride(sceneData);
	}
}
void BaseComponent::Destroy(const SceneData& sceneData)
{
	if (IsDestroyed() == false)
	{
		ObservedData d{};
		d.AddData<BaseComponent*>("BaseComponent", this);
		Notify(ObservedEvent::Destroyed, d);

		SetDestroyed(true);
		DestroyOverride(sceneData);
	}
	DestroyObserver();
}



void BaseComponent::InitializeOverride(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);
}
void BaseComponent::UpdateFirstOverride(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);
}
void BaseComponent::UpdateSecondOverride(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);
}
void BaseComponent::DestroyOverride(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);
}
void BaseComponent::OnNotify(ObservedEvent event, const ObservedData& data)
{
	UNREFERENCED_PARAMETER(event);
	UNREFERENCED_PARAMETER(data);
}


const GameObject* BaseComponent::GetGameObject() const
{
	return m_pGameObject;
}
GameObject* BaseComponent::GetGameObject()
{
	return m_pGameObject;
}
void BaseComponent::SetGameObject(GameObject* pObject)
{
	m_pGameObject = pObject;
}



void BaseComponent::SetEnabled(bool e)
{
	if (e)
	{
		m_State = m_State | char(State::enabled);
	}
	else
	{
		m_State = m_State & ~char(State::enabled);
	}
}
bool BaseComponent::IsEnabled() const
{
	return (m_State & char(State::enabled));
}



bool BaseComponent::IsConstructed() const
{
	return (m_State & char(State::constructed));
}
void BaseComponent::SetConstructed(bool c)
{
	if (c)
	{
		m_State = m_State | char(State::constructed);
	}
	else
	{
		m_State = m_State & ~char(State::constructed);
	}
}
bool BaseComponent::IsDestroyed() const
{
	return (m_State & char(State::destroyed));
}
void BaseComponent::SetDestroyed(bool d)
{
	if (d)
	{
		m_State = m_State | char(State::destroyed);
	}
	else
	{
		m_State = m_State & ~char(State::destroyed);
	}
}