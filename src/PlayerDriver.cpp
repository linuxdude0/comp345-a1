#include <iostream>
#include "Player.h"
#include "PlayerDriver.h"

void testPlayers(std::string filename, Deck& testDeck){

    std::cout << "\n THIS IS A TEST OF THE PLAYER CLASS:" << std::endl;

    Map* testMap = new Map(filename);
    
    Player* p1 = new Player(1, "John Doe", 1, testMap, testDeck);



    std::cout << "\n The player 1:\n" << *p1 << std::endl;


    std::cout << "TESTING issueOrder()";
    p1->issueOrder(OrderKind::ADVANCE);
    p1->issueOrder(OrderKind::BLOCKADE);
    p1->issueOrder(OrderKind::DEPLOY);


    std::cout << "Missing some cards, let's add them" << std::endl;

    p1 -> getHand()->addCard(new Card(CardType::BOMB));

    std::cout << "BOMB card added" << std::endl;

    std::cout << "Bomb Order issued:" << std::endl;
    std::cout << ((p1 -> issueOrder(OrderKind::BOMB)) ? "true" : "false")<< std::endl;

    
    std::cout << *p1 << std::endl;

    delete p1; 

}