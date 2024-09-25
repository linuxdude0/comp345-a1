#include "GameEngine.h"

void clear_extra()
{
  // ignores all of the characters up until newline
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

template <typename T>
T prompt_for_numeric(std::string message)
{
  while(1)
  {
    T tEntry;
    bool bIsNumeric {false};
    std::cout << message;
    if(std::cin >> tEntry)
    {
      bIsNumeric = true;
      if(tEntry < 0)
      {
        std::cerr << "ERROR: Cannot be (less than) < 0. \n";
        continue;
      }
      clear_extra();
      return tEntry;
    } else {
      bIsNumeric = false;
      std::cerr << "ERROR: Invalid entry. \n";
      std::cin.clear();
      clear_extra();
      continue;
    }
  }
}


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
GameEngine::GameEngine(const std::string map_name)
    : mCurrentState{CurrentState::START}, mMapFileName{map_name}
{
    mIsRunning = true;

    // initialize commands
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
    commandMap["assign_countries"] = std::bind(&GameEngine::assignCountries, this);
    commandMap["issue_order"] = std::bind(&GameEngine::issueOrder, this);
    commandMap["end_issue_orders"] = std::bind(&GameEngine::endIssueOrders, this); 
    commandMap["exec_orders"] = std::bind(&GameEngine::execOrder, this);
    commandMap["win"] = std::bind(&GameEngine::win, this);
    commandMap["play"] = std::bind(&GameEngine::play, this);
}

// -- accessors & mutators --
Map& GameEngine::getMap() {return *mMap_ptr;}
//Player& GameEngine::getPlayer() {return *mPlayer_ptr;}
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
    this->mMap_ptr = new Map(mMapFileName);
    
    // mMap_ptr = std::make_unique<Map>(mMapFileName);
    setState(MAP_LOADED);
}

void GameEngine::validateMap()
{
    mMap_ptr->validate();
    setState(MAP_VALIDATED);
}

void GameEngine::addPlayer()
{
    int n_playerId{1};
    int n_numberOfPlayers = prompt_for_numeric<int>(">> How many players? : ");
    for(int i{0}; i!=n_numberOfPlayers; ++i)
    {
        std::unique_ptr<Player> mPlayer_ptr;
        std::string s_name;
        std::cout << ">> Enter player name: ";
        std::cin >> s_name;

        // TODO: -- make player choose in which territory to start: --
        mPlayer_ptr = std::make_unique<Player>(n_playerId,s_name,1,*mMap_ptr);

        n_playerId ++;
    }

    setState(PLAYERS_ADDED);
}

void GameEngine::assignCountries()
{
    setState(ASSIGN_REINFORCEMENTS);
}

void GameEngine::issueOrder()
{

}

void GameEngine::endIssueOrders()
{

}

void GameEngine::execOrder()
{

}

void GameEngine::win()
{

}

void GameEngine::play()
{

}