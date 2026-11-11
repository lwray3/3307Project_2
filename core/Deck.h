
#ifndef DECK_H
#define DECK_H

#include <vector>
#include <cstddef>
#include "Card.h"

class Deck {
private:
    std::vector<CardPtr> cards;
    std::size_t currCard;

public:
    Deck();

    void shuffle();
    void reset();
    CardPtr dealCard();
};

#endif
