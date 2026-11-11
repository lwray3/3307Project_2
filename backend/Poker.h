
#ifndef POKER_H
#define POKER_H

#include "Deck.h"
#include "Player.h"
#include "pokerEvaluator.h"
#include "IGame.h"

#include <vector>

enum class PokerPhase {
    PreFlop,
    Flop,
    Turn,
    River,
    Showdown,
    Finished
};

enum class PokerResult {
    None,
    PlayerWins,
    DealerWins,
    Tie
};

class Poker: public IGame {
private:
    Deck deck;
    Player player;
    Player dealer;

    std::vector<CardPtr> communityCards;

    PokerPhase phase = PokerPhase::PreFlop;
    PokerResult result = PokerResult::None;

    int pot = 0;
    int playerChips = 1000;
    int dealerChips = 1000;
    int currentBet = 0;
    int playerBet = 0;
    int opponentBet = 0;
    int minimumRaise = 20;
    bool playerTurn = true;
    bool roundOver = false;

    static constexpr int BET_AMOUNT = 20;

    void advancePhase();
    void showdown();

public:
    Poker();
    void explain_rules() const;
    void startRound();
    bool canCall() const;
    bool canRaise(int raiseAmount) const;
    void call();
    void raise(int raiseAmount);
    int getPlayerChips() const;
    int getPot() const;
    int getCurrentBet() const;
    // Simplified player actions
    void check();
    void bet();
    void fold();
    bool canCheck() const;
    bool canBet() const;
    void reset();
    bool isRoundOver() const;
    PokerPhase getPhase() const;
    PokerResult getResult() const;
    int getDealerChips() const;
    bool canFold() const;
    const Player& getPlayer() const;
    const Player& getDealer() const;

    const std::vector<CardPtr>& getCommunityCards() const;

    PokerHandRank getPlayerHandRank() const;
    PokerHandRank getDealerHandRank() const;
};

#endif
