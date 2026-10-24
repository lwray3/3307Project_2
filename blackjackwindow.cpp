
#include "blackjackwindow.h"
#include "ui_blackjackwindow.h"

#include <QPushButton>
#include <QLabel>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QAbstractAnimation>
#include <QDebug>

BlackjackWindow::BlackjackWindow(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::BlackjackWindow)
{
    ui->setupUi(this);

    // Back button
    connect(ui->Back_Button,
            &QPushButton::clicked,
            this,
            &BlackjackWindow::backRequested);

    // HIT button
    connect(ui->HitButton,
            &QPushButton::clicked,
            this,
            &BlackjackWindow::onHit);

    // Show the initial cards dealt by the backend.
    // Your Blackjack constructor already calls startRound().
    displayInitialCards();
}


// --------------------------------------------------
// Display initial player and dealer hands
// --------------------------------------------------

void BlackjackWindow::displayInitialCards()
{
    // Copy hands so they remain valid independently
    // of how getPlayer() and getDealer() return data.
    const auto playerHand = game.getPlayer().getHand();
    const auto dealerHand = game.getDealer().getHand();

    // Display player's initial cards
    for (const Card& card : playerHand) {
        animateCard(card, false, true);
    }

    // Display dealer's initial cards.
    // Keep the second card hidden.
    for (int i = 0; i < static_cast<int>(dealerHand.size()); i++) {
        bool faceUp = (i == 0);

        animateCard(dealerHand[i], true, faceUp);
    }

    updatePlayerScore();
}


// --------------------------------------------------
// Animate a new playing card
// --------------------------------------------------

void BlackjackWindow::animateCard(
    const Card& card,
    bool isDealer,
    bool faceUp)
{
    // Create the visual card
    CardWidget *cardWidget = new CardWidget(card, this);

    cardWidget->setFaceUp(faceUp);

    // Select which visual hand receives the card
    QVector<CardWidget*>& cards =
        isDealer ? dealerCards : playerCards;

    cards.append(cardWidget);

    int cardIndex = cards.size() - 1;

    // Card dimensions
    const int cardWidth = 72;
    const int spacing = 82;

    // Position where cards are dealt from
    QPoint startPosition(width() - 110, 80);

    // Calculate hand position
    int totalWidth =
        static_cast<int>(cards.size()) * cardWidth
        + (static_cast<int>(cards.size()) - 1)
              * (spacing - cardWidth);

    int startX = (width() - totalWidth) / 2;

    int targetX = startX + cardIndex * spacing;

    // Dealer cards appear above player cards
    int targetY = isDealer ? 145 : 370;

    QPoint targetPosition(targetX, targetY);

    // Place card at starting position
    cardWidget->move(startPosition);
    cardWidget->show();
    cardWidget->raise();

    // Create movement animation
    QPropertyAnimation *animation =
        new QPropertyAnimation(cardWidget, "pos", cardWidget);

    animation->setDuration(450);

    animation->setStartValue(startPosition);
    animation->setEndValue(targetPosition);

    animation->setEasingCurve(QEasingCurve::OutCubic);

    // Prevent repeated HIT clicks while a player
    // card is moving.
    if (!isDealer) {
        animationRunning = true;
        ui->HitButton->setEnabled(false);
    }

    connect(animation,
            &QPropertyAnimation::finished,
            this,
            [this, isDealer]() {

                if (!isDealer) {
                    animationRunning = false;

                    updatePlayerScore();

                    // Re-enable HIT only if the round is active.
                    ui->HitButton->setEnabled(
                        !game.isRoundOver()
                        );
                }
            });

    // Automatically delete the animation when done
    animation->start(
        QAbstractAnimation::DeleteWhenStopped
        );
}


// --------------------------------------------------
// Update player's score
// --------------------------------------------------

void BlackjackWindow::updatePlayerScore()
{
    const auto hand = game.getPlayer().getHand();

    int score = game.calcScore(hand);

    // This assumes a QLabel called playerScoreLabel
    // exists in your .ui file.
    ui->playerScoreLabel->setText(
        "Your Score: " + QString::number(score)
        );

    qDebug() << "Player score:" << score;
}
void BlackjackWindow::onHit() {
    if (animationRunning || game.isRoundOver()) {
        return;
    }
    game.hit();
    const auto hand = game.getPlayer().getHand();

    if (hand.empty()) {
        return;
    }
    const Card& newCard = hand.back();

    qDebug() << "Player received another card";
    qDebug() << "Number of cards:" << hand.size();

    animateCard(newCard, false, true);

    int score = game.calcScore(hand);

    qDebug() << "Player score:" << score;

    // Check for bust
    if (score > 21) {
        qDebug() << "Player busted!";

        // The backend should have set roundOver = true.
        // Disable HIT immediately.
        ui->HitButton->setEnabled(false);
    }
}


// --------------------------------------------------
// Destructor
// --------------------------------------------------

BlackjackWindow::~BlackjackWindow()
{
    delete ui;
}
