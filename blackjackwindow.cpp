#include "blackjackwindow.h"
#include "ui_blackjackwindow.h"

#include <QPushButton>
#include <QLabel>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QAbstractAnimation>
#include <QDebug>
#include <QString>

#include <stdexcept>
#include <utility>

// --------------------------------------------------
// Constructor
// --------------------------------------------------

BlackjackWindow::BlackjackWindow(

    std::unique_ptr<Blackjack> game,
    QWidget* parent
    )
    : QWidget(parent),
    ui(new Ui::BlackjackWindow),
    game(std::move(game))
{
    if (!this->game) {
        throw std::invalid_argument("Blackjack game is null");
    }

    ui->setupUi(this);

    // Back button
    connect(
        ui->Back_Button,
        &QPushButton::clicked,
        this,
        &BlackjackWindow::backRequested
        );

    // Hit button
    connect(
        ui->HitButton,
        &QPushButton::clicked,
        this,
        &BlackjackWindow::onHit
        );

    // Stand button
    connect(
        ui->StandButton,
        &QPushButton::clicked,
        this,
        &BlackjackWindow::onStand
        );

    // Start the round in the backend
    this->game->startRound();

    // Display the initial cards
    displayInitialCards();

    updatePlayerScore();
    updateButtons();
}


// --------------------------------------------------
// Display initial cards
// --------------------------------------------------

void BlackjackWindow::displayInitialCards()
{
    const auto& playerHand =
        game->getPlayer().getHand();

    const auto& dealerHand =
        game->getDealer().getHand();

    // Display player's initial cards
    for (const Card& card : playerHand) {
        animateCard(card, false, true);
    }

    // Display dealer's initial cards
    for (int i = 0;
         i < static_cast<int>(dealerHand.size());
         i++) {

        bool faceUp = (i == 0);

        animateCard(
            dealerHand[i],
            true,
            faceUp
            );
    }
}


// --------------------------------------------------
// Animate a card
// --------------------------------------------------

void BlackjackWindow::animateCard(
    const Card& card,
    bool isDealer,
    bool faceUp
    )
{
    CardWidget* cardWidget =
        new CardWidget(card, this);

    cardWidget->setFaceUp(faceUp);

    QVector<CardWidget*>& cards =
        isDealer ? dealerCards : playerCards;

    cards.append(cardWidget);

    int cardIndex = cards.size() - 1;

    const int cardWidth = 72;
    const int spacing = 82;

    QPoint startPosition(
        width() - 110,
        80
        );

    int totalWidth =
        static_cast<int>(cards.size()) * cardWidth
        + (static_cast<int>(cards.size()) - 1)
              * (spacing - cardWidth);

    int startX =
        (width() - totalWidth) / 2;

    int targetX =
        startX + cardIndex * spacing;

    int targetY =
        isDealer ? 145 : 370;

    QPoint targetPosition(
        targetX,
        targetY
        );

    cardWidget->move(startPosition);
    cardWidget->show();
    cardWidget->raise();

    QPropertyAnimation* animation =
        new QPropertyAnimation(
            cardWidget,
            "pos",
            cardWidget
            );

    animation->setDuration(450);

    animation->setStartValue(startPosition);
    animation->setEndValue(targetPosition);

    animation->setEasingCurve(
        QEasingCurve::OutCubic
        );

    // Count running animations
    activeAnimations++;
    animationRunning = true;

    updateButtons();

    connect(
        animation,
        &QPropertyAnimation::finished,
        this,
        [this]() {

            activeAnimations--;

            animationRunning =
                (activeAnimations > 0);

            updatePlayerScore();
            updateButtons();
        }
        );

    animation->start(
        QAbstractAnimation::DeleteWhenStopped
        );
}


// --------------------------------------------------
// Update player's score
// --------------------------------------------------

void BlackjackWindow::updatePlayerScore()
{
    int score = game->getPlayerScore();

    ui->playerScoreLabel->setText(
        "Your Score: "
        + QString::number(score)
        );

    qDebug() << "Player score:" << score;
}


// --------------------------------------------------
// Update dealer's score
// --------------------------------------------------

void BlackjackWindow::updateDealerScore()
{
    if (!game->isRoundOver()) {
        return;
    }

    int score = game->getDealerScore();

    qDebug() << "Dealer score:" << score;

    // If you have a dealer score QLabel,
    // update it here.
}


// --------------------------------------------------
// Update buttons
// --------------------------------------------------

void BlackjackWindow::updateButtons()
{
    bool canInteract =
        !animationRunning;

    ui->HitButton->setEnabled(
        canInteract && game->canHit()
        );

    ui->StandButton->setEnabled(
        canInteract && game->canStand()
        );
}


// --------------------------------------------------
// Hit button
// --------------------------------------------------

void BlackjackWindow::onHit()
{
    if (animationRunning || !game->canHit()) {
        return;
    }

    try {

        // Backend handles dealing and scoring
        Card newCard = game->hit();

        qDebug() << "Player received another card";

        // UI animates the new card
        animateCard(
            newCard,
            false,
            true
            );

        updatePlayerScore();

        if (game->isRoundOver()) {

            qDebug() << "Round ended";

            // Reveal dealer's cards
            revealDealerCards();

            // Animate any dealer cards
            displayMissingDealerCards();

            updateDealerScore();
        }

        updateButtons();
    }
    catch (const std::exception& e) {
        qWarning() << "Hit error:" << e.what();
    }
}


// --------------------------------------------------
// Stand button
// --------------------------------------------------

void BlackjackWindow::onStand()
{
    if (animationRunning || !game->canStand()) {
        return;
    }

    try {

        // Backend handles dealer turn
        game->stand();

        // Reveal dealer's hidden card
        revealDealerCards();

        // Animate additional dealer cards
        displayMissingDealerCards();

        updatePlayerScore();
        updateDealerScore();
        updateButtons();
    }
    catch (const std::exception& e) {
        qWarning() << "Stand error:" << e.what();
    }
}


// --------------------------------------------------
// Animate missing dealer cards
// --------------------------------------------------

void BlackjackWindow::displayMissingDealerCards()
{
    const auto& dealerHand =
        game->getDealer().getHand();

    int displayed = dealerCards.size();

    for (int i = displayed;
         i < static_cast<int>(dealerHand.size());
         i++) {

        animateCard(
            dealerHand[i],
            true,
            true
            );
    }
}


// --------------------------------------------------
// Reveal dealer's cards
// --------------------------------------------------

void BlackjackWindow::revealDealerCards()
{
    if (!game->isRoundOver()) {
        return;
    }

    for (CardWidget* card : dealerCards) {
        card->setFaceUp(true);
    }
}


// --------------------------------------------------
// Destructor
// --------------------------------------------------

BlackjackWindow::~BlackjackWindow()
{
    delete ui;
}