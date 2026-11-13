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

CardPtr Blackjack::hit() // Player requests another card
{

    if (!canHit()) { // Prevent hitting after the round has ended
        throw std::logic_error("Cannot hit: round is over or hitting is not allowed");
    }
    CardPtr new_card = deck.dealCard(); // Prevent hitting after the round has ended
    player.addCard(new_card); // Prevent hitting after the round has ended
    int score = getPlayerScore();

    if (score > 21) {
        // Check if the player busted
        roundOver = true;
        dealerTurn = false;
        player.addLoss();
    }
    else if (score == 21) { // Automatically stand if the player reaches 21
        stand();
    }
    return new_card; // Automatically stand if the player reaches 21
}

void Blackjack::stand()// Player stands, dealer draws until reaching 17
{
    if (!canStand()) {
        throw std::logic_error("Cannot stand");
    }

    dealerTurn = true;
}

const Player& Blackjack::getPlayer() const {// Return the player
    return player;
}

const Player& Blackjack::getDealer() const {// Return the dealer
    return dealer;
}

int Blackjack::getPlayerScore() const {
    return calcScore(player.getHand());
}

CardPtr Blackjack::dealerHit() //add a card to the dealer hand
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
           && getDealerScore() < 17; //the dealer should only hit if it's less than 17
}

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
void Blackjack::finishRound() //perform all of the computations to determine the winner
{
    if (roundOver || !dealerTurn) {
        return;
    }

    if (dealerShouldHit()) {
        throw std::logic_error(
            "Dealer must finish drawing first"
            );
    }
    int playerScore = getPlayerScore(); //get the player score
    int dealerScore = getDealerScore(); //get the score for the dealer

    if (dealerScore > 21) {
        player.addWin();
    }
    else if (playerScore > dealerScore) {
        player.addWin();
    }
    else if (dealerScore > playerScore) {
        player.addLoss();
    }
    roundOver = true; // Return the dealer
    dealerTurn = false;
}

bool Blackjack::isRoundOver() const { // Check whether the round is finished
    return roundOver;
}

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


int Blackjack::calcScore(const std::vector<CardPtr>& hand) const // Calculate the score of a hand
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

void Blackjack::reset() { // Reset the game
    startRound();
}

// Console version of Blackjack