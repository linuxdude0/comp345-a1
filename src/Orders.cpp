#include "Orders.h"

//Default constructor
Order::Order() = default;


Order::~Order() {

}

//Implementing Deploy Order
bool DeployOrder::validate() {
    return true;
}

void DeployOrder::execute() {

    if (validate()){
        std::cout << "Executing Deploy\n";

    }
}

ostream & operator << (ostream & out, const DeployOrder & deployOrder){
    out << "Deploy Order";
    return out;
}

//Implementing Advance Order
bool AdvanceOrder::validate() {
    return true;
}

void AdvanceOrder::execute() {

    if (validate()){
        std::cout << "Executing Advance\n";

    }
}

ostream & operator << (ostream & out, const AdvanceOrder & advanceOrder){
    out << "Advance Order";
    return out;
}

//Implementing Bomb Order
bool BombOrder::validate() {
    return true;
}

void BombOrder::execute() {

    if (validate()){
        std::cout << "Executing Bomb\n";

    }
}

ostream & operator << (ostream & out, const BombOrder & bombOrder){
    out << "Bomb Order";
    return out;
}

//Implementing Blockade Order
bool BlockadeOrder::validate() {
    return true;
}

void BlockadeOrder::execute() {

    if (validate()){
        std::cout << "Executing Blockade\n";

    }
}

ostream & operator << (ostream & out, const BlockadeOrder & blockadeOrder){
    out << "Blockade Order";
    return out;
}

//Implementing Airlift Order
bool AirliftOrder::validate() {
    return true;
}

void AirliftOrder::execute() {

    if (validate()){
        std::cout << "Executing Airlift\n";

    }
}

ostream & operator << (ostream & out, const AirliftOrder & airliftOrder){
    out << "Airlift Order";
    return out;
}

//Implementing Negotiate Order
bool NegotiateOrder::validate() {
    return true;
}

void NegotiateOrder::execute() {

    if (validate()){
        std::cout << "Executing Negotiate\n";

    }
}
ostream & operator << (ostream & out, const NegotiateOrder & negotiateOrder){
    out << "Negotiate Order";
    return out;
}

//Implementing OrderList
//Add an order to the list
void OrderList::add(Order* order) {

    OrderListItem item = {order, (int)orders.size()};
    orders.push_back(item);
}

//Remove an order from the list
void OrderList::remove(unsigned int index) {

    for (OrderListItem item: this->orders){
        if ((unsigned)item.index == index){
            item.index = -1;
            delete item.order;
            item.order = nullptr;
        }
    }
}

//Move an order in the list
void OrderList::move(unsigned int oldPosition, unsigned int newPosition) {

    bool foundSource = false;
    bool foundDestination = false;

    OrderListItem source {};
    OrderListItem destination{};
    for (OrderListItem item: this->orders){
        if ((unsigned)item.index == oldPosition){
            source = item;
            foundSource = true;
        }
        if ((unsigned)item.index == newPosition){
            destination = item;
            foundDestination = true;
        }
    }

    if (foundSource) {
        if (foundDestination){
            int temp = destination.index;
            destination.index = source.index;
            source.index = temp;
        }
        else {
            source.index = (int)newPosition;
        }
    }
    else {
        throw "Cannot find order";
    }

}

