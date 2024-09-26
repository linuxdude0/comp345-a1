#include "Player.h"
#include <algorithm>

using std::string;
using std::vector;

// constructor
Player::Player(int playerID, string name, int startTerrIndex, Map* map):
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




/*

    IN: takes an OrderKind that the user wishes to push into a given player's order list
    OUT:
    --> true if an Order object was allocated and successfully pushed in the orders list
    --> false if an Order object couldnt be pushed in the order list (Card missing in the Hand or wrong kind of order)


*/
bool Player::issueOrder(OrderKind ok){

        if (ok == OrderKind::DEPLOY){

            orders->add(new DeployOrder());

        }
        if (ok == OrderKind::ADVANCE){
            orders->add(new AdvanceOrder());
        }
        if(ok == OrderKind::BOMB){

            // needs a card
            int handLen = this->currHand->handCards.size();

            for(int i = 0; i < currHand->handCards.size(); i++){

                if (currHand->handCards.at(i).getType() == CardType::BOMB){
                    currHand->handCards.at(i).play();
                    orders->add(new BombOrder());
                    return true;
                }

            }

            std::cout << "\nPlayer doesn't have a Bomb card, order aborted" << std::endl;
            return false;
       }

        if(ok == OrderKind::BLOCKADE){

            // needs a card
            for(int i = 0; i < currHand->handCards.size(); i++){

                if (currHand->handCards.at(i).getType() == CardType::BLOCKADE){
                    currHand->handCards.at(i).play();
                    orders->add(new BlockadeOrder());
                    return true;
                }

            }
            std::cout << "\nPlayer doesn't have a Blockade card, order aborted" << std::endl;
            return false;

        }
        if(ok == OrderKind::AIRLIFT){


            // needs a card
            for(int i = 0; i < currHand->handCards.size(); i++){

                if (currHand->handCards.at(i).getType() == CardType::AIRLIFT){
                    currHand->handCards.at(i).play();
                    orders->add(new AirliftOrder());
                    return true;
                }

            }
            std::cout << "\nPlayer doesn't have an Airlift card, order aborted" << std::endl;
            return false;
        }
        if(ok == OrderKind::NEGOTIATE){


           // needs a card
            for(int i = 0; i < currHand->handCards.size(); i++){

                if (currHand->handCards.at(i).getType() == CardType::BLOCKADE){
                    currHand->handCards.at(i).play();
                    orders->add(new BlockadeOrder());
                    return true;
                }

            }
            std::cout << "\nPlayer doesn't have a Blockade card, order aborted" << std::endl;
            return false;

        }
        else{
            
            std::cout << "\nAttempt to push a wrong kind of Order in the Order list of " << Player::name << std::endl;  
            return false;
        }    
    
    };


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
    for (Card c : obj.currHand->handCards){
        
        switch(c.getType()){

            case CardType::BOMB: 

                os << counter << " Bomb Card " << std::endl;
            
            case CardType::BLOCKADE:

                os << counter << " Blockade Card " << std::endl;

            case CardType::AIRLIFT:
                
                os << counter << " Airlift Card " << std::endl;

            case CardType::DIPLOMACY:

                os << counter << " Diplomacy Card " << std::endl;
            
            case CardType::REINFORCEMENT:

                os << counter << " Reinforcements Card " << std::endl;
        }

        counter++;

    }


    os << "Order List contents: " << std::endl;
    os << obj.orders;
    return os;
};


