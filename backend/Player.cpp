
#include "Player.h"

Player::Player() {
}

void Player::addCard(const Card& card) {
    hand.push_back(card);
}

std::vector<Card> Player::getHand() const {
    return hand;
}

void Player::clearHand() {
    hand.clear();
}

int Player::getWins() const {
    return wins;
}

int Player::getLosses() const {
    return losses;
}

void Player::addWin() {
    wins++;
}

void Player::addLoss() {
    losses++;
}
