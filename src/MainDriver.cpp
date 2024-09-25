#include <iostream>
#include <ostream>
#include "Map.h"
#include "MapDriver.h"
#include "GameEngine.h"
#include "CardsDriver.h"
#include "Orders.h"

int main() {
	testOrderLists();
	testLoadMaps("./maps/Atlantis.map");
	testLoadMaps("./maps/USA.map");
	testCard();
	GameEngine game("./maps/Atlantis.map");
	game.testGameStates();


	return 0;
}
