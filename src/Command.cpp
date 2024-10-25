#include "Command.h"
#include "GameEngine.h"

Command::Command()
{
    
}

Command::~Command()
{

}

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

// -- issueorder command --
void issueOrderCommand::executeCommand(GameEngine& ge)
{
    ge.issueOrder();
}

void issueOrderCommand::saveEffect(const std::string& s_effect)
{
    mEffect = s_effect;
}

// -- endissueorder command --
void endIssueOrderCommand::executeCommand(GameEngine& ge)
{
    ge.endIssueOrders();
}

void endIssueOrderCommand::saveEffect(const std::string& s_effect)
{
    mEffect = s_effect;
}
// -- execorder command --
void execOrderCommand::executeCommand(GameEngine& ge)
{
    ge.execOrder();
}

void execOrderCommand::saveEffect(const std::string& s_effect)
{
    mEffect = s_effect;
}

// -- endexecorder command --
void endExecOrderCommand::executeCommand(GameEngine& ge)
{
    ge.endExecOrders();
}

void endExecOrderCommand::saveEffect(const std::string& s_effect)
{
    mEffect = s_effect;
}

// -- win command --
void winCommand::executeCommand(GameEngine& ge)
{
    ge.win();
}

void winCommand::saveEffect(const std::string& s_effect)
{
    mEffect = s_effect;
}

// -- play command --
void playCommand::executeCommand(GameEngine& ge)
{
    ge.play();
}

void playCommand::saveEffect(const std::string& s_effect)
{
    mEffect = s_effect;
}

// -- end command --
void endCommand::executeCommand(GameEngine& ge)
{
    ge.end();
}

void endCommand::saveEffect(const std::string& s_effect)
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

