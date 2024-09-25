#include "GameEngine.h"


std::ostream& operator<<(std::ostream& out, const CurrentState value){
    static const auto strings = []{
        std::map<CurrentState, std::string_view> result;
        result.emplace(START,"START");     
        result.emplace(MAP_LOADED,"MAP_LOADED");     
        result.emplace(PLAYERS_ADDED,"PLAYERS_ADDED");             
        result.emplace(ASSIGN_REINFORCEMENTS,"ASSIGN_REINFORCEMENTS");             
        result.emplace(ISSUE_ORDERS,"ISSUE_ORDERS");             
        result.emplace(EXECUTE_ORDERS,"EXECUTE_ORDERS");             
        result.emplace(WIN,"WIN");             
        return result;
    };
    return out << strings()[value];
}


// -- constructor & destructor --
GameEngine::GameEngine(const std::string& map_name)
    : mCurrentState{CurrentState::START}, mMapFileName{map_name}
{
    mIsRunning = true;

    // -- run all of the initializer functions --
    mMap_ptr = std::make_unique<Map>(mMapFileName);
    mPlayer_ptr = std::make_unique<Player>(1,"-",1,*mMap_ptr);
    initializeCommands();
}

GameEngine::~GameEngine()
{

}

// -- initializer functions --
void GameEngine::initializeCommands()
{
    commandMap["load_map"] = std::bind(&GameEngine::loadMap, this);
    commandMap["validate_map"] = std::bind(&GameEngine::validateMap, this);
    commandMap["add_player"] = std::bind(&GameEngine::addPlayer, this);
}

// -- accessors & mutators --
Map& GameEngine::getMap() {return *mMap_ptr;}
Player& GameEngine::getPlayer() {return *mPlayer_ptr;}
CurrentState GameEngine::getState() {return mCurrentState;}
void GameEngine::setState(CurrentState state) {mCurrentState = state;}
bool GameEngine::isRunning() {return mIsRunning;}

CommandMap& GameEngine::getCommandMap() {return commandMap;}

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


void GameEngine::execute(const std::string& s_command_name)
{
    // check if command is in the map
    auto it = commandMap.find(s_command_name);
    if(it != commandMap.end())
    {
        it->second();
    }
    else
    {
        std::cerr << "[ERROR]: Command not found." << std::endl;
    } 
}

// -- main functions --
void GameEngine::userQuery()
{
    std::string s_command;
    while(true)
    {
        std::cout << "Current state:[" << mCurrentState << "]" << std::endl;
        std::cout << ">>Enter command: ";
        std::getline(std::cin, s_command);
        if(s_command == "quit")
        {
            mIsRunning = false;
        }
        std::cout << s_command << std::endl;
        execute(s_command);
    }
}

void GameEngine::run()
{
    while(isRunning())
    {
       userQuery(); 
    }
}

// -- game commands --
void GameEngine::loadMap()
{
    //engine.getMap().loadMap();
    setState(MAP_LOADED);
    //TODO: see with prof if we can skip this or if we should make a separate function for its initialization
}

void GameEngine::validateMap()
{
    getMap().validate();
    setState(MAP_VALIDATED);
}

void GameEngine::addPlayer()
{
    setState(PLAYERS_ADDED);
}

void GameEngine::assignCountries()
{
    setState(ASSIGN_REINFORCEMENTS);
}
