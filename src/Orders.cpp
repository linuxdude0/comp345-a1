#include "Orders.h"
#include "common.h"
#include "Mappings.h"
#include <algorithm>
#include <cassert>

extern std::vector<std::tuple<unsigned, Player*, unsigned>> territory_owner_troops_mappings;

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
    for( unsigned int i : player->toDefend()){

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
    if(num_troops > static_cast<unsigned int>(player->getReinforcementPool())){
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
                std::get<2>(smth) += num_troops;
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
AdvanceOrder::AdvanceOrder(Player* player, unsigned territory_target, unsigned territory_source, unsigned num_troops, Map* map_ptr) {
    this->player = player;
    this->territory_target = territory_target;
    this->territory_source = territory_source;
    this->num_troops = num_troops;
    this->orderKind = OrderKind::ADVANCE;
    this->map_ptr = map_ptr;
}

bool AdvanceOrder::targetbelongsToPlayer(){

    for(unsigned i : player->toDefend()){

        if(i == territory_target){return true;}
    }
    return false;
}

bool AdvanceOrder::fight(){

    std::tuple<unsigned, Player*, unsigned>* source = nullptr; 
    std::tuple<unsigned, Player*, unsigned>* target = nullptr;

    // let's get those map entries... 
    for(auto& smth : territory_owner_troops_mappings){
        if(std::get<0>(smth) == territory_source){
            source = &smth;
        }
        if(std::get<0>(smth) == territory_target){
            target = &smth;
        }
    }
    assert(source && target);
    std::get<2>(*source) -= num_troops; // they left from source, they ll either die or occupy the other terr.

    unsigned attackers = num_troops;
    unsigned defenders = std::get<2>(*target);

    std::cout << "[info] Battle:  " << std::get<1>(*source)->getName() <<" (" << attackers << " units)" << " vs " << std::get<1>(*target)->getName() <<" (" << defenders << " units)"  << " to control territory=" << territory_target << std::endl;
    // attackers attack and defenders fight until either all attackers or defenders are dead.
    while (attackers > 0 && defenders > 0) {
        //attacker has a 60% chance to kill a defender
        if ((rand() % 100) < 60) {
            defenders--;
        }
        //defender has a 70% chance to kill an attacker
        if (defenders > 0 && (rand() % 100) < 70) {
            attackers--;
        }
    }

    // means attackers won
    if(defenders == 0 && attackers > 0){

        std::cout << "[info] [Player: " << std::get<1>(*source)->getName() << "] won the battle against" << std::get<1>(*target)->getName() << " and is capturing a new territory  " << territory_target << " with the remaining " << attackers << " units" << std::endl;

        vector<int>& losersTerrs = std::get<1>(*target)->toDefend();
        losersTerrs.erase(std::remove(losersTerrs.begin(), losersTerrs.end(), territory_target), losersTerrs.end());
        std::get<1>(*target) = std::get<1>(*source); // new owner in the global mapping
        std::get<2>(*target) = attackers; // the leftover attackers occupy the new terr
        std::get<1>(*source)->toDefend().push_back(territory_target);
        std::get<1>(*source)->setCardToIssueFlag(); // flag to issue a card at the end of this turn, given that a territory was captured
        
        

        return true;
        
    } // means attackers lost
    else{
    
        std::get<2>(*target) = defenders; // the defendants lost warriors.
        std::cout << "[info] [Player: " << std::get<1>(*source)->getName() << "] lost the battle against" << std::get<1>(*target)->getName() << " over territory =  " << territory_target << ". There are still " << defenders << " remaining units belonging to " << std::get<1>(*target)->getName() << " stationed."<< std::endl;
        return false;
    }

}

bool AdvanceOrder::validate(){

    // come on why would you do that
    if(territory_target == territory_source){
        std::cout << "[!] Advance Order Validation failed:" << " source territory and target territory are the same (really?) " << std::endl;
        return false;
    }
    
    if (num_troops == 0) {
        std::cout << "[!] Advance Order Validation failed:" << " no troops to advance (really?) " << std::endl;
        return false;
    }

    // territory_owner_troops_mapping

    // 1) if source belongs to player
    // 2) if num_troops to advance < num_troops at that terr in the global territory_owner_troops_mapping
    // 3) if target territory is adjacent to source
    

    // ---  source belongs to player invoking ? YES (from linuxdude)
    bool match = false;
    for(unsigned i : player -> toDefend()){
        if(i == territory_source){
            match = true;
            break;
        }
    }
    if(!match){

        std::cout << "[!] Advance Order Validation failed:" << " source territory does not belong to player " << player->getName() << std::endl;    
        return false;
    }



    //  check that correct num of troops to advance
    for (const auto& tuple : territory_owner_troops_mappings){


        if(std::get<0>(tuple) == territory_source){
            if(std::get<1>(tuple) != player){
                // if the owner in the global map is not correct... must be a bug then needa fix the logic
                std::cout << "[!] Advance Order Validation failed:" << " source territory does not belong to player " << player->getName() << " in the global mapping" <<std::endl;
                
                return false;
                
            }

            if(std::get<2>(tuple) < num_troops){
                std::cout << std::get<2>(tuple);
                // if wrong number of troops demanded to be advanced (more than available) --> this check should prob be performed in the game engine tbh...
                std::cout << "[!] Advance Order Validation failed:" << " number of troops requested to advance is superior to that in the source territory  " << territory_source << " for player " << player->getName() <<std::endl;
                return false;
            }

        }
    }


    // check that the target terr is an adjacent terr to the source terr
    match = false;
    Map::Territory source = (map_ptr->getTerritory(territory_source));
    for(unsigned i = 0; i < source.num_adjacent_territories; i++){

        if(source.adjacent_territories_indexes[i] == territory_target){

            match = true;
            break;
        }
    }
    if(!match){
        std::cout << "[!] Advance Order Validation failed for player " << player->getName() <<  ": target territory= " << territory_target << " is not adjacent to source =" << territory_source <<std::endl;
        return false;
    }

    
    return true;
}

void AdvanceOrder::execute(){
    if (validate()){
        
        std::cout << "Executing Advance Order for Player [" << player->getName() << "]"<< std::endl;
        

        // if advancing to own territory
        if(targetbelongsToPlayer()){
            
            std::cout << "[info] [Player: " << player->getName() << "] : " << num_troops << " advancing from " << territory_source << " to " << territory_target << std::endl;

            for(auto& smth : territory_owner_troops_mappings){

                // remove troops from source in the global mapping
                if(std::get<0>(smth) == territory_source){
                    std::get<2>(smth) -= num_troops;
                }
                if(std::get<0>(smth) == territory_target){
                    std::get<2>(smth) += num_troops;
                }

            }
            std::cout << "[ok] Advanced Troops for [Player: " << player->getName() << "] from " << territory_source << " to " << territory_target << std::endl; 
        }
        //else fight mechanics and buncha problems...
        else{

            bool won = fight();
            if(won){
                std::cout << "[ok] [Player: " << player->getName() << "] captured a new territory " << territory_target << std::endl; 
            }
            else{
                std::cout << "[ok] [Player: " << player->getName() << "] didn't manage to capture a new territory  " << territory_target << std::endl;
    
            }
        }
    }
    else{
        std::cout << "[!] Advance execution failed for player " << player->getName() << std::endl;
        return;
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
BlockadeOrder::BlockadeOrder(Player* player, unsigned territory_target,Player* neutralPlayer) {
    this->player = player;
    this->territory_target = territory_target;
    this->orderKind = OrderKind::BLOCKADE;
    this->neutralPlayer = neutralPlayer;
}

bool BlockadeOrder::validate() {
        // check if deployment territory truly belongs to the player
    bool match = false; 
    for( unsigned int i : player->toDefend()){

        if(i == territory_target){
            match = true;
            break;
        }
    }
    if(!match){
        std::cout << "[!] Order Validation failed: Territory assigned for deployment does not belong to player " << player->getName() << std::endl; 
        return false;
    }
    return true;
}

void BlockadeOrder::execute() {
 if (validate()) {
        std::cout << "Executing Blockade\n";

        bool validOrder = false;

        for (auto& mapping : territory_owner_troops_mappings) {
            unsigned territoryId = std::get<0>(mapping);
            Player* owner = std::get<1>(mapping);
            unsigned& troops = std::get<2>(mapping);

            if (territoryId == territory_target) {
                if (owner == player) {
                    // Double the number of army units
                    troops *= 2;

                    // Transfer ownership to the Neutral player
                    std::get<1>(mapping) = neutralPlayer;

                    validOrder = true;
                    break;
                } else {
                    // The target territory belongs to an enemy player
                    std::cout << "[!] Order Execution failed: Territory belongs to an enemy player." << std::endl;
                    validOrder = false;
                    break;
                }
            }
        }

        if (!validOrder) {
            std::cout << "[!] Blockade order execution failed." << std::endl;
        } else {
            std::cout << "Blockade order executed successfully." << std::endl;
        }
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

