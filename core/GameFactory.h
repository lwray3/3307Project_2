#ifndef GAMEFACTORY_H
#define GAMEFACTORY_H

#include <memory>

#include "IGame.h"
#include "Blackjack.h"
#include "Poker.h"
#include "Baccarat.h"

enum class GameType {
    Blackjack,
    Poker,
    Baccarat
};

class GameFactory {
public:
    static std::unique_ptr<IGame> createGame(
        GameType type
        );

    static std::unique_ptr<Blackjack> createBlackjack();
    static std::unique_ptr<Poker> createPoker();
    static std::unique_ptr<Baccarat> createBaccarat();
};

#endif
