#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Map.h"
#include "Cards.h"
#include "Orders.h"

using std::string;
using std::vector;
/*

Player:
    - owns territories
    - owns a hand of cards ===== Hand is a collection of cards from the Deck --> 2 classes to be implemented
    - owns a list of orders
    toDefend() -- list of terr
    issueOrder() -- creates an order object, adds to the list of orders

*/
class Player{

    private:
        int playerID;
        string name;
        vector<int>* territoriesToDefend;
        Hand* currHand;
        OrderList* orders;
        Map& currMap;
    
    public:
        Player(int, string, int, Map&);
        Player(const Player&);
        ~Player();

        vector<int>& toDefend();
        vector<int> toAttack();
        Hand& getHand();
        OrderList& getOrderList();
        void issueOrder(Order* o);

        // getters
        int getID();
        string getName();
        // setters
        void setID(int);
        void setName(string);

        bool clearOrders();

        // overloaded stream insertion
        friend std::ostream& operator<<(std::ostream& os, Player& obj);
        

};


void testPlayers();
