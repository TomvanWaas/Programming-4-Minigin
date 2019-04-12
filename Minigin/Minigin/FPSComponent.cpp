#include "MiniginPCH.h"
#include "FPSComponent.h"
#include "TextComponent.h"
#include "GameObject.h"
#include "Logger.h"
#include "sstream"
#include "iomanip"
#include "SceneData.h"
#include "Time.h"

FPSComponent::FPSComponent()
	: BaseComponent()
	, m_Precision(2)
	, m_pTextComponent(nullptr)
{
}

void FPSComponent::Initialize(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);

	if (m_pGameObject != nullptr)
	{
		m_pTextComponent = m_pGameObject->GetComponent<TextComponent>();
		if (m_pTextComponent == nullptr)
		{
			Logger::GetInstance().LogWarning("FPSComponent::Initialize > Cannot find TextComponent");
		}
	}
}

void FPSComponent::Update(const SceneData& sceneData)
{
	if (m_pTextComponent != nullptr)
	{
		//Calc FPS
		float fps = 1 / sceneData.pTime->GetDeltaTime();
		std::stringstream ss{};
		ss << std::fixed << std::setprecision(m_Precision) << fps;
		m_pTextComponent->SetText("FPS:" + ss.str());
	}
}

void FPSComponent::ReInitialize(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);

	if (m_pGameObject != nullptr)
	{
		m_pTextComponent = m_pGameObject->GetComponent<TextComponent>();
		if (m_pTextComponent == nullptr)
		{
			Logger::GetInstance().LogWarning("FPSComponent::ReInitialize > Cannot find TextComponent");
		}
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
