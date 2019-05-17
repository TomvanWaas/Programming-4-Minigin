#pragma once

struct LevelData
{
	LevelData() = default;

	std::pair<int, int> size;
	std::vector<std::pair<int, int>> marked;
	std::vector<std::pair<int, int>> pooka;
	std::vector<std::pair<int, int>> fygar;
	std::vector<std::pair<int, int>> obstacle;
};