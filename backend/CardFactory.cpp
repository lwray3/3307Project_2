
#include "CardFactory.h"

std::map<CardFactory::CardKey, CardPtr>
    CardFactory::cardPool;

CardPtr CardFactory::getCard(Rank rank, Suit suit)
{
    CardKey key = {rank, suit};

    auto it = cardPool.find(key);

    // If the card exists, return the shared object.
    if (it != cardPool.end()) {
        return it->second;
    }

    // Otherwise, create the card once.
    CardPtr newCard =
        std::make_shared<const Card>(rank, suit);

    cardPool[key] = newCard;

    return newCard;
}
