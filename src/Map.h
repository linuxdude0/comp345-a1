#pragma once

#include <cstddef>
#include <ostream>
#include <string>

#define MAX_CONTINENTS 32
#define MAX_TERRITORIES 256
#define MAX_ADJACENT_TERRITORIES 10

class Map {
public:
	struct Continent {
		std::string name;
		unsigned score;
	};
	class Territory {
	public:
		unsigned index;
		std::string name;
		unsigned continent_index;
		unsigned pos_x;
		unsigned pos_y;
		unsigned adjacent_territories_indexes[MAX_ADJACENT_TERRITORIES];
		size_t num_adjacent_territories;
	};
	enum class ScrollDirection {
		NONE,
		HORIZONTAL,
		VERTICAL,
	} scroll;
	std::string author;
	bool warn;
	std::string image_filename;
	bool wrap;
	std::string continents[MAX_CONTINENTS];
	unsigned scores_to_own_continent[MAX_CONTINENTS];
	unsigned num_territories_per_continent[MAX_CONTINENTS];
	size_t num_continents = 0;
	Territory territories[MAX_TERRITORIES];
	size_t num_territories = 0;
	Map(std::string filename);
	Map(Map* map);
	bool validate();
	Territory getTerritory(unsigned index);
	Continent getContinent(unsigned index);
	unsigned getScore(size_t num_territories, unsigned territories[MAX_TERRITORIES]);
	bool isAdjacent(unsigned t1, unsigned t2);
	friend std::ostream& operator<<(std::ostream& os, const Map& map);
};
