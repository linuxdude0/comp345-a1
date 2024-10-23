#ifndef COMMAND_H
#define COMMAND_H

#include <string>

class GameEngine;


// -- Command base class --
class Command
{
    public:
        Command();
        virtual ~Command();

        virtual void executeCommand(GameEngine& ge);
        virtual void saveEffect(const std::string& s_effect);
    private:
        std::string mEffect;
};

// -- All derived command classes (for the game engine) --

class loadMapCommand : public Command
{
    public:
        loadMapCommand(const std::string& s_argument);
        void executeCommand(GameEngine& ge) override;
        void saveEffect(const std::string& s_effect) override;
    private:
        std::string mEffect;
        std::string mArgument;
};

class validateMapCommand : public Command
{
    public:
        void executeCommand(GameEngine& ge);
        void saveEffect(const std::string& s_effect);
    private:
        std::string mEffect;
};

class addPlayerCommand : public Command
{
    public:
        addPlayerCommand(const std::string& s_argument);
        void executeCommand(GameEngine& ge) override;
        void saveEffect(const std::string& s_effect) override;
    private:
        std::string mEffect;
        std::string mArgument;
};

class assignCountriesCommand : public Command
{
    public:
        void executeCommand(GameEngine& ge);
        void saveEffect(const std::string& s_effect);
    private:
        std::string mEffect;
};

class issueOrderCommand : public Command
{
    public:
        void executeCommand(GameEngine& ge);
        void saveEffect(const std::string& s_effect);
    private:
        std::string mEffect;
};

class endIssueOrderCommand : public Command
{
    public:
        void executeCommand(GameEngine& ge);
        void saveEffect(const std::string& s_effect);
    private:
        std::string mEffect;
};

class execOrderCommand : public Command
{
    public:
        void executeCommand(GameEngine& ge);
        void saveEffect(const std::string& s_effect);
    private:
        std::string mEffect;
};

class endExecOrderCommand : public Command
{
    public:
        void executeCommand(GameEngine& ge);
        void saveEffect(const std::string& s_effect);
    private:
        std::string mEffect;
};

class winCommand : public Command
{
    public:
        void executeCommand(GameEngine& ge);
        void saveEffect(const std::string& s_effect);
    private:
        std::string mEffect;
};

class playCommand : public Command
{
    public:
        void executeCommand(GameEngine& ge);
        void saveEffect(const std::string& s_effect);
    private:
        std::string mEffect;
};

class endCommand : public Command
{
    public:
        void executeCommand(GameEngine& ge);
        void saveEffect(const std::string& s_effect);
    private:
        std::string mEffect;
};

class quitCommand : public Command
{
    public:
        void executeCommand(GameEngine& ge);
        void saveEffect(const std::string& s_effect);
    private:
        std::string mEffect;
};

class helpCommand : public Command
{
    public:
        void executeCommand(GameEngine& ge);
        void saveEffect(const std::string& s_effect);
    private:
        std::string mEffect;
};
#endif //! COMMAND_H
