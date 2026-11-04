#ifndef BLACKJACK_H
#define BLACKJACK_H

#include "IGame.h"
#include "Deck.h"
#include "Player.h"

class Blackjack : public IGame {
private:
    Deck deck;
    Player player;
    Player dealer;
    bool dealerTurn = false;
    bool roundOver = false;

public:
    Blackjack();

    // IGame functions
    void startRound() override;
    bool dealerShouldHit() const;
    Card dealerHit();
    void finishRound();
    bool isDealerTurn() const;
    int calcScore(const std::vector<Card>& hand) const override;
    void explain_rules() const override;
    void reset() override;
    bool isRoundOver() const override;

    // Blackjack-specific functions
    Card hit();
    void stand();

    bool canHit() const;
    bool canStand() const;

    int getPlayerScore() const;
    int getDealerScore() const;

    const Player& getPlayer() const;
    const Player& getDealer() const;
};

#endif