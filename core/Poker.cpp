
#include "Poker.h"
#include <stdexcept>
#include <iostream>
#include <algorithm>

Poker::Poker()
{
    startRound(); // Initialize the game by starting the first round
}

void Poker::startRound()
{
    player.clearHand(); // Clear the player's cards from the previous round
    dealer.clearHand(); // Clear the dealer's cards
    communityCards.clear();
    deck.reset();
    deck.shuffle();

    phase = PokerPhase::PreFlop; // Set the initial phase to PreFlop
    result = PokerResult::None; // Reset the previous round's result
    pot = 0; // Reset the total pot

    for (int i = 0; i < 2; ++i) { // Deal two private cards to each participant
        player.addCard(deck.dealCard());
        dealer.addCard(deck.dealCard());
    }
}

bool Poker::isRoundOver() const
{
    return phase == PokerPhase::Finished; // Check whether the game has reached its final phase
}

bool Poker::canCall() const
{
    if (roundOver || !playerTurn) { // Cannot call if the round is over or it is not the player's turn
        return false;
    }
    int amountToCall = currentBet - playerBet; // Calculate how many chips are needed to match the current bet
    return amountToCall > 0 && playerChips > 0; // Allow calling if there is an outstanding bet and the player has chips
}

bool Poker::canFold() const
{
    return !isRoundOver() && // Folding is only allowed while the round is active
           phase != PokerPhase::Showdown &&
           phase != PokerPhase::Finished;
}

void Poker::call()
{
    if (!canCall()) { // Check whether the player is allowed to call
        throw std::logic_error("Player cannot call");
    }
    int amountToCall = currentBet - playerBet; // Calculate the amount required to match the bet
    int amountPaid = std::min(amountToCall, playerChips); // Limit payment to the player's available chips
    playerChips -= amountPaid;
    playerBet += amountPaid;
    pot += amountPaid;
    playerTurn = false; // End the player's turn
}

bool Poker::canCheck() const
{
    return !isRoundOver(); // Allow checking as long as the round has not finished
}

bool Poker::canRaise(int raiseAmount) const
{
    if (roundOver || !playerTurn) { // Cannot raise if the round is over or it is not the player's turn
        return false;
    }
    if (raiseAmount < minimumRaise) { // Ensure the raise meets the minimum required amount
        return false;
    }
    int amountToCall = currentBet - playerBet; // Determine the amount needed to match the current bet
    int totalRequired = amountToCall + raiseAmount; // Calculate the total chips required for the raise
    return totalRequired <= playerChips; // Check whether the player has enough chips
}

void Poker::reset() {
    startRound(); // Restart the game by resetting the cards, deck, phase, and pot
}

void Poker::raise(int raiseAmount)
{
    if (!canRaise(raiseAmount)) { // Validate the raise before modifying the game state
        throw std::logic_error("Invalid raise");
    }

    int amountToCall = currentBet - playerBet; // Calculate the chips needed to match the current bet
    int totalRequired = amountToCall + raiseAmount; // Add the additional raise amount
    playerChips -= totalRequired; // Deduct the total amount from the player's chips
    playerBet += totalRequired;
    pot += totalRequired;
    currentBet = playerBet; // Update the highest current bet
    minimumRaise = raiseAmount;
    playerTurn = false; // End the player's turn
}

bool Poker::canBet() const
{
    return !isRoundOver() && playerChips >= BET_AMOUNT && dealerChips >= BET_AMOUNT; // Ensure both participants can afford the bet
}

int Poker::getCurrentBet() const
{
    return currentBet; // Return the current highest bet
}

void Poker::check()
{
    if (!canCheck()) { // Verify that checking is allowed
        throw std::logic_error("Cannot check");
    }
    advancePhase(); // Move to the next stage of the Poker round
}

void Poker::bet()
{
    if (!canBet()) { // Verify that both participants have enough chips
        throw std::logic_error("Cannot bet");
    }
    playerChips -= BET_AMOUNT; // Deduct the fixed bet from the player and dealer
    dealerChips -= BET_AMOUNT;
    pot += BET_AMOUNT * 2; // Add both contributions to the pot
    advancePhase();
}

