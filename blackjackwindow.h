#ifndef BLACKJACKWINDOW_H
#define BLACKJACKWINDOW_H
#include <functional>
#include <QPoint>
#include <QVector>
#include <QWidget>

#include <memory>

#include "backend/Blackjack.h"
#include "backend/CardWidget.h"
#include "GameWindow.h"

namespace Ui {
class BlackjackWindow;
}

class BlackjackWindow : public GameWindow
{
    Q_OBJECT

public:
    explicit BlackjackWindow(std::unique_ptr<Blackjack> game, QWidget *parent = nullptr);

    ~BlackjackWindow();

private slots:
    void onHit();
    void onStand();
    void onPlayAgain();
    void onMainMenu();

private:
    Ui::BlackjackWindow *ui;

    // Backend game created by GameFactory
    std::unique_ptr<Blackjack> game;

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

    void animateCard(const Card &card, bool isDealer, bool faceUp = true, std::function<void()> onFinished = {});
    void afterPlayerHitAnimation();
    void displayMissingDealerCards();
    void revealDealerCards();
    void displayRoundResult();
    void finishRoundDisplay();
    void clearDisplayedCards();
    void afterPlayerAnimation();
};


#endif