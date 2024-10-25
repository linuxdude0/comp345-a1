#include "Command.h"
#include "GameEngine.h"

Command::Command()
{
    
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
}

// -- validatemap command --
void validateMapCommand::executeCommand(GameEngine& ge)
{
    ge.validateMap();
}

void validateMapCommand::saveEffect(const std::string& s_effect)
{
    mEffect = s_effect;
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
}

// -- assigncountries command --
void assignCountriesCommand::executeCommand(GameEngine& ge)
{
    ge.assignCountries();
}

void assignCountriesCommand::saveEffect(const std::string& s_effect)
{
    mEffect = s_effect;
}

// -- quit command --
void quitCommand::executeCommand(GameEngine& ge)
{
    ge.quit();
}

void quitCommand::saveEffect(const std::string& s_effect)
{
    mEffect = s_effect;
}

// -- help command --
void helpCommand::executeCommand(GameEngine& ge)
{
    ge.help();
}

void helpCommand::saveEffect(const std::string& s_effect)
{
    mEffect = s_effect;
}


// MEOW
void gamestartCommand::executeCommand(GameEngine& ge)
{
    ge.gamestart();
}
void gamestartCommand::saveEffect(const std::string& s_effect)
{
    mEffect = s_effect;
}

