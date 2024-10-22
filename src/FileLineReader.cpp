#include "FileLineReader.h"

FileLineReader::FileLineReader()
{

}

FileLineReader::~FileLineReader()
{

}

Command FileLineReader::readLineFromFile(const std::string& s_fileName)
{
    Command t_command;
    std::ifstream s_file(s_fileName);
    if(!s_file)
    {
        std::cerr << ">> [ERROR]: File could not be found." << std::endl;
    }

    std::string s_line;
    while(std::getline(s_file, s_line))
    {
        std::cout << s_line << std::endl;
    }

    s_file.close();
    return t_command;
}