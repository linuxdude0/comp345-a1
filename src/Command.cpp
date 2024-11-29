#include "Command.h"
#include "GameEngine.h"
#include "common.h"
#include <cassert>
#include <cctype>
#include <iomanip>
#include <sstream>

Command::Command()
{
    logObserver->attachSubject(this);
}

Command::~Command()
{
    
}

std::string Command::getEffect() {return mEffect;}

std::string Command::stringToLog() {
    std::stringstream s;
    s << "Effect: " << this->getEffect();
    return s.str();
}

// -- tournament command --
tournamentCommand::tournamentCommand(
    std::string map_files[TOURNAMENT_MAX_MAP_FILES],
    size_t num_map_files,
    PlayerStrategyEnum player_strategies[TOURNAMENT_MAX_PLAYER_STRATEGIES],
    size_t num_player_strategies,
    size_t num_games_per_map,
    size_t max_turns_per_game
) {
    assert(BETWEEN(num_map_files, TOURNAMENT_MIN_MAP_FILES, TOURNAMENT_MAX_MAP_FILES));
    assert(BETWEEN(num_player_strategies, TOURNAMENT_MIN_PLAYER_STRATEGIES, TOURNAMENT_MAX_PLAYER_STRATEGIES));
    assert(BETWEEN(num_games_per_map, TOURNAMENT_MIN_NUM_GAMES_PER_MAP, TOURNAMENT_MAX_NUM_GAMES_PER_MAP));
    assert(BETWEEN(max_turns_per_game, TOURNAMENT_MIN_TURNS_PER_GAME, TOURNAMENT_MAX_TURNS_PER_GAME));
    logObserver->attachSubject(this);
    for (size_t i=0; i<num_map_files; i++) {
        this->map_files[i]=map_files[i];
    }
    for (size_t i=0; i<num_player_strategies; i++) {
        this->player_strategies[i]=player_strategies[i];
    }
    this->num_player_strategies = num_player_strategies;
    this->num_games_per_map = num_games_per_map;
    this->max_turns_per_game = max_turns_per_game;
    this->num_map_files = num_map_files;
}

void tournamentCommand::executeCommand(GameEngine& ge) {
    std::stringstream s{};
    s << "Tournament mode:\n"
      << "M: ";
    for (size_t i=0; i<this->num_map_files; i++) {
        s << map_files[i] << ", ";
    }
    s << "\nP: ";
    for (size_t i=0; i<this->num_player_strategies; i++) {
        s << player_strategy_strings[static_cast<unsigned>(this->player_strategies[i])] << ", ";
    }
    s << "\nG: " << this->num_games_per_map << "\nD: " << this->max_turns_per_game << "\n";
    s << "Results: " << "\n";
    s << std::setw(15) << " " << " |";
    for (size_t i=0; i<this->num_games_per_map; i++) {
        s << std::setw(15) << "Game " << i+1 << "  |";
    }
    s << "\n";
    for (size_t map_num=0; map_num<this->num_map_files; map_num++) {
        s <<std::setw(15) << map_files[map_num] << " |";
        for (size_t game_num=0; game_num<this->num_games_per_map; game_num++) {
            std::cout << "\t!!execute!!" << std::endl;
            PlayerStrategyEnum winner = PlayerStrategyEnum::STRATEGIES_MAX;
            if(!ge.tournament(this->map_files[map_num], this->player_strategies, this->num_player_strategies, this->max_turns_per_game, &winner)) {
                std::cout << "Tournament failed: Map: " << this->map_files[map_num] << "Player types: ";
                for (size_t i=0; i<this->num_player_strategies; i++) {
                    std::cout << player_strategy_strings[static_cast<unsigned>(this->player_strategies[i])] << " ";
                }
                std::cout << "max turns: " << this->max_turns_per_game << std::endl;
                continue;
            }
            std::cout << "\t!!END execute!!" << std::endl;
            s << std::setw(15) << player_strategy_strings[static_cast<unsigned>(winner)] << "   |";
        }
        s << "\n";
    }
    this->saveEffect(s.str());
    ge.transition(GameEngine::START);
}

void tournamentCommand::saveEffect(const std::string& s_effect) {
    mEffect = s_effect;
    std::cout << s_effect << std::endl;
    this->notify(this);
}

