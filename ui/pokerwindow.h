
#ifndef POKERWINDOW_H
#define POKERWINDOW_H

#include "GameWindow.h"
#include "core/Poker.h"
#include "core/CardWidget.h"
#include "core/GameManager.h"

#include <QVector>
#include <QPoint>

#include <functional>
#include <stdexcept>

namespace Ui {
class PokerWindow;
}

class PokerWindow : public GameWindow
{
    Q_OBJECT

public:
    explicit PokerWindow(
        GameManager& manager,
        QWidget* parent = nullptr
        );

    ~PokerWindow() override;

private slots:
    void onCheck();
    void onBet();
    void onFold();
    void onPlayAgain();
    void onMainMenu();
    void onCall();
    void onRaise();

private:
    Ui::PokerWindow* ui;

    // GameManager owns the Poker object
    GameManager& manager;

    // Access the active Poker game
    Poker& game() const {
        Poker* poker =
            dynamic_cast<Poker*>(manager.getGame());

        if (!poker) {
            throw std::logic_error(
                "No active Poker game"
                );
        }

        return *poker;
    }

    // Card widgets
    QVector<CardWidget*> playerCards;
    QVector<CardWidget*> dealerCards;
    QVector<CardWidget*> communityCardWidgets;

    // Animation management
    int activeAnimations = 0;
    bool animationRunning = false;

    // Card animation
    void animateCard(
        const CardPtr& card,
        QVector<CardWidget*>& collection,
        const QPoint& target,
        bool faceUp,
        std::function<void()> onFinished = nullptr);

    // UI methods
    void displayInitialCards();
    void displayNewCommunityCards();
    void afterBettingAction();
    void updatePlayerStats();
    void updateLabels();
    void updateButtons();
    void finishRoundDisplay();
    void clearDisplayedCards();
};

#endif
