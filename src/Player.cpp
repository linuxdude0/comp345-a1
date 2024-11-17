#include "Player.h"
#include "Cards.h"
#include "Orders.h"
#include <algorithm>
#include <set>
#include <vector>
#include "Mappings.h"

using std::string;
using std::vector;

// constructor
Player::Player(int playerID, string name, int startTerrIndex, Map* map, Deck* currDeck):
    reinforcementPool{0},
    playerID(playerID),
    name(name),
    currMap(map),
    currDeck(currDeck),
    cardToIssueFlag(false), 
    no_aggression_this_turn_list{}
    {
        territoriesToDefend = new vector<int>;
        territoriesToDefend->push_back(startTerrIndex);
        currHand = new Hand(currDeck);
        orders = new OrderList();
    };

// мяу: constructor with PlayerStrategy
Player::Player(int playerID, string name, int startTerrIndex, Map* map, Deck* currDeck, PlayerStrategy* ps):
    reinforcementPool{0},
    playerID(playerID),
    name(name),
    currMap(map),
    currDeck(currDeck),
    cardToIssueFlag(false), 
    no_aggression_this_turn_list{}
    {
        playerStrat = ps;
        territoriesToDefend = new vector<int>;
        territoriesToDefend->push_back(startTerrIndex);
        currHand = new Hand(currDeck);
        orders = new OrderList();
    };

// copy constructor
Player::Player(const Player& other):
    playerID(other.playerID),
    name(other.name), 
    currMap(other.currMap),
    currDeck(other.currDeck),
    playerStrat(other.playerStrat)
    {
    
        territoriesToDefend = new vector<int>(*other.territoriesToDefend );
        currHand = new Hand(*other.currHand);
        orders = new OrderList(* other.orders);
    };

// destructor
Player::~Player(){

    delete currHand;
    delete orders;
    delete territoriesToDefend;

}


vector<int>& Player::toDefend(){ return this->playerStrat->toDefend(this);}
/*
    returns a vector of all the adjacent territories to the territories currently owned by the player
*/
vector<int> Player::toAttack(){return this->playerStrat->toAttack(this);}

void Player::clearCardToIssueFlag(){cardToIssueFlag = false;};
void Player::setCardToIssueFlag(){cardToIssueFlag = true;};
bool Player::cardToReceiveThisTurn(){return cardToIssueFlag;};



bool Player::playCard(CardType ct) {
    if (this->currHand == nullptr) {
        return false;
    }
    for(size_t i = 0; i < currHand->handCards.size(); i++){
        if (this->currHand->handCards.at(i) && currHand->handCards.at(i)->getType() == ct){
            currHand->handCards.at(i)->play(currDeck);
            currHand->handCards[i] = nullptr;
            return true;
        }
    }
    return false;
}

bool Player::issueOrder(Order* order) {return this->playerStrat->issueOrder(this, order);}

bool Player::ownsTerritory(unsigned index) {
    for (unsigned i : *this->territoriesToDefend) {
        if (i == index) {
            return true;
        }
    }
    return false;
}

bool Player::toAttackTerritory(unsigned index) {
    for (unsigned i : this->toAttack()) {
        if (i == index) {
            return true;
        }
    }
    return false;
}

const int& Player::getReinforcementPool(){return reinforcementPool;}


void Player::setReinforcementPool(int i){reinforcementPool = i;}

void Player::addToReinforcementPool(int i){reinforcementPool += i;}

void Player::removeFromReinforcementPool(int i){reinforcementPool -= i;}


Hand* Player::getHand(){return currHand;}
OrderList* Player::getOrderList(){return orders;}

string& Player::getName(){return name;}
int Player::getID(){return playerID;}

void Player::setID(int newID){this->playerID = newID;}
void Player::setName(std::string newName){this->name = newName;}


std::ostream& operator<<(std::ostream& os, Player& obj){

    os 
    << "Player: " 
    << "\nid :" << obj.getID()
    << "\nname :" << obj.getName();


    os << "\n\nTo Defend (executing toDefend() method):\n";

    vector<int>& toDefend = obj.toDefend();
    for(size_t i = 0; i < toDefend.size(); i++){
        os << i << ": " << toDefend[i] << std::endl;
    }

    os << "\n\nTo Attack (executing toAttack() method):\n";

    vector<int> toAttack = obj.toAttack();
    for(size_t i = 0; i < toAttack.size(); i++){
        os << i << ": " << toAttack[i] << std::endl;
    }

    os << "\n\nCurrent Hand:\n";
    
    int counter = 1;
    for (Card* c : obj.currHand->handCards){
        if (!c) {
            continue;
        }
        switch(c->getType()){

            case CardType::BOMB: 

                os << counter << " Bomb Card " << std::endl;
                break;
            
            case CardType::BLOCKADE:

                os << counter << " Blockade Card " << std::endl;
                break;

            case CardType::AIRLIFT:
                
                os << counter << " Airlift Card " << std::endl;
                break;

            case CardType::DIPLOMACY:

                os << counter << " Diplomacy Card " << std::endl;
                break;
            
            case CardType::REINFORCEMENT:

                os << counter << " Reinforcements Card " << std::endl;
                break;
            case CardType::CARD_TYPE_MAX:
                break;
        }
        counter++;
    }


    os << "Order List contents: " << std::endl;
    os << *obj.orders;
    return os;
};


