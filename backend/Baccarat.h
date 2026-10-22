#ifndef BACCARAT_H
#define BACCARAT_H

#include <vector>
#include "Card.h"
#include "Deck.h"
#include "IGame.h"

class Baccarat : public IGame {
public:
    enum class Result { Player, Banker, Tie };

    Baccarat();

    void startRound();
    void dealInitialHands();
    int calculateScore(const std::vector<Card>& hand) const;
    int calcScore(std::vector<Card> hand) override;
    void explain_rules() override;
    void play() override;

    const std::vector<Card>& getPlayerHand() const;
    const std::vector<Card>& getBankerHand() const;
    Result getResult() const;

private:
    Deck deck;
    std::vector<Card> playerHand;
    std::vector<Card> bankerHand;
    Result result = Result::Tie;

    static int cardValue(const Card& card);
    void applyThirdCardRules();
};

#endif
