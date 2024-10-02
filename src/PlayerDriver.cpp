#include <iostream>
#include "Cards.h"
#include "Player.h"
#include "PlayerDriver.h"

void testPlayers(std::string filename, Deck* testDeck){
    std::cout << "==================TEST PLAYER==================" << std::endl;
    Map* testMap = new Map(filename);
    Player* p1 = new Player(1, "John Doe", 1, testMap, testDeck);
    std::cout << "\n The player 1:\n" << *p1 << std::endl;
    std::cout << "Map filename: " << filename << std::endl;
    std::cout << "TESTING issueOrder()";
    p1->issueOrder(OrderKind::ADVANCE);
    p1->issueOrder(OrderKind::BLOCKADE);
    p1->issueOrder(OrderKind::DEPLOY);
    std::cout << "Missing some cards, let's add them" << std::endl;
    /*Card* card = new Card(CardType::BOMB);*/
    /*std::cout << card << std::endl;*/
    Card* card = testDeck->draw();
    p1 -> getHand()->addCard(card);
    switch (card->getType()) {
        case CardType::BOMB:
            std::cout << "BOMB card drawn" << std::endl;
            std::cout << "Bomb Order issued:" << std::endl;
            std::cout << ((p1 -> issueOrder(OrderKind::BOMB)) ? "true" : "false")<< std::endl;
            break;
        case CardType::AIRLIFT:
            std::cout << "Airlift card drawn" << std::endl;
            std::cout << "Airlift Order issued:" << std::endl;
            std::cout << ((p1 -> issueOrder(OrderKind::AIRLIFT)) ? "true" : "false")<< std::endl;
            break;
        case CardType::BLOCKADE:
            std::cout << "Blockade card drawn" << std::endl;
            std::cout << "Blockade Order issued:" << std::endl;
            std::cout << ((p1 -> issueOrder(OrderKind::BLOCKADE)) ? "true" : "false")<< std::endl;
            break;
        case CardType::DIPLOMACY:
            std::cout << "Diplomacy card drawn" << std::endl;
            std::cout << "Negotiation Order issued:" << std::endl;
            std::cout << ((p1 -> issueOrder(OrderKind::NEGOTIATE)) ? "true" : "false")<< std::endl;
            break;
        case CardType::REINFORCEMENT:
            std::cout << "who knows what order this is supposed to do" << std::endl;
            /*std::cout << "Reinforcement card drawn" << std::endl;*/
            /*std::cout << "Reinforcement Order issued:" << std::endl;*/
            /*std::cout << ((p1 -> issueOrder(OrderKind::)) ? "true" : "false")<< std::endl;*/
            break;
    }
    std::cout << *p1 << std::endl;
    delete testMap;
    delete p1; 
    std::cout << "==================END TEST PLAYER==================" << std::endl;
}
