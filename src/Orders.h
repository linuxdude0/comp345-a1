#pragma once

#include <random>
#include <vector>
#include <string>

class Order{
public:
    Order(std::string ordername);
    virtual bool validate() = 0;
    virtual void execute() = 0;
};

class DeployOrder : public Order {
public:
    DeployOrder();
    virtual bool validate();
    virtual void execute();
};

class AdvanceOrder : public Order {
public:
    AdvanceOrder();
    virtual bool validate();
    virtual void execute();
};

class BombOrder : public Order {
public:
    BombOrder();
    virtual bool validate();
    virtual void execute();
};

class BlockadeOrder : public Order {
public:
    BlockadeOrder();
    virtual bool validate();
    virtual void execute();
};

class AirliftOrder : public Order {
public:
    AirliftOrder();
    virtual bool validate();
    virtual void execute();
};

class NegotiateOrder : public Order {
public:
    NegotiateOrder();
    virtual bool validate();
    virtual void execute();
};

class OrderList{
private:
    struct OrderItem {
	Order* ptr;
	size_t index;
    };
    std::vector<OrderItem> orders;
public:
    OrderList();
    void remove(size_t index);
    void move(size_t source, size_t destination);
    friend std::ostream& operator<<(std::ostream& os, const OrderList& map);
    ~OrderList();
};
