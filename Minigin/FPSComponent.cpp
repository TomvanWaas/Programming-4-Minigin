#include "MiniginPCH.h"
#include "FPSComponent.h"
#include "TextComponent.h"
#include "GameObject.h"
#include "Time.h"
#include "Logger.h"
#include "sstream"
#include "iomanip"

FPSComponent::FPSComponent()
	: BaseComponent()
	, m_pTextComponent(nullptr)
	, m_Precision(2)
{
}

void FPSComponent::Initialize()
{
	if (m_pOwner != nullptr)
	{
		m_pTextComponent = m_pOwner->GetComponent<TextComponent>(false);
	}
	if (m_pTextComponent == nullptr)
	{
		Logger::GetInstance().LogWarning("FPSComponent cannot find TextComponent");
	}
}

void FPSComponent::Update()
{
	//Set value
	if (m_pTextComponent != nullptr)
	{
		//Calc FPS
		float fps = 1 / Time::GetInstance().GetDeltaTime();
		std::stringstream ss{};
		ss << std::fixed << std::setprecision(m_Precision) << fps;
		m_pTextComponent->SetText("FPS:" + ss.str());
	}
}

void FPSComponent::SetPrecision(unsigned int prec)
{
	m_Precision = prec;
}

unsigned int FPSComponent::GetPrecision() const
{
	return m_Precision;
}
