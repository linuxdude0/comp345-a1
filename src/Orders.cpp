#include "Orders.h"
#include "common.h"
#include "Mappings.h"

//Default constructor
Order::Order() = default;

Order::~Order() = default;

ostream& operator<< (ostream & out, const Order & order){
    switch (order.orderKind) {
        case OrderKind::DEPLOY:
            out << (DeployOrder&)order;
            break;
        case OrderKind::ADVANCE:
            out << (AdvanceOrder&)order;
            break;
        case OrderKind::BOMB:
            out << (BombOrder&)order;
            break;
        case OrderKind::BLOCKADE:
            out << (BlockadeOrder&)order;
            break;
        case OrderKind::AIRLIFT:
            out << (AirliftOrder&)order;
            break;
        case OrderKind::NEGOTIATE:
            out << (NegotiateOrder&)order;
            break;
    }
    return out;
}

OrderList::OrderList(OrderList* orders) : orders(orders->orders) {}

//Implementing Deploy Order
DeployOrder::DeployOrder(Player* player, unsigned territory_target, unsigned num_troops){
    this->player = player;
    this->territory_target = territory_target;
    this->num_troops = num_troops;
    this->orderKind = OrderKind::DEPLOY;
}

bool DeployOrder::validate() {
    std::cout << "Validating Deploy" << std::endl;
    // 

    // check if deployment territory truly belongs to the player
    bool match = false; 
    for(int i : player->toDefend()){

        if(i == territory_target){
            match = true;
            break;
        }
    }
    if(!match){
        std::cout << "[!] Order Validation failed: Territory assigned for deployment does not belong to player " << player->getName() << std::endl; 
        return false;
    }

    // check that the number of troops requested for deployment is adequate
    if(num_troops > player->getReinforcementPool()){
        std::cout << "[!] Order Validation failed: Number of troops requested to deploy on territory " << territory_target << " is larger than available pool for player " << player->getName() << std::endl; 
        return false;
    }
    return true;
}

void DeployOrder::execute() {
    if (validate()){
        std::cout << "Executing Deploy for Player..." << player->getName() << std::endl;

        // deploy number of troops requested to target territory, remove them from the player's pool
        for(auto& smth : territory_owner_troops_mappings){

            if(std::get<0>(smth) == territory_target){
                std::get<2>(smth) = num_troops;
                player->removeFromReinforcementPool(num_troops);
            }
        }
        std::cout << "[ok] Deploy Executed for [Player: " << player->getName() << "] on territory " << territory_target << "with " << num_troops << " units" << std::endl;
    }
    else{
        std::cout << "[!] Deploy execution failed for player " << player->getName() << std::endl;
        return;
    }
}

ostream& operator << (ostream & out, const DeployOrder & deployOrder){
    UNUSED(deployOrder);
    out << "Deploy Order";
    return out;
}

//Implementing Advance Order
AdvanceOrder::AdvanceOrder(Player* player, unsigned territory_target, unsigned territory_source, unsigned num_troops) {
    this->player = player;
    this->territory_target = territory_target;
    this->territory_source = territory_source;
    this->num_troops = num_troops;
    this->orderKind = OrderKind::ADVANCE;
}

bool AdvanceOrder::validate() {
    return true;
}

void AdvanceOrder::execute() {
    if (validate()){
        std::cout << "Executing Advance\n";
    }
}

ostream & operator << (ostream & out, const AdvanceOrder & advanceOrder){
    UNUSED(advanceOrder);
    out << "Advance Order";
    return out;
}

//Implementing Bomb Order
BombOrder::BombOrder(Player* player, unsigned territory_target) {
    this->player = player;
    this->territory_target = territory_target;
    this->orderKind = OrderKind::BOMB;
}

bool BombOrder::validate() {
    return true;
}

void BombOrder::execute() {
    if (validate()){
        std::cout << "Executing Bomb\n";
    }
}

ostream& operator << (ostream & out, const BombOrder & bombOrder){
    UNUSED(bombOrder);
    out << "Bomb Order";
    return out;
}

//Implementing Blockade Order
BlockadeOrder::BlockadeOrder(Player* player, unsigned territory_target) {
    this->player = player;
    this->territory_target = territory_target;
    this->orderKind = OrderKind::BLOCKADE;
}

bool BlockadeOrder::validate() {
    return true;
}

void BlockadeOrder::execute() {
    if (validate()){
        std::cout << "Executing Blockade\n";
    }
}

ostream & operator << (ostream & out, const BlockadeOrder & blockadeOrder){
    UNUSED(blockadeOrder);
    out << "Blockade Order";
    return out;
}

//Implementing Airlift Order
AirliftOrder::AirliftOrder(Player* player, unsigned territory_target, unsigned territory_source, unsigned num_troops) {
    this->player = player;
    this->territory_target = territory_target;
    this->territory_source = territory_source;
    this->num_troops = num_troops;
    this->orderKind = OrderKind::AIRLIFT;
}

bool AirliftOrder::validate() {
    return true;
}

void AirliftOrder::execute() {
    if (validate()){
        std::cout << "Executing Airlift\n";
    }
}

ostream& operator << (ostream & out, const AirliftOrder & airliftOrder){
    UNUSED(airliftOrder);
    out << "Airlift Order";
    return out;
}

//Implementing Negotiate Order
NegotiateOrder::NegotiateOrder(Player* player, unsigned territory_target) {
    this->player = player;
    this->territory_target = territory_target;
    this->orderKind = OrderKind::NEGOTIATE;
}

bool NegotiateOrder::validate() {
    return true;
}

void NegotiateOrder::execute() {
    if (validate()){
        std::cout << "Executing Negotiate\n";
    }
}
ostream & operator << (ostream & out, const NegotiateOrder & negotiateOrder){
    UNUSED(negotiateOrder);
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
    for (size_t i = 0; i < this->orders.size(); ++i) {
        if ((unsigned)this->orders[i].index == index){
            orders[i].index = -1;
            delete orders[i].order;
            orders[i].order = nullptr;
        }
    }
}

//Move an order in the list
void OrderList::move(unsigned int oldPosition, unsigned int newPosition) {

    bool foundSource = false;
    bool foundDestination = false;

    int source_index = -1;
    int destination_index = -1;
    for (size_t i = 0; i < this->orders.size(); ++i){
        if ((unsigned)orders[i].index == oldPosition){
            source_index = i;
            foundSource = true;
        }
        if ((unsigned)orders[i].index == newPosition){
            destination_index = i;
            foundDestination = true;
        }
    }

    if (foundSource) {
        if (foundDestination){
            int temp = orders[destination_index].index;
            orders[destination_index].index = orders[source_index].index;
            orders[source_index].index = temp;
        }
        else {
            orders[source_index].index = (int)newPosition;
        }
    }
    else {
        throw "Cannot find order";
    }

}

void OrderList::executeAll() {
    for (size_t i = 0; i < this->orders.size(); ++i) {
        if (this->orders[i].index >= 0){
            orders[i].order->execute();
            orders[i].index = -1;
            delete orders[i].order;
            orders[i].order = nullptr;
        }
    }
}

OrderList::OrderList(){}

OrderList::~OrderList() {
    for (size_t i = 0; i < this->orders.size(); ++i) {
        if (this->orders[i].index >= 0){
            orders[i].index = -1;
            delete orders[i].order;
            orders[i].order = nullptr;
        }
    }
}

 ostream & operator << (ostream & out, const OrderList & orderList){
     for (OrderList::OrderListItem item: orderList.orders){
         if (item.index >= 0){
             out << "Index: " << item.index << " " << *item.order << "\n";
         }
     }
     return out;
}