// -- loadmap command --
loadMapCommand::loadMapCommand(const std::string& s_argument)
    :mArgument{s_argument}
{
    // default constructor
}

void loadMapCommand::executeCommand(GameEngine& ge)
{
    ge.loadMap(mArgument);
}

void loadMapCommand::saveEffect(const std::string& s_effect)
{
    mEffect = s_effect;
    std::cout << s_effect << std::endl;
    this->notify(this);
}

// -- validatemap command --
void validateMapCommand::executeCommand(GameEngine& ge)
{
    ge.validateMap();
}

void validateMapCommand::saveEffect(const std::string& s_effect)
{
    mEffect = s_effect;
    std::cout << s_effect << std::endl;
    this->notify(this);
}

// -- addplayer command --
addPlayerCommand::addPlayerCommand(const std::string& s_argument)
    :mArgument{s_argument}
{
    // default constructor
}

void addPlayerCommand::executeCommand(GameEngine& ge)
{
    ge.addPlayer(mArgument, PlayerStrategyEnum::HUMAN_STRATEGY);
}

void addPlayerCommand::saveEffect(const std::string& s_effect)
{
    mEffect = s_effect;
    std::cout << s_effect << std::endl;
    this->notify(this);
}


addBotCommand::addBotCommand(const std::string& s_argument)
    :mArgument{s_argument}
{
    // default constructor
}

void addBotCommand::executeCommand(GameEngine& ge)
{
    size_t i=0;
    for (; i<mArgument.length(); i++) {
        if (!isspace(mArgument[i])) {
            break;
        }
    }
    std::string player_type = "";
    for (; i<mArgument.length(); i++) {
        if (isspace(mArgument[i])) {
            break;
        }
        player_type+=mArgument[i];
    }
    for (; i<mArgument.length(); i++) {
        if (!isspace(mArgument[i])) {
            break;
        }
    }
    std::string player_name = "";
    for (; i<mArgument.length(); i++) {
        if (isspace(mArgument[i])) {
            break;
        }
        player_name+=mArgument[i];
    }
    PlayerStrategyEnum pt = PlayerStrategyEnum::STRATEGIES_MAX;
    for (size_t j=0; j<static_cast<unsigned>(PlayerStrategyEnum::STRATEGIES_MAX); j++) {
        if (player_type == player_strategy_strings[j]) {
            pt = static_cast<PlayerStrategyEnum>(j);
            break;
        }
    }
    if (pt == PlayerStrategyEnum::HUMAN_STRATEGY) {
        std::cout << "only bots allowed" << std::endl;
        return;
    }
    if(pt == PlayerStrategyEnum::STRATEGIES_MAX) {
        std::cout << "type not found" << std::endl;
        return;
    }
    std::cout << player_strategy_strings[static_cast<unsigned>(pt)];
    ge.addPlayer(player_name, pt);
}

void addBotCommand::saveEffect(const std::string& s_effect)
{
    mEffect = s_effect;
    std::cout << s_effect << std::endl;
    this->notify(this);
}


// -- assigncountries command --
void assignCountriesCommand::executeCommand(GameEngine& ge)
{
    ge.assignCountries();
}

void assignCountriesCommand::saveEffect(const std::string& s_effect)
{
    mEffect = s_effect;
    std::cout << s_effect << std::endl;
    this->notify(this);
}

// -- quit command --
void quitCommand::executeCommand(GameEngine& ge)
{
    ge.quit();
}

void quitCommand::saveEffect(const std::string& s_effect)
{
    mEffect = s_effect;
    std::cout << s_effect << std::endl;
    this->notify(this);
}

// -- help command --
void helpCommand::executeCommand(GameEngine& ge)
{
    ge.help();
}

void helpCommand::saveEffect(const std::string& s_effect)
{
    mEffect = s_effect;
    std::cout << s_effect << std::endl;
    this->notify(this);
}

// MEOW
void gamestartCommand::executeCommand(GameEngine& ge)
{
    ge.gamestart();
}
void gamestartCommand::saveEffect(const std::string& s_effect)
{
    mEffect = s_effect;
    std::cout << s_effect << std::endl;
    this->notify(this);
}

