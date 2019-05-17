#pragma once
#include "ResourceManager.h"
#include "LevelData.h"

template <>
class ResourceManager::ResourceLoader<LevelData> final : public ResourceManager::Loader
{
	std::map<std::string, std::shared_ptr<LevelData>> m_pResources;
public:
	explicit ResourceLoader() = default;
	virtual ~ResourceLoader() = default;
	ResourceLoader(const ResourceLoader& other) = delete;
	ResourceLoader(ResourceLoader&& other) noexcept = delete;
	ResourceLoader& operator=(const ResourceLoader& other) = delete;
	ResourceLoader& operator=(ResourceLoader&& other) noexcept = delete;

	std::shared_ptr<LevelData> GetResource(const std::string& path);
	virtual const type_info& GetType() const override { return typeid(LevelData); }

};

