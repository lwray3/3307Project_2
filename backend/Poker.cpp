#include "Poker.h"

#include <algorithm>
#include <array>
#include <stdexcept>
#include <vector>

namespace {

constexpr int BASE = 15;

int encodeScore(
    int category,
    std::vector<int> ranks
    ) {
    // Five rank positions, padded with zeroes.
    ranks.resize(5, 0);

    int score = category;

    for (int rank : ranks) {
        score = score * BASE + rank;
    }

    return score;
}

} // namespace

Poker::Poker()
{
    deck.shuffle();
}

void Poker::startRound()
{
    player.clearHand();
    opponent.clearHand();

    deck.reset();
    deck.shuffle();

    for (int i = 0; i < 5; i++) {
        player.addCard(deck.dealCard());
        opponent.addCard(deck.dealCard());
    }

    roundOver = false;
}

int Poker::calcScore(
    const std::vector<Card>& hand
    ) const
{
    if (hand.size() != 5) {
        throw std::invalid_argument(
            "Poker scoring requires exactly five cards"
            );
    }

    std::array<int, 15> counts{};
    std::vector<int> ranks;

    bool flush = true;
    Suit firstSuit = hand.front().getSuit();

    for (const Card& card : hand) {
        int rank = static_cast<int>(card.getRank());

        counts[rank]++;
        ranks.push_back(rank);

        if (card.getSuit() != firstSuit) {
            flush = false;
        }
    }

    std::sort(ranks.begin(), ranks.end(),
              std::greater<int>());

    // Identify a straight.
    std::vector<int> uniqueRanks = ranks;

    uniqueRanks.erase(
        std::unique(
            uniqueRanks.begin(),
            uniqueRanks.end()
            ),
        uniqueRanks.end()
        );

    bool straight = false;
    int straightHigh = 0;

    if (uniqueRanks.size() == 5) {
        if (uniqueRanks.front() -
                uniqueRanks.back() == 4) {

            straight = true;
            straightHigh = uniqueRanks.front();
        }
        else if (uniqueRanks ==
                 std::vector<int>{14, 5, 4, 3, 2}) {

            straight = true;
            straightHigh = 5;
        }
    }

    // Group ranks by their occurrence counts.
    std::vector<std::pair<int, int>> groups;

    for (int rank = 14; rank >= 2; rank--) {
        if (counts[rank] > 0) {
            groups.push_back({counts[rank], rank});
        }
    }

    std::sort(groups.begin(), groups.end(),
              [](const auto& a, const auto& b) {
                  if (a.first != b.first) {
                      return a.first > b.first;
                  }
                  return a.second > b.second;
              });

    // Straight flush
    if (straight && flush) {
        return encodeScore(8, {straightHigh});
    }

    // Four of a kind
    if (groups[0].first == 4) {
        return encodeScore(
            7, {groups[0].second, groups[1].second}
            );
    }

    // Full house
    if (groups[0].first == 3 &&
        groups[1].first == 2) {

        return encodeScore(
            6, {groups[0].second, groups[1].second}
            );
    }

    // Flush
    if (flush) {
        return encodeScore(5, ranks);
    }

    // Straight
    if (straight) {
        return encodeScore(4, {straightHigh});
    }

    // Three of a kind
    if (groups[0].first == 3) {
        return encodeScore(
            3, {
                groups[0].second,
                groups[1].second,
                groups[2].second
            }
            );
    }

    // Two pair
    if (groups[0].first == 2 &&
        groups[1].first == 2) {

        return encodeScore(
            2, {
                groups[0].second,
                groups[1].second,
                groups[2].second
            }
            );
    }

    // One pair
    if (groups[0].first == 2) {
        return encodeScore(
            1, {
                groups[0].second,
                groups[1].second,
                groups[2].second,
                groups[3].second
            }
            );
    }

    // High card
    return encodeScore(0, ranks);
}

void Poker::evaluateRound()
{
    if (roundOver) {
        return;
    }

    int playerScore = calcScore(player.getHand());
    int opponentScore = calcScore(opponent.getHand());

    if (playerScore > opponentScore) {
        player.addWin();
    }
    else if (playerScore < opponentScore) {
        player.addLoss();
    }

    roundOver = true;
}

const Player& Poker::getPlayer() const
{
    return player;
}

const Player& Poker::getOpponent() const
{
    return opponent;
}

bool Poker::isRoundOver() const
{
    return roundOver;
}

void Poker::reset()
{
    player.clearHand();
    opponent.clearHand();
    deck.reset();
    roundOver = true;
}

void Poker::explain_rules() const
{
    std::cout<<"poker rules to finish later";
}
