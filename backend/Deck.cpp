
#include "Deck.h"

#include <algorithm>
#include <random>
#include <stdexcept>

// Constructor: create a standard deck of 52 cards
Deck::Deck() : currCard(0) {
    reset();
}

// Shuffle the deck
void Deck::shuffle() {
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(cards.begin(), cards.end(), g);

    currCard = 0; // Start dealing from the beginning
}

// Reset the deck to its original 52 cards
void Deck::reset() {
    cards.clear();
    currCard = 0;

    for (const auto& suit : {
        Suit::hearts,
        Suit::diamonds,
        Suit::clubs,
        Suit::spades
    }) {
        for (const auto& rank : {
            Rank::two,
            Rank::three,
            Rank::four,
            Rank::five,
            Rank::six,
            Rank::seven,
            Rank::eight,
            Rank::nine,
            Rank::ten,
            Rank::jack,
            Rank::queen,
            Rank::king,
            Rank::ace
        }) {
            cards.push_back(Card(rank, suit));
        }
    }
}

// Deal one card from the deck
Card Deck::dealCard() {
    if (currCard >= cards.size()) {
        throw std::runtime_error("No cards remaining.");
    }

    Card dealt_card = cards[currCard];
    currCard++;

    return dealt_card;
}
