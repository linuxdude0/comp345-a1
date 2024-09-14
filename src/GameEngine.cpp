#include "GameEngine.h"


// -- constructor & destructor --
GameEngine::GameEngine(const std::string& map_name)
    : mCurrentState{CurrentState::STARTUP}, mMapFileName{map_name}
{
    mGameIsWon = false;

    // -- run all of the initializer functions --
    mPlayer = new Player(1,"-");

    loadMap();
    validateMap();
    addPlayer();
}

GameEngine::~GameEngine()
{
    // -- deallocate memory --
}

// -- accessors & mutators --
Map& GameEngine::getMap() {return *mMap;}



// -- main functions --
void GameEngine::userQuery()
{
    std::string s_command{};
    while(1)
    {
        std::getline(std::cin, s_command);

    }
}

void GameEngine::run()
{
    while(mIsRunning)
    {
        
    }
}