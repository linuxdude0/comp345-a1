#include "Cards.h"

// Card class implementation
Card::Card(CardType type) : type(type) {} // Constructor to initialize card type

void Card::play() {
    if (type == BOMB) {
        std::cout << "Playing bomb card" << std::endl; // Print message when bomb card is played
    }
    else if (type == REINFORCEMENT) {
        std::cout << "Playing reinforcement card" << std::endl; // Print message when reinforcement card is played
    }
    else if (type == BLOCKADE) {
        std::cout << "Playing blockade card" << std::endl; // Print message when blockade card is played
    }
    else if (type == AIRLIFT) {
        std::cout << "Playing airlift card" << std::endl; // Print message when airlift card is played
    }
    else if (type == DIPLOMACY) {
        std::cout << "Playing diplomacy card" << std::endl; // Print message when diplomacy card is played
    }
    else {
        std::cout << "Playing unknown card" << std::endl; // Print message when unknown card is played
    }

    // TODO: add play card to player orderlist 

}

CardType Card::getType() const {
    return type; // Return the card type
}

// Deck class implementation
Deck::Deck() {
    // Initialize the deck with one card of each type
    for (int i = 0; i < 5; ++i) {
        cards.push_back(Card(static_cast<CardType>(i)));
    }
    std::srand(std::time(0)); // Seed random number generator
}

Card Deck::draw() {
    if (cards.empty()) {
        throw std::out_of_range("No more cards in the deck"); // Throw exception if deck is empty
    }
    int index = std::rand() % cards.size(); // Get random index
    Card drawnCard = cards[index]; // Get card at random index
    cards.erase(cards.begin() + index); // Remove card from deck
    std::cout << "Drawing a card " << std::endl; // Print message when card is drawn   
    return drawnCard; // Return drawn card
}

void Deck::returnCard(const Card& card) {
    cards.push_back(card); // Add card back to the deck
    //may need to shuffle the deck?
}

// Hand class implementation
void Hand::addCard(const Card& card) {
    handCards.push_back(card); // Add card to hand
}

void Hand::playAll(Deck& deck) {
    for (Card& card : handCards) {
        card.play(); // Play each card in hand
        deck.returnCard(card); // Return card to deck

    }
    handCards.clear(); // Clear hand after playing all cards
}

// testCards function implementation
void testCards() {
    Deck deck; // Create a deck
    Hand hand; // Create a hand

    // Draw 5 cards from the deck and add to hand
    for (int i = 0; i < 5; ++i) {
        hand.addCard(deck.draw());
    }

    hand.playAll(deck); // Play all cards in hand
}