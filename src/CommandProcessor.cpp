#include "CommandProcessor.h"

// -- constructor & destructor --

CommandProcessor::CommandProcessor()
{

}
        
CommandProcessor::~CommandProcessor()
{

}


// -- accessors & mutators --
Command& CommandProcessor::getCommand() {}
CommandMap& CommandProcessor::getCommandMap() { return mCommandMap; }

// -- main functionality --
void CommandProcessor::readCommand(const std::string& s_commandName)
{
    auto it = mCommandMap.find(s_commandName);
    if(it != mCommandMap.end())
    {
        // check if command recieved through user input is in the map
        // execute command if found
        it->second->executeCommand();
    }
    else
    {
        std::cout << "[ERROR]: Command not found." << std::endl;
    }
}

void CommandProcessor::saveCommand(const std::string& s_commandName, std::unique_ptr<Command> s_commandPtr)
{
    mCommandMap[s_commandName] = std::move(s_commandPtr);
}


void CommandProcessor::validate(Command& command, CurrentState& state)
{

}