
#ifndef CARD_H
#define CARD_H

enum class Rank {
    two = 2,
    three,
    four,
    five,
    six,
    seven,
    eight,
    nine,
    ten,
    jack,
    queen,
    king,
    ace
};

enum class Suit {
    hearts,
    diamonds,
    clubs,
    spades
};

class Card {
private:
    Rank rank;
    Suit suit;

public:
    Card(Rank rank, Suit suit);

    Rank getRank() const;
    Suit getSuit() const;
};

#endif
