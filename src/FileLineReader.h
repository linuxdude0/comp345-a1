#ifndef FILE_LINE_READER_H
#define FILE_LINE_READER_H

#include "Command.h"
#include <fstream>
#include <iostream>

class Command;

class FileLineReader
{
    private:
    public:
        FileLineReader();
        ~FileLineReader();

        Command readLineFromFile(const std::string& s_fileName);
};

#endif