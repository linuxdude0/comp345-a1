#ifndef COMMAND_H
#define COMMAND_H

#include "LoggingObserver.h"
#include <string>

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
    private:
        std::string mEffect;
};

// -- All derived command classes (for the game engine) --

class loadMapCommand : public Command
{
    public:
        loadMapCommand(const std::string& s_argument);
        virtual void executeCommand(GameEngine& ge) override;
        virtual void saveEffect(const std::string& s_effect) override;
    private:
        std::string mEffect;
        std::string mArgument;
};

class validateMapCommand : public Command
{
    public:
        virtual void executeCommand(GameEngine& ge) override;
        virtual void saveEffect(const std::string& s_effect) override;
    private:
        std::string mEffect;
};

class addPlayerCommand : public Command
{
    public:
        addPlayerCommand(const std::string& s_argument);
        virtual void executeCommand(GameEngine& ge) override;
        virtual void saveEffect(const std::string& s_effect) override;
    private:
        std::string mEffect;
        std::string mArgument;
};

class assignCountriesCommand : public Command
{
    public:
        virtual void executeCommand(GameEngine& ge) override;
        virtual void saveEffect(const std::string& s_effect) override;
    private:
        std::string mEffect;
};


class winCommand : public Command
{
    public:
        virtual void executeCommand(GameEngine& ge) override;
        virtual void saveEffect(const std::string& s_effect) override;
    private:
        std::string mEffect;
};

class gamestartCommand : public Command{

    public:
        virtual void executeCommand(GameEngine& ge) override;
        virtual void saveEffect(const std::string& s_effect) override;
    private:
        std::string mEffect;

};

class quitCommand : public Command
{
    public:
        virtual void executeCommand(GameEngine& ge) override;
        virtual void saveEffect(const std::string& s_effect) override;
    private:
        std::string mEffect;
};

class helpCommand : public Command
{
    public:
        virtual void executeCommand(GameEngine& ge) override;
        virtual void saveEffect(const std::string& s_effect) override;
    private:
        std::string mEffect;
};

#endif //! COMMAND_H
