#ifndef ORDERS_H
#define ORDERS_H

#include <vector>
#include <iostream>
using std::vector;
using std::ostream;

class Order{

public:
    Order();
    virtual bool validate() = 0 ;
    virtual void execute() = 0;

};

class DeployOrder : public Order {
public:
    bool validate() override;
    void execute() override;
    friend ostream & operator << (ostream & out, const DeployOrder & deployOrder);

};

class AdvanceOrder : public Order {
public:
    bool validate() override;
    void execute() override;
    friend ostream & operator << (ostream & out, const AdvanceOrder & advanceOrder);
};

class BombOrder : public Order {
public:
    bool validate() override;
    void execute() override;
    friend ostream & operator << (ostream & out, const BombOrder & bombOrder);
};

class BlockadeOrder : public Order {
public:
    bool validate() override;
    void execute() override;
    friend ostream & operator << (ostream & out, const BlockadeOrder & blockadeOrder);
};

class AirliftOrder : public Order {
public:
    bool validate() override;
    void execute() override;
    friend ostream & operator << (ostream & out, const AirliftOrder & airliftOrder);
};

class NegotiateOrder : public Order {
public:
    bool validate() override;
    void execute() override;
    friend ostream & operator << (ostream & out, const NegotiateOrder & negotiateOrder);
};

class OrderList{

private:
    vector<Order*> orders;

public:
    void remove(unsigned index);
    void move(unsigned oldPosition, unsigned newPosition);

};

#endif