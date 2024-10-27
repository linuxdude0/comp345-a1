#include "GameEngine.h"
#include "Map.h"
#include "Orders.h"
#include "Player.h"
#include "common.h"
#include <algorithm>
#include <cassert>
#include "Mappings.h"

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
    std::cout << "2.ADVANCE\n3.BOMB\n4.BLOCKADE\n5.AIRLIFT\n6.NEGOTIATE\n1.EXIT" << std::endl;
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
                    break;
                case(2):
                    return OrderKind::ADVANCE;
                    break;
                case(3):
                    return OrderKind::BOMB;
                    break;
                case(4):
                    return OrderKind::BLOCKADE;
                    break;
                case(5):
                    return OrderKind::AIRLIFT;
                    break;
                case(6):
                    return OrderKind::NEGOTIATE;
                    break;
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
        result.emplace(GameEngine::GAMESTART,"GAMESTART");               
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
GameEngine::GameEngine(const std::string map_name, int argc, char* argv[])
    : mCurrentState{CurrentState::START}, mMapFileName{map_name}
{
    mMap_ptr = nullptr;
    // push arguments to args list


    string file="-file";
    for(int i{0}; i<argc; ++i) {
        if (file == argv[i] && i+1<argc) {
            this->filename = argv[i+1];
        }
        mArgs.push_back(argv[i]);
    }

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
    delete mCommandProcessor_ptr;
    for(auto p : mPlayer_v)
        delete p;         
}

