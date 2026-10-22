#include <iostream>
#include <vector>
#include "Card.h"
#include "IGame.h"

class GameManager {
    IGame& currentGame;

    void startGame() {
        currentGame.explain_rules();
        currentGame.play();
    }
};