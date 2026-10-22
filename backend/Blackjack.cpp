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

    for (int i = 0; i < 2; i++) {
        player.addCard(deck.dealCard());
        dealer.addCard(deck.dealCard());
    }
}

// Player requests another card
void Blackjack::hit() {
    if (roundOver) {
        return;
    }

    Card new_card = deck.dealCard();
    player.addCard(new_card);

    int score = calcScore(player.getHand());

    if (score > 21) {
        roundOver = true;
    }
}

// Player stands, dealer draws until reaching 17
void Blackjack::stand() {
    if (roundOver) {
        return;
    }

    while (calcScore(dealer.getHand()) < 17) {
        dealer.addCard(deck.dealCard());
    }

    roundOver = true;
}

// Return the player
const Player& Blackjack::getPlayer() const {
    return player;
}

// Return the dealer
const Player& Blackjack::getDealer() const {
    return dealer;
}

// Check whether the round is finished
bool Blackjack::isRoundOver() const {
    return roundOver;
}

// Explain Blackjack rules
void Blackjack::explain_rules() {
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
int Blackjack::calcScore(std::vector<Card> hand) {
    int score = 0;
    int aces = 0;

    for (const auto& card : hand) {
        int value = static_cast<int>(card.getRank());

        if (card.getRank() == Rank::ace) {
            score += 11;
            aces++;
        }
        else if (value >= 11) {
            score += 10;
        }
        else {
            score += value;
        }
    }

    // Convert Aces from 11 to 1 if necessary
    while (score > 21 && aces > 0) {
        score -= 10;
        aces--;
    }

    return score;
}

// Reset the game
void Blackjack::reset() {
    startRound();
}

// Console version of Blackjack
void Blackjack::play() {
    reset();

    std::cout << "Welcome to Blackjack!" << std::endl;
    std::cout << "Would you like to hear the rules? (y/n): ";

    char choice;

    if (std::cin >> choice && (choice == 'y' || choice == 'Y')) {
        explain_rules();
    }

    // Player's turn
    while (!roundOver) {
        std::cout << "Your score: "
                  << calcScore(player.getHand()) << std::endl;

        std::cout << "Would you like to hit or stand? (h/s): ";

        if (!(std::cin >> choice)) {
            return;
        }

        if (choice == 'h' || choice == 'H') {
            hit();

            if (roundOver) {
                std::cout << "You busted!" << std::endl;
                player.addLoss();
                return;
            }
        }
        else if (choice == 's' || choice == 'S') {
            stand();
        }
        else {
            std::cout << "Invalid input." << std::endl;
        }
    }

    // Compare final scores
    int playerScore = calcScore(player.getHand());
    int dealerScore = calcScore(dealer.getHand());

    std::cout << "Final Scores - Player: " << playerScore
              << ", Dealer: " << dealerScore << std::endl;

    if (dealerScore > 21 || playerScore > dealerScore) {
        std::cout << "You win!" << std::endl;
        player.addWin();
    }
    else if (playerScore < dealerScore) {
        std::cout << "Dealer wins!" << std::endl;
        player.addLoss();
    }
    else {
        std::cout << "It's a tie!" << std::endl;
    }
}