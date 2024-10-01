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
#include <ranges>
#include <chrono>

class Command;
class GameEngine;

using CommandFunction = std::function<void()>;
using CommandMap = std::unordered_map<std::string,CommandFunction>;

// Game Engine class:
//      - 

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

        // -- constructors & destructor --
        GameEngine(const std::string map_name);
        GameEngine(const GameEngine&);
        ~GameEngine();

        // -- overloaded stream input function --
        friend std::ostream& operator<<(std::ostream& out, const GameEngine& ge);

        // -- accessors & mutators --
        Map& getMap();
        Player& getPlayer();
        bool isRunning();
        CommandMap& getCommandMap(); 
        CurrentState getState();
        void setState(CurrentState state);

        // -- initializer functions --
        void initializeCommands();
        void distributeTerritories(int n_playerCount, int n_totalIndexes);
        
        // --  main functions --
        void userQuery();
        void updateGame();
        void execute(const std::string& s_command_name);

        void run(); // game loop
        void closeGame();
        
        // -- game commands --
        void loadMap();
        void validateMap();
        void addPlayer();
        void assignCountries();
        void issueOrder();
        void execOrder();
        void endExecOrders();
        void endIssueOrders();
        void win();
        void play();
        void end();
        void quit();

    private:
        // -- logic --
        CurrentState mCurrentState;
        bool mIsRunning;
        // -- in-game objects --
        const std::string mMapFileName;
        Map* mMap_ptr;
        std::vector<Player*> mPlayer_v;
        std::unordered_map<std::string,CommandFunction> commandMap;

};


#endif // !GAME_ENGINE_H

