#include "GameFactory.h"
#include <stdexcept>

std::unique_ptr<Blackjack> GameFactory::createBlackjack() //create a blackjack object
{
    return std::make_unique<Blackjack>();
}

std::unique_ptr<Poker> GameFactory::createPoker() //create a poker object
{
    return std::make_unique<Poker>();
}

std::unique_ptr<Baccarat> GameFactory::createBaccarat()
{
    return std::make_unique<Baccarat>();
}

std::unique_ptr<IGame> GameFactory::createGame(
    GameType type
    )
{
    switch (type) {
    case GameType::Blackjack:
        return createBlackjack();

    case GameType::Poker:
        return createPoker();

    case GameType::Baccarat:
        return createBaccarat();
    }

    throw std::invalid_argument("Invalid game type");
}