#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "Map.h"
#include "Player.h"

#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <memory>

class GameEngine;

enum CurrentState
{
    STARTUP,
    PLAY,
    END
};

class Command
{
    // -- commands base class --
    public:
        virtual ~Command();
        virtual void executeCommand(GameEngine& engine);

};

class loadMapCommand : public Command
{
    public:
        void executeCommand(GameEngine& engine) override
        {
            
        }
};


class CommandManager
{
    public:

    private:
        std::unordered_map<std::string, std::unique_ptr<Command>> mCommandMap;
};



class GameEngine
{
    public:

        // -- constructor & destructor --
        GameEngine(const std::string& map_name);
        ~GameEngine();

        // -- accessors & mutators --
        bool getQuit();
        Map& getMap();
    
        // -- initializer functions --
        void loadMap();
        void validateMap();
        void addPlayer();

        // --  main functions --
        void userQuery();

        void assignReinforcements();
        void issueOrders();
        void execOrder();
        void run(); // game loop

    private:
        // -- logic --
        CurrentState mCurrentState;
        bool mGameIsWon;
        bool mIsRunning;

        // -- in-game objects --
        const std::string& mMapFileName;

        Map* mMap;
        Player* mPlayer;
};

#endif // !GAME_ENGINE_H