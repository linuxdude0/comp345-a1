#include <iostream>
#include <ostream>
#include "Map.h"
#include "GameEngine.h"
#include "CardsDriver.h"

int main() {
	try {
		Map map("Atlantis.map");
		std::cout << map;
		std::cout << ((map.validate()) ? "true" : "false") << std::endl;
		std::cout << "this code to get all adjacent to territory 0" << std::endl;
		Map::Territory terr0 = map.getTerritory(0);
		for (size_t i=0; i<terr0.num_adjacent_territories; i++) {
			Map::Territory terr_adjacent = map.getTerritory(terr0.adjacent_territories_indexes[i]);
			std::cout << "\t" << terr_adjacent.name << " at " << map.getContinent(terr_adjacent.continent_index).name << std::endl;
		}
	} catch(std::string e) {
		std::cout << e << std::endl;
	}

	/*GameEngine game("Atlantis.map");*/
	/*game.testGameStates();*/
	testOrdersLists();
	return 0;
}
