#ifndef FILE_COMMAND_PROCESSOR_H
#define FILE_COMMAND_PROCESSOR_H

#include "CommandProcessor.h"
#include "FileLineReader.h"

class FileCommandProcessorAdapter
{
    private:
        CommandProcessor* mCommandProcessor_ptr;
        FileLineReader* mFileLineReader_ptr;
    public:

        FileCommandProcessorAdapter();
        ~FileCommandProcessorAdapter();

        void readCommand(const std::string& s_fileName);
};


#endif