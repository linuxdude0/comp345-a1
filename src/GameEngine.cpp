#include "GameEngine.h"

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


// -- constructor, copy constructor & destructor --
GameEngine::GameEngine(const std::string map_name)
    : mCurrentState{CurrentState::START}, mMapFileName{map_name}
{
    mIsRunning = true;

    // initialize commands
    initializeCommands();
}

GameEngine::GameEngine(const GameEngine& ge_obj)
    : mCurrentState{ge_obj.mCurrentState},mMapFileName{ge_obj.mMapFileName},
    mMap_ptr{ge_obj.mMap_ptr},mPlayer_v{ge_obj.mPlayer_v},commandMap{ge_obj.commandMap},
    mIsRunning{ge_obj.mIsRunning}
{

}

GameEngine::~GameEngine()
{
    delete mMap_ptr;

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
    commandMap["win"] = std::bind(&GameEngine::win, this);
    commandMap["play"] = std::bind(&GameEngine::play, this);
    commandMap["quit"] = std::bind(&GameEngine::quit, this);
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
    // switch cases here for all of the game states
}


void GameEngine::execute(const std::string& s_command_name)
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

// -- main functions --
void GameEngine::userQuery()
{
    // asks user to enter specific game engine commands
    // in order to switch states and perform game operations
    std::string sCommand;
    while(true)
    {
        std::cout << "Current state:[" << mCurrentState << "]" << std::endl;
        std::cout << ">>Enter command: ";
        std::getline(std::cin, sCommand);
        if(sCommand == "quit")
            mIsRunning = false;

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

void GameEngine::addPlayer()
{
    // create new player, associate random territory index and give player a Card Deck object

    std::mt19937_64 mt{std::random_device{}()}; // initialize mersenne twister generator
    std::uniform_int_distribution<int> distrib{0,41};
    //std::uniform_int_distribution<int> distrib{1,mMap_ptr->getTerritory(1).index};
    static int n_playerId{1};
    int n_territoryIndex{};
    int n_playerCount{};
    n_playerCount = prompt_for_numeric(">> Enter the number of players: ");
    for(int i{0}; i!=n_playerCount; ++i)
    {
        Player *mPlayer_ptr;
        Deck *mDeck_ptr;
        std::string s_name = prompt_for_string(">> Enter name: ");
        n_territoryIndex = distrib(mt); // assign random territory index (from 0-41)
        std::cout << ">> Territory given: " << n_territoryIndex << std::endl;

        mDeck_ptr = new Deck();
        mPlayer_ptr = new Player(n_playerId,s_name,n_territoryIndex,mMap_ptr,mDeck_ptr);
        mPlayer_v.push_back(mPlayer_ptr); // insert players into a vector container
        n_playerId ++;
    }
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

void GameEngine::win()
{

}

void GameEngine::play()
{

}

void GameEngine::quit()
{
    closeGame();
}