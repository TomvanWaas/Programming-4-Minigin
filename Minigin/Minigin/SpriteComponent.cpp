#include "MiniginPCH.h"
#include "SpriteComponent.h"
#include "Logger.h"
#include "ResourceManager.h"
#include "RenderComponent.h"
#include "GameObject.h"
#include "SceneData.h"
#include "Time.h"
#include <algorithm>

SpriteComponent::SpriteComponent()
	: m_pTexture()
	, m_pSources()
	, m_pCurrentSource(nullptr)
	, m_CurrentRect(0)
	, m_SpriteSpeed(1.0f)
	, m_AccuTime(0.0f)
{
}
SpriteComponent::SpriteComponent(const std::shared_ptr<Texture2D>& pTexture, float speed)
	: m_pTexture()
	, m_pSources()
	, m_pCurrentSource(nullptr)
	, m_CurrentRect(0)
	, m_SpriteSpeed(speed)
	, m_AccuTime(0.0f)
{
	SetTexture(pTexture);
}
SpriteComponent::SpriteComponent(const std::string& path, float speed)
	: m_pTexture()
	, m_pSources()
	, m_pCurrentSource(nullptr)
	, m_CurrentRect(0)
	, m_SpriteSpeed(speed)
	, m_AccuTime(0.0f)
{
	SetTexture(path);
}


void SpriteComponent::Update(const SceneData& sceneData)
{
	UNREFERENCED_PARAMETER(sceneData);

	if (m_pCurrentSource != nullptr && m_pGameObject != nullptr)
	{
		RenderComponent* pRenderComp = m_pGameObject->GetComponent<RenderComponent>();
		if (pRenderComp != nullptr)
		{
			//Calc Source
			Rect source = m_pCurrentSource->sourceStart;
			source.x += m_CurrentRect * source.width;

			//Adopt RenderComponent
			pRenderComp->SetSource(source);
			pRenderComp->SetTexture(m_pTexture);

			//Next
			m_AccuTime += sceneData.pTime->GetDeltaTime();
			if (m_AccuTime >= m_SpriteSpeed)
			{
				m_AccuTime -= m_SpriteSpeed;
				(++m_CurrentRect) %= (m_pCurrentSource->nrSourcesHor);
			}
		}
	}
}

void SpriteComponent::SetTexture(const std::shared_ptr<Texture2D>& pTexture)
{
	m_pTexture = pTexture;
}
void SpriteComponent::SetTexture(const std::string& path)
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(path);
}
const std::shared_ptr<Texture2D>& SpriteComponent::GetTexture() const
{
	return m_pTexture;
}


void SpriteComponent::AddSprite(unsigned id, const SpriteSource& sprite)
{
	if (m_pSources.find(id) == m_pSources.end())
	{
		m_pSources.insert(std::pair<unsigned int, SpriteSource>(id, sprite));
		if (m_pCurrentSource == nullptr)
		{
			SetCurrentSource(id);
		}
	}
	else
	{
		Logger::GetInstance().LogWarning("SpriteComponent::AddSprite > Id already exists");
	}
}
void SpriteComponent::RemoveSprite(unsigned id)
{
	auto i = m_pSources.find(id);

	if (i != m_pSources.end())
	{
		m_pSources.erase(i);

		//If invalid CurrentSource
		if (m_pCurrentSource != nullptr && std::find_if(m_pSources.begin(), m_pSources.end(), [this](const std::pair<unsigned int, SpriteSource>& p)
		{
			return this->m_pCurrentSource == &p.second;

		}) != m_pSources.end())
		{
			Logger::GetInstance().LogWarning("SpriteComponent::RemoveSprite > Removed CurrentSource, Forced to nullptr");
			m_pCurrentSource = nullptr;
		}
	}
	else
	{
		Logger::GetInstance().LogWarning("SpriteComponent::RemoveSprite > Id doesn't exist");
	}
}
const SpriteSource* SpriteComponent::GetSprite(unsigned id) const
{
	auto i = m_pSources.find(id);

	if (i != m_pSources.end())
	{
		return &((*i).second);
	}
	else
	{
		Logger::GetInstance().LogWarning("SpriteComponent::GetSprite > Id doesn't exist");
		return nullptr;
	}
}


void SpriteComponent::SetCurrentSource(unsigned id)
{
	if (m_pSources.find(id) != m_pSources.end())
	{
		m_pCurrentSource = &m_pSources[id];
	}
	else
	{
		Logger::GetInstance().LogWarning("SpriteComponent::SetCurrentSource > Id doesn't exist");
	}
}
const SpriteSource* SpriteComponent::GetCurrentSource() const
{
	return m_pCurrentSource;
}


void SpriteComponent::SetSpriteSpeed(float speed)
{
	m_SpriteSpeed = speed;
}
float SpriteComponent::GetSpriteSpeed() const
{
	return m_SpriteSpeed;
}
