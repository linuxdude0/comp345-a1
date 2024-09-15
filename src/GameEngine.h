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
           //engine.getMap().loadMap();
           //TODO: see with prof if we can skip this or if we should make a separate function for its initialization
        }
};

class validateMapCommand : public Command
{
    public:
        void executeCommand(GameEngine& engine) override
        {
            engine.getMap().validate();
        }
};

class addPlayerCommand : public Command
{
    public:
        void executeCommand(GameEngine& engine)
        {
            engine.addPlayer();
        }
};

class assignCountriesCommand : public Command
{
    public:
        void executeCommand(GameEngine& engine)
        {
            engine.assignCountries();
        }
};

class assignReinforcementCommand : public Command
{
    public:
        void executeCommand(GameEngine& engine)
        {
            engine.assignReinforcement();
        }
};



class CommandManager
{
    public:
        void addCommand(const std::string& s_command_name, std::unique_ptr<Command> s_command)
        {
            mCommandMap[s_command_name] = std::move(s_command);
        }

        void execute(const std::string& s_command_name, GameEngine& engine)
        {
            // check if command is in the map
            auto it = mCommandMap.find(s_command_name);
            if(it != mCommandMap.end())
            {
                it->second->executeCommand(engine);
            }
            else
            {
                std::cerr << "[ERROR]: Command not found." << std::endl;
            }
        }

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
        Player& getPlayer();
    
        // -- initializer functions --
        void loadMap();
        void addPlayer();

        // --  main functions --
        void userQuery();

        void assignCountries();
        void assignReinforcement();
        void execOrder();
        void endExecOrder();
        void endIssueOrders();
        void run(); // game loop

    private:
        // -- logic --
        CurrentState mCurrentState;
        bool mGameIsWon;
        bool mIsRunning;

        // -- in-game objects --
        const std::string& mMapFileName;

        std::unique_ptr<Map> mMap_ptr;
        std::unique_ptr<Player> mPlayer_ptr;
};

#endif // !GAME_ENGINE_H