#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include "Command.h"
#include "LoggingObserver.h"
#include <unordered_map>
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>



// -- following the adapter design pattern: --
// (target class)

class GameEngine;
class Command;

using CommandMap = std::unordered_map<std::string, std::unique_ptr<Command>>; 

class CommandProcessor : public ILoggable, public Subject
{
    public:
        // -- constructor & destructor --
        CommandProcessor();
        virtual ~CommandProcessor();
        // -- accessors & mutators --
        CommandMap& getCommandMap();
        std::string stringToLog() override;
        bool isEmptyOrWhitespace(const std::string& s_input);

        void saveCommand(const std::string& s_commandName, std::unique_ptr<Command> s_commandPtr); // add command to the commandMap
        virtual void getCommand(GameEngine& ge); // process user input
    
    protected:
        // -- main functionality --
        void readCommand(const std::string& s_userInput, GameEngine& ge); // read command from the player's input
        void validate(const std::string& s_command_name, GameEngine& ge);
    
    private:
        CommandMap mCommandMap;
        std::string last_command;
};

#endif // !COMMAND_PROCESSOR_H
