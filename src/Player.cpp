#include "Player.h"
#include <algorithm>

using std::string;
using std::vector;

// constructor
Player::Player(int playerID, string name, int startTerrIndex, Map& map):
    playerID(playerID),
    name(name),
    currMap(map)
    {
        territoriesToDefend = new vector<int>;
        territoriesToDefend->push_back(startTerrIndex);
        currHand = new Hand();
        orders = new OrderList();
    };

// copy constructor
Player::Player(const Player& other):
    playerID(other.playerID),
    name(other.name), 
    currMap(other.currMap)
    {
    
        territoriesToDefend = new vector<int>(*other.territoriesToDefend );
        currHand = new Hand(* other.currHand);
        orders = new OrderList(* other.orders);
    }

// destructor
Player::~Player(){

    delete currHand;
    delete orders;
    delete territoriesToDefend;

}


vector<int>& Player::toDefend(){ return *territoriesToDefend;}


// move semantics would kick in given that the vector will disappear here after function returns
// would return an empty vector if the player owns nothing, it means he can attack nothing
vector<int> Player::toAttack(){
    vector<int> can_attack{};
    if (this->territoriesToDefend->size() == 0){
        return can_attack;
    }
    
    for (int terrID : *(this->territoriesToDefend)){
        
        Map::Territory ownedTerr = this->currMap.getTerritory(terrID);
        
        for(size_t i = 0; i < ownedTerr.num_adjacent_territories; i++){

            int candidateTerr = ownedTerr.adjacent_territories_indexes[i];

            // check that not in the owned territories already
            if(std::find(this->territoriesToDefend->begin(), this->territoriesToDefend->end(), candidateTerr) == this->territoriesToDefend->end()){
                can_attack.push_back(candidateTerr);
            }
        }
    }
    return can_attack;
}


void Player::issueOrder(Order* o){
    // TODO: --> would append the Order object to whatever structure the OrderList is
    // or an order reference ....
}


Hand& Player::getHand(){return *currHand;}
OrderList& Player::getOrderList(){return *orders;}

string Player::getName(){return name;}
int Player::getID(){return playerID;}

void Player::setID(int newID){this->playerID = newID;}
void Player::setName(std::string newName){this->name = newName;}

std::ostream& operator<<(std::ostream& os, Player& obj){

    os 
    << "Player: " 
    << "\nid :" << obj.getID()
    << "\nname :" << obj.getName();


    os << "\n\nTo Defend:";

    vector<int>& toDefend = obj.toDefend();
    for(size_t i = 0; i < toDefend.size(); i++){
        os << i << ": " << toDefend[i];
    }

    vector<int> toAttack = obj.toAttack();
    for(size_t i = 0; i < toAttack.size(); i++){
        os << i << ": " << toAttack[i];
    }



    // TODO: --> 
    // when implemented
    // os << "\nHand: " 
    // os << "\nOrders: " 


    return os;

}
