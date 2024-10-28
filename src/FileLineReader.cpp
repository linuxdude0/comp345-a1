#include "FileLineReader.h"

FileLineReader::FileLineReader(const std::string& s_filename)
: mCommandsFile(s_filename)
{
    if(!mCommandsFile.is_open())
    {
        std::cerr << ">>[ERROR]: Could not open commands file for reading." << std::endl;
    }
}

FileLineReader::~FileLineReader()
{
    if(mCommandsFile.is_open())
        mCommandsFile.close();
}

bool FileLineReader::readLineFromFile(std::string& s_line)
{
    if(std::getline(mCommandsFile,s_line))
    {
        return true;
    }

    if(mCommandsFile.eof())
    {
        std::cout << "[WARNING]: Reached EOF." << std::endl;
    }
    else if(mCommandsFile.fail())
    {
        std::cout << "[ERROR]: Failed to read from file." << std::endl;
    }
    return false;
}