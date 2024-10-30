#include "LoggingObserverDriver.h"
#include "Command.h"
#include "CommandProcessor.h"
#include "GameEngine.h"
#include "LoggingObserver.h"
#include "Orders.h"
#include <fstream>

void testLoggingObserver() {
    Command* command = new validateMapCommand();
    CommandProcessor* command_processor = new CommandProcessor();
    Order* order = new NegotiateOrder(nullptr, nullptr);
    OrderList* order_list = new OrderList(nullptr);
    int argc = 2;
    const char* argv[3] = {
        "",
        "-file",
        "test_commands.txt",
    };
    GameEngine* game_engine = new GameEngine("./maps/Atlantis.map", argc, argv);
    if (dynamic_cast<ILoggable*>(command) && dynamic_cast<Subject*>(command)) {
        std::cout << "Command class is a subclass to ILoggable and Subject" << std::endl;
    }
    if (dynamic_cast<ILoggable*>(command_processor) && dynamic_cast<Subject*>(command_processor)) {
        std::cout << "CommandProcessor class is a subclass to ILoggable and Subject" << std::endl;
    }
    if (dynamic_cast<ILoggable*>(order) && dynamic_cast<Subject*>(order)) {
        std::cout << "Order class is a subclass to ILoggable and Subject" << std::endl;
    }
    if (dynamic_cast<ILoggable*>(order_list) && dynamic_cast<Subject*>(order_list)) {
        std::cout << "OrderList class is a subclass to ILoggable and Subject" << std::endl;
    }
    if (dynamic_cast<ILoggable*>(game_engine) && dynamic_cast<Subject*>(game_engine)) {
        std::cout << "GameEngine class is a subclass to ILoggable and Subject" << std::endl;
    }
    delete order_list;
    delete order;
    delete command;
    delete command_processor;
    game_engine->run();
    std::ifstream i;
    i.open("gamelog.txt");
    std::string buf;
    while (!i.fail() && !i.eof() )
    {
        i >> buf ;
        std::cout << buf << std::endl;
    }
    i.close();
    delete game_engine;
}
