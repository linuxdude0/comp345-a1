#ifndef ORDERS_H
#define ORDERS_H

#include <vector>
#include <iostream>
using std::vector;
using std::ostream;

enum class OrderKind{
    DEPLOY,
    ADVANCE,
    BOMB,
    BLOCKADE,
    AIRLIFT,
    NEGOTIATE
};

class Order{

protected:
    OrderKind orderKind;

public:
    Order();
    virtual bool validate() = 0 ;
    virtual void execute() = 0;
    friend ostream & operator << (ostream & out, const Order & order);
    virtual ~Order();
    
};

class DeployOrder : public Order {
public:
    Player* p; 
    int target_territory_index;
    GameEngine ge;
    DeployOrder(Player* p, int target_territory_index, GameEngine& ge);
    DeployOrder(DeployOrder* deployOrder);
    bool validate() override;
    void execute() override;
    friend ostream & operator << (ostream & out, const DeployOrder & deployOrder);
    DeployOrder* operator=(DeployOrder* order);
};

class AdvanceOrder : public Order {
public:
    AdvanceOrder();
    AdvanceOrder(AdvanceOrder* advanceOrder);
    bool validate() override;
    void execute() override;
    friend ostream & operator << (ostream & out, const AdvanceOrder & advanceOrder);
    AdvanceOrder* operator=(AdvanceOrder* order);
};

class BombOrder : public Order {
public:
    BombOrder();
    BombOrder(BombOrder* bombOrder);
    bool validate() override;
    void execute() override;
    friend ostream & operator << (ostream & out, const BombOrder & bombOrder);
    BombOrder* operator=(BombOrder* order);
};

class BlockadeOrder : public Order {
public:
    BlockadeOrder();
    BlockadeOrder(BlockadeOrder* blockadeOrder);
    bool validate() override;
    void execute() override;
    friend ostream & operator << (ostream & out, const BlockadeOrder & blockadeOrder);
    BlockadeOrder* operator=(BlockadeOrder* order);
};

class AirliftOrder : public Order {
public:
    AirliftOrder();
    AirliftOrder(AirliftOrder* airliftOrder);
    bool validate() override;
    void execute() override;
    friend ostream & operator << (ostream & out, const AirliftOrder & airliftOrder);
    AirliftOrder* operator=(AirliftOrder* orderrder);
};

class NegotiateOrder : public Order {
public:
    NegotiateOrder();
    NegotiateOrder(NegotiateOrder* negotiateOrder);
    bool validate() override;
    void execute() override;
    friend ostream & operator << (ostream & out, const NegotiateOrder & negotiateOrder);
    NegotiateOrder* operator=(NegotiateOrder* order);
};

class OrderList{

private:
    struct OrderListItem {
        Order* order;
        int index;
    };
    vector<OrderListItem> orders;


public:
    OrderList();
    OrderList(OrderList* orders);
    void add(Order* order);
    void remove(unsigned index);
    void move(unsigned oldPosition, unsigned newPosition);
    void executeAll();
    ~OrderList();

    NegotiateOrder* operator=(NegotiateOrder* order);
    friend ostream & operator << (ostream & out, const OrderList & orderList);

};

int testOrderLists();

#endif
