
#ifndef POKERHANDEVALUATOR_H
#define POKERHANDEVALUATOR_H

#include "Card.h"
#include <array>
#include <string>
#include <vector>

enum class PokerHandCategory {
    HighCard = 0,
    OnePair,
    TwoPair,
    ThreeOfAKind,
    Straight,
    Flush,
    FullHouse,
    FourOfAKind,
    StraightFlush
};

// A royal flush is an Ace-high straight flush.
struct PokerHandRank {
    PokerHandCategory category = PokerHandCategory::HighCard;

    // Used to break ties within the same category.
    // For example, pair rank followed by kickers.
    std::array<int, 5> tieBreakers{};

    bool operator<(const PokerHandRank& other) const {
        if (category != other.category)
            return category < other.category;

        return tieBreakers < other.tieBreakers;
    }

    bool operator==(const PokerHandRank& other) const {
        return category == other.category
               && tieBreakers == other.tieBreakers;
    }
};

class PokerHandEvaluator {
public:
    static PokerHandRank evaluate(
        const std::vector<Card>& cards
        );

    static std::string categoryName(PokerHandCategory category);

private:
    static PokerHandRank evaluateFive(
        const std::array<Card, 5>& cards
        );
};

#endif
