
#ifndef BLACKJACKWINDOW_H
#define BLACKJACKWINDOW_H

#include <QWidget>
#include <QVector>
#include <QPoint>

#include "backend/Blackjack.h"
#include "backend/CardWidget.h"

namespace Ui {
class BlackjackWindow;
}

class BlackjackWindow : public QWidget
{
    Q_OBJECT

public:
    explicit BlackjackWindow(QWidget *parent = nullptr);
    ~BlackjackWindow();

signals:
    void backRequested();

private slots:
    void onHit();

private:
    Ui::BlackjackWindow *ui;

    // Existing Blackjack game logic
    Blackjack game;

    // Cards currently displayed on screen
    QVector<CardWidget*> playerCards;
    QVector<CardWidget*> dealerCards;

    // Prevent multiple clicks during an animation
    bool animationRunning = false;

    // Create and animate a new card
    void animateCard(const Card& card,
                     bool isDealer,
                     bool faceUp = true);

    // Display the player's current score
    void updatePlayerScore();

    // Display the initial two cards for each hand
    void displayInitialCards();
};

#endif
