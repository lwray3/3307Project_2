
#ifndef BLACKJACK_H
#define BLACKJACK_H

#include <vector>

#include "IGame.h"
#include "Card.h"
#include "Deck.h"
#include "Player.h"

class Blackjack : public IGame {
private:
    Deck deck;
    Player player;
    Player dealer;
    std::vector<Card> hand;
    int score;
    bool roundOver = false;

public:
    Blackjack();

    void startRound();
    void hit();
    void stand();

    void explain_rules() override;

    int calcScore(std::vector<Card> hand) override;

    void reset();
    void play();

    const Player& getPlayer() const;
    const Player& getDealer() const;

    bool isRoundOver() const;
};

#endif
