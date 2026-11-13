#include "Baccarat.h"
#include <stdio.h>
#include <iostream>

Baccarat::Baccarat()
{
    startRound();
}

void Baccarat::startRound() //starts the round right when the
{
    player.clearHand(); //
    banker.clearHand();

    deck.reset();
    deck.shuffle();

    player.addCard(deck.dealCard());
    banker.addCard(deck.dealCard());

    player.addCard(deck.dealCard());
    banker.addCard(deck.dealCard());

    roundOver = false;
}

int Baccarat::calcScore(
    const std::vector<CardPtr>& hand
    ) const
{
    int total = 0;

    for (CardPtr card : hand) {
        int value = static_cast<int>(card->getRank());

        if (value == 14) {
            value = 1;  // Ace
        }
        else if (value >= 10) {
            value = 0;  // 10, J, Q, K
        }

        total += value;
    }

    return total % 10;
}

void Baccarat::playRound()
{
    if (roundOver) {
        return;
    }

    int playerScore = getPlayerScore();
    int bankerScore = getBankerScore();

    // Natural 8 or 9: no additional cards.
    if (playerScore >= 8 || bankerScore >= 8) {
        roundOver = true;
    }
    else {
        bool playerDrew = false;
        int thirdCardValue = -1;

        // Player draws on totals of 0-5.
        if (playerScore <= 5) {
            CardPtr thirdCard = deck.dealCard();
            player.addCard(thirdCard);
            playerDrew = true;

            thirdCardValue = static_cast<int>(
                thirdCard->getRank()
                );

            if (thirdCardValue == 14) {
                thirdCardValue = 1;
            }
            else if (thirdCardValue >= 10) {
                thirdCardValue = 0;
            }
        }

        // Standard banker third-card rules.
        bool bankerDraws = false;

        if (!playerDrew) {
            bankerDraws = bankerScore <= 5;
        }
        else {
            if (bankerScore <= 2) {
                bankerDraws = true;
            }
            else if (bankerScore == 3) {
                bankerDraws = thirdCardValue != 8;
            }
            else if (bankerScore == 4) {
                bankerDraws =
                    thirdCardValue >= 2 &&
                    thirdCardValue <= 7;
            }
            else if (bankerScore == 5) {
                bankerDraws =
                    thirdCardValue >= 4 &&
                    thirdCardValue <= 7;
            }
            else if (bankerScore == 6) {
                bankerDraws =
                    thirdCardValue == 6 ||
                    thirdCardValue == 7;
            }
        }

        if (bankerDraws) {
            banker.addCard(deck.dealCard());
        }

        roundOver = true;
    }

    if (getPlayerScore() > getBankerScore()) {
        player.addWin();
    }
    else if (getPlayerScore() < getBankerScore()) {
        player.addLoss();
    }
}

int Baccarat::getPlayerScore() const
{
    return calcScore(player.getHand());
}

int Baccarat::getBankerScore() const
{
    return calcScore(banker.getHand());
}

const Player& Baccarat::getPlayer() const
{
    return player;
}

const Player& Baccarat::getBanker() const
{
    return banker;
}

bool Baccarat::isRoundOver() const
{
    return roundOver;
}

void Baccarat::reset()
{
    player.clearHand();
    banker.clearHand();
    deck.reset();
    roundOver = true;
}

void Baccarat::explain_rules() const
{
    std::cout<<"Write Baccarat Rules here";
}