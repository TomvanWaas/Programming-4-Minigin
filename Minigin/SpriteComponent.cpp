#include "MiniginPCH.h"
#include "SpriteComponent.h"
#include "Logger.h"
#include "ResourceManager.h"
#include "RenderComponent.h"
#include "GameObject.h"
#include "SceneData.h"
#include "Time.h"
#include <algorithm>


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
			current = amount;
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

SpriteComponent::SpriteComponent(const std::shared_ptr<Texture2D>& pTexture, float speed)
	: m_pTexture(pTexture)
	, m_SpriteSpeed(speed)
	, m_pCurrentSource(nullptr)
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
	if (m_pCurrentSource != nullptr && GetGameObject() != nullptr)
	{
		RenderComponent* pRenderComp = GetGameObject()->GetComponent<RenderComponent>();
		if (pRenderComp != nullptr)
		{			
			m_AccuTime += sceneData.pTime->GetDeltaTime();

			//Adopt RenderComponent
			pRenderComp->SetSource(m_pCurrentSource->GetSource(m_AccuTime, m_SpriteSpeed));
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
		if (m_pCurrentSource != nullptr && std::find_if(m_pSources.begin(), m_pSources.end(), [this](const std::pair<unsigned int, SpriteSource*>& p)
		{
			return (this->m_pCurrentSource) == (p.second);

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
		return ((*i).second);
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
		m_pCurrentSource = m_pSources[id];
		if (!m_pCurrentSource->looped)
		{
			m_AccuTime = 0.0f;
		}
	}
	else
	{
		Logger::GetInstance().LogWarning("SpriteComponent::SetCurrentSource > Id doesn't exist");
	}
}
Rect SpriteComponent::GetCurrentSource() const
{
	if (m_pCurrentSource != nullptr)
	{
		float time = m_AccuTime;
		return m_pCurrentSource->GetSource(time, m_SpriteSpeed);
	}
	return Rect{};
}


void SpriteComponent::SetSpriteSpeed(float speed)
{
	m_SpriteSpeed = speed;
}
float SpriteComponent::GetSpriteSpeed() const
{
	return m_SpriteSpeed;
}

