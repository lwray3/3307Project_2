
#include "Player.h"

Player::Player() {
}

void Player::addCard(CardPtr card) {
    hand.push_back(card);
}

std::vector<CardPtr> Player::getHand() const {
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
