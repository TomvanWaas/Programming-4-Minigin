#include "MiniginPCH.h"
#include "Button.h"
#include "SceneData.h"
#include "GameObject.h"
#include "TextComponent.h"

using namespace DigDug;
using namespace Minigin;

void ButtonManager::Next()
{
	if (m_pButtons.size() > 0 && m_CurrentID < m_pButtons.size())
	{
		if (m_pButtons[m_CurrentID]) m_pButtons[m_CurrentID]->SetState(ButtonComponent::ButtonState::None);
		++m_CurrentID;
		if (m_CurrentID >= m_pButtons.size()) m_CurrentID = 0;
		if (m_pButtons[m_CurrentID]) m_pButtons[m_CurrentID]->SetState(ButtonComponent::ButtonState::Hovered);
	}
}
void ButtonManager::Previous()
{
	if (m_pButtons.size() > 0 && m_CurrentID < m_pButtons.size())
	{
		if (m_pButtons[m_CurrentID]) m_pButtons[m_CurrentID]->SetState(ButtonComponent::ButtonState::None);
		--m_CurrentID;
		if (m_CurrentID >= m_pButtons.size()) m_CurrentID = unsigned int(m_pButtons.size() - 1);
		if (m_pButtons[m_CurrentID]) m_pButtons[m_CurrentID]->SetState(ButtonComponent::ButtonState::Hovered);
	}
}
void ButtonManager::Select()
{
	if (m_pButtons.size() > 0 && m_CurrentID < m_pButtons.size())
	{
		if (m_pButtons[m_CurrentID]) m_pButtons[m_CurrentID]->SetState(ButtonComponent::ButtonState::Selected);
	}
}

void ButtonManager::RegisterButton(ButtonComponent* pButton)
{
	if (pButton == nullptr) return;
	auto i = std::find(m_pButtons.begin(), m_pButtons.end(), pButton);
	if (i == m_pButtons.end())
	{
		m_pButtons.push_back(pButton);
		if (m_pButtons.size() == 1)
		{
			m_pButtons[0]->SetState(ButtonComponent::ButtonState::Hovered);
		}
	}
}
void ButtonManager::UnregisterButton(ButtonComponent* pButton)
{
	if (pButton == nullptr) return;
	auto i = std::find(m_pButtons.begin(), m_pButtons.end(), pButton);
	if (i != m_pButtons.end())
	{
		m_pButtons.erase(i);
	}
}



void ButtonComponent::InitializeOverride(const SceneData& sceneData)
{
	auto* bm = sceneData.GetManager<ButtonManager>();
	if (bm)
	{
		bm->RegisterButton(this);
	}

	if (GetGameObject())
	{
		auto* pText = GetGameObject()->GetComponent<TextComponent>();
		if (pText)
		{
			switch (m_State)
			{
			case ButtonState::None:
				pText->SetFont(m_pNoneFont);
				pText->SetColor(m_NoneColor);
				break;
			case ButtonState::Hovered:
				pText->SetFont(m_pHoverFont);
				pText->SetColor(m_HoverColor);
				break;
			case ButtonState::Selected:
				if (m_pAction) m_pAction->Execute();
				SetState(ButtonState::Hovered);
				break;
			}
		}
	}
}
void ButtonComponent::DestroyOverride(const SceneData& sceneData)
{
	auto* bm = sceneData.GetManager<ButtonManager>();
	if (bm)
	{
		bm->UnregisterButton(this);
	}
}


ButtonComponent::ButtonState ButtonComponent::GetState() const
{
	return m_State;
}
void ButtonComponent::SetState(ButtonState state)
{
	m_State = state;

	if (GetGameObject())
	{
		auto* pText = GetGameObject()->GetComponent<TextComponent>();
		if (pText)
		{
			switch (m_State)
			{
			case ButtonState::None:
				pText->SetFont(m_pNoneFont);
				pText->SetColor(m_NoneColor);
				break;
			case ButtonState::Hovered:
				pText->SetFont(m_pHoverFont);
				pText->SetColor(m_HoverColor);
				break;
			case ButtonState::Selected:
				if (m_pAction) m_pAction->Execute();
				SetState(ButtonState::Hovered);
				break;
			}
		}
	}
}
