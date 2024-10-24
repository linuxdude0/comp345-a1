#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>

// Enum for card types
enum class CardType { BOMB, REINFORCEMENT, BLOCKADE, AIRLIFT, DIPLOMACY, CARD_TYPE_MAX};

// Forward declaration of Deck class
class Deck;
// Card class definition
class Card {
public:
    Card(CardType type); // Constructor to initialize card type
    void play(Deck* deck); // Method to play the card
    CardType getType() const; // Getter for card type

private:
    CardType type; // Card type
};

// Deck class definition
class Deck {
public:
    Deck(); // Constructor to initialize the deck with cards
    ~Deck(); // Destructor to clean up dynamically allocated cards
    Card* draw(); // Method to draw a card at random
    void returnCard(Card* card); // Method to return a card to the deck

private:
    std::vector<Card*> cards; // Vector to store cards
};

// Hand class definition
class Hand {
private:
    Deck* deck;
public:
    Hand(Deck* deck);
    ~Hand(); // Destructor to clean up dynamically allocated cards
    void addCard(Card* card); // Method to add a card to the hand
    void playAll(void); // Method to play all cards in the hand
    
    std::vector<Card*> handCards; // Vector to store cards in hand
};
// Function to test the card functionality
void testCards();
