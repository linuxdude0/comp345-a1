#include "Player.h"

using std::string;
using std::vector;

// constructor
Player::Player(int id, string name):
    playerID(id),
    name(name)
    {
        territoriesToDefend = new vector<int>;
        currHand = new Hand();
        orders = new OrderList();
    };

// copy constructor
Player::Player(Player& other):
    playerID(other.playerID), 
    name(other.name)
    {
    
        territoriesToDefend = new vector<int>(* other.territoriesToDefend );
        currHand = new Hand(* other.currHand);
        orders = new OrderList(* other.orders);
    }

// destructor
Player::~Player(){

    delete currHand;
    delete orders;
    delete territoriesToDefend;

}


vector<int>* Player::toDefend(){ return territoriesToDefend;}


//     TO DO: i would loop thru the current map object to find all adjacent
//           territories to those the player currently owns and return a list (meaning he can attack them)
vector<int> Player::toAttack(Map* currMap){

}


void Player::issueOrder(Order* o){

    // TO DO --> would append the Order object to whatever structure the OrderList is
}


Hand* Player::getHand(){return currHand;}
OrderList* Player::getOrderList(){return orders;}

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
    for(int index : *(obj.toDefend())){
        os << "\n" << index;
    }

    // TO DO --> 
    // when implemented
    // os << "\nHand: " << *(obj.getHand());
    // os << "\nOrders: " << *(obj.getOrderList());


    return os;

}
