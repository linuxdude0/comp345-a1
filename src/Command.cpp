#include "Command.h"
#include "GameEngine.h"
#include <sstream>

Command::Command()
{
    logObserver->attachSubject(this);
}

Command::~Command()
{

}

std::string Command::getEffect() {return mEffect;}

void Command::executeCommand(GameEngine& ge)
{
    std::cout << ge << std::endl;
}

void Command::saveEffect(const std::string& s_effect)
{
    std::cout << s_effect << std::endl;
}

std::string Command::stringToLog() {
    std::stringstream s;
    s << "Effect: " << this->getEffect();
    return s.str();
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
    ge.addPlayer(mArgument);
}

void addPlayerCommand::saveEffect(const std::string& s_effect)
{
    mEffect = s_effect;
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
    this->notify(this);
}

