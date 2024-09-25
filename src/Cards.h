#include "Orders.h"
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>

// Enum for card types
enum CardType { DEPLOY, ADVANCE, BOMB, BLOCKADE, AIRLIFT, NEGOTIATE };

// Card class definition
class Card {
public:
    Card(CardType type); // Constructor to initialize card type
    Order* play();
    CardType getType() const; // Getter for card type

private:
    CardType type; // Card type
};

// Deck class definition
class Deck {
public:
    Deck(); // Constructor to initialize the deck with cards
    Card* draw(); // Method to draw a card at random
    void returnCard(const Card& card); // Method to return a card to the deck
    Order* play(Card* card);
private:
    std::vector<Card*> cards; // Vector to store cards
};

// Hand class definition
class Hand {
public:
    void addCard(Card* card); // Method to add a card to the hand
    Order* playCard(Card* card);
    void playAll(Deck& deck); // Method to play all cards in the hand
    std::vector<Card*> handCards; // Vector to store cards in hand
};

// Function to test the card functionality
void testCards();
