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
	testOrderLists();
	testLoadMaps("./maps/Atlantis.map");
	testLoadMaps("./maps/USA.map");
	testCard();

	Deck* deck = new Deck;
	testPlayers("./maps/Atlantis.map", deck);
	delete deck;
	GameEngine game("./maps/Atlantis.map");
	game.run();
	//testGameEngine("./maps/Atlantis.map");
	return 0;
}
