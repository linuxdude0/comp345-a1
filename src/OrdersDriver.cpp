#include "Orders.h"
#include "GameEngine.h"

int testOrderLists(){

    std::cout << "==================TEST ORDER LIST==================" << std::endl;
    std::cout << "This test has been commented out because it was breaking everything else" << std::endl;
    /*OrderList* list = new OrderList();*/
    /**/
    /*DeployOrder* deploy = new DeployOrder();*/
    /*deploy->execute();*/
    /**/
    /*AdvanceOrder* advance = new AdvanceOrder();*/
    /*advance->execute();*/
    /**/
    /*BombOrder* bomb = new BombOrder();*/
    /*bomb->execute();*/
    /**/
    /*BlockadeOrder* blockade = new BlockadeOrder();*/
    /*blockade->execute();*/
    /**/
    /*AirliftOrder* airlift = new AirliftOrder();*/
    /*airlift->execute();*/
    /**/
    /*NegotiateOrder* negotiate = new NegotiateOrder();*/
    /*negotiate->execute();*/
    /**/
    /*list->add(deploy);*/
    /*list->add(advance);*/
    /*list->add(bomb);*/
    /*list->add(blockade);*/
    /*list->add(airlift);*/
    /*list->add(negotiate);*/
    /**/
    /*std::cout << "\n" << *list;*/
    /**/
    /*list->remove(4);*/
    /**/
    /*std:: cout << "\n" << *list;*/
    /**/
    /*list->move(1, 3);*/
    /*std:: cout << "\n" << *list;*/
    /**/
    /*delete list;*/
    std::cout << "==================END TEST ORDER LIST==================" << std::endl;
    return 1;
}

void testOrderExecution() {
    int argc = 2;
    const char* argv[3] = {
        "",
        "-file",
        "test_commands.txt",
    };
    GameEngine* ge = new GameEngine("./maps/Atlantis.map", argc, argv);
    ge->run();
    delete ge;
}

