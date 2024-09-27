#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "Map.h"
#include "Player.h"
#include "Cards.h"

#include <iostream>
#include <string>
#include <string_view>
#include <map>
#include <unordered_map>
#include <functional>
#include <memory>
#include <limits>
#include <random>

class Command;
class GameEngine;

using CommandFunction = std::function<void()>;
using CommandMap = std::unordered_map<std::string,CommandFunction>;

enum CurrentState
{
    START,
    MAP_LOADED,
    MAP_VALIDATED,
    PLAYERS_ADDED,
    ASSIGN_REINFORCEMENTS,
    ISSUE_ORDERS,
    EXECUTE_ORDERS,
    WIN
};


std::ostream& operator<<(std::ostream& out, const CurrentState value);

class GameEngine
{
    public:

        // -- constructor & destructor --
        GameEngine(const std::string map_name);
        ~GameEngine();

        // -- accessors & mutators --
        bool mIsRunning;
        Map& getMap();
        Player& getPlayer();
        bool isRunning();

        CommandMap& getCommandMap();
    
        // -- initializer functions --
        void initializeCommands();        
        

        // --  main functions --
        void userQuery();
        void updateGame();

        void execute(const std::string& s_command_name);

        CurrentState getState();
        void setState(CurrentState state);
        void run(); // game loop
        void closeGame();
        
        // -- game commands --
        void loadMap();
        void validateMap();
        void addPlayer();

        void assignCountries();
        void issueOrder();
        void execOrder();
        void endExecOrder();
        void endIssueOrders();

        void win();
        void play();

        // -- testing --
        void testGameStates();

    private:
        // -- logic --
        CurrentState mCurrentState;

        // -- other --

        // -- in-game objects --
        const std::string mMapFileName;

        Map* mMap_ptr;

        std::vector<Player*> mPlayer_v;
        std::unordered_map<std::string,CommandFunction> commandMap;
};


#endif // !GAME_ENGINE_H

