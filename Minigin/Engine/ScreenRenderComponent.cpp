#include "stdafx.h"
#include "ScreenRenderComponent.h"
#include "ResourceManager.h"
#include "SceneData.h"
#include "EngineObject.h"
#include "RenderManager.h"
#include "Texture2D.h"
#include "WindowSettings.h"

using namespace Engine;

void ScreenRenderComponent::InitializeOverride(const SceneData& sceneData)
{
	RegisterRenderable(sceneData.GetRender());
}

void ScreenRenderComponent::DestroyOverride(const SceneData& sceneData)
{
	UnRegisterRenderable(sceneData.GetRender());
}

void ScreenRenderComponent::Render(const RenderManager& renderer) const
{
	if (m_pTexture)
	{
		const auto& set = EngineObject::EngineObject::GetWindowSettings();
		Vector2 c{ set.width*0.5f, set.height*0.5f };


		if (m_FlipMode == FlipMode::None)
		{
			if (m_HasSource)
			{
				Vector2 s{ set.width / m_Source.width, set.height / m_Source.height };
				renderer.RenderTexture(*m_pTexture, c, s, m_Source);
			}
			else
			{
				Vector2 s{ set.width / m_pTexture->GetWidth(), set.height / m_pTexture->GetHeight() };
				renderer.RenderTexture(*m_pTexture, c, s);
			}
		}
		else
		{
			if (m_HasSource)
			{
				Vector2 s{ set.width / m_Source.width, set.height / m_Source.height };
				renderer.RenderTexture(*m_pTexture, c, s, m_Source, 0, c, m_FlipMode);
			}
			else
			{
				Vector2 s{ set.width / m_pTexture->GetWidth(), set.height / m_pTexture->GetHeight() };
				renderer.RenderTexture(*m_pTexture, c, s, 0, c, m_FlipMode);
			}
		}
	}
}

void ScreenRenderComponent::SetTexture(const std::string& filename)
{
	SetTexture(ResourceManager::GetInstance().LoadTexture(filename));
}
