#include "stdafx.h"
#include "FPSComponent.h"
#include "TextComponent.h"
#include "GameObject.h"
#include "Logger.h"
#include "sstream"
#include "iomanip"
#include "SceneData.h"
#include "Time.h"

using namespace Minigin;

FPSComponent::FPSComponent(unsigned int prec)
	: m_Precision(prec)
	, m_pTextComponent(nullptr)
{
}

void FPSComponent::InitializeOverride(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);

	if (GetGameObject() != nullptr)
	{
		m_pTextComponent = GetGameObject()->GetComponent<TextComponent>();
		if (m_pTextComponent == nullptr)
		{
			Logger::GetInstance().LogWarning("FPSComponent::InitializeOverride > Cannot find TextComponent");
		}
	}
}

void FPSComponent::UpdateFirstOverride(const SceneData& sceneData)
{
	if (m_pTextComponent != nullptr)
	{
		//Calc FPS
		float fps = 1 / sceneData.GetTime()->GetDeltaTime();
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
