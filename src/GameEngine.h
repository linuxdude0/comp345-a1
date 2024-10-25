#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "Map.h"
#include "Player.h"
#include "Cards.h"
#include "CommandProcessor.h"

#include <iostream>
#include <string>
#include <string_view>
#include <map>
#include <set>
#include <unordered_map>
#include <functional>
#include <memory>
#include <limits>
#include <random>
#include <ranges>
#include <chrono>
#include <algorithm> 
#include <cctype>    

class GameEngine;
class CommandProcessor;

// Game Engine class:
//      - 

class GameEngine
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
        GameEngine(const std::string map_name, int argc, char* argv[]);
        GameEngine(const GameEngine&);
        ~GameEngine();

        // -- overloaded stream input function --
        friend std::ostream& operator<<(std::ostream& out, const GameEngine& ge);
        GameEngine& operator=(const GameEngine& ge);
        // -- accessors & mutators --
        Map& getMap();
        Deck& getDeck();
        Player& getPlayer();
        void setIsRunning(bool val);
        bool isRunning();
        CurrentState getState();
        std::string getMapFileName();
        std::vector<Player*>& getPlayersContainer();
        void setState(CurrentState state);
        std::map<CurrentState, std::set<std::string>> getCommandMap();

        // -- initializer functions --
        void initializeStateCommands();
        void distributeTerritories(int n_playerCount, int n_totalIndexes);
        
        // --  main functions --
        void userQuery();
        void updateGame();
        void execute(const std::string& s_command_name);

        void run(); // game loop
        void closeGame();
        
        // -- game commands --
        void loadMap(std::string map_filename);
        void validateMap();
        void addPlayer(const std::string& player_name);
        void assignCountries();
        void gamestart(); //meow
        void issueOrder();
        void execOrder();
        void endExecOrders();
        void endIssueOrders();
        void win();
        void play();
        void end();
        void quit();
        void help();
        void startupPhase(void);

    private:
        CommandProcessor* mCommandProcessor_ptr;
        // -- logic --
        CurrentState mCurrentState;
        bool mIsRunning;
        // -- in-game objects --
        const std::string mMapFileName;
        Map* mMap_ptr;
        std::vector<Player*> mPlayer_v;
        Deck* mDeck_ptr;
        std::map<CurrentState, std::set<std::string>> stateCommandMap;
};

#endif // !GAME_ENGINE_H

