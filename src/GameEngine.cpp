#include "GameEngine.h"


// -- constructor & destructor --
GameEngine::GameEngine(const std::string& map_name)
    : mCurrentState{CurrentState::STARTUP}, mMapFileName{map_name}
{
    mGameIsWon = false;

    // -- run all of the initializer functions --
    mMap_ptr = std::make_unique<Map>(mMapFileName);
    mPlayer_ptr = std::make_unique<Player>(1,"-",1,*mMap_ptr);

}

GameEngine::~GameEngine()
{

}

// -- accessors & mutators --
Map& GameEngine::getMap() {return *mMap_ptr;}
Player& GameEngine::getPlayer() {return *mPlayer_ptr;}



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