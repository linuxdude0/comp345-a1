#include <iostream>
#include <ostream>
#include "Map.h"
#include "MapDriver.h"
#include "GameEngine.h"
#include "CardsDriver.h"
#include "Player.h"

int main() {
	testLoadMaps("./maps/Atlantis.map");
	testLoadMaps("./maps/USA.map");
	/*GameEngine game("Atlantis.map");*/
	/*game.testGameStates();*/
	testOrdersLists();
	return 0;
}
