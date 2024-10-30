#ifndef COMMAND_PROCESSING_DRIVER_H
#define COMMAND_PROCESSING_DRIVER_H

#include "CommandProcessor.h"
#include "Command.h"
#include "FileCommandProcessorAdapter.h"

class CommandProcessingDriver
{
    public:
        void testCommandProcessor(const std::string& arg);

    private:
        GameEngine* game_engine_ptr;
        CommandProcessor* command_processor_ptr;
        std::string filename;
};

#endif