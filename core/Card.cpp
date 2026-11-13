#include "Card.h"

Card::Card(Rank rank, Suit suit) //the card has a rank and suit
    : rank(rank), suit(suit) {
}

Rank Card::getRank() const { //
    return rank;
}

Suit Card::getSuit() const {
    return suit;
}
