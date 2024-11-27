#ifndef ORDERS_H
#define ORDERS_H

#include <vector>
#include <iostream>
#include "Map.h"
#include "LoggingObserver.h"
using std::vector;
using std::ostream;

class Player;

enum class OrderKind{
    DEPLOY,
    ADVANCE,
    BOMB,
    BLOCKADE,
    AIRLIFT,
    NEGOTIATE
};

class Order : public ILoggable, public Subject{
protected:
    Player* player;
    unsigned territory_target;
public:
    OrderKind orderKind;
    Order();
    virtual bool validate() = 0 ;
    virtual void execute() = 0;
    friend ostream & operator << (ostream & out, const Order & order);
    virtual ~Order();
    std::string stringToLog() override;   
};

class DeployOrder : public Order {
    unsigned num_troops;
public:
    DeployOrder(Player* player, unsigned territory_target, unsigned num_troops);
    DeployOrder(DeployOrder* deployOrder);
    bool validate() override;
    void execute() override;
    friend ostream & operator << (ostream & out, const DeployOrder & deployOrder);
    DeployOrder* operator=(DeployOrder* order);
};

class AdvanceOrder : public Order {
    unsigned num_troops;
    unsigned territory_source;
    Map* map_ptr;
public:
    AdvanceOrder(Player* player, unsigned territory_target, unsigned territory_source, unsigned num_troops, Map*);
    AdvanceOrder(AdvanceOrder* advanceOrder);
    bool validate() override;
    void execute() override;
    bool targetbelongsToPlayer();
    bool fight(); 
    friend ostream & operator << (ostream & out, const AdvanceOrder & advanceOrder);
    AdvanceOrder* operator=(AdvanceOrder* order);
};

class BombOrder : public Order {
    Map* map_ptr;
public:
    BombOrder(Player* player, unsigned territory_target);
    BombOrder(BombOrder* bombOrder);
    bool validate() override;
    void execute() override;
    friend ostream & operator << (ostream & out, const BombOrder & bombOrder);
    BombOrder* operator=(BombOrder* order);
};

class BlockadeOrder : public Order {
public:
    BlockadeOrder(Player* player, unsigned territory_target,Player* neutralPlayer);
    BlockadeOrder(BlockadeOrder* blockadeOrder);
    bool validate() override;
    void execute() override;
    friend ostream & operator << (ostream & out, const BlockadeOrder & blockadeOrder);
    BlockadeOrder* operator=(BlockadeOrder* order);
private:
    Player* player;
    unsigned territory_target;
    Player* neutralPlayer; // Pointer to the Neutral player
};

class AirliftOrder : public Order {
    unsigned num_troops;
    unsigned territory_source;
public:
    AirliftOrder(Player* player, unsigned territory_target, unsigned territory_source, unsigned num_troops);
    AirliftOrder(AirliftOrder* airliftOrder);
    bool validate() override;
    void execute() override;
    friend ostream & operator << (ostream & out, const AirliftOrder & airliftOrder);
    AirliftOrder* operator=(AirliftOrder* orderrder);
};

class NegotiateOrder : public Order {
public:
    NegotiateOrder(Player* requesting, Player* target);
    NegotiateOrder(NegotiateOrder* negotiateOrder);
    Player* targetPlayer;
    bool validate() override;
    void execute() override;
    friend ostream & operator << (ostream & out, const NegotiateOrder & negotiateOrder);
    NegotiateOrder* operator=(NegotiateOrder* order);
};

class OrderList : public ILoggable, public Subject{
private:
    struct OrderListItem {
        Order* order;
        int index;
    };
    vector<OrderListItem> orders;
    Order* last_added_order = nullptr;
public:
    OrderList();
    OrderList(OrderList* orders);
    void add(Order* order);
    void remove(unsigned index);
    void move(unsigned oldPosition, unsigned newPosition);
    void executeAllOtherOrders();
    void executeNegotiateOrders(); // must be executed before others
    void executeDeployOrders();
    virtual ~OrderList();
    std::string stringToLog() override;
    NegotiateOrder* operator=(NegotiateOrder* order);
    friend ostream & operator << (ostream & out, const OrderList & orderList);
};

int testOrderLists();
void testOrderExecution();

#endif
