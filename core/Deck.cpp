#include "Deck.h"
#include "CardFactory.h"

#include <algorithm>
#include <random>
#include <stdexcept>

static CardFactory cardFactory;

Deck::Deck() : currCard(0) {
    reset();
}


void Deck::shuffle() { // Shuffle the deck
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(cards.begin(), cards.end(), g);

    currCard = 0;
}

void Deck::reset() { //make a new deck full of cards
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
            cards.push_back(cardFactory.getCard(rank, suit));
        }
    }
}

CardPtr Deck::dealCard() { //deal the cards to the hands of the player or dealer
    if (currCard >= cards.size()) {
        throw std::runtime_error("No cards remaining.");
    }

    CardPtr dealt_card = cards[currCard];
    currCard++;

    return dealt_card;
}
