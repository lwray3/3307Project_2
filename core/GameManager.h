#pragma once
#include <memory>
#include <stdexcept>
#include <utility>
#include "IGame.h"

class GameManager {
private:
    std::unique_ptr<IGame> currentGame;

public:
    void setGame(std::unique_ptr<IGame> game) {
        if (!game) {
            throw std::invalid_argument("Game cannot be null");
        }
        currentGame = std::move(game);
    }

    IGame* getGame() const { //get the current game
        return currentGame.get();
    }

    void startRound() { //start the new round
        if (!currentGame) {
            throw std::logic_error("No active game");
        }

        currentGame->startRound();
    }

    void restartRound() {
        if (!currentGame) {
            throw std::logic_error("No active game");
        }
        currentGame->reset();
        currentGame->startRound();
    }

    bool isRoundOver() const {
        return !currentGame || currentGame->isRoundOver();
    }

    void clearGame() {
        currentGame.reset();
    }
};