void Poker::fold()
{
    if (isRoundOver()) { // Prevent folding after the round has ended
        throw std::logic_error("Round already finished");
    }
    result = PokerResult::DealerWins; // The dealer automatically wins when the player folds
    dealerChips += pot; // Award the entire pot to the dealer
    pot = 0; // Clear the pot
    player.addLoss(); // Increment the player's loss count
    phase = PokerPhase::Finished; // Mark the round as finished
}

void Poker::advancePhase()
{
    switch (phase) { // Determine which stage of Texas Hold'em is currently active
    case PokerPhase::PreFlop:
        deck.dealCard(); // Burn the top card before dealing the flop
        for (int i = 0; i < 3; ++i) { // Deal three community cards
            communityCards.push_back(deck.dealCard());
        }
        phase = PokerPhase::Flop; // Advance to the Flop phase
        break;

    case PokerPhase::Flop:
        deck.dealCard(); // Burn the top card before the turn
        communityCards.push_back(deck.dealCard()); // Deal the fourth community card
        phase = PokerPhase::Turn; // Advance to the Turn phase
        break;

    case PokerPhase::Turn:
        deck.dealCard(); // Burn the top card before the river
        communityCards.push_back(deck.dealCard()); // Deal the fifth and final community card
        phase = PokerPhase::River; // Advance to the River phase
        break;

    case PokerPhase::River:
        phase = PokerPhase::Showdown; // Enter the final hand comparison phase
        showdown(); // Determine the winner based on the best Poker hand
        break;

    default:
        throw std::logic_error(
            "Cannot advance the current Poker phase" // Prevent invalid phase transitions
            );
    }
}

void Poker::showdown()
{
    PokerHandRank playerRank = getPlayerHandRank(); // Evaluate the player's best five-card hand
    PokerHandRank dealerRank = getDealerHandRank(); // Evaluate the dealer's best five-card hand

    if (dealerRank < playerRank) { // The player wins if their hand ranks higher
        result = PokerResult::PlayerWins;
        playerChips += pot; // Award the pot to the player
        player.addWin(); // Increment the player's win count
    }
    else if (playerRank < dealerRank) { // The dealer wins if their hand ranks higher
        result = PokerResult::DealerWins;
        dealerChips += pot; // Award the pot to the dealer
        player.addLoss(); // Increment the player's loss count
    }
    else {
        result = PokerResult::Tie; // Both participants have equally ranked hands
        playerChips += pot / 2; // Return half the pot to the player
        dealerChips += pot / 2; // Return half the pot to the dealer
    }

    pot = 0; // Reset the pot after distributing winnings
    phase = PokerPhase::Finished; // Mark the round as completed
}

PokerHandRank Poker::getPlayerHandRank() const
{
    std::vector<CardPtr> cards = player.getHand(); // Copy the player's two private cards
    cards.insert(
        cards.end(),
        communityCards.begin(),
        communityCards.end() // Add all community cards to the player's hand
        );
    return PokerHandEvaluator::evaluate(cards); // Determine the player's strongest five-card combination
}

PokerHandRank Poker::getDealerHandRank() const
{
    std::vector<CardPtr> cards = dealer.getHand(); // Copy the dealer's two private cards
    cards.insert(
        cards.end(),
        communityCards.begin(),
        communityCards.end() // Add the five shared community cards
        );
    return PokerHandEvaluator::evaluate(cards); // Determine the dealer's strongest five-card combination
}

PokerPhase Poker::getPhase() const {
    return phase; // Return the current phase of the Poker round
}

PokerResult Poker::getResult() const {
    return result; // Return the result of the round
}

int Poker::getPot() const {
    return pot; // Return the total number of chips currently in the pot
}

void Poker::explain_rules() const {
    std::cout << "Texas Hold'em Poker Rules:\n"; // Display the game's rules in the console
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

const Player& Poker::getDealer() const {
    return dealer;
}

const std::vector<CardPtr>& Poker::getCommunityCards() const {
    return communityCards; // Return a read-only reference to the shared community cards
}
