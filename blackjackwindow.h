#ifndef BLACKJACKWINDOW_H
#define BLACKJACKWINDOW_H

#include <QPoint>
#include <QVector>
#include <QWidget>

#include <memory>

#include "backend/Blackjack.h"
#include "backend/CardWidget.h"

namespace Ui {
class BlackjackWindow;
}

class BlackjackWindow : public GameWindow
{
    Q_OBJECT

public:
    explicit BlackjackWindow(std::unique_ptr<Blackjack> game, QWidget *parent = nullptr);

    ~BlackjackWindow();

signals:
    void backRequested();

private slots:
    void onHit();
    void onStand();

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

    void animateCard(const Card &card, bool isDealer, bool faceUp = true);

    void displayMissingDealerCards();
    void revealDealerCards();

    void updatePlayerScore();
    void updateDealerScore();
    void updateButtons();
};

#endif