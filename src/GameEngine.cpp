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

// -- initializer functions --
void GameEngine::initializeCommands()
{
    mCommandManager.addCommand("load_map",std::make_unique<loadMapCommand>());
    mCommandManager.addCommand("validate_map",std::make_unique<validateMapCommand>());
    mCommandManager.addCommand("add_player",std::make_unique<addPlayerCommand>());
    mCommandManager.addCommand("assign_countries",std::make_unique<assignCountriesCommand>());
    mCommandManager.addCommand("assign_reinforcements",std::make_unique<assignReinforcementCommand>());
}

// -- accessors & mutators --
Map& GameEngine::getMap() {return *mMap_ptr;}
Player& GameEngine::getPlayer() {return *mPlayer_ptr;}



// -- main functions --
void GameEngine::userQuery()
{
    std::string s_command{};
    while(std::getline(std::cin, s_command))
    {
        mCommandManager.execute(s_command,*this);
    }
}

void GameEngine::run()
{
    while(mIsRunning)
    {
       userQuery(); 
    }
}