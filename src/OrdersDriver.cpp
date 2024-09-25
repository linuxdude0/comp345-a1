#include "Orders.h"

int testOrderLists(){

    OrderList* list = new OrderList();

    DeployOrder* deploy = new DeployOrder();
    deploy->execute();

    AdvanceOrder* advance = new AdvanceOrder();
    advance->execute();

    BombOrder* bomb = new BombOrder();
    bomb->execute();

    BlockadeOrder* blockade = new BlockadeOrder();
    blockade->execute();

    AirliftOrder* airlift = new AirliftOrder();
    airlift->execute();

    NegotiateOrder* negotiate = new NegotiateOrder();
    negotiate->execute();

    list->add(deploy);
    list->add(advance);
    list->add(bomb);
    list->add(blockade);
    list->add(airlift);
    list->add(negotiate);

    std::cout << "\n" << *list;

    list->remove(4);

    std:: cout << "\n" << *list;

    list->move(1, 3);
    std:: cout << "\n" << *list;

    delete list;
    return 1;
}