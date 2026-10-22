#include "Baccarat.h"

#include <iostream>

Baccarat::Baccarat() {
    deck.shuffle();
}

int Baccarat::cardValue(const Card& card) {
    const int rank = static_cast<int>(card.getRank());
    if (card.getRank() == Rank::ace) return 1;
    if (rank >= 10) return 0;
    return rank;
}

int Baccarat::calculateScore(const std::vector<Card>& hand) const {
    int total = 0;
    for (const Card& card : hand) {
        total += cardValue(card);
    }
    return total % 10;
}

int Baccarat::calcScore(std::vector<Card> hand) {
    return calculateScore(hand);
}

void Baccarat::dealInitialHands() {
    playerHand.clear();
    bankerHand.clear();

    playerHand.push_back(deck.dealCard());
    bankerHand.push_back(deck.dealCard());
    playerHand.push_back(deck.dealCard());
    bankerHand.push_back(deck.dealCard());
}

void Baccarat::applyThirdCardRules() {
    const int playerTotal = calculateScore(playerHand);
    const int bankerTotal = calculateScore(bankerHand);

    // A natural 8 or 9 on either side ends drawing immediately.
    if (playerTotal >= 8 || bankerTotal >= 8) return;

    if (playerTotal <= 5) {
        const Card thirdCard = deck.dealCard();
        playerHand.push_back(thirdCard);
        const int thirdValue = cardValue(thirdCard);

        bool bankerDraws = false;
        if (bankerTotal <= 2) bankerDraws = true;
        else if (bankerTotal == 3) bankerDraws = thirdValue != 8;
        else if (bankerTotal == 4) bankerDraws = thirdValue >= 2 && thirdValue <= 7;
        else if (bankerTotal == 5) bankerDraws = thirdValue >= 4 && thirdValue <= 7;
        else if (bankerTotal == 6) bankerDraws = thirdValue == 6 || thirdValue == 7;

        if (bankerDraws) bankerHand.push_back(deck.dealCard());
    } else {
        // Player stands on 6 or 7; banker draws on 0 through 5.
        if (bankerTotal <= 5) bankerHand.push_back(deck.dealCard());
    }
}

void Baccarat::startRound() {
// Requires Deck::reset() to restore a full deck and its deal position.
    deck.reset();
    deck.shuffle();
    dealInitialHands();
    applyThirdCardRules();

    const int playerTotal = calculateScore(playerHand);
    const int bankerTotal = calculateScore(bankerHand);
    if (playerTotal > bankerTotal) result = Result::Player;
    else if (bankerTotal > playerTotal) result = Result::Banker;
    else result = Result::Tie;
}

const std::vector<Card>& Baccarat::getPlayerHand() const {
    return playerHand;
}

const std::vector<Card>& Baccarat::getBankerHand() const {
    return bankerHand;
}

Baccarat::Result Baccarat::getResult() const {
    return result;
}

void Baccarat::explain_rules() {
    std::cout << "Baccarat (Punto Banco): Player and Banker are each dealt two cards.\n"
              << "Aces count as 1; cards 2-9 count at face value; 10s and face cards count as 0.\n"
              << "Only the last digit of the total counts (for example, 15 becomes 5).\n"
              << "A natural 8 or 9 stops drawing. Otherwise, fixed third-card rules apply.\n"
              << "The hand closer to 9 wins; equal totals are a tie.\n";
}

void Baccarat::play() {
    startRound();

    std::cout << "Player Hand: ";
    for (const Card& card : playerHand) {
        std::cout << static_cast<int>(card.getRank()) << ' ';
    }
    std::cout << "Score: " << calculateScore(playerHand) << '\n';

    std::cout << "Banker Hand: ";
    for (const Card& card : bankerHand) {
        std::cout << static_cast<int>(card.getRank()) << ' ';
    }
    std::cout << "Score: " << calculateScore(bankerHand) << '\n';

    if (result == Result::Player) std::cout << "Player wins!\n";
    else if (result == Result::Banker) std::cout << "Banker wins!\n";
    else std::cout << "It's a tie!\n";
}
