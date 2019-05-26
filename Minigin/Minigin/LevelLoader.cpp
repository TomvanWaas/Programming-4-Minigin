#include "MiniginPCH.h"
#include "LevelLoader.h"
#include "fstream"
#include "sstream"

using namespace Engine;
using namespace DigDug;

std::shared_ptr<LevelData> ResourceManager::ResourceLoader<LevelData>::GetResource(const std::string& path)
{
	auto i = m_pResources.find(path);
	if (i != m_pResources.end()) return i->second;

	try
	{
		//////Else Load
		std::shared_ptr<LevelData> data = std::make_shared<LevelData>();
		//Fill
		std::ifstream in;
		in.open(path);
		if (in.is_open())
		{
			std::string b{};

			//Read Grid Size
			std::getline(in, b, ':');
			data->size.first = std::stoi(b);
			std::getline(in, b, '\n');
			data->size.second = std::stoi(b);

			//Other
			while (in.eof() == false)
			{
				std::getline(in, b, '\n');
				if (in.eof()) break;
				int id = std::stoi(b);
				std::getline(in, b, '\n');
				int length = std::stoi(b);

				while (length > 0)
				{
					//Read Pair
					std::pair<int, int> p{};
					std::getline(in, b, ':');
					p.first = std::stoi(b);
					std::getline(in, b, '\n');
					p.second = std::stoi(b);

					//Store Pair
					switch (id)
					{
					case 0:
						data->pooka.push_back(p);
						break;
					case 1:
						data->fygar.push_back(p);
						break;
					case 2:
						data->obstacle.push_back(p);
						break;
					case 3:
						data->marked.push_back(p);
						break;
					default:
						break;
					}
					--length;
				}
			}
		}

		m_pResources[path] = data;
		return data;
	}
	catch (...)
	{
		Logger::GetInstance().LogError("ResourceManager::ResourceLoader<LevelData>::GetResource > Failed to Load: " + path);
	}
	return nullptr;
}
