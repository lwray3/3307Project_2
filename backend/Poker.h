#ifndef POKER_H
#define POKER_H

#include "IGame.h"
#include "Deck.h"
#include "Player.h"

class Poker : public IGame {
private:
    Deck deck;
    Player player;
    Player opponent;

    bool roundOver = true;

public:
    Poker();

    void startRound() override;

    int calcScore(const std::vector<Card>& hand) const override;

    void explain_rules() const override;
    void reset() override;
    bool isRoundOver() const override;

    void evaluateRound();

    const Player& getPlayer() const;
    const Player& getOpponent() const;
};

#endif