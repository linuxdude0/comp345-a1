#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include "Command.h"
#include <unordered_map>
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>


// -- following the adapter design pattern: --
// (target class)

class GameEngine;
class Command;

using CommandMap = std::unordered_map<std::string, std::unique_ptr<Command>>; 

class CommandProcessor
{
    public:
        // -- constructor & destructor --
        CommandProcessor();
        virtual ~CommandProcessor();
        // -- accessors & mutators --
        CommandMap& getCommandMap();

        void saveCommand(const std::string& s_commandName, std::unique_ptr<Command> s_commandPtr); // add command to the commandMap
        virtual void getCommand(GameEngine& ge); // process user input
    
    protected:
        // -- main functionality --
        void readCommand(const std::string& s_userInput, GameEngine& ge); // read command from the player's input
        void validate(const std::string& s_command_name, GameEngine& ge);
    
    private:
        CommandMap mCommandMap;
};

#endif // !COMMAND_PROCESSOR_H
