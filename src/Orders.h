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
    DeployOrder();
    bool validate() override;
    void execute() override;
    friend ostream & operator << (ostream & out, const DeployOrder & deployOrder);

};

class AdvanceOrder : public Order {
public:
    AdvanceOrder();
    bool validate() override;
    void execute() override;
    friend ostream & operator << (ostream & out, const AdvanceOrder & advanceOrder);
};

class BombOrder : public Order {
public:
    BombOrder();
    bool validate() override;
    void execute() override;
    friend ostream & operator << (ostream & out, const BombOrder & bombOrder);
};

class BlockadeOrder : public Order {
public:
    BlockadeOrder();
    bool validate() override;
    void execute() override;
    friend ostream & operator << (ostream & out, const BlockadeOrder & blockadeOrder);
};

class AirliftOrder : public Order {
public:
    AirliftOrder();
    bool validate() override;
    void execute() override;
    friend ostream & operator << (ostream & out, const AirliftOrder & airliftOrder);
};

class NegotiateOrder : public Order {
public:
    NegotiateOrder();
    bool validate() override;
    void execute() override;
    friend ostream & operator << (ostream & out, const NegotiateOrder & negotiateOrder);
};

class OrderList{

private:
    struct OrderListItem {
        Order* order;
        int index;
    };
    vector<OrderListItem> orders;


public:
    void add(Order* order);
    void remove(unsigned index);
    void move(unsigned oldPosition, unsigned newPosition);
    void executeAll();
    ~OrderList();

    friend ostream & operator << (ostream & out, const OrderList & orderList);

};

int testOrderLists();

#endif
