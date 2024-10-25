#include "CommandProcessor.h"
#include "GameEngine.h"

// -- constructor & destructor --

CommandProcessor::CommandProcessor()
{

}
        
CommandProcessor::~CommandProcessor()
{

}

// -- accessors & mutators --
CommandMap& CommandProcessor::getCommandMap() { return mCommandMap; }

// -- main functionality --
void CommandProcessor::getCommand(GameEngine& ge)
{
    // asks user to enter specific game engine commands
    // in order to switch states and perform game operations
    std::string sCommand;
    while(true)
    {
        ge.updateGame(); // MEOW : correct state 

        std::cout << "Current state:[" << ge.getState() << "]" << std::endl;
        std::cout << ">> Enter command: ";
        std::getline(std::cin, sCommand);

        // -- edge cases --
        if(sCommand == "quit")
            ge.setIsRunning(false);
        if(std::cin.eof())
        {
            // handle case where user enters: "ctrl+d" (EOF) (bug made it so there was an infinite loop upon EOF)
            ge.setIsRunning(false);
            ge.closeGame();
        }
        if(std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << ">> [ERROR]: Invalid input, please try again." << std::endl;
        }

        // -- split up the command and arguments --
        std::istringstream i_stream(sCommand);
        std::string s_command;
        std::string s_argument;

        i_stream >> s_command;
        std::getline(i_stream, s_argument); // get remaining input from user as command argument
        // -- main cases --
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

        std::cout << s_command << std::endl; // !! test function output !! 
        validate(s_command,ge);
    }
}

void CommandProcessor::readCommand(const std::string& s_commandName, GameEngine& ge)
{
    // takes command entered by user as input 
    // check if bound command is in the map
    auto it = mCommandMap.find(s_commandName);
    if(it != mCommandMap.end())
    {
        // check if command recieved through user input is in the map
        // execute command if found
        it->second->executeCommand(ge);
        it->second->saveEffect(s_commandName);
    }
    else
    {
        std::cout << "[ERROR]: Command not found." << std::endl;
    }
}

void CommandProcessor::saveCommand(const std::string& s_commandName, std::unique_ptr<Command> s_commandPtr)
{
    mCommandMap[s_commandName] = std::move(s_commandPtr);
}


void CommandProcessor::validate(const std::string& s_command_name, GameEngine& ge)
{
    auto stateCommandMap = ge.getCommandMap();
    GameEngine::CurrentState mCurrentState = ge.getState();

    // -- special cases if it is the loadmap or addplayer commands --
    if(s_command_name.find("loadmap") == 0)
    {
        readCommand("loadmap", ge); // accept, regardless of arguments
        return;
    }

    // check if entered command is valid in the current state
    auto stateIt = stateCommandMap.find(mCurrentState);
    if(stateIt != stateCommandMap.end())
    {
        const std::set<std::string>& validCommands = stateIt->second;
        if(validCommands.find(s_command_name) != validCommands.end())
        {
            readCommand(s_command_name,ge);
        }
        else
        {
            // if entered in the wrong state, save effect with error message
            mCommandMap[s_command_name]->saveEffect(s_command_name+" command was used in the wrong state.");
            std::cerr << "[ERROR]: Command is not valid in the current state." << std::endl;
        }
    }
    else
    {
        std::cerr << "[ERROR]: Current state is invalid." << std::endl;
    }
}
