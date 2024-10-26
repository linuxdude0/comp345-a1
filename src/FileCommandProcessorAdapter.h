#ifndef FILE_COMMAND_PROCESSOR_H
#define FILE_COMMAND_PROCESSOR_H

#include "GameEngine.h"
#include "CommandProcessor.h"
#include "FileLineReader.h"

// -- following the adapter design pattern: --
// (adapter class)

class FileCommandProcessorAdapter : public CommandProcessor
{
    private:
        FileLineReader mFileReader;
    public:

        FileCommandProcessorAdapter(const std::string& s_filename);
        ~FileCommandProcessorAdapter() override;

        void getCommand(GameEngine& ge) override;
};


#endif