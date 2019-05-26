#pragma once
#include "Singleton.h"
#include <map>

namespace Engine
{
	class Texture2D;
	class Font;



	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		class Loader abstract
		{
		public:
			Loader() = default;
			virtual ~Loader() = default;
			virtual const type_info& GetType() const = 0;
		};
	private:
		template <class Return>
		class ResourceLoader final : public Loader
		{
		public:
			ResourceLoader() = default;
			~ResourceLoader() = default;
			ResourceLoader(const ResourceLoader& other) = delete;
			ResourceLoader(ResourceLoader&& other) noexcept = delete;
			ResourceLoader& operator=(const ResourceLoader& other) = delete;
			ResourceLoader& operator=(ResourceLoader&& other) noexcept = delete;
			virtual const type_info& GetType() const override { return typeid(Return); }
		private:
		};

		//Texture2D
		template <>
		class ResourceLoader<Texture2D> final : public Loader
		{
		public:
			ResourceLoader();
			~ResourceLoader() = default;
			ResourceLoader(const ResourceLoader& other) = delete;
			ResourceLoader(ResourceLoader&& other) noexcept = delete;
			ResourceLoader& operator=(const ResourceLoader& other) = delete;
			ResourceLoader& operator=(ResourceLoader&& other) noexcept = delete;
			std::shared_ptr<Texture2D> GetResource(const std::string& path);
			virtual const type_info& GetType() const override;
		private:
			std::map<std::string, std::shared_ptr<Texture2D>> m_pResources;
		};
		//Font
		template <>
		class ResourceLoader<Font> final : public Loader
		{
		public:
			ResourceLoader();
			~ResourceLoader() = default;
			ResourceLoader(const ResourceLoader& other) = delete;
			ResourceLoader(ResourceLoader&& other) noexcept = delete;
			ResourceLoader& operator=(const ResourceLoader& other) = delete;
			ResourceLoader& operator=(ResourceLoader&& other) noexcept = delete;
			std::shared_ptr<Font> GetResource(const std::string& path, unsigned int size);
			virtual const type_info& GetType() const override;
		private:
			std::map<std::pair<std::string, unsigned int>, std::shared_ptr<Font>> m_pResources;
		};




		//Variables
		std::string m_BasePath;
		std::vector<Loader*> m_pLoaders;

	public:
		ResourceManager() = default;
		~ResourceManager();

		template <class T>
		ResourceLoader<T>* GetLoader()
		{
			//Find existing loader
			const type_info& info = typeid(T);
			for (auto* pLoader : m_pLoaders)
			{
				if (pLoader && pLoader->GetType() == info)
				{
					return static_cast<ResourceLoader<T>*>(pLoader);
				}
			}
			//Create new loader
			ResourceLoader<T>* pNewLoader = new ResourceLoader<T>();
			m_pLoaders.push_back(pNewLoader);
			return pNewLoader;
		}

		void SetPath(const std::string& path);
		const std::string& GetBasePath() const { return m_BasePath; }

		std::shared_ptr<Texture2D> LoadTexture(const std::string& file);
		std::shared_ptr<Font> LoadFont(const std::string& file, unsigned int size);
	};

}

