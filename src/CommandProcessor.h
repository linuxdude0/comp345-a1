#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include "Command.h"
#include <unordered_map>
#include <memory>
#include <iostream>
#include <sstream>

class GameEngine;
class Command;

using CommandMap = std::unordered_map<std::string, std::unique_ptr<Command>>; 

class CommandProcessor
{
    public:
        // -- constructor & destructor --
        CommandProcessor();
        ~CommandProcessor();
        // -- accessors & mutators --
        //Command& getCommand(); // returns command object to GameEngine or Player
        CommandMap& getCommandMap();

        void saveCommand(const std::string& s_commandName, std::unique_ptr<Command> s_commandPtr); // add command to the commandMap
        void processUserInput(GameEngine& ge); // process user input 
    private:
        // -- main functionality --
        void readCommand(const std::string& s_userInput, GameEngine& ge); // read command from the player's input
        void validate(const std::string& s_command_name, GameEngine& ge);

        CommandMap mCommandMap;
};

#endif // !COMMAND_PROCESSOR_H
