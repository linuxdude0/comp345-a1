#include "Cards.h"
#include <iostream>

// Card class implementation
Card::Card(CardType type) : type(type) {} // Constructor to initialize card type



CardType Card::getType() const {
    return type; // Return the card type
}

// Deck class implementation
Deck::Deck() {
    // Initialize the deck with one card of each type
    for (int i = 0; i < 1000; ++i) {
        cards.push_back(new Card(static_cast<CardType>(i%static_cast<int>((CardType::CARD_TYPE_MAX)))));
    }
    std::srand(std::time(0)); // Seed random number generator
}

Deck::~Deck() {
    // Clean up dynamically allocated cards
#ifdef DEBUG
    std::cout << "DEBUG" << std::endl;
#endif
    for (Card* card : cards) {
#ifdef DEBUG
        std::cout << card << std::endl;
#endif
        delete card;
    }
#ifdef DEBUG
    std::cout << "DEBUG" << std::endl;
#endif
}

Card* Deck::draw() {
    if (cards.empty()) {
        throw std::out_of_range("No more cards in the deck"); // Throw exception if deck is empty
    }
    int index = std::rand() % cards.size(); // Get random index
    Card* drawnCard = cards[index]; // Get card at random index
    cards.erase(cards.begin() + index); // Remove card from deck
    std::cout << "Drawing a card " << std::endl; // Print message when card is drawn   
#ifdef DEBUG
    std::cout << "DEBUG" << std::endl;
    for (Card* c: cards) {
        std::cout << c << std::endl;
    }
    std::cout << "END DEBUG" << std::endl;
#endif
    return drawnCard; // Return drawn card
}

void Deck::returnCard(Card* card) {
    cards.push_back(card);  // Add card back to the deck
}

Hand::Hand(Deck* deck) : deck(deck){}

// Hand class implementation
Hand::~Hand() {
    // Clean up dynamically allocated cards
    for (Card* card : handCards) {
        if (card) {
            deck->returnCard(card);
        }
    }
}

// Hand class implementation
void Hand::addCard(Card* card) {
    handCards.push_back(card); // Add card to hand
}

void Hand::playAll() {
    while (!handCards.empty()) {
        Card* card = handCards.back(); // Get the last card in hand
        handCards.pop_back(); // Remove the last card from hand
        card->play(deck); // Play the card and return it to the deck
    }
}

// testCards function implementation
void testCards() {

    Deck* deck = new Deck(); // Create a deck
    Hand hand(deck); // Create a hand

    // Draw 5 cards from the deck and add to hand
    for (int i = 0; i < 5; ++i) {
        hand.addCard(deck->draw());
    }

    hand.playAll(); // Play all cards in hand
    delete deck;
}

void Card::play(Deck* deck) {
    if (type == CardType::BOMB) {
        std::cout << "Playing bomb card" << std::endl; // Print message when bomb card is played
    }
    else if (type == CardType::REINFORCEMENT) {
        std::cout << "Playing reinforcement card" << std::endl; // Print message when reinforcement card is played
    }
    else if (type == CardType::BLOCKADE) {
        std::cout << "Playing blockade card" << std::endl; // Print message when blockade card is played
    }
    else if (type == CardType::AIRLIFT) {
        std::cout << "Playing airlift card" << std::endl; // Print message when airlift card is played
    }
    else if (type == CardType::DIPLOMACY) {
        std::cout << "Playing diplomacy card" << std::endl; // Print message when diplomacy card is played
    }
    else {
        std::cout << "Playing unknown card" << std::endl; // Print message when unknown card is played
    }  
    deck->returnCard(this); // Return the card to the deck
}
