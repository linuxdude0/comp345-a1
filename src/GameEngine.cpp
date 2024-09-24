#include "GameEngine.h"


// -- constructor & destructor --
GameEngine::GameEngine(const std::string& map_name)
    : mCurrentState{CurrentState::START}, mMapFileName{map_name}
{
    mIsRunning = true;

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
    mCommandManager.addCommand("issue_orders",std::make_unique<issueOrderCommand>());
    mCommandManager.addCommand("exec_orders",std::make_unique<execOrdersCommand>());
    
    
    mCommandManager.addCommand("exit",std::make_unique<endGameCommand>());
}

// -- accessors & mutators --
Map& GameEngine::getMap() {return *mMap_ptr;}
Player& GameEngine::getPlayer() {return *mPlayer_ptr;}
CurrentState GameEngine::getState() {return mCurrentState;}
void GameEngine::setState(CurrentState state) {mCurrentState = state;}
const bool GameEngine::isRunning() const {return mIsRunning;}

// -- testing --
void GameEngine::testGameStates()
{
    while(isRunning())
    {
        userQuery(); // run user input 
    } 
}

void GameEngine::closeGame()
{
    // -- perform cleanup here --
}

void GameEngine::updateGame()
{
    // switch cases here for all of the game states
}

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
    while(isRunning())
    {
       userQuery(); 
    }
}
