#include "FileCommandProcessorAdapter.h"

FileCommandProcessorAdapter::FileCommandProcessorAdapter(const std::string& s_filename)
    : CommandProcessor(), mFileReader(s_filename)
{

}

FileCommandProcessorAdapter::~FileCommandProcessorAdapter()
{

}

void FileCommandProcessorAdapter::getCommand(GameEngine& ge)
{
    std::string sCommand;

    // read lines from file (using FileLineReader) 
    while(mFileReader.readLineFromFile(sCommand))
    {
        if(sCommand == "quit")
        {
            ge.setIsRunning(false);
            break;
        }

        // -- split up the command and arguments --
        std::istringstream i_stream(sCommand);
        std::string s_command;
        std::string s_argument;

        i_stream >> s_command;
        std::getline(i_stream, s_argument); // get remaining input from user as command argument
        
        if(s_command == "loadmap")
        {
            if(!s_argument.empty())
            {
                saveCommand(s_command,std::make_unique<loadMapCommand>(s_argument));
            }
            else
            {
                std::cout << "[ERROR]: map file name required." << std::endl;
                std::cout << "usage: loadmap <filename>.map" << std::endl;
                continue;
            }
        }
        else if(s_command == "validatemap")
            saveCommand(s_command,std::make_unique<validateMapCommand>());
        
        else if(s_command == "addplayer")
        {
            if(!s_argument.empty())
            {
                saveCommand(s_command,std::make_unique<addPlayerCommand>(s_argument));
            }
            else
            {
                std::cout << "[ERROR]: player name required." << std::endl;
                std::cout << "usage: addplayer <player name>" << std::endl;
                continue;
            }
        }
        else if(s_command == "assigncountries")
            saveCommand(s_command,std::make_unique<assignCountriesCommand>());
        else if(s_command == "gamestart") // MEOW
            saveCommand(s_command,std::make_unique<gamestartCommand>());    
        else if(s_command == "quit")
            saveCommand(s_command,std::make_unique<quitCommand>());
        else if(s_command == "help")
            saveCommand(s_command,std::make_unique<helpCommand>());
        else
        {
            std::cout << "[WARNING]: Unknown command." << std::endl;
        }

        validate(s_command,ge);
    }
}