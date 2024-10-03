#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include "Command.h"
#include "GameEngine.h"
#include <unordered_map>
#include <memory>
#include <iostream>

using CommandMap = std::unordered_map<std::string, std::unique_ptr<Command>>; 

class CommandProcessor
{
    public:
        // -- constructor & destructor --
        CommandProcessor();
        ~CommandProcessor();
        // -- accessors & mutators --
        Command& getCommand(); // returns command object to GameEngine or Player
        CommandMap& getCommandMap();

    private:
        // -- main functionality --
        void readCommand(const std::string& s_userInput); // read command from the player's input
        void saveCommand(const std::string& s_commandName, std::unique_ptr<Command> s_commandPtr); // add command to the commandMap
        void validate(Command& command, CurrentState& state);

        CommandMap mCommandMap;
};

#endif // !COMMAND_PROCESSOR_H