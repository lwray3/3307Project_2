
#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "Card.h"

class Player {
private:
    std::vector<CardPtr> hand;
    int wins = 0;
    int losses = 0;

public:
    Player();
    void addCard(CardPtr card); //uses the flyweight pattern to point to a shared card pointer
    std::vector<CardPtr> getHand() const; //this one too bc all games share the same created cards
    void clearHand();
    int getWins() const;
    int getLosses() const;
    void addWin();
    void addLoss();
};

#endif
