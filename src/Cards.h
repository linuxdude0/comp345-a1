#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>

// Enum for card types
enum CardType { BOMB, REINFORCEMENT, BLOCKADE, AIRLIFT, DIPLOMACY };

// Card class definition
class Card {
public:
    Card(CardType type); // Constructor to initialize card type
    void play(); // Method to play the card
    CardType getType() const; // Getter for card type

private:
    CardType type; // Card type
};

// Deck class definition
class Deck {
public:
    Deck(); // Constructor to initialize the deck with cards
    Card draw(); // Method to draw a card at random
    void returnCard(const Card& card); // Method to return a card to the deck

private:
    std::vector<Card> cards; // Vector to store cards
};

// Hand class definition
class Hand {
public:
    void addCard(const Card& card); // Method to add a card to the hand
    void playAll(Deck& deck); // Method to play all cards in the hand

private:
    std::vector<Card> handCards; // Vector to store cards in hand
};
// Function to test the card functionality
void testCards();