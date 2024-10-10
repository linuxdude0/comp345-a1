#include "GameEngine.h"
#include <algorithm>

// -- helper functions --
void clear_extra()
{
    // clear input buffer from all unwanted characters up until newline character
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int prompt_for_numeric(const std::string& message)
{
    // ask user for an integer input (with error checking)
    while(true)
    {
        // continue looping until user enters correct input
        int tEntry;
        std::cout << message;
        if(std::cin >> tEntry)
        {
            if(tEntry < 0)
            {
            std::cerr << "ERROR: Cannot be (less than) < 0. \n";
            continue;
        }
        clear_extra();
        return tEntry;
        } else {
            std::cerr << "ERROR: Invalid entry. \n";
            std::cin.clear();
            clear_extra();
            continue;
        }
    }
}
std::string prompt_for_string(const std::string& message)
{
    // asks user for a string input (with error checking)
    std::string sInput;
    while(true)
    {
        // continue looping until user enters correct input
        std::cout << message;
        std::getline(std::cin, sInput);

        if(std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << ">> ERROR: Invalid input, please try again." << std::endl;
        }
        else
            break;   
    }
    return sInput;
}

OrderKind prompt_order_kind(const std::string& message)
{
    // asks user to input number corresponding to an order
    std::cout << "1.DEPLOY\n2.ADVANCE\n3.BOMB\n4.BLOCKADE\n5.AIRLIFT\n6.NEGOTIATE" << std::endl;
    while(true)
    { 
        // continue looping until user enters correct input
        int tEntry;
        std::cout << message;
        if(std::cin >> tEntry)
        {
            switch(tEntry)
            {
                case(1):
                    return OrderKind::DEPLOY;
                case(2):
                    return OrderKind::ADVANCE;
                case(3):
                    return OrderKind::BOMB;
                case(4):
                    return OrderKind::BLOCKADE;
                case(5):
                    return OrderKind::AIRLIFT;
                case(6):
                    return OrderKind::NEGOTIATE;
                default:
                {
                    std::cout << "[WARNING]: Unknown order." << std::endl;
                    std::cin.clear();
                    clear_extra();
                    continue;
                }
            }   
        }
        else
        {
            std::cout << "[ERROR]: Invalid input." << std::endl;
            std::cin.clear();
            clear_extra();
        }
        clear_extra();
    }
}

// Game Engine's overload stream operator
std::ostream& operator<<(std::ostream& out, const GameEngine& ge)
{
    out << "-- Warzone Game Engine --\n"
    << "Map: " << ge.mMapFileName << "\n"
    << "Current state: " << ge.mCurrentState << "\n";

    return out; 
}

std::ostream& operator<<(std::ostream& out, const CurrentState value)
{
    // << operator overload with to print out states in std::string_view form
    // lambda function that creates a map of states corresponding to their string-form representation
    static const auto strings = []{
        std::map<CurrentState, std::string_view> result;
        result.emplace(START,"START");     
        result.emplace(MAP_LOADED,"MAP_LOADED");     
        result.emplace(MAP_VALIDATED,"MAP_VALIDATED");     
        result.emplace(PLAYERS_ADDED,"PLAYERS_ADDED");             
        result.emplace(ASSIGN_REINFORCEMENTS,"ASSIGN_REINFORCEMENTS");             
        result.emplace(ISSUE_ORDERS,"ISSUE_ORDERS");             
        result.emplace(EXECUTE_ORDERS,"EXECUTE_ORDERS");             
        result.emplace(WIN,"WIN");             
        return result;
    };
    // print out the string corresponding to the enum
    return out << strings()[value];
}

GameEngine& GameEngine::operator=(const GameEngine& ge)
{
    mCurrentState = ge.mCurrentState;
    mIsRunning = ge.mIsRunning;
    mMap_ptr = ge.mMap_ptr;
    mPlayer_v = ge.mPlayer_v;
    mDeck_ptr = ge.mDeck_ptr;
    commandMap = ge.commandMap;

    return *this;
}

// -- constructor, copy constructor & destructor --
GameEngine::GameEngine(const std::string map_name)
    : mCurrentState{CurrentState::START}, mMapFileName{map_name}
{
    mIsRunning = true;
    this->mDeck_ptr = new Deck();

    // initialize commands
    initializeCommands();
    initializeStateCommands();
}

GameEngine::GameEngine(const GameEngine& ge_obj)
    : mCurrentState{ge_obj.mCurrentState},mIsRunning{ge_obj.mIsRunning},mMapFileName{ge_obj.mMapFileName},
    mMap_ptr{ge_obj.mMap_ptr},mPlayer_v{ge_obj.mPlayer_v},mDeck_ptr(ge_obj.mDeck_ptr),commandMap{ge_obj.commandMap}
{

}

GameEngine::~GameEngine()
{
    delete mMap_ptr;
    delete mDeck_ptr;
    for(auto p : mPlayer_v)
        delete p;         
}

// -- initializer functions --
void GameEngine::initializeCommands()
{
    // adds member function to a map associated to a specific command
    // function will then later be called via provided function
    commandMap["load_map"] = std::bind(&GameEngine::loadMap, this);
    commandMap["validate_map"] = std::bind(&GameEngine::validateMap, this);
    commandMap["add_player"] = std::bind(&GameEngine::addPlayer, this);
    commandMap["assign_countries"] = std::bind(&GameEngine::assignCountries, this);
    commandMap["issue_order"] = std::bind(&GameEngine::issueOrder, this);
    commandMap["end_issue_orders"] = std::bind(&GameEngine::endIssueOrders, this); 
    commandMap["exec_orders"] = std::bind(&GameEngine::execOrder, this);
    commandMap["end_exec_orders"] = std::bind(&GameEngine::endExecOrders,this);
    commandMap["win"] = std::bind(&GameEngine::win, this);
    commandMap["play"] = std::bind(&GameEngine::play, this);
    commandMap["end"] = std::bind(&GameEngine::end, this);
    commandMap["quit"] = std::bind(&GameEngine::quit, this);
    commandMap["help"] = std::bind(&GameEngine::help, this);
}


void GameEngine::initializeStateCommands()
{
    stateCommandMap[START] = {"load_map", "help", "quit"};
    stateCommandMap[MAP_LOADED] = {"validate_map", "help", "quit"};
    stateCommandMap[MAP_VALIDATED] = {"add_player", "help", "quit"};
    stateCommandMap[PLAYERS_ADDED] = {"add_player","assign_countries", "help", "quit"};
    stateCommandMap[ASSIGN_REINFORCEMENTS] = {"issue_order", "help", "quit"};
    stateCommandMap[ISSUE_ORDERS] = {"issue_order", "end_issue_orders", "help", "quit"};
    stateCommandMap[EXECUTE_ORDERS] = {"exec_orders", "end_exec_orders", "help", "quit"};
    stateCommandMap[WIN] = {"win", "quit"}; 
}

// -- accessors & mutators --
Map& GameEngine::getMap() {return *mMap_ptr;}
CurrentState GameEngine::getState() {return mCurrentState;}
void GameEngine::setState(CurrentState state) {mCurrentState = state;}
bool GameEngine::isRunning() {return mIsRunning;}
CommandMap& GameEngine::getCommandMap() {return commandMap;}


void GameEngine::closeGame()
{
    // -- perform cleanup here --
    std::cout << ">> [DEBUG]: Quitting game" << std::endl;
    mIsRunning = false;

    std::exit(EXIT_SUCCESS);
}

void GameEngine::updateGame()
{
    auto it = stateCommandMap.find(mCurrentState);
    if(it != stateCommandMap.end())
    {
        std::cout << "Command(s):";
        for(const auto& cmd : it->second)
            std::cout << "[" << cmd << "] ";
        std::cout << std::endl;
    }
    else
        std::cout << "<No available commands in this state>" << std::endl;
}

// -- main functions --
void GameEngine::userQuery()
{
    // asks user to enter specific game engine commands
    // in order to switch states and perform game operations
    std::string sCommand;
    while(true)
    {
        updateGame();

        std::cout << "Current state:[" << mCurrentState << "]" << std::endl;
        std::cout << ">> Enter command: ";
        std::getline(std::cin, sCommand);
        if(sCommand == "quit")
            mIsRunning = false;
        if(std::cin.eof())
        {
            // handle case where user enters: "ctrl+d" (EOF) (bug made it so there was an infinite loop upon EOF)
            mIsRunning = false;
            closeGame();
        }
        if(std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << ">> [ERROR]: Invalid input, please try again." << std::endl;
        }
        std::cout << sCommand << std::endl; // !! test function output !! 
        execute(sCommand);
    }
}

void GameEngine::execute(const std::string& s_command_name)
{
    // check if entered command is valid in the current state
    auto stateIt = stateCommandMap.find(mCurrentState);
    if(stateIt != stateCommandMap.end())
    {
        const std::set<std::string>& validCommands = stateIt->second;
        if(validCommands.find(s_command_name) != validCommands.end())
        {
            // takes command entered by user as input 
            // check if bound command is in the map
            auto it = commandMap.find(s_command_name);
            if(it != commandMap.end())
            {
                // if command is found, call the bound function
                it->second();
            }
            else
            {
                std::cerr << "[ERROR]: Command not found." << std::endl;
            } 
        }
        else
        {
            std::cerr << "[ERROR]: Command is not valid in the current state." << std::endl;
        }
    }
    else
    {
        std::cerr << "[ERROR]: Current state is invalid." << std::endl;
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
// after each command is run: switch the game's state (setState(<currentState>))

// -- startup phase --
void GameEngine::loadMap()
{   
    // initialize Map object 
    this->mMap_ptr = new Map(mMapFileName); 
    setState(MAP_LOADED);
}

void GameEngine::validateMap()
{
    mMap_ptr->validate();
    setState(MAP_VALIDATED);
}

void GameEngine::distributeTerritories(int n_playerCount, int n_totalIndexes)
{
    // -- distribute territory indexes evenly to all players --
    int nIndexesPerPlayer = n_totalIndexes/n_playerCount;
    int nRemainder = n_totalIndexes%n_playerCount; 

    /*
    v_indexDistribution = 
    {
        {1,2,3,4,5,...},
        {11,12,13,14,...},
        {...}
    }
    */
    std::vector<std::vector<int>> v_indexDistribution;
    int nStartIndex = 0;
    for(int i{0}; i!=n_playerCount; ++i)
    {
        int nEndIndex = nStartIndex + nIndexesPerPlayer - 1;
        if(i<nRemainder)
            nEndIndex+=1;

        // create a temporary vector of territory index ranges
        std::vector<int> tempV;
        for(int i{0};i!=(nEndIndex-nStartIndex); ++i)
        {
            std::cout << ">> pushing index: " << nStartIndex+i << std::endl;
            tempV.push_back(nStartIndex+i); // push all indexes to temporary vector
        }
        v_indexDistribution.push_back(tempV); // insert vector of indexes to our main vector of vectors of index ranges
        nStartIndex = nEndIndex+1; // move on to the next range of indexes
    }


    // -- randomly distribute territory index ranges to each player --
    unsigned r_seed = std::chrono::system_clock::now().time_since_epoch().count(); // initialize seed
    std::shuffle(v_indexDistribution.begin(),v_indexDistribution.end(),std::default_random_engine(r_seed));
    for(std::size_t i{0}; i!=v_indexDistribution.size(); ++i)
    {
        mPlayer_v[i]->toDefend() = v_indexDistribution[i];    
    }

    for(const auto& p : mPlayer_v)
    {
        std::cout << p->getName() << std::endl;
            for(const auto& i : p->toDefend())
                std::cout << i << std::endl;
            std::cout << std::endl;
    }
    // -- [!!]TODO fix bug where each new range skips an index for some reason... --
}


void GameEngine::addPlayer()
{
    // create new player, associate random territory index and give player a Card Deck object

    if(!mPlayer_v.empty())
    {
        // adding more players to the match, clear all reserved territories to re-distribute them
        for(const auto& p : mPlayer_v)
            p->toDefend().clear(); // remove all territory indexes from vector
    }

    //std::uniform_int_distribution<int> distrib{1,mMap_ptr->getTerritory(1).index};
    static int n_playerId{1};
    int n_territoryIndex{0};
    int n_playerCount{0};
    n_playerCount = prompt_for_numeric(">> Enter the number of players: ");
    for(int i{0}; i!=n_playerCount; ++i)
    {
        Player *mPlayer_ptr;
        std::string s_name = prompt_for_string(">> Enter name: ");
        std::cout << ">> Territory given: " << n_territoryIndex << std::endl;
        mPlayer_ptr = new Player(n_playerId,s_name,n_territoryIndex,mMap_ptr,mDeck_ptr);
        mPlayer_v.push_back(mPlayer_ptr); // insert players into a vector container
        n_playerId ++;
    }
    distributeTerritories(mPlayer_v.size(),mMap_ptr->num_territories);
    setState(PLAYERS_ADDED);
}

void GameEngine::assignCountries()
{
    std::cout << ">> Assigned countries." << std::endl;
    setState(ASSIGN_REINFORCEMENTS);
}

void GameEngine::issueOrder()
{
    OrderKind m_orderKind;
    // iterate over each player in player vector
    for(auto const p : mPlayer_v)
    {
        // ask each player to enter order to issue of their choosing
        std::cout << "Player:[" << p->getName() << "]" << std::endl;
        m_orderKind = prompt_order_kind("Enter the order to issue (1-6): ");
        p->issueOrder(m_orderKind);   
    }
    setState(ISSUE_ORDERS);
}

void GameEngine::endIssueOrders()
{
    std::cout << ">> Finished issuing orders." << std::endl;
    setState(EXECUTE_ORDERS);
}

void GameEngine::execOrder()
{
    for(auto const p : mPlayer_v)
    {
        p->getOrderList()->executeAll();
    }
    setState(ASSIGN_REINFORCEMENTS);
}

void GameEngine::endExecOrders()
{
    std::cout << ">> Finished executing orders." << std::endl;
    setState(ASSIGN_REINFORCEMENTS);
}

void GameEngine::win()
{
    std::cout << ">> Play again? (play/end)" << std::endl;
    setState(WIN);
}

void GameEngine::play()
{
    // playGame();
    setState(START);    
}

void GameEngine::end()
{
    closeGame();
}

void GameEngine::quit()
{
    // -- if user enters the "quit" command --
    closeGame();
}


void GameEngine::help()
{
    std::cout << "Commands available:" << std::endl;
    std::cout << "\tload_map" << std::endl;
    std::cout << "\tvalidate_map" << std::endl;
    std::cout << "\tadd_player" << std::endl;
    std::cout << "\tassign_countries" << std::endl;
    std::cout << "\tissue_order" << std::endl;
    std::cout << "\tend_issue_orders" << std::endl; 
    std::cout << "\texec_orders" << std::endl;
    std::cout << "\tend_exec_orders" << std::endl;
    std::cout << "\twin" << std::endl;
    std::cout << "\tplay" << std::endl;
    std::cout << "\tend" << std::endl;
    std::cout << "\tquit" << std::endl;
    std::cout << "\thelp" << std::endl;
}
