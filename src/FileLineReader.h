#ifndef FILE_LINE_READER_H
#define FILE_LINE_READER_H

#include "Command.h"
#include <fstream>
#include <iostream>


// -- following the adapter design pattern: --
// (adaptee class)

class Command;

class FileLineReader
{
    private:
        std::ifstream mCommandsFile;
    public:
        FileLineReader(const std::string& s_filename);
        ~FileLineReader();

        bool readLineFromFile(std::string& line);
};

#endif