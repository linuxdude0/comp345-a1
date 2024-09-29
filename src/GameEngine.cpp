#include "GameEngine.h"

void clear_extra()
{
  // ignores all of the characters up until newline
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int prompt_for_numeric(std::string message)
{
    while(1)
    {
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
std::string getPlayerInput(const std::string& message)
{
    std::string sInput;
    while(true)
    {
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

OrderKind prompt_order_kind(const std::string message)
{
    std::cout << "1.DEPLOY\n2.ADVANCE\n3.BOMB\n4.BLOCKADE\n5.AIRLIFT\n6.NEGOTIATE" << std::endl;
    while(1)
    {
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

// << operator overload with to print out states in std::string_view form
std::ostream& operator<<(std::ostream& out, const CurrentState value){
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
    delete mMap_ptr;

    for(auto p : mPlayer_v)
        delete p;
    
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
    std::mt19937_64 mt{std::random_device{}()}; // seeds generator
    std::uniform_int_distribution<int> distrib{1,42};
    //std::uniform_int_distribution<int> distrib{1,mMap_ptr->getTerritory(1).index};

    static int n_playerId{1};
    int n_territoryIndex{-1};
     
    Player *mPlayer_ptr;
    Deck *mDeck_ptr;
    std::string s_name = getPlayerInput(">> Enter name: ");
    n_territoryIndex = distrib(mt);
    std::cout << ">> Territory given: " << n_territoryIndex << std::endl;

    mDeck_ptr = new Deck();
    mPlayer_ptr = new Player(n_playerId,s_name,n_territoryIndex,mMap_ptr,mDeck_ptr);
    mPlayer_v.push_back(mPlayer_ptr);
    n_playerId ++;

    setState(PLAYERS_ADDED);
}

void GameEngine::assignCountries()
{
    std::cout << ">>[DEBUG]: Assigned countries." << std::endl;
    setState(ASSIGN_REINFORCEMENTS);
}


void GameEngine::issueOrder()
{
    OrderKind m_orderKind;
    for(auto const p : mPlayer_v)
    {
        std::cout << "" << std::endl;
        m_orderKind = prompt_order_kind("Enter the order to issue (1-6): ");
        p->issueOrder(m_orderKind);   
    }
    setState(ISSUE_ORDERS);
}

void GameEngine::endIssueOrders()
{
    std::cout << "Finished issuing orders." << std::endl;
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
