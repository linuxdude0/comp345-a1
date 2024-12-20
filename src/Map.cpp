#include "Map.h"
#include <fstream>
#include <iostream>
#include <ostream>
#include <queue>
#include <stdexcept>
#include <string>
#include <vector>

Map::Map(Map* map) {
	this->author = map->author;
	this->warn = map->warn;
	this->image_filename = map->image_filename;
	this->wrap = map->wrap;
	this->scroll = map->scroll;
	this->num_territories = map->num_territories;
	this->num_continents = map->num_continents;
	for (size_t i=0; i<this->num_continents; i++) {
		this->num_territories_per_continent[i] = map->num_territories_per_continent[i];
		this->scores_to_own_continent[i] = map->scores_to_own_continent[i];
		this->continents[i] = map->continents[i];
	}
	for (size_t i=0; i<this->num_territories; i++) {
		this->territories[i] = map->territories[i];
	}
}

Map::Map(std::string filename) {
	std::ifstream file;
	file.open(filename);
	std::string line;
	enum class CurrentSection{
		NONE,
		MAP,
		CONTINENT,
		TERRITORIES,
	} current_section = CurrentSection::NONE;
	std::string territories_adjacent[MAX_ADJACENT_TERRITORIES][MAX_TERRITORIES];
	while (std::getline(file, line)) {
		if (line.length() == 1) {
			continue;
		}
		if (line[0] == '[') {
			if (line == "[Map]\r") {
				current_section = CurrentSection::MAP;
			} else if (line == "[Continents]\r") {
				current_section = CurrentSection::CONTINENT;
			} else if (line == "[Territories]\r") {
				current_section = CurrentSection::TERRITORIES;
			} else {
				throw "Invalid string in file: \"" + line + "\b\b\"";
			}
			continue;
		}
		switch (current_section) {
			case CurrentSection::MAP: {
				std::string field {};
				std::string value {};
				bool after_equal = false;
				for (char c : line) {
					if (c == '\r') {
						break;
					}
					if (c == '=') {
						after_equal = true;
						continue;
					}
					if (after_equal) {
						value+=c;
					} else {
						field+=c;
					}
				}
				if (field == "author") {
					this->author = field;
				} else if (field == "warn") {
					if (value == "yes") {
						this->warn = true;
					} else if (value == "no") {
						this->warn = false;
					} else {
						throw "warn value is wrong: " + value;
					}
				} else if (field == "image") {
					this->image_filename = value;
				} else if (field == "wrap") {
					if (value == "yes") {
						this->wrap = true;
					} else if (value == "no") {
						this->wrap = false;
					} else {
						throw "wrap value is wrong: " + value;
					}
				} else if (field == "scroll") {
					if (value == "horizontal") {
						this->scroll = ScrollDirection::HORIZONTAL;
					} else if (value == "vertical") {
						this->scroll = ScrollDirection::VERTICAL;
					} else if (value == "none") {
						this->scroll = ScrollDirection::NONE;
					} else {
						throw "scroll value is wrong: " + value;
					}
				}
			}
				break;
			case CurrentSection::CONTINENT: {
				std::string field {};
				std::string value {};
				bool after_equal = false;
				for (char c : line) {
					if (c == '\r') {
						break;
					}
					if (c == '=') {
						after_equal = true;
						continue;
					}
					if (after_equal) {
						value+=c;
					} else {
						field+=c;
					}
				}
				try {
					this->scores_to_own_continent[this->num_continents] = std::stoul(value);
				} catch (std::invalid_argument& e) {
					throw "wrong value: \"" + value + "\"";
				}
				this->num_territories_per_continent[this->num_continents] = 0;
				this->continents[this->num_continents++] = field;
			}
				break;
			case CurrentSection::TERRITORIES: {
				unsigned curr_index = 0;
				std::string x{};
				std::string y{};
				std::string territory_continent{};
				for (char c : line) {
					if (c == '\r') {
						break;
					}
					if (c == ',') {
						curr_index++;
						continue;
					}
					switch (curr_index) {
						case 0:
							this->territories[this->num_territories].name += c;
							break;
						case 1:
							x+=c;
							break;
						case 2:
							y+=c;
							break;
						case 3:
							territory_continent += c;
							break;
						default:
							territories_adjacent[curr_index-4][this->num_territories]+=c; 
							break;
					}
					if (curr_index == 1) {
						try {
							this->territories[this->num_territories].pos_x = std::stoul(x);
						} catch (std::invalid_argument& e) {
							throw "wrong value: \"" + x + "\"";
						}
					} else if (curr_index == 2) {
						try {
							this->territories[this->num_territories].pos_y = std::stoul(y);
						} catch (std::invalid_argument& e) {
							throw "wrong value: \"" + y + "\"";
						}
					}
				}
				curr_index++;
				bool found = false;
				for (size_t i=0; i<this->num_continents; i++) {
					if (territory_continent == this->continents[i]) {
						found = true;
						this->territories[this->num_territories].continent_index = i;
						this->num_territories_per_continent[i]++;
					}
				}
				if (!found) {
					throw "invalid continent: \"" + territory_continent + "\"";
				}
				this->territories[num_territories].num_adjacent_territories = curr_index-4;
				this->territories[num_territories].index = this->num_territories;
				this->num_territories++;
			}
				break;
			case CurrentSection::NONE:
				throw "no section found yet";
				break;
		}
	}
	for (size_t i=0; i<this->num_territories; i++) {
		/*std::cout << "Territory " << i << ": ";*/
		for (size_t j=0; j<this->territories[i].num_adjacent_territories; j++) {
			/*std::cout << territories_adjacent[j][i] << ", ";*/
			bool found = false;
			for (size_t k=0; k<this->num_territories; k++) {
				if (territories_adjacent[j][i] == this->territories[k].name) {
					found = true;
					this->territories[i].adjacent_territories_indexes[j] = k;
				}
			}
			if (!found) {
				throw "invalid adjacent territory: \"" + territories_adjacent[j][i] + "\"";
			}
		}
		/*std::cout << "\b\b\n";*/
	}
	file.close();
}

bool Map::validate() {
	if (this->num_territories) {
		std::queue<int> queue;
		bool visited[MAX_TERRITORIES]{};
		visited[0] = true;
		queue.push(0);
		int current = 0;
		size_t count = 1;
		while (!queue.empty()) {
			std::cout << current << ", ";
			current = queue.front();
			queue.pop();
			for (size_t i=0; i<this->territories[current].num_adjacent_territories; i++) {
				if (!visited[this->territories[current].adjacent_territories_indexes[i]]) {
					visited[this->territories[current].adjacent_territories_indexes[i]] = true;
					queue.push(this->territories[current].adjacent_territories_indexes[i]);
					count++;
				}
			}
		}
		std::cout << std::endl;
		if (count != this->num_territories) {
			return false;
		}
		for(size_t i=0; i<this->num_continents; i++) {
			std::queue<int> queue;
			/*bool visited[MAX_TERRITORIES]{};*/
			std::vector<int> continent_territories;
			/*unsigned continent_count = 0;*/
			for (size_t j=0; j<this->num_territories; j++) {
				if (this->territories[j].continent_index == i) {
					continent_territories.push_back(j);
					current = j;
				}
			}
			if (count != this->num_territories) {
				return false;
			}
		}
	}
	return true;
}

Map::Territory Map::getTerritory(unsigned index) {
	if (index >= this->num_territories) {
		throw "invalid territory index";
	}
	return this->territories[index];
}

Map::Continent Map::getContinent(unsigned index) {
	if (index >= this->num_continents) {
		throw "invalid continent index";
	}
	Continent continent = {
		.name = this->continents[index],
		.score = this->scores_to_own_continent[index],
	};
	return continent;
}

unsigned Map::getScore(size_t num_territories, unsigned territories[MAX_TERRITORIES]) {
	unsigned num_terr_per_continent[MAX_CONTINENTS]{};
	unsigned score = 0;
	for (size_t i=0; i<num_territories; i++) {
		Territory t = this->getTerritory(territories[i]);
		num_terr_per_continent[t.continent_index]++;
	}
#ifdef DEBUG
	std::cout << "-----------DEBUG------------" << std::endl;
#endif
	for (size_t i=0; i<MAX_CONTINENTS; i++) {
#ifdef DEBUG
		std::cout << num_terr_per_continent[i] << ":" << this->num_territories_per_continent[i] << std::endl;
#endif
		if(num_terr_per_continent[i] == this->num_territories_per_continent[i]) {
			score+=this->scores_to_own_continent[i];
		}
	}
#ifdef DEBUG
	std::cout << "-----------END DEBUG------------" << std::endl;
#endif
	return score;
}

bool Map::isAdjacent(unsigned t1, unsigned t2) {
	Territory t = this->getTerritory(t1);
	for (size_t i=0; i<t.num_adjacent_territories; i++) {
		if (t.adjacent_territories_indexes[i] == t2) {
			return true;
		}
	}
	return false;
}

std::ostream& operator<<(std::ostream& os, const Map& map) {
	os << "Author = " << map.author << std::endl;
	os << "Warn = " << map.warn << std::endl;
	os << "Image File = " << map.image_filename << std::endl;
	os << "wrap = " << map.wrap << std::endl;
	os << "Scroll = ";
	switch (map.scroll) {
		case Map::ScrollDirection::NONE:
			os << "NONE" << std::endl;
			break;
		case Map::ScrollDirection::HORIZONTAL:
			os << "HORIZONTAL" << std::endl;
			break;
		case Map::ScrollDirection::VERTICAL:
			os << "VERTICAL" << std::endl;
			break;
	}
	os << "Continents" << std::endl;
	for (size_t i=0; i<map.num_continents; i++) {
		os << "\t" << map.continents[i] << ": " << map.scores_to_own_continent[i] << std::endl;
	}
	os << "Territories" << std::endl;
	for (size_t i=0; i<map.num_territories; i++) {
		os << "\tindex = " << i << ", " << map.territories[i].name <<
			" Continent: " << map.territories[i].continent_index <<
			" Position = (" <<
			map.territories[i].pos_x  <<
			", " << map.territories[i].pos_y << "), Adjacent = ";
		for (size_t j=0; j<map.territories[i].num_adjacent_territories; j++) {
			os << map.territories[i].adjacent_territories_indexes[j] << ", ";
		}
		os << "\b\b" << std::endl;
	}
	return os;
}