// -- initializer functions --
bool GameEngine::parseOptions(const std::string& s_option)
{
    auto it = std::find(mArgs.begin(),mArgs.end(), s_option);
    if(it!=std::end(mArgs))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void GameEngine::initializeStateCommands()
{
    stateCommandMap[START] = {"loadmap", "help", "quit"};
    stateCommandMap[MAP_LOADED] = {"validatemap", "help", "quit"};
    stateCommandMap[MAP_VALIDATED] = {"addplayer", "help", "quit"};
    stateCommandMap[PLAYERS_ADDED] = {"addplayer","assigncountries", "help", "quit"};
    stateCommandMap[GAMESTART] = {"gamestart", "help", "quit"}; // meow
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
    else if(mCurrentState == WIN)
    {
        mIsRunning = false;
    }
    else
        std::cout << "<No available commands in this state>" << std::endl;
}

// -- main functions --
void GameEngine::userQuery()
{
    mCommandProcessor_ptr->getCommand(*this);
}


void GameEngine::automaticQuery()
{
    // here is a small gift
    /*this->filename*/ // here ya go,  a filename for automaticQuery
    // -- read commands from file --
    // mCommandProcessor_ptr->
}

void GameEngine::reset()
{
    // -- in case user decides to replay the game --
    // free allocated memory 
    delete mMap_ptr;
    mPlayer_v.clear();     
}



void GameEngine::run()
{
    if(parseOptions("-console"))
    {
        while(isRunning())
        {
            userQuery();
        }
    }
    else if(parseOptions("-file"))
    {
        while(isRunning())
        {
            automaticQuery();
        }
    }
}

// -- game commands --
// after each command is run: switch the game's state (setState(<currentState>))

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
    for(int i{0}; i < n_playerCount; i++)
    {
        int nEndIndex = nStartIndex + nIndexesPerPlayer - 1;
        if(i<nRemainder)
            nEndIndex+=1;

        // create a temporary vector of territory index ranges
        std::vector<int> tempV;
        for(int i{0};i <= (nEndIndex - nStartIndex); i++)
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
    setState(GAMESTART);
}


// -- play phase --

// given that territories were already assigned elsewhere 
void GameEngine::gamestart(){

    

    std::cout << ">> Gamestart." << std::endl;
    
    // loops thru all the players in the main players vector
    for(Player* p : mPlayer_v){

        // adds 50 units to the reinforcement pool of each player, pool is initialized to 0 initially
        p->addToReinforcementPool(50);
        std::cout << ">> Player " << p->getName() << " just received 50 units." << std::endl;
        
        // adds 2 card to each player's hand
        p->getHand()->addCard(this->mDeck_ptr->draw());
        p->getHand()->addCard(this->mDeck_ptr->draw());
        std::cout << ">> Player " << p->getName() << " just received 2 cards" << std::endl;
        

        // for each player assign their owner territories in the global ownership map with current owned = 0, given no deployment yet
        // i=terrIndex, p = Player* , 0 = currently troops stationed
        for(int i : p->toDefend()){
            territory_owner_troops_mappings.push_back(std::make_tuple(i, p, 0));
        }

        setState(ASSIGN_REINFORCEMENTS); // switches to the main game state
        
    }

    // -- beginning of play phase --
        while(mCurrentState != WIN)
        {
            reinforcementPhase(); // 0. prompts each player in the lobby to issue their orders
            issueOrdersPhase(); // 1. prompts each player in the lobby to issue their orders and  end issuing orders and change state to execute orders
            executeOrdersPhase(); // 2. execute orders for each player in the lobby and end executing orders and change state to assign reinforcements or win, the win check is performed at this stage
            // issueOrder(); // 1. prompts each player in the lobby to issue their orders
            // endIssueOrders(); // 2. end issuing orders and change state to execute orders
            // execOrder(); // 3. execute orders for each player in the lobby
            // endExecOrders(); // 4. end executing orders and change state to assign reinforcements or win, the win check is performed at this stage
        
        }

}

unsigned chooseTerritory(Map m, OrderKind ok, Player* player, int src=-1) {
    assert(player);
    switch (ok) {
        case OrderKind::ADVANCE: {
            if (src == -1) {
                for(unsigned i: player->toDefend()) {
                    Map::Territory t = m.getTerritory(i);
                    unsigned num_troops = 0;
                    for (std::tuple<unsigned, Player*, unsigned> tuple : territory_owner_troops_mappings) {
                        unsigned terr = std::get<0>(tuple);
                        if (terr == i) {
                            num_troops = std::get<2>(tuple);
                            break;
                        }
                    }
                    std::cout << "\t" << t.index << ": " << t.name << " in continent " << m.continents[t.continent_index] << " with " << num_troops << " troops, owner: " << player->getName() << std::endl;
                }
            } else {
                Map::Territory s = m.getTerritory(src);
                for(unsigned i=0; i<s.num_adjacent_territories; i++) {
                    Map::Territory t = m.getTerritory(s.adjacent_territories_indexes[i]);
                    unsigned num_troops = 0;
                    Player* p = nullptr;
                    for (std::tuple<unsigned, Player*, unsigned> tuple : territory_owner_troops_mappings) {
                        unsigned terr = std::get<0>(tuple);
                        if (terr == i) {
                            num_troops = std::get<2>(tuple);
                            p = std::get<1>(tuple);
                            break;
                        }
                    }
                    std::cout << "\t" << t.index << ": " << t.name << " in continent " << m.continents[t.continent_index] << " with " << num_troops << " troops, owner: " << p->getName() << std::endl;
                }
            }
            int n = -1;
            do {
                std::cout << "Please write the territory number you choose: ";
                std::cin >> n;
                if ((src == -1 && !player->ownsTerritory(n)) || (src >= 0 && !m.isAdjacent(n, src))) {
                    std::cout << "Territory not found, please reenter\n";
                }
            } while((src == -1 && !player->ownsTerritory(n)) || (src >= 0 && !m.isAdjacent(n, src)));
            return (unsigned)n;
        }
            break;
        case OrderKind::DEPLOY: 
        case OrderKind::AIRLIFT:
        case OrderKind::BOMB:
        case OrderKind::BLOCKADE:
        case OrderKind::NEGOTIATE: {
            for(unsigned i: player->toDefend()) {
                Map::Territory t = m.getTerritory(i);
                unsigned num_troops = 0;
                for (std::tuple<unsigned, Player*, unsigned> tuple : territory_owner_troops_mappings) {
                    unsigned terr = std::get<0>(tuple);
                    if (terr == i) {
                        num_troops = std::get<2>(tuple);
                        break;
                    }
                }
                std::cout << "\t" << t.index << ": " << t.name << " in continent " << m.continents[t.continent_index] << " with " << num_troops << " troops, owner: " << player->getName() << std::endl;
            }
            int n = -1;
            do {
                std::cout << "Please write the territory number you choose: ";
                std::cin >> n;
                if (!player->ownsTerritory(n)) {
                    std::cout << "Territory not found, please reenter\n";
                }
            } while(!player->ownsTerritory(n));
            return (unsigned)n;
        }
            break;
        default:
            throw "huh????";
    }
    /*for (size_t i=0; i<m.num_territories; i++) {*/
    /*    Map::Territory t = m.getTerritory(i);*/
    /*    std::cout << "\t" << t.index << ": " << t.name << " in continent " << m.continents[t.continent_index] << std::endl;*/
    /*}*/
    /*int n = -1;*/
    /*do {*/
    /*    std::cout << "Please write the territory number you want to target: ";*/
    /*    std::cin >> n;*/
    /*    if (n < 0 || n >= (int)m.num_territories) {*/
    /*        std::cout << "Territory not found, please reenter\n";*/
    /*    }*/
    /*} while(n < 0 || n >= (int)m.num_territories);*/
    /*return (unsigned)n;*/
}

void GameEngine::reinforcementPhase()
{    
    // iterate over each player in player vector
    for(Player* const p : mPlayer_v)
    {
        // ask each player to enter order to issue of their choosing
        std::cout << "Deployment phase for Player:[" << p->getName() << "]" << std::endl;
        while (p->getReinforcementPool() > 0) {
            std::cout << "You need to deploy as you still have " << p->getReinforcementPool()<< " troops to deploy" << std::endl;
            std::cout << "Choose the target territory: \n";
            unsigned territory = chooseTerritory(*this->mMap_ptr, OrderKind::DEPLOY, p);
            int num_troops = -1;
            do {
                std::cout << "Please choose how many of the " << p->getReinforcementPool()<< " troops you want to deploy at " << this->mMap_ptr->getTerritory(territory).name << ": ";
                std::cin >> num_troops;
                if (num_troops < 1 || num_troops > p->getReinforcementPool()) {
                    std::cout << "Wrong number of troops, please reenter" << std::endl;
                }
            } while(num_troops < 1 && num_troops > p->getReinforcementPool());
            p->issueOrder(new DeployOrder(p, territory, num_troops));
            p->getOrderList()->executeAll();
        }
    }
        setState(ISSUE_ORDERS);
}

void GameEngine::issueOrder()
{
    OrderKind m_orderKind;
    for(Player* const p : mPlayer_v){

        do {
            std::cout << "[Player: " << p->getName() << "] is issuing orders" << std::endl;
            m_orderKind = prompt_order_kind("Enter the order to issue (1-6): ");
            Player* player = p;
            unsigned target = 0;
            unsigned source = 0;
            unsigned num_troops = 0;
            // ask questrions
            switch (m_orderKind) {
                case OrderKind::DEPLOY: // here deploy is exit
                    break;
                case OrderKind::ADVANCE:
                case OrderKind::AIRLIFT:
                    std::cout << "Choose source territory: " << std::endl;
                    source = chooseTerritory(*this->mMap_ptr, m_orderKind, p);
                    std::cout << "Choose target territory: " << std::endl;
                    target = chooseTerritory(*this->mMap_ptr, m_orderKind, p, source);
                    do {
                        std::cout << "Please the number of troops: ";
                        std::cin >> num_troops;
                        if (num_troops <= 0) {
                            std::cout << "wrong number of troops" << std::endl;
                        }
                    } while(num_troops <=0);
                    break;
                case OrderKind::BOMB:
                case OrderKind::BLOCKADE:
                case OrderKind::NEGOTIATE:
                    std::cout << "Choose target territory: " << std::endl;
                    target = chooseTerritory(*this->mMap_ptr, m_orderKind, p);
                    break;
                default:
                    throw "huh????";
            }

            switch (m_orderKind) {
                case OrderKind::DEPLOY: // here deploy is exit
                    break;
                case OrderKind::ADVANCE:
                    p->issueOrder(new AdvanceOrder(player, target, source, num_troops, mMap_ptr));
                    break;
                case OrderKind::AIRLIFT:
                    p->issueOrder(new AirliftOrder(player, target, source, num_troops));
                    break;
                case OrderKind::BOMB:
                    p->issueOrder(new BombOrder(player, target));
                    break;
                case OrderKind::BLOCKADE:
                    p->issueOrder(new BlockadeOrder(player, target));
                    break;
                case OrderKind::NEGOTIATE:
                    p->issueOrder(new NegotiateOrder(player, target));
                    break;
                default:
                    throw "huh????";
            }
        } while(m_orderKind != OrderKind::DEPLOY);
    }
}

void GameEngine::endIssueOrders()
{
    clear_extra();
    std::cout << ">> Finished issuing orders." << std::endl;
    setState(EXECUTE_ORDERS);
}

void GameEngine::issueOrdersPhase()
{
    issueOrder();
    endIssueOrders();
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
    if(allConqueredByOne()){
        win();
    }
    else{

        // TODO : need to remove players from main array mPlayer_v and destroy them if they got eliminated this turn (aka lost all terrs to others) if they lost the game
        
        // ^^ this needs to be done before filling the deployment pools, as the method uses that main v_player array

        fillPlayerReinforcementPools(); // fills the deployment pools in preparation for next phase;
        distributeCardsToWinners();
        setState(ASSIGN_REINFORCEMENTS);
    }
}

void GameEngine::executeOrdersPhase()
{

    execOrder();
    endExecOrders();
}


bool GameEngine::allConqueredByOne(){

    Player* firstPlayer = std::get<1>(territory_owner_troops_mappings[0]);
    for (const auto& entry : territory_owner_troops_mappings) {
        if (std::get<1>(entry) != firstPlayer) {
            return false; // found a different player
        }
    }

    return true; // all tuples have the same player
}


void GameEngine::fillPlayerReinforcementPools(){


    // for each player still in the main player array (not those who got eliminated)
    for(Player* const currPlayer : mPlayer_v){

        int total_fresh_troops = 0;
        int curr_owned_terrs = static_cast<int>(currPlayer->toDefend().size());
        total_fresh_troops += (curr_owned_terrs) / 3; //  (# of territories owned divided by 3, rounded down) as per assignment
        

        // TODO: (?) need a method from Map class that checks if a player owns a whole continent, if so returns the bonus for all continents owned
        // then we add this number to the fresh troops

        unsigned currOwned[MAX_TERRITORIES];
        for(std::vector<int>::size_type i = 0; i < currPlayer->toDefend().size(); ++i) {
            currOwned[i] = currPlayer->toDefend().at(i);
        }    

        // well - well, this is a question of principles, sig: unsigned getScore(size_t num_territories, unsigned territories[MAX_TERRITORIES]);
        int continental_bonus_score = mMap_ptr -> getScore(currPlayer->toDefend().size(), currOwned);

        total_fresh_troops += continental_bonus_score;
        
        if(total_fresh_troops < 3){ // "In any case the minimal number of reinforcement army units per turn for any player is 3." 
            total_fresh_troops = 3;
        }

        currPlayer->addToReinforcementPool(total_fresh_troops);
    }
}


void GameEngine::distributeCardsToWinners(){
// if a player managed to capture a territory during a turn, he gets a card from the deck (there's a flag raised in fight() method)
// a player can't receive more than 1!!! card per turn, no matter how many terrs he captured.
    for(Player* currPlayer : mPlayer_v){
        if(currPlayer->cardToReceiveThisTurn()){
            currPlayer->getHand()->addCard(mDeck_ptr->draw()); // draw a card and clear flag
            currPlayer->clearCardToIssueFlag(); // done
        }
    }
}
void GameEngine::win()
{
    setState(WIN);
    
    std::cout << ">> Play again? (replay/quit)" << std::endl;
    while(true)
    {
        std::string s_choice = prompt_for_string("Enter your choice: ");
        if(s_choice == "replay")
            replay();
        else if(s_choice == "quit")
            quit();
        else
        {
            std::cout << ">> [ERROR]: Invalid choice." << std::endl;
            continue;
        }
    }
}

void GameEngine::replay()
{
    setState(START);
    reset();
    run();    
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
    std::cout << "\tgamestart" << std::endl;
    std::cout << "\thelp" << std::endl;
}
