
#ifndef BLACKJACKWINDOW_H
#define BLACKJACKWINDOW_H

#include <functional>
#include <stdexcept>

#include <QPoint>
#include <QVector>
#include <QWidget>

#include "core/Blackjack.h"
#include "core/CardWidget.h"
#include "core/GameManager.h"
#include "GameWindow.h"

namespace Ui {
class BlackjackWindow;
}

class BlackjackWindow : public GameWindow
{
    Q_OBJECT
public:
    explicit BlackjackWindow(
        GameManager& manager, //gameManager is in charge of the game
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
    GameManager& manager;
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
    QVector<CardWidget *> playerCards;
    QVector<CardWidget *> dealerCards;
    int activeAnimations = 0;
    bool animationRunning = false;
    void displayInitialCards();
    void beginDealerTurn();
    void processDealerTurn();
    void updatePlayerScore();
    void updateDealerScore();
    void updateButtons();
    void animateCard(
        const CardPtr &card,
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
