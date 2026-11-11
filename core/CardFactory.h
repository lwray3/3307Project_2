#ifndef CARDFACTORY_H
#define CARDFACTORY_H

#include "Card.h"
#include <map>
#include <utility>

class CardFactory {
public:
    static CardPtr getCard(Rank rank, Suit suit);

private:
    using CardKey = std::pair<Rank, Suit>;

    static std::map<CardKey, CardPtr> cardPool;
};

#endif
