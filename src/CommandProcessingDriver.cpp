#include "CommandProcessingDriver.h"
#include "CommandProcessor.h"

void testCommandProcessor(const std::string& arg)
{

    int argc = 2;
    const char* argv[3] = {
        "",
        "-file",
        "test_commands.txt",
    };
    GameEngine* ge = new GameEngine("./maps/Atlantis.map", argc, argv);
    ge->run();
    delete ge;
    CommandProcessor* p = new CommandProcessor();
    if(arg == "-console")
        p->getCommand(*ge);
    else if(arg == "-file")
    {
        FileCommandProcessorAdapter* FCPA = new FileCommandProcessorAdapter(ge->getCommandsFileName());
        FCPA->getCommand(*ge);
    }
    delete ge;
}
