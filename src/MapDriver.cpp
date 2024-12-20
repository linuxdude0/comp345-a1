#include "Map.h"
#include <iostream>

void testLoadMaps(std::string file) {
    std::cout << "==================TEST LOAD MAPS==================" << std::endl;
    try {
        Map m(file);
        Map map(&m);
        std::cout << map;
        std::cout << ((map.validate()) ? "true" : "false") << std::endl;
        std::cout << "this code to get all adjacent to territory 0" << std::endl;
        Map::Territory terr0 = map.getTerritory(0);
        for (size_t i=0; i<terr0.num_adjacent_territories; i++) {
            Map::Territory terr_adjacent = map.getTerritory(terr0.adjacent_territories_indexes[i]);
            std::cout << "\t" << terr_adjacent.name << " at " << map.getContinent(terr_adjacent.continent_index).name << std::endl;
        }
        unsigned terrs[MAX_TERRITORIES]{}; 
        for (size_t i=0; i<map.num_territories; i++) {
            terrs[i]=i;
        }
        unsigned sum = 0;
        for (size_t i=0; i<map.num_continents; i++) {
            sum+=map.scores_to_own_continent[i];
        }
        if (map.getScore(map.num_territories, terrs)!=sum) {
            std::cerr << map.getScore(map.num_territories, terrs) << "!=" << sum << std::endl;
        } else {
            std::cout << map.getScore(map.num_territories, terrs) << "==" << sum << std::endl;
        }
    } catch(char const* e) {
        std::cout << "Map error in file " << file << ": "<< e << std::endl;
    } catch(std::string e) {
        std::cout << "Map error in file " << file << ": "<< e << std::endl;
    }
    std::cout << "==================END TEST LOAD MAPS==================" << std::endl;
}
