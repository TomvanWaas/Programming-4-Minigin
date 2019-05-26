#include "stdafx.h"
#include "ResourceManager.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Exceptions.h"
#include "Texture2D.h"
#include "Font.h"
#include "RenderManager.h"

using namespace Engine;

ResourceManager::ResourceLoader<Texture2D>::ResourceLoader()
{
	//Load support for png and jpg
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		Logger::GetInstance().LogError("ResourceManager::Initialize > Failed to load support for png's");
		throw std::runtime_error(std::string("Failed to load support for png's: ") + SDL_GetError());
	}

	if ((IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG) != IMG_INIT_JPG)
	{
		Logger::GetInstance().LogError("ResourceManager::Initialize > Failed to load support for jpg's");
		throw std::runtime_error(std::string("Failed to load support for jpg's: ") + SDL_GetError());
	}
}
std::shared_ptr<Texture2D> ResourceManager::ResourceLoader<Texture2D>::GetResource(const std::string& path)
{
	auto i = m_pResources.find(path);
	if (i == m_pResources.end())
	{
		SDL_Texture *pTexture = IMG_LoadTexture(RenderManager::GetSDLRenderer(), path.c_str());
		if (pTexture == nullptr)
		{
			Logger::GetInstance().LogError("ResourceLoader<Texture2D>::GetResource > Failed to load Resource: " + path);
			throw ResourceException(path);
		}
		m_pResources[path] = std::make_shared<Texture2D>(pTexture);
	}
	return m_pResources[path];
}
const type_info& ResourceManager::ResourceLoader<Texture2D>::GetType() const
{
	return typeid(Texture2D);
}


ResourceManager::ResourceLoader<Font>::ResourceLoader()
{
	//Load support for Fonts
	if (TTF_Init() != 0)
	{
		Logger::GetInstance().LogError("ResourceManager::Initialize > Failed to load support for fonts");
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}
std::shared_ptr<Font> ResourceManager::ResourceLoader<Font>::GetResource(const std::string& path, unsigned size)
{
	std::pair<std::string, unsigned int> key(path, size);
	auto i = m_pResources.find(key);
	if (i == m_pResources.end())
	{
		m_pResources[key] = std::make_shared<Font>(path, size);
	}
	return m_pResources[key];
}
const type_info& ResourceManager::ResourceLoader<Font>::GetType() const
{
	return typeid(Font);
}






ResourceManager::~ResourceManager()
{
	for (auto*& pLoader : m_pLoaders)
	{
		SAFE_DELETE(pLoader);
	}
	m_pLoaders.clear();
}



void ResourceManager::SetPath(const std::string& path)
{
	std::stringstream p{};
	p << "../" << path;
	m_BasePath = p.str();
}

//Specialized
std::shared_ptr<Texture2D> ResourceManager::LoadTexture(const std::string& file)
{
	auto* pLoader = GetLoader<Texture2D>();
	return pLoader->GetResource(m_BasePath + file);
}
std::shared_ptr<Font> ResourceManager::LoadFont(const std::string& file, unsigned int size)
{
	auto* pLoader = GetLoader<Font>();
	return pLoader->GetResource(m_BasePath + file, size);
}
