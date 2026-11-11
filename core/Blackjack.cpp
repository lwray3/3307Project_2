#include <iostream>
#include "Blackjack.h"


Blackjack::Blackjack() { //constuctor
    startRound();
}

// Deal the initial cards
void Blackjack::startRound() {
    player.clearHand();
    dealer.clearHand();

    deck.reset();
    deck.shuffle();

    roundOver = false;
    dealerTurn = false;

    for (int i = 0; i < 2; i++) {
        player.addCard(deck.dealCard());
        dealer.addCard(deck.dealCard());
    }
}


// Player requests another card
CardPtr Blackjack::hit()
{
    // Prevent hitting after the round has ended
    if (!canHit()) {
        throw std::logic_error("Cannot hit: round is over or hitting is not allowed");
    }

    // Deals a new card
    CardPtr new_card = deck.dealCard();

    // Add the card to the player's hand
    player.addCard(new_card);

    // Calculate the player's new score
    int score = getPlayerScore();

    // Check if the player busted
    if (score > 21) {
        roundOver = true;
        dealerTurn = false;
        player.addLoss();
    }
    // Automatically stand if the player reaches 21
    else if (score == 21) {
        stand();
    }

    // Return the card so the UI can animate it
    return new_card;
}

// Player stands, dealer draws until reaching 17
void Blackjack::stand()
{
    if (!canStand()) {
        throw std::logic_error("Cannot stand");
    }

    dealerTurn = true;
}
// Return the player
const Player& Blackjack::getPlayer() const {
    return player;
}

// Return the dealer
const Player& Blackjack::getDealer() const {
    return dealer;
}

int Blackjack::getPlayerScore() const {
    return calcScore(player.getHand());
}

CardPtr Blackjack::dealerHit()
{
    if (!dealerShouldHit()) {
        throw std::logic_error("Dealer cannot hit");
    }

    CardPtr newCard = deck.dealCard();

    dealer.addCard(newCard);

    return newCard;
}

bool Blackjack::dealerShouldHit() const
{
    return !roundOver
           && dealerTurn
           && getDealerScore() < 17;
}
// Return the dealer
int Blackjack::getDealerScore() const {
    return calcScore(dealer.getHand());
}

bool Blackjack::canHit() const
{
    return !roundOver
           && !dealerTurn
           && getPlayerScore() < 21;
}

bool Blackjack::canStand() const
{
    return !roundOver && !dealerTurn;
}

bool Blackjack::isDealerTurn() const
{
    return dealerTurn;
}
void Blackjack::finishRound()
{
    if (roundOver || !dealerTurn) {
        return;
    }

    if (dealerShouldHit()) {
        throw std::logic_error(
            "Dealer must finish drawing first"
            );
    }

    int playerScore = getPlayerScore();
    int dealerScore = getDealerScore();

    if (dealerScore > 21) {
        player.addWin();
    }
    else if (playerScore > dealerScore) {
        player.addWin();
    }
    else if (dealerScore > playerScore) {
        player.addLoss();
    }

    // Equal scores represent a tie.

    roundOver = true;
    dealerTurn = false;
}
// Check whether the round is finished
bool Blackjack::isRoundOver() const {
    return roundOver;
}

// Explain Blackjack rules
void Blackjack::explain_rules() const {
    std::cout << "In Blackjack, the goal is to have a hand value "
              << "as close to 21 as possible without going over."
              << std::endl;

    std::cout << "Face cards are worth 10 points, and an Ace "
              << "can be worth either 1 or 11 points."
              << std::endl;

    std::cout << "You are dealt 2 cards at the beginning of each game. "
              << "You can hit or stand."
              << std::endl;
}

// Calculate the score of a hand
int Blackjack::calcScore(const std::vector<CardPtr>& hand) const
{
    int score = 0;
    int aces = 0;

    for (const CardPtr& card : hand) {
        int rank = static_cast<int>(card->getRank());

        if (rank == 14) {
            score += 11;
            ++aces;
        }
        else if (rank >= 10) {
            score += 10;
        }
        else {
            score += rank;
        }
    }

    while (score > 21 && aces > 0) {
        score -= 10;
        --aces;
    }

    return score;
}

// Reset the game
void Blackjack::reset() {
    startRound();
}

// Console version of Blackjack