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

int main() {
	std::string map_filenames[] = {
		"./maps/Atlantis.map",
		"./maps/Corruption.map",
		"./maps/USA.map"
	};
	for (std::string f : map_filenames) {
		testLoadMaps(f);
	}
	Deck* deck = new Deck;
	testPlayers("./maps/Atlantis.map", deck);
	delete deck;
	testCard();
	testOrderLists();
	GameEngine game("./maps/Atlantis.map");
	game.run();
	//testGameEngine("./maps/Atlantis.map");
	return 0;
}
