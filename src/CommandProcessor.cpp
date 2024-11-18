#include "CommandProcessor.h"
#include "Command.h"
#include "GameEngine.h"
#include <cctype>
#include <limits>
#include <memory>
#include "common.h"

// -- constructor & destructor --

CommandProcessor::CommandProcessor()
{
    logObserver->attachSubject(this);
}
        
CommandProcessor::~CommandProcessor()
{

}

std::string CommandProcessor::stringToLog() {
    std::stringstream s;
    s << "Command: " << this->last_command;
    return s.str();
}

bool CommandProcessor::isEmptyOrWhitespace(const std::string& s_input)
{
    // checks if the input string is either empty or contains only whitespaces
    return s_input.empty() || std::all_of(s_input.begin(), s_input.end(), isspace);
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
        {
            ge.setIsRunning(false);
            break;
        }
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
            if(!isEmptyOrWhitespace(s_argument))
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
        else if(s_command == "tournament")
        {
            std::string err = "";
            size_t char_index = 0;
            std::string map_strings[TOURNAMENT_MAX_MAP_FILES]{};
            PlayerStrategyEnum player_strats[TOURNAMENT_MAX_PLAYER_STRATEGIES]{};
            size_t num_maps = 0;
            size_t num_player_types = 0;
            size_t num_games = 0;
            size_t max_turns = 0;
            for (; char_index < s_argument.length(); char_index++) {
                if (!isspace(s_argument[char_index])) {
                    break;
                }
            }
            if (char_index+2 >= s_argument.length()) {
                err = "missing all args";
                goto error;
            }
            if (s_argument[char_index] != '-' || s_argument[char_index+1] != 'M') {
                err = "-M is written incorrectely";
                goto error;
            }
            char_index+=2;
            for (; char_index < s_argument.length(); char_index++) {
                if (!isspace(s_argument[char_index])) {
                    break;
                }
            }
            for (;char_index < s_argument.length() && s_argument[char_index] != '-'; num_maps++) {
                if (num_maps >= TOURNAMENT_MAX_MAP_FILES) {
                    err = "Too many maps";
                    goto error;
                }
                map_strings[num_maps] = "";
                for (; char_index < s_argument.length(); char_index++) {
                    if (isspace(s_argument[char_index])) {
                        break;
                    }
                    map_strings[num_maps]+=s_argument[char_index];
                }
                for (; char_index < s_argument.length(); char_index++) {
                    if (!isspace(s_argument[char_index])) {
                        break;
                    }
                }
            }
            if (num_maps < TOURNAMENT_MIN_MAP_FILES) {
                err = "Too little maps";
                goto error;
            }
            if (char_index+2 >= s_argument.length()) {
                err = "missing -P -G and -D args";
                goto error;
            }
            if (s_argument[char_index] != '-' || s_argument[char_index+1] != 'P') {
                err = "-P is written incorrectely";
                goto error;
            }
            char_index+=2;
            for (; char_index < s_argument.length(); char_index++) {
                if (!isspace(s_argument[char_index])) {
                    break;
                }
            }
            for (;char_index < s_argument.length() && s_argument[char_index] != '-'; num_player_types++) {
                if (num_player_types >= TOURNAMENT_MAX_PLAYER_STRATEGIES) {
                    err = "Too many player strats";
                    goto error;
                }
                std::string s="";
                for (; char_index < s_argument.length(); char_index++) {
                    if (isspace(s_argument[char_index])) {
                        break;
                    }
                    s+=s_argument[char_index];
                }
                bool found = false;
                for (size_t i=0; i<static_cast<size_t>(PlayerStrategyEnum::STRATEGIES_MAX); i++) {
                    if (s == player_strategy_strings[i]) {
                        found = true;
                        player_strats[num_player_types] = static_cast<PlayerStrategyEnum>(i);
                    }
                }
                if (!found) {
                    err = "wrong player strat:" + s;
                    goto error;
                }
                for (; char_index < s_argument.length(); char_index++) {
                    if (!isspace(s_argument[char_index])) {
                        break;
                    }
                }
            }
            if (num_player_types < TOURNAMENT_MIN_PLAYER_STRATEGIES) {
                err = "Too little player strats";
                goto error;
            }
            if (char_index+2 >= s_argument.length()) {
                err = "missing -G and -D args";
                goto error;
            }
            if (s_argument[char_index] != '-' || s_argument[char_index+1] != 'G') {
                err = "-G is written incorrectely";
                goto error;
            }
            char_index+=2;
            for (; char_index < s_argument.length(); char_index++) {
                if (!isspace(s_argument[char_index])) {
                    break;
                }
            }
            if (char_index+1 >= s_argument.length()) {
                err = "missing -G number";
                goto error;
            }
            if (!isdigit(s_argument[char_index])) {
                err = "-G number is not a number";
                goto error;
            }
            num_games = s_argument[char_index++]-'0';
            if (!isspace(s_argument[char_index]) || !BETWEEN(num_games, TOURNAMENT_MIN_NUM_GAMES_PER_MAP, TOURNAMENT_MAX_NUM_GAMES_PER_MAP)) {
                err = "Wrong number of games";
                goto error;
            }
            for (; char_index < s_argument.length(); char_index++) {
                if (!isspace(s_argument[char_index])) {
                    break;
                }
            }
            if (char_index+2 >= s_argument.length()) {
                err = "missing -D args";
                goto error;
            }
            if (s_argument[char_index] != '-' || s_argument[char_index+1] != 'D') {
                err = "-D is written incorrectely";
                goto error;
            }
            char_index+=2;
            for (; char_index < s_argument.length(); char_index++) {
                if (!isspace(s_argument[char_index])) {
                    break;
                }
            }
            if (char_index+1 >= s_argument.length()) {
                err = "missing -D number";
                goto error;
            }
            if (!isdigit(s_argument[char_index]) || !isdigit(s_argument[char_index+1])) {
                err = "Wrong digit for -D option";
                goto error;
            }
            if (char_index+3 < s_argument.length() && !isspace(s_argument[char_index+2])) {
                err = "-D number too big";
                goto error;
            }
            max_turns = (s_argument[char_index]-'0')*10+(s_argument[char_index+1]-'0');
            if (!BETWEEN(max_turns, TOURNAMENT_MIN_TURNS_PER_GAME, TOURNAMENT_MAX_TURNS_PER_GAME)) {
                err = "-D not within range";
                goto error;
            }
            char_index+=2;
            for (; char_index < s_argument.length(); char_index++) {
                if (!isspace(s_argument[char_index])) {
                    err = "extra string after command is over";
                    goto error;
                }
            }
            std::cout << "Maps: " << std::endl;
            for (size_t i=0; i< num_maps; i++) {
                std::cout << "\t" << map_strings[i] << std::endl;
            }
            std::cout << "Player types: " << std::endl;
            for (size_t i=0; i< num_player_types; i++) {
                std::cout << "\t" << player_strategy_strings[static_cast<unsigned>(player_strats[i])] << std::endl;
            }
            std::cout << "number of games: " << num_games << std::endl;
            std::cout << "max turns: " << max_turns << std::endl;
            continue;
        error:
            std::cout << "[ERROR]: " << err << std::endl;
            std::cout << "Args: " << s_argument << std::endl;
            std::cout << "USAGE: tournament -M <listofmapfiles> -P <listofplayerstrategies> -G <numberofgames> -D <maxnumberofturns>" << std::endl;
            std::cout << "Maps: " << std::endl;
            for (size_t i=0; i< num_maps; i++) {
                std::cout << "\t" << map_strings[i] << std::endl;
            }
            std::cout << "Player types: " << std::endl;
            for (size_t i=0; i< num_player_types; i++) {
                std::cout << "\t" << player_strategy_strings[static_cast<unsigned>(player_strats[i])] << std::endl;
            }
            std::cout << "number of games: " << num_games << std::endl;
            std::cout << "max turns: " << max_turns << std::endl;
            continue;
        }
        else if(s_command == "validatemap")
            saveCommand(s_command,std::make_unique<validateMapCommand>());
        
        else if(s_command == "addplayer")
        {
            if(!isEmptyOrWhitespace(s_argument))
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
    this->last_command = s_commandName;
    this->notify(this);
    this->last_command = "";
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
            if(mCommandMap[s_command_name] != nullptr) {
                mCommandMap[s_command_name]->saveEffect(s_command_name+" command was used in the wrong state.");
                std::cerr << "[ERROR]: Command is not valid in the current state." << std::endl;
            } else {
                std::cerr << "[ERROR]: Command not found." << std::endl;
            }
        }
    }
    else
    {
        std::cerr << "[ERROR]: Current state is invalid." << std::endl;
    }
}
