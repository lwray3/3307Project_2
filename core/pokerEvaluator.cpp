
#include "pokerEvaluator.h"

#include <algorithm>
#include <stdexcept>

PokerHandRank PokerHandEvaluator::evaluateFive(
    const std::array<CardPtr, 5>& cards)
{
    std::array<int, 15> counts{};
    std::vector<int> values;
    values.reserve(5);

    const Suit firstSuit = cards[0]->getSuit();
    bool flush = true;

    for (CardPtr card : cards) {
        int value = static_cast<int>(card->getRank());

        // Assumes Rank::two = 2 ... Rank::ace = 14
        if (value < 2 || value > 14) {
            throw std::logic_error("Invalid card rank");
        }

        counts[value]++;
        values.push_back(value);

        if (card->getSuit() != firstSuit) {
            flush = false;
        }
    }

    std::sort(values.begin(), values.end(), std::greater<int>());

    std::vector<int> unique = values;
    unique.erase(
        std::unique(unique.begin(), unique.end()),
        unique.end()
        );

    int straightHigh = 0;

    if (unique.size() == 5) {
        if (unique[0] - unique[4] == 4) {
            straightHigh = unique[0];
        }
        // A-2-3-4-5 is a five-high straight.
        else if (unique ==
                 std::vector<int>{14, 5, 4, 3, 2}) {
            straightHigh = 5;
        }
    }

    std::vector<std::pair<int, int>> groups;

    for (int rank = 2; rank <= 14; ++rank) {
        if (counts[rank] > 0) {
            groups.emplace_back(counts[rank], rank);
        }
    }

    std::sort(groups.rbegin(), groups.rend());

    PokerHandRank result;

    auto setTie = [&](std::initializer_list<int> ranks) {
        int i = 0;
        for (int rank : ranks) {
            result.tieBreakers[i++] = rank;
        }
    };

    if (flush && straightHigh > 0) {
        result.category = PokerHandCategory::StraightFlush;
        setTie({straightHigh});
    }
    else if (groups[0].first == 4) {
        result.category = PokerHandCategory::FourOfAKind;
        setTie({groups[0].second, groups[1].second});
    }
    else if (groups[0].first == 3 &&
             groups[1].first == 2) {
        result.category = PokerHandCategory::FullHouse;
        setTie({groups[0].second, groups[1].second});
    }
    else if (flush) {
        result.category = PokerHandCategory::Flush;
        std::copy(values.begin(), values.end(),
                  result.tieBreakers.begin());
    }
    else if (straightHigh > 0) {
        result.category = PokerHandCategory::Straight;
        setTie({straightHigh});
    }
    else if (groups[0].first == 3) {
        result.category = PokerHandCategory::ThreeOfAKind;
        setTie({
            groups[0].second,
            groups[1].second,
            groups[2].second
        });
    }
    else if (groups[0].first == 2 &&
             groups[1].first == 2) {
        result.category = PokerHandCategory::TwoPair;
        setTie({
            groups[0].second,
            groups[1].second,
            groups[2].second
        });
    }
    else if (groups[0].first == 2) {
        result.category = PokerHandCategory::OnePair;
        setTie({
            groups[0].second,
            groups[1].second,
            groups[2].second,
            groups[3].second
        });
    }
    else {
        result.category = PokerHandCategory::HighCard;
        std::copy(values.begin(), values.end(),
                  result.tieBreakers.begin());
    }

    return result;
}

PokerHandRank PokerHandEvaluator::evaluate(
    const std::vector<CardPtr>& cards)
{
    if (cards.size() < 5 || cards.size() > 7) {
        throw std::invalid_argument(
            "Poker evaluation requires 5 to 7 cards"
            );
    }

    bool found = false;
    PokerHandRank best;

    // Examine every possible combination of five cards.
    const int n = static_cast<int>(cards.size());

    for (int a = 0; a < n - 4; ++a)
        for (int b = a + 1; b < n - 3; ++b)
            for (int c = b + 1; c < n - 2; ++c)
                for (int d = c + 1; d < n - 1; ++d)
                    for (int e = d + 1; e < n; ++e) {
                        std::array<CardPtr, 5> hand{
                            cards[a], cards[b], cards[c],
                            cards[d], cards[e]
                        };

                        PokerHandRank current = evaluateFive(hand);

                        if (!found || best < current) {
                            best = current;
                            found = true;
                        }
                    }

    return best;
}

std::string PokerHandEvaluator::categoryName(
    PokerHandCategory category)
{
    switch (category) {
    case PokerHandCategory::StraightFlush:
        return "Straight Flush";
    case PokerHandCategory::FourOfAKind:
        return "Four of a Kind";
    case PokerHandCategory::FullHouse:
        return "Full House";
    case PokerHandCategory::Flush:
        return "Flush";
    case PokerHandCategory::Straight:
        return "Straight";
    case PokerHandCategory::ThreeOfAKind:
        return "Three of a Kind";
    case PokerHandCategory::TwoPair:
        return "Two Pair";
    case PokerHandCategory::OnePair:
        return "One Pair";
    default:
        return "High Card";
    }
}
