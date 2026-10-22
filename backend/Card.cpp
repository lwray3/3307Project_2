
#include "Card.h"

Card::Card(Rank rank, Suit suit)
    : rank(rank), suit(suit) {
}

Rank Card::getRank() const {
    return rank;
}

Suit Card::getSuit() const {
    return suit;
}
