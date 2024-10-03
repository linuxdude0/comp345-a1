#ifndef COMMAND_H
#define COMMAND_H

#include <string>


// -- Command base class --
class Command
{
    public:
        Command();
        virtual ~Command();

        virtual void executeCommand();
        virtual void saveEffect();
    private:
        std::string mEffect;
};

// -- All derived command classes (for the game engine) --

#endif //! COMMAND_H
