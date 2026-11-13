
#include "CardFactory.h"

std::map<CardFactory::CardKey, CardPtr>
    CardFactory::cardPool;

CardPtr CardFactory::getCard(Rank rank, Suit suit)
{
    CardKey key = {rank, suit};
    auto it = cardPool.find(key);
    if (it != cardPool.end()) { // If the card exists, return the shared object.
        return it->second;
    }
    CardPtr newCard = std::make_shared<const Card>(rank, suit);
    cardPool[key] = newCard;
    return newCard;
}
