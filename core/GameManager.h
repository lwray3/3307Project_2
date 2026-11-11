
#pragma once

#include <memory>
#include <stdexcept>
#include <utility>

#include "IGame.h"

class GameManager {
private:
    std::unique_ptr<IGame> currentGame;

public:
    // Set the currently active game
    void setGame(std::unique_ptr<IGame> game) {
        if (!game) {
            throw std::invalid_argument("Game cannot be null");
        }

        currentGame = std::move(game);
    }

    // Access the current game
    IGame* getGame() const {
        return currentGame.get();
    }

    // Start a new round
    void startRound() {
        if (!currentGame) {
            throw std::logic_error("No active game");
        }

        currentGame->startRound();
    }

    // Restart the current game
    void restartRound() {
        if (!currentGame) {
            throw std::logic_error("No active game");
        }

        currentGame->reset();
        currentGame->startRound();
    }

    // Check whether the round is finished
    bool isRoundOver() const {
        return !currentGame || currentGame->isRoundOver();
    }

    // Remove the active game
    void clearGame() {
        currentGame.reset();
    }
};
