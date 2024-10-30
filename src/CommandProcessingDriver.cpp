#include "CommandProcessingDriver.h"

void CommandProcessingDriver::testCommandProcessor(const std::string& arg)
{
    if(arg == "-console")
        command_processor_ptr->getCommand(*game_engine_ptr);

    else if(arg == "-file")
    {
        FileCommandProcessorAdapter* FCPA = new FileCommandProcessorAdapter(game_engine_ptr->getCommandsFileName());
        FCPA->getCommand(*game_engine_ptr);
    }
}