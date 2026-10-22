
#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "Card.h"

class Player {
private:
    std::vector<Card> hand;
    int wins = 0;
    int losses = 0;

public:
    Player();

    void addCard(const Card& card);

    std::vector<Card> getHand() const;

    void clearHand();

    int getWins() const;
    int getLosses() const;

    void addWin();
    void addLoss();
};

#endif
