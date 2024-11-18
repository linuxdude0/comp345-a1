#ifndef COMMAND_H
#define COMMAND_H

#include "LoggingObserver.h"
#include "PlayerStrategy.h"
#include <string>

#define TOURNAMENT_MAX_MAP_FILES            5
#define TOURNAMENT_MIN_MAP_FILES            1

#define TOURNAMENT_MAX_PLAYER_STRATEGIES    4
#define TOURNAMENT_MIN_PLAYER_STRATEGIES    2

#define TOURNAMENT_MAX_NUM_GAMES_PER_MAP    5
#define TOURNAMENT_MIN_NUM_GAMES_PER_MAP    1

#define TOURNAMENT_MAX_TURNS_PER_GAME       50
#define TOURNAMENT_MIN_TURNS_PER_GAME       50

class GameEngine;


// -- Command base class --
class Command : public ILoggable, public Subject
{
    public:
        Command();
        virtual ~Command();
        std::string stringToLog();
        virtual void executeCommand(GameEngine& ge) = 0;
        virtual void saveEffect(const std::string& s_effect) = 0;
        std::string getEffect();
    protected:
        std::string mEffect;
};

// -- All derived command classes (for the game engine) --

class tournamentCommand : public Command 
{
    public:
        tournamentCommand(
            std::string map_files[TOURNAMENT_MAX_MAP_FILES],
            size_t num_map_files,
            PlayerStrategyEnum player_strategies[TOURNAMENT_MAX_PLAYER_STRATEGIES],
            size_t num_player_strategies,
            size_t num_games_per_map,
            size_t max_turns_per_game
        );
        virtual void executeCommand(GameEngine& ge) override;
        virtual void saveEffect(const std::string& s_effect) override;
    private:
        std::string map_files[TOURNAMENT_MAX_MAP_FILES];
        size_t num_map_files;
        PlayerStrategyEnum player_strategies[TOURNAMENT_MAX_PLAYER_STRATEGIES];
        size_t num_player_strategies;
        size_t num_games_per_map;
        size_t max_turns_per_game;
};

class loadMapCommand : public Command
{
    public:
        loadMapCommand(const std::string& s_argument);
        virtual void executeCommand(GameEngine& ge) override;
        virtual void saveEffect(const std::string& s_effect) override;
    private:
        std::string mArgument;
};

class validateMapCommand : public Command
{
    public:
        virtual void executeCommand(GameEngine& ge) override;
        virtual void saveEffect(const std::string& s_effect) override;
    private:
};

class addPlayerCommand : public Command
{
    public:
        addPlayerCommand(const std::string& s_argument);
        virtual void executeCommand(GameEngine& ge) override;
        virtual void saveEffect(const std::string& s_effect) override;
    private:
        std::string mArgument;
};

class assignCountriesCommand : public Command
{
    public:
        virtual void executeCommand(GameEngine& ge) override;
        virtual void saveEffect(const std::string& s_effect) override;
    private:
};


class winCommand : public Command
{
    public:
        virtual void executeCommand(GameEngine& ge) override;
        virtual void saveEffect(const std::string& s_effect) override;
    private:
};

class gamestartCommand : public Command{

    public:
        virtual void executeCommand(GameEngine& ge) override;
        virtual void saveEffect(const std::string& s_effect) override;
    private:

};

class quitCommand : public Command
{
    public:
        virtual void executeCommand(GameEngine& ge) override;
        virtual void saveEffect(const std::string& s_effect) override;
    private:
};

class helpCommand : public Command
{
    public:
        virtual void executeCommand(GameEngine& ge) override;
        virtual void saveEffect(const std::string& s_effect) override;
    private:
};

#endif //! COMMAND_H
