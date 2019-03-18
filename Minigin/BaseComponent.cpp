#include "MiniginPCH.h"
#include "BaseComponent.h"
#include "GameObject.h"

void BaseComponent::Initialize()
{
}

void BaseComponent::Update()
{
}

void BaseComponent::Render() const
{
}

void BaseComponent::SetOwner(dae::GameObject* owner)
{
	if (m_pOwner == nullptr && owner != nullptr)
	{
		m_pOwner = owner;
	}
}
