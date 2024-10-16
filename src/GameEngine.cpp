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
// -- end of helper functions --

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

std::ostream& operator<<(std::ostream& out, const GameEngine::CurrentState value)
{
    // << operator overload with to print out states in std::string_view form
    // lambda function that creates a map of states corresponding to their string-form representation
    static const auto strings = []{
        std::map<GameEngine::CurrentState, std::string_view> result;
        result.emplace(GameEngine::START,"START");     
        result.emplace(GameEngine::MAP_LOADED,"MAP_LOADED");     
        result.emplace(GameEngine::MAP_VALIDATED,"MAP_VALIDATED");     
        result.emplace(GameEngine::PLAYERS_ADDED,"PLAYERS_ADDED");             
        result.emplace(GameEngine::ASSIGN_REINFORCEMENTS,"ASSIGN_REINFORCEMENTS");             
        result.emplace(GameEngine::ISSUE_ORDERS,"ISSUE_ORDERS");             
        result.emplace(GameEngine::EXECUTE_ORDERS,"EXECUTE_ORDERS");             
        result.emplace(GameEngine::WIN,"WIN");             
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

    return *this;
}

// -- constructor, copy constructor & destructor --
GameEngine::GameEngine(const std::string map_name)
    : mCurrentState{CurrentState::START}, mMapFileName{map_name}
{
    mIsRunning = true;
    this->mDeck_ptr = new Deck();
    this->mCommandProcessor_ptr = new CommandProcessor();

    // initialize valid state commands
    initializeStateCommands();
}

GameEngine::GameEngine(const GameEngine& ge_obj)
    : mCurrentState{ge_obj.mCurrentState},mIsRunning{ge_obj.mIsRunning},mMapFileName{ge_obj.mMapFileName},
    mMap_ptr{ge_obj.mMap_ptr},mPlayer_v{ge_obj.mPlayer_v},mDeck_ptr{ge_obj.mDeck_ptr}
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
void GameEngine::initializeStateCommands()
{
    stateCommandMap[START] = {"loadmap", "help", "quit"};
    stateCommandMap[MAP_LOADED] = {"validatemap", "help", "quit"};
    stateCommandMap[MAP_VALIDATED] = {"addplayer", "help", "quit"};
    stateCommandMap[PLAYERS_ADDED] = {"addplayer","assigncountries", "help", "quit"};
    stateCommandMap[ASSIGN_REINFORCEMENTS] = {"issueorder", "help", "quit"};
    stateCommandMap[ISSUE_ORDERS] = {"issueorder", "endissueorders", "help", "quit"};
    stateCommandMap[EXECUTE_ORDERS] = {"execorders", "endexecorders", "help", "quit"};
    stateCommandMap[WIN] = {"win", "quit"}; 
}

// -- accessors & mutators --
Map& GameEngine::getMap() {return *mMap_ptr;}
Deck& GameEngine::getDeck() {return *mDeck_ptr;}
GameEngine::CurrentState GameEngine::getState() {return mCurrentState;}
void GameEngine::setState(CurrentState state) {mCurrentState = state;}
void GameEngine::setIsRunning(bool val) {mIsRunning = val;}
bool GameEngine::isRunning() {return mIsRunning;}
std::string GameEngine::getMapFileName() {return mMapFileName;}
std::vector<Player*>& GameEngine::getPlayersContainer() {return mPlayer_v;}
std::map<GameEngine::CurrentState, std::set<std::string>> GameEngine::getCommandMap() {return stateCommandMap;}

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
    mCommandProcessor_ptr->processUserInput(*this);
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
void GameEngine::loadMap(std::string map_filename)
{   
    // initialize Map object
    // remove potential whitespaces from map file name:
    map_filename.erase(std::remove_if(map_filename.begin(),map_filename.end(), ::isspace), map_filename.end());
    std::string map_filePath = "./maps/";
    map_filePath += map_filename; 
    std::cout << "map filename:" << map_filePath << std::endl;
    this->mMap_ptr = new Map(map_filePath); 
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


void GameEngine::addPlayer(const std::string& player_name)
{
    // create new player, associate random territory index and give player a Card Deck object

    if(!mPlayer_v.empty())
    {
        // adding more players to the match, clear all reserved territories to re-distribute them
        for(const auto& p : mPlayer_v)
            p->toDefend().clear(); // remove all territory indexes from vector
    }

    static int n_playerId{1};
    int n_territoryIndex{0};
    
    Player *mPlayer_ptr;
    std::string s_name = player_name;
    std::cout << ">> Territory given: " << n_territoryIndex << std::endl;
    mPlayer_ptr = new Player(n_playerId,s_name,n_territoryIndex,mMap_ptr,mDeck_ptr);
    mPlayer_v.push_back(mPlayer_ptr); // insert players into a vector container
    n_playerId ++;
    std::cout << "vector size: " << mPlayer_v.size() << std::endl;
    
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
    std::cout << "\tloadmap" << std::endl;
    std::cout << "\tvalidatemap" << std::endl;
    std::cout << "\taddplayer" << std::endl;
    std::cout << "\tassigncountries" << std::endl;
    std::cout << "\tissueorder" << std::endl;
    std::cout << "\tendissueorders" << std::endl; 
    std::cout << "\texecorders" << std::endl;
    std::cout << "\tendexecorders" << std::endl;
    std::cout << "\twin" << std::endl;
    std::cout << "\tplay" << std::endl;
    std::cout << "\tend" << std::endl;
    std::cout << "\tquit" << std::endl;
    std::cout << "\thelp" << std::endl;
}
