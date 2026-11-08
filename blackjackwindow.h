
#ifndef BLACKJACKWINDOW_H
#define BLACKJACKWINDOW_H

#include <functional>
#include <stdexcept>

#include <QPoint>
#include <QVector>
#include <QWidget>

#include "backend/Blackjack.h"
#include "backend/CardWidget.h"
#include "backend/GameManager.h"
#include "GameWindow.h"

namespace Ui {
class BlackjackWindow;
}

class BlackjackWindow : public GameWindow
{
    Q_OBJECT

public:
    // GameManager owns the game
    explicit BlackjackWindow(
        GameManager& manager,
        QWidget *parent = nullptr
        );

    ~BlackjackWindow();

private slots:
    void onHit();
    void onStand();
    void onPlayAgain();
    void onMainMenu();

private:
    Ui::BlackjackWindow *ui;

    // Reference to GameManager
    GameManager& manager;

    // Access the active Blackjack backend
    Blackjack& game() const {
        Blackjack* blackjack =
            dynamic_cast<Blackjack*>(manager.getGame());

        if (!blackjack) {
            throw std::logic_error(
                "No active Blackjack game"
                );
        }

        return *blackjack;
    }

    // Visual card widgets
    QVector<CardWidget *> playerCards;
    QVector<CardWidget *> dealerCards;

    // Animation management
    int activeAnimations = 0;
    bool animationRunning = false;

    // UI methods
    void displayInitialCards();
    void beginDealerTurn();
    void processDealerTurn();
    void updatePlayerScore();
    void updateDealerScore();
    void updateButtons();

    void animateCard(
        const Card &card,
        bool isDealer,
        bool faceUp = true,
        std::function<void()> onFinished = {}
        );

    void afterPlayerHitAnimation();
    void displayMissingDealerCards();
    void revealDealerCards();
    void displayRoundResult();
    void finishRoundDisplay();
    void clearDisplayedCards();
    void afterPlayerAnimation();
};

#endif
