#include "Player.h"
#include "Cards.h"
#include "Orders.h"
#include <algorithm>
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
    cardIssuedThisTurn(false)
    {
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
    currDeck(other.currDeck)
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


vector<int>& Player::toDefend(){ return *territoriesToDefend;}


/*
    returns a vector of all the adjacent territories to the territories currently owned by the player
*/
vector<int> Player::toAttack(){
    vector<int> can_attack{};
    if (this->territoriesToDefend->size() == 0){
        return can_attack;
    }
    
    for (int terrID : *(this->territoriesToDefend)){
        
        Map::Territory ownedTerr = this->currMap->getTerritory(terrID);
        
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

void Player::clearIssuedCardFlag(){cardIssuedThisTurn = false;};
void Player::setIssuedCardFlag(){cardIssuedThisTurn = true;};
bool Player::issuedThisTurn(){return cardIssuedThisTurn;};




/*

    IN: takes an OrderKind that the user wishes to push into a given player's order list
    OUT:
    --> true if an Order object was allocated and successfully pushed in the orders list
    --> false if an Order object couldnt be pushed in the order list (Card missing in the Hand or wrong kind of order)


*/
/**/
/*bool Player::issueOrder(OrderKind ok){*/
/*    if (ok == OrderKind::DEPLOY){*/
/*        orders->add(new DeployOrder());*/
/*        return true;*/
/*    }else if (ok == OrderKind::ADVANCE){*/
/*        orders->add(new AdvanceOrder());*/
/*        return true;*/
/*    }else if(ok == OrderKind::BOMB){*/
/*        // needs a card*/
/*        int handLen = this->currHand->handCards.size();*/
/*        for(size_t i = 0; i < currHand->handCards.size(); i++){*/
/*            if (currHand->handCards.at(i)->getType() == CardType::BOMB){*/
/*                currHand->handCards.at(i)->play(currDeck);*/
/*                currHand->handCards[i] = nullptr;*/
/*                orders->add(new BombOrder());*/
/*                return true;*/
/*            }*/
/*        }*/
/*        std::cout << "\nPlayer doesn't have a Bomb card, order aborted" << std::endl;*/
/*        return false;*/
/*    }else if(ok == OrderKind::BLOCKADE){*/
/*         needs a card*/
/*        int handLen = this->currHand->handCards.size();*/
/*        for(size_t i = 0; i < currHand->handCards.size(); i++){*/
/*            if (currHand->handCards.at(i)->getType() == CardType::BLOCKADE){*/
/*                currHand->handCards.at(i)->play(currDeck);*/
/*                currHand->handCards[i] = nullptr;*/
/*                orders->add(new BlockadeOrder());*/
/*                return true;*/
/*            }*/
/*        }*/
/*        std::cout << "\nPlayer doesn't have a Blockade card, order aborted" << std::endl;*/
/*        return false;*/
/*    }else if(ok == OrderKind::AIRLIFT){*/
/*        // needs a card*/
/*        int handLen = this->currHand->handCards.size();*/
/*        for(size_t i = 0; i < currHand->handCards.size(); i++){*/
/*            if (currHand->handCards.at(i)->getType() == CardType::AIRLIFT){*/
/*                currHand->handCards.at(i)->play(currDeck);*/
/*                currHand->handCards[i] = nullptr;*/
/*                orders->add(new AirliftOrder());*/
/*                return true;*/
/*            }*/
/*        }*/
/*        std::cout << "\nPlayer doesn't have an Airlift card, order aborted" << std::endl;*/
/*        return false;*/
/*    }else if(ok == OrderKind::NEGOTIATE){*/
/*        // needs a card*/
/*        int handLen = this->currHand->handCards.size();*/
/*        for(size_t i = 0; i < currHand->handCards.size(); i++){*/
/*            if (currHand->handCards.at(i)->getType() == CardType::DIPLOMACY){*/
/*                currHand->handCards.at(i)->play(currDeck);*/
/*                currHand->handCards[i] = nullptr;*/
/*                orders->add(new NegotiateOrder());*/
/*                return true;*/
/*            }*/
/*        }*/
/*        std::cout << "\nPlayer doesn't have a Blockade card, order aborted" << std::endl;*/
/*        return false;*/
/*    }else{*/
/*        std::cout << "\nAttempt to push a wrong kind of Order in the Order list of " << Player::name << std::endl;  */
/*        return false;*/
/*    }*/
/*};*/
/**/


bool Player::playCard(CardType ct) {
    for(size_t i = 0; i < currHand->handCards.size(); i++){
        if (currHand->handCards.at(i)->getType() == ct){
            currHand->handCards.at(i)->play(currDeck);
            currHand->handCards[i] = nullptr;
            return true;
        }
    }
    return false;
}

bool Player::issueOrder(Order* order) {
    switch (order->orderKind) {
        case OrderKind::DEPLOY:
        case OrderKind::ADVANCE:
            this->orders->add(order);
            break;
        case OrderKind::BOMB:
            if (this->playCard(CardType::BOMB)) {
                this->orders->add(order);
            }
            break;
        case OrderKind::AIRLIFT:
            if (this->playCard(CardType::AIRLIFT)) {
                this->orders->add(order);
            }
            break;
        case OrderKind::BLOCKADE:
            if (this->playCard(CardType::BLOCKADE)) {
                this->orders->add(order);
            }
            break;
        case OrderKind::NEGOTIATE:
            if (this->playCard(CardType::DIPLOMACY)) {
                this->orders->add(order);
            }
            break;
    }
    return true;
}

bool Player::ownsTerritory(unsigned index) {
    for (unsigned i : *this->territoriesToDefend) {
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


