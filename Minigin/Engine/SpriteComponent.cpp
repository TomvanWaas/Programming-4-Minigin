#include "stdafx.h"
#include "SpriteComponent.h"
#include "Logger.h"
#include "ResourceManager.h"
#include "RenderComponent.h"
#include "GameObject.h"
#include "SceneData.h"
#include "Time.h"
#include <algorithm>

using namespace Engine;

Rect TickSource::GetSource(float& accuTime, float speed)
{
	Rect src = start;
	if (amount <= 1) return src;
	int sign = abs(amount) / amount;
	int current = int(accuTime / speed);
	if (current >= amount * sign)
	{
		if (looped)
		{
			current = 0;
			accuTime -= amount * sign * speed;
		}
		else
		{
			current = amount-1;
			accuTime = amount * sign * speed;
		}
	}

	src.x += src.width * (current*sign) * int((flags & char(Flag::X)) != 0);
	src.y += src.height * (current*sign) * int((flags & char(Flag::Y)) != 0);
	src.width += src.width * (current*sign) * int((flags & char(Flag::W)) != 0);
	src.height += src.height * (current*sign) * int((flags & char(Flag::H)) != 0);
	return src;
}
Rect SweepSource::GetSource(float& accuTime, float speed)
{
	Rect src = start;
	if (amount == 0)return src;
	if (accuTime >= speed * amount)
	{
		if (looped)
		{
			accuTime -= speed * amount;
		}
		else
		{
			accuTime = speed * amount;
		}
	}

	float t = accuTime / (speed * amount);
	src.x = start.x * (1 - t) + end.x * t;
	src.y = start.y * (1 - t) + end.y * t;
	src.width = start.width * (1 - t) + end.width * t;
	src.height = start.height * (1 - t) + end.height * t;
	return src;
}
Rect FixedSource::GetSource(float& accuTime, float speed)
{
	UNREFERENCED_PARAMETER(speed);
	accuTime = 0.0f;
	return source;
}
Rect GrowSource::GetSource(float& accuTime, float speed)
{
	if (accuTime > sources.size() * speed)
	{
		if (looped) accuTime -= sources.size() * speed;
		else accuTime = sources.size() * speed;
	}
	int id = int(accuTime / speed);
	if (id >= 0 && id < int(sources.size()))
	{
		return sources[id];
	}
	return Rect{};
}





SpriteComponent::SpriteComponent(const std::shared_ptr<Texture2D>& pTexture, float speed)
	: m_pTexture(pTexture)
	, m_SpriteSpeed(speed)
	, m_pCurrentSprite(nullptr)
	, m_AccuTime(0)
	, m_pSources()
{
}

SpriteComponent::SpriteComponent(const std::string& path, float speed)
	: SpriteComponent(ResourceManager::GetInstance().LoadTexture(path), speed)
{
}




SpriteComponent::~SpriteComponent()
{
	for (auto& pSource: m_pSources)
	{
		SAFE_DELETE(pSource.second);
	}
}


void SpriteComponent::UpdateFirstOverride(const SceneData& sceneData)
{
	if (m_pCurrentSprite != nullptr && GetGameObject() != nullptr)
	{
		RenderComponent* pRenderComp = GetGameObject()->GetComponent<RenderComponent>();
		if (pRenderComp != nullptr)
		{			
			if (!m_IsFreezed) m_AccuTime += sceneData.GetTime()->GetDeltaTime();

			//Adopt RenderComponent
			m_CurrentSource = m_pCurrentSprite->GetSource(m_AccuTime, m_SpriteSpeed);
			pRenderComp->SetSource(m_CurrentSource);
			pRenderComp->SetTexture(m_pTexture);
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


void SpriteComponent::AddSprite(unsigned int id, SpriteSource* pSprite)
{
	if (m_pSources.find(id) == m_pSources.end())
	{
		m_pSources.insert(std::pair<unsigned int, SpriteSource*>(id, pSprite));
		if (m_pCurrentSprite == nullptr)
		{
			SetCurrentSprite(id);
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
		if (m_pCurrentSprite != nullptr && std::find_if(m_pSources.begin(), m_pSources.end(), [this](const std::pair<unsigned int, SpriteSource*>& p)
		{
			return (this->m_pCurrentSprite) == (p.second);

		}) != m_pSources.end())
		{
			Logger::GetInstance().LogWarning("SpriteComponent::RemoveSprite > Removed CurrentSource, Forced to nullptr");
			m_pCurrentSprite = nullptr;
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
		return ((*i).second);
	}
	else
	{
		Logger::GetInstance().LogWarning("SpriteComponent::GetSprite > Id doesn't exist");
		return nullptr;
	}
}


void SpriteComponent::SetCurrentSprite(unsigned id, bool force)
{
	if (m_pSources.find(id) != m_pSources.end())
	{
		m_pCurrentSprite = m_pSources[id];
		if (!m_pCurrentSprite->looped)
		{
			m_AccuTime = 0.0f;
		}
		if (GetGameObject() && force)
		{
			RenderComponent* pRenderComp = GetGameObject()->GetComponent<RenderComponent>();
			if (pRenderComp)
			{
				m_CurrentSource = m_pCurrentSprite->GetSource(m_AccuTime, m_SpriteSpeed);
				pRenderComp->SetSource(m_CurrentSource);
			}
		}
	}
	else
	{
		Logger::GetInstance().LogWarning("SpriteComponent::SetCurrentSprite > Id doesn't exist");
	}
}



void SpriteComponent::SetSpriteSpeed(float speed)
{
	m_SpriteSpeed = speed;
}
float SpriteComponent::GetSpriteSpeed() const
{
	return m_SpriteSpeed;
}

