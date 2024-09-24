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
class CommandManager;
class Command;

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


class Command
{
    // -- commands base class --
    public:
        virtual ~Command();
        virtual void executeCommand(GameEngine& engine);
};

class CommandManager
{
    using CommandMap = std::unordered_map<std::string, std::unique_ptr<Command>>;

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
    
        // -- accessors & mutators --
        CommandMap& getCommandMap() {return mCommandMap;}

    private:
        CommandMap mCommandMap;
};


class GameEngine
{
    public:

        // -- constructor & destructor --
        GameEngine(const std::string& map_name);
        ~GameEngine();

        // -- accessors & mutators --
        bool mIsRunning;
        Map& getMap();
        Player& getPlayer();
        const bool isRunning() const;
        const bool getEndGame() const;
    
        // -- initializer functions --
        void initializeCommands();        
        
        void loadMap();
        void addPlayer();

        // --  main functions --
        void userQuery();
        void updateGame();
        CurrentState getState();
        void setState(CurrentState state);
        void run(); // game loop
        void closeGame();
        
        // -- game commands --
        void assignCountries();
        void assignReinforcement();
        void execOrder();
        void endExecOrder();
        void endIssueOrders();

        // -- testing --
        void testGameStates();

    private:
        // -- logic --
        CurrentState mCurrentState;

        // -- other --
        CommandManager mCommandManager;

        // -- in-game objects --
        const std::string& mMapFileName;

        std::unique_ptr<Map> mMap_ptr;
        std::unique_ptr<Player> mPlayer_ptr;
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
            engine.setState(MAP_VALIDATED);
            std::cout << ">>[EXECUTED]: validateMap()" << std::endl;
        }
};

class addPlayerCommand : public Command
{
    public:
        void executeCommand(GameEngine& engine)
        {
            engine.addPlayer();
            engine.setState(PLAYERS_ADDED);
            std::cout << ">>[EXECUTED]: addPlayer()" << std::endl;
        }
};

class assignCountriesCommand : public Command
{
    public:
        void executeCommand(GameEngine& engine)
        {
            engine.assignCountries();
            engine.setState(ASSIGN_REINFORCEMENTS);
            std::cout << ">>[EXECUTED]: assignCountries()" << std::endl;
        }
};

class assignReinforcementCommand : public Command
{
    public:
        void executeCommand(GameEngine& engine)
        {
            engine.assignReinforcement();
            std::cout << ">>[EXECUTED]: assignReinforcements()" << std::endl;
        }
};

class issueOrderCommand : public Command
{
    public:
        void executeCommand(GameEngine& engine)
        {
            // engine.getPlayer().issueOrder();
            engine.setState(ISSUE_ORDERS);
            std::cout << ">>[EXECUTED]: issueOrder()" << std::endl;
        }
};

class execOrdersCommand : public Command
{
    public:
        void executeCommand(GameEngine& engine)
        {
            // execute orders
            engine.setState(EXECUTE_ORDERS);
            std::cout << ">>[EXECUTED]: execOrder()" << std::endl;
        }
};



class endGameCommand : public Command
{
    public:
        void executeCommand(GameEngine& engine)
        {
            engine.mIsRunning = false;
            std::cout << ">>[EXECUTED]: endGame()." << std::endl;
            engine.closeGame();
            std::exit(EXIT_SUCCESS); 
        }
};
 

#endif // !GAME_ENGINE_H
