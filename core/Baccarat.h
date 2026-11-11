#ifndef BACCARAT_H
#define BACCARAT_H

#include "IGame.h"
#include "Deck.h"
#include "Player.h"

class Baccarat : public IGame {
private:
    Deck deck;
    Player player;
    Player banker;

    bool roundOver = true;

public:
    Baccarat();

    void startRound() override;

    int calcScore(
        const std::vector<CardPtr>& hand
        ) const;

    void explain_rules() const override;
    void reset() override;
    bool isRoundOver() const override;

    void playRound();

    int getPlayerScore() const;
    int getBankerScore() const;

    const Player& getPlayer() const;
    const Player& getBanker() const;
};

#endif
