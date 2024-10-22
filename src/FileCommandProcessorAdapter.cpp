#include "FileCommandProcessorAdapter.h"

FileCommandProcessorAdapter::FileCommandProcessorAdapter()
{
    mCommandProcessor_ptr = new CommandProcessor();
    mFileLineReader_ptr = new FileLineReader();
}

FileCommandProcessorAdapter::~FileCommandProcessorAdapter()
{
    delete mCommandProcessor_ptr;
    delete mFileLineReader_ptr;
}

void FileCommandProcessorAdapter::readCommand(const std::string& s_fileName)
{
    Command t_command = mFileLineReader_ptr->readLineFromFile(s_fileName);
}