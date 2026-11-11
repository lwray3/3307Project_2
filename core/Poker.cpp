
#include "Poker.h"
#include <stdexcept>
#include <iostream>

Poker::Poker()
{
    startRound();
}

void Poker::startRound()
{
    player.clearHand();
    dealer.clearHand();
    communityCards.clear();
    deck.reset();
    deck.shuffle();

    phase = PokerPhase::PreFlop;
    result = PokerResult::None;
    pot = 0;

    for (int i = 0; i < 2; ++i) { //2 cards for both players
        player.addCard(deck.dealCard());
        dealer.addCard(deck.dealCard());
    }
}

bool Poker::isRoundOver() const
{
    return phase == PokerPhase::Finished;
}

bool Poker::canCall() const
{
    if (roundOver || !playerTurn) {
        return false;
    }
    int amountToCall = currentBet - playerBet;
    return amountToCall > 0 && playerChips > 0;
}

#include <algorithm>
#include <stdexcept>
bool Poker::canFold() const
{
    return !isRoundOver() &&
           phase != PokerPhase::Showdown &&
           phase != PokerPhase::Finished;
}

void Poker::call()
{
    if (!canCall()) {
        throw std::logic_error("Player cannot call");
    }
    int amountToCall = currentBet - playerBet;
    int amountPaid = std::min(amountToCall, playerChips);
    playerChips -= amountPaid;
    playerBet += amountPaid;
    pot += amountPaid;
    playerTurn = false;
}

bool Poker::canCheck() const
{
    return !isRoundOver();
}

bool Poker::canRaise(int raiseAmount) const
{
    if (roundOver || !playerTurn) {
        return false;
    }
    if (raiseAmount < minimumRaise) {
        return false;
    }
    int amountToCall = currentBet - playerBet;
    int totalRequired = amountToCall + raiseAmount;
    return totalRequired <= playerChips;
}

void Poker::reset() {
    startRound();
}

void Poker::raise(int raiseAmount)
{
    if (!canRaise(raiseAmount)) {
        throw std::logic_error("Invalid raise");
    }

    int amountToCall = currentBet - playerBet;
    int totalRequired = amountToCall + raiseAmount;
    playerChips -= totalRequired;
    playerBet += totalRequired;
    pot += totalRequired;
    currentBet = playerBet;
    minimumRaise = raiseAmount;
    playerTurn = false;
}

bool Poker::canBet() const
{
    return !isRoundOver() && playerChips >= BET_AMOUNT && dealerChips >= BET_AMOUNT;
}

int Poker::getCurrentBet() const
{
    return currentBet;
}
void Poker::check()
{
    if (!canCheck()) {
        throw std::logic_error("Cannot check");
    }
    advancePhase();
}

void Poker::bet()
{
    if (!canBet()) {
        throw std::logic_error("Cannot bet");
    }
    playerChips -= BET_AMOUNT;
    dealerChips -= BET_AMOUNT;
    pot += BET_AMOUNT * 2;
    advancePhase();
}

void Poker::fold()
{
    if (isRoundOver()) {
        throw std::logic_error("Round already finished");
    }
    result = PokerResult::DealerWins;
    dealerChips += pot;
    pot = 0;
    player.addLoss();
    phase = PokerPhase::Finished;
}

void Poker::advancePhase()
{
    switch (phase) {
    case PokerPhase::PreFlop:
        deck.dealCard();
        for (int i = 0; i < 3; ++i) {
            communityCards.push_back(deck.dealCard());
        }
        phase = PokerPhase::Flop;
        break;

    case PokerPhase::Flop:
        deck.dealCard(); // Burn the top card
        communityCards.push_back(deck.dealCard());
        phase = PokerPhase::Turn;
        break;

    case PokerPhase::Turn:
        deck.dealCard(); // Burn the top card
        communityCards.push_back(deck.dealCard());
        phase = PokerPhase::River;
        break;

    case PokerPhase::River:
        phase = PokerPhase::Showdown;
        showdown();
        break;

    default:
        throw std::logic_error(
            "Cannot advance the current Poker phase"
            );
    }
}

void Poker::showdown()
{
    PokerHandRank playerRank = getPlayerHandRank();
    PokerHandRank dealerRank = getDealerHandRank();

    if (dealerRank < playerRank) {
        result = PokerResult::PlayerWins;
        playerChips += pot;
        player.addWin();
    }
    else if (playerRank < dealerRank) {
        result = PokerResult::DealerWins;
        dealerChips += pot;
        player.addLoss();
    }
    else {
        result = PokerResult::Tie;
        playerChips += pot / 2;
        dealerChips += pot / 2;
    }

    pot = 0;
    phase = PokerPhase::Finished;
}

PokerHandRank Poker::getPlayerHandRank() const
{
    std::vector<CardPtr> cards = player.getHand();
    cards.insert(
        cards.end(),
        communityCards.begin(),
        communityCards.end()
        );
    return PokerHandEvaluator::evaluate(cards);
}

PokerHandRank Poker::getDealerHandRank() const
{
    std::vector<CardPtr> cards = dealer.getHand();
    cards.insert(
        cards.end(),
        communityCards.begin(),
        communityCards.end()
        );
    return PokerHandEvaluator::evaluate(cards);
}

PokerPhase Poker::getPhase() const {
    return phase;
}

PokerResult Poker::getResult() const {
    return result;
}

int Poker::getPot() const {
    return pot;
}


void Poker::explain_rules() const {
    std::cout << "Texas Hold'em Poker Rules:\n";
    std::cout << "1. Each player receives two private cards.\n";
    std::cout << "2. Five community cards are dealt.\n";
    std::cout << "3. Players can check, bet, call, raise, or fold.\n";
    std::cout << "4. The best five card poker hand wins. The rankings are as follows: \n";
    std::cout << "1: Royal Flush (ace, king, queen, jack, 10 of the same suit)\n";
    std::cout << "2. Straight Flush (5 consecutive cards of the same suit)\n";
    std::cout << "3. Four of a Kind (4 cards of the same rank\n";
    std::cout << "4. Full house (3 of the same card and a 2 pair)\n";
    std::cout << "5. Straight (Five consecutive cards ordered by rank) \n";
}

int Poker::getPlayerChips() const {
    return playerChips;
}

int Poker::getDealerChips() const {
    return dealerChips;
}

const Player& Poker::getPlayer() const {
    return player;
}

const Player& Poker::getDealer() const{
    return dealer;
}

const std::vector<CardPtr>& Poker::getCommunityCards() const {
    return communityCards;
}