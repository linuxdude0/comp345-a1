#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "LoggingObserver.h"
#include "Map.h"
#include "Player.h"
#include "Cards.h"
#include "CommandProcessor.h"

#include <iostream>
#include <string>
#include <map>
#include <set>
#include <cctype>    

class GameEngine;
class CommandProcessor;
class FileCommandProcessorAdapter;

// Game Engine class:
//      - 

class GameEngine : public ILoggable, public Subject
{
    public:

        enum CurrentState
        {
            START,
            MAP_LOADED,
            MAP_VALIDATED,
            PLAYERS_ADDED,
            GAMESTART,
            ASSIGN_REINFORCEMENTS,
            ISSUE_ORDERS,
            EXECUTE_ORDERS,
            WIN
        };

        friend std::ostream& operator<<(std::ostream& out, const CurrentState value);
        
        // -- constructors & destructor --
        GameEngine(const std::string map_name, int argc, const char** argv);
        GameEngine(const GameEngine&);
        virtual ~GameEngine();

        // -- overloaded stream input function --
        friend std::ostream& operator<<(std::ostream& out, const GameEngine& ge);
        GameEngine& operator=(const GameEngine& ge);
        // -- accessors & mutators --
        Map& getMap();
        Deck& getDeck();
        Player& getPlayer();
        Player* getNeutralPlayer(); // New getter method
        std::string getMapFileName();
        std::string getCommandsFileName();
        void setIsRunning(bool val);
        bool isRunning();
        CurrentState getState();
        void transition(CurrentState state);
        std::vector<Player*>& getPlayersContainer();
        std::map<CurrentState, std::set<std::string>> getCommandMap();

        // -- initializer functions --
        bool parseOptions(const std::string& s_option);
        void initializeStateCommands();
        void distributeTerritories(int n_playerCount, int n_totalIndexes);
        
        // --  main functions --
        void userQuery();
        void automaticQuery();
        void updateGame();
        void execute(const std::string& s_command_name);

        void reset();
        void run(); // game loop
        void closeGame();
        
        // -- game commands --
        void loadMap(std::string map_filename);
        void validateMap();
        void addPlayer(const std::string& player_name);
        void assignCountries();
        void gamestart(); //meow meow meow
        void issueOrder();
        void execOrder();
        void endExecOrders();
        void endIssueOrders();
        void win();
        void replay();
        void quit();
        void help();
        void startupPhase(void);
        bool allConqueredByOne();
        void fillPlayerReinforcementPools();
        void distributeCardsToWinners();
        void kickLosers();
        Player* chooseAPlayerToTarget(Player*, GameEngine&);
        void clearNegotiationAgreements();

        // wrapper functions 
        void reinforcementPhase();
        void issueOrdersPhase();
        void executeOrdersPhase();


        // Log functions
        std::string stringToLog() override;


    private:
        CommandProcessor* mCommandProcessor_ptr;
        FileCommandProcessorAdapter* mFCPA_ptr;
        // -- logic --
        CurrentState mCurrentState;
        bool mIsRunning;
        // -- in-game objects --
        const std::string mMapFileName;
        Map* mMap_ptr;
        std::vector<Player*> mPlayer_v;
        Deck* mDeck_ptr;
        std::map<CurrentState, std::set<std::string>> stateCommandMap;
        std::vector<std::string> mArgs;
        std::string filename;
        Player* mNeutralPlayer;
};

void testAll(void);

#endif // !GAME_ENGINE_H

