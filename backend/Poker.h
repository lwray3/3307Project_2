#ifndef POKER_H
#define POKER_H

#include <random>
#include <string>
#include <vector>
#include "IGame.h"
#include "Card.h"
#include "Deck.h"
#include "Player.h"

// Heads-up Texas Hold'em: one human versus a computer dealer.
class Poker : public IGame {
private:
    enum : int { POW5 = 759375, START_CHIPS = 1000, SMALL_BLIND = 10, BIG_BLIND = 20 };
    enum class ActionType { Fold, Check, Call, Raise };
    struct Decision { ActionType type; int raiseTo; };

    Deck deck;
    std::vector<Card> playerHole;
    std::vector<Card> dealerHole;
    std::vector<Card> board;
    int playerChips = START_CHIPS;
    int dealerChips = START_CHIPS;
    int pot = 0;
    int streetPlayerBet = 0;
    int streetDealerBet = 0;
    int lastRaiseSize = BIG_BLIND;
    bool buttonIsPlayer = true;
    bool playerFolded = false;
    bool dealerFolded = false;
    bool inputClosed = false;
    std::mt19937 rng{std::random_device{}()};

    static std::string rankStr(Rank r);
    static std::string suitStr(Suit s);
    static std::string cardStr(const Card& c);
    static std::string handStr(const std::vector<Card>& cards);
    static std::string categoryName(int score);
    void showTable();
    std::string readLine();
    static int evaluate5(const std::vector<Card>& hand);
    void putIn(bool isPlayer, int amount);
    double handStrength();
    Decision dealerDecision();
    Decision humanDecision();
    bool applyDecision(bool isPlayer, const Decision& d);
    bool bettingRound(bool playerActsFirst);
    void startStreet();
    void awardPot(bool toPlayer);
    void playHand();

public:
    Player player;
    Player dealer;
    Poker() = default;
    void explain_rules() override;
    int calcScore(std::vector<Card> cards) override;
    void play() override;
};

#endif
