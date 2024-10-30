#include <iostream>
#include <ostream>
#include "Cards.h"
#include "Map.h"
#include "MapDriver.h"
#include "GameEngine.h"
#include "CardsDriver.h"
#include "Orders.h"
#include "PlayerDriver.h"
#include "GameEngineDriver.h"

int main(int argc, char* argv[]) {
	// std::string map_filenames[] = {
	// 	"./maps/Atlantis.map",
	// 	"./maps/Corruption.map",
	// 	"./maps/USA.map"
	// };
	// for (std::string f : map_filenames) {
	// 	testLoadMaps(f);
	// }
	// Deck* deck = new Deck;
	// testPlayers("./maps/Atlantis.map", deck);
	// delete deck;
	// testCard();
	// testOrderLists();

	GameEngine* game = new GameEngine("./maps/Atlantis.map", argc, (const char**)argv);
	game->run();
	delete game;
	//testGameEngine("./maps/Atlantis.map");
	return 0;
}
