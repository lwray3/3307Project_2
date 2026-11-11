#include "blackjackwindow.h"
#include "ui_blackjackwindow.h"
#include <QTimer>
#include <QAbstractAnimation>
#include <QDebug>
#include <QEasingCurve>
#include <QLabel>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QString>
#include "GameWindow.h"
#include <stdexcept>
#include <utility>

BlackjackWindow::BlackjackWindow( //constructor
    GameManager& manager,
    QWidget* parent
    )
    : GameWindow(parent)
    , ui(new Ui::BlackjackWindow)
    , manager(manager)
{
    ui->setupUi(this);

    setActionLabel(ui->actionLabel);

    connect(ui->HitButton,
            &QPushButton::clicked,
            this,
            &BlackjackWindow::onHit);

    connect(ui->StandButton,
            &QPushButton::clicked,
            this,
            &BlackjackWindow::onStand);

    connect(ui->playAgainButton,
            &QPushButton::clicked,
            this,
            &BlackjackWindow::onPlayAgain);

    connect(ui->mainMenuButton,
            &QPushButton::clicked,
            this,
            &BlackjackWindow::onMainMenu);

    connect(ui->Back_Button,
            &QPushButton::clicked,
            this,
            &BlackjackWindow::backRequested);

    ui->playAgainButton->hide();
    ui->mainMenuButton->hide();

    displayInitialCards();
    updatePlayerScore();
    updateDealerScore();
    updateButtons();
}

void BlackjackWindow::displayInitialCards() {
    const auto &playerHand = game().getPlayer().getHand();
    const auto &dealerHand = game().getDealer().getHand();
    for (const CardPtr &card : playerHand) { // Display player's initial cards
        animateCard(card, false, true);
    }

    for (int i = 0; i < static_cast<int>(dealerHand.size()); i++) { // Display dealer's initial cards
        bool faceUp = (i == 0);
        animateCard(dealerHand[i], true, faceUp);
    }
}

void BlackjackWindow::animateCard(
    const CardPtr& card,
    bool isDealer,
    bool faceUp,
    std::function<void()> onFinished)
{
    CardWidget* cardWidget = new CardWidget(card, this);
    cardWidget->setFaceUp(faceUp);
    QVector<CardWidget*>& cards =
        isDealer ? dealerCards : playerCards;

    cards.append(cardWidget);
    int cardIndex = cards.size() - 1;
    const int cardWidth = 72;
    const int spacing = 82;
    QPoint startPosition(width() - 110, 80);
    int totalWidth =
        static_cast<int>(cards.size()) * cardWidth
        + (static_cast<int>(cards.size()) - 1)
              * (spacing - cardWidth);
    int startX = (width() - totalWidth) / 2;
    int targetX = startX + cardIndex * spacing;
    int targetY = isDealer ? 145 : 370;

    QPoint targetPosition(targetX, targetY);
    ++activeAnimations;
    animationRunning = true;
    updateButtons();
    animateDeal(
        cardWidget,
        startPosition,
        targetPosition,
        [this, onFinished = std::move(onFinished)]() mutable {

            --activeAnimations;

            animationRunning = (activeAnimations > 0);

            updatePlayerScore();
            updateButtons();

            // Continue the game after the animation
            if (onFinished) {
                onFinished();
            }
        }
        );
}

void BlackjackWindow::updatePlayerScore() {
    int score = game().getPlayerScore();
    ui->playerScoreLabel->setText("Your Score: " + QString::number(score));
    qDebug() << "Player score:" << score;
}

void BlackjackWindow::updateDealerScore() {
    int score = game().getDealerScore();
    qDebug() << "updateDealerScore() called!";
    qDebug() << "Backend dealer score:" << score;
    qDebug() << "Dealer turn:" << game().isDealerTurn();
    qDebug() << "Round over:" << game().isRoundOver();

    if (!game().isDealerTurn() && !game().isRoundOver()) {
        ui->dealerScoreLabel->setText("Dealer Score: ?");
        return;
    }

    ui->dealerScoreLabel->setText(
        "Dealer Score: "
        + QString::number(game().getDealerScore())
        );
}

void BlackjackWindow::updateButtons() {
    bool canInteract = !animationRunning &&!game().isRoundOver();
    ui->HitButton->setEnabled(canInteract && game().canHit());
    ui->StandButton->setEnabled(canInteract && game().canStand());
}

void BlackjackWindow::onHit() {
    if (animationRunning || !game().canHit()) {
        return;
    }

    try {
        showActionMessage("Player hits!");
        CardPtr newCard = game().hit();
        animateCard(
            newCard,
            false,
            true,
            [this]() {
                updatePlayerScore();
                if (game().getPlayerScore() > 21) {
                    showActionMessage("Player busts!");
                    revealDealerCards();
                    finishRoundDisplay();

                    return;
                }
                if (game().isDealerTurn()) {
                    beginDealerTurn();
                    return;
                }
                updateButtons();
            }
            );
        updateButtons();
    }
    catch (const std::exception& e) {
        qWarning() << "Hit error:" << e.what();
    }
}

void BlackjackWindow::finishRoundDisplay()
{
    if (!game().isRoundOver()) {
        return;
    }
    revealDealerCards();
    int playerScore = game().getPlayerScore();
    int dealerScore = game().getDealerScore();
    QString result;
    if (playerScore > 21) {
        result = "Player busts! You lose!";
    }
    else if (dealerScore > 21) {
        result = "Dealer busts! You win!";
    }
    else if (playerScore > dealerScore) {
        result = "You win!";
    }
    else if (dealerScore > playerScore) {
        result = "Dealer wins! You lose!";
    }
    else {
        result = "Push! It's a tie!";
    }
    showResultMessage(result); //display the result

    updatePlayerScore();
    updateDealerScore();
    // Disable player actions
    ui->HitButton->setEnabled(false);
    ui->StandButton->setEnabled(false);
    //these are the buttons used after the game ends
    ui->playAgainButton->show();
    ui->mainMenuButton->show();
}

void BlackjackWindow::afterPlayerHitAnimation()
{
    if (game().isRoundOver()) {
        finishRoundDisplay();
    }
    else if (game().isDealerTurn()) {
        beginDealerTurn();
    }
    else {
        updateButtons();
    }
}

void BlackjackWindow::beginDealerTurn()
{
    if (!game().isDealerTurn() || game().isRoundOver()) {
        return;
    }
    revealDealerCards();
    updateDealerScore();
    updateButtons();

    QTimer::singleShot(600, this, [this]() {
        processDealerTurn();
    });
}

void BlackjackWindow::processDealerTurn()
{
    if (!game().isDealerTurn() || game().isRoundOver()) {
        return;
    }
    if (animationRunning) {
        return;
    }
    try {
        if (game().dealerShouldHit()) {
            showActionMessage("Dealer hits!");
            CardPtr newCard = game().dealerHit();
            qDebug() << "Dealer cards:" << game().getDealer().getHand().size();
            qDebug() << "Dealer score:" << game().getDealerScore();
            qDebug() << "Dealer turn:" << game().isDealerTurn();
            animateCard(
                newCard,
                true,
                true,
                [this]() {
                    updateDealerScore();
                    processDealerTurn();
                }
                );

            updateButtons();
        }
        else {
            if (game().getDealerScore() > 21) {
                showActionMessage("Dealer busts!");
            }
            else {
                showActionMessage("Dealer stands!");
            }

            game().finishRound();

            QTimer::singleShot(600, this, [this]() {
                finishRoundDisplay();
            });
        }
    }
    catch (const std::exception& e) {
        qWarning() << "Dealer turn error:" << e.what();
    }
}
void BlackjackWindow::displayRoundResult()
{
    int playerScore = game().getPlayerScore();
    int dealerScore = game().getDealerScore();

    QString result;

    if (playerScore > 21) {
        result = "Player busts! Dealer wins!";
    }
    else if (dealerScore > 21) {
        result = "Dealer busts! Player wins!";
    }
    else if (playerScore > dealerScore) {
        result = "Player wins!";
    }
    else if (dealerScore > playerScore) {
        result = "Dealer wins!";
    }
    else {
        result = "It's a tie!";
    }
    showResultMessage(result);
}
void BlackjackWindow::onStand()
{
    if (animationRunning || !game().canStand()) {
        return;
    }
    try {
        showActionMessage("Player stands!");

        game().stand();

        updateButtons();

        beginDealerTurn();
    }
    catch (const std::exception& e) {
        qWarning() << "Stand error:" << e.what();
    }
}

void BlackjackWindow::displayMissingDealerCards()
{
    const auto &dealerHand = game().getDealer().getHand();
    int displayed = dealerCards.size();
    for (int i = displayed; i < static_cast<int>(dealerHand.size()); i++) {
        animateCard(dealerHand[i], true, true);
    }
}

void BlackjackWindow::revealDealerCards()
{
    if (!game().isDealerTurn() && !game().isRoundOver()) {
        return;
    }
    for (CardWidget* card : dealerCards) {
        if (card) {
            card->setFaceUp(true);
        }
    }
}

void BlackjackWindow::clearDisplayedCards()
{
    for (CardWidget* card : playerCards) {
        delete card;
    }

    for (CardWidget* card : dealerCards) {
        delete card;
    }

    playerCards.clear();
    dealerCards.clear();
    activeAnimations = 0;
    animationRunning = false;
}


void BlackjackWindow::onPlayAgain()
{
    if (animationRunning || !manager.isRoundOver()) {
        return;
    }
    manager.restartRound();
    clearDisplayedCards();
    ui->playAgainButton->hide();
    ui->mainMenuButton->hide();
    ui->playerScoreLabel->setText("Your Score: 0");
    ui->dealerScoreLabel->setText("Dealer Score: ?");

    displayInitialCards();
    updatePlayerScore();
    updateDealerScore();
    updateButtons();
    showActionMessage("New Blackjack round started!");
}

void BlackjackWindow::onMainMenu()
{
    if (animationRunning) {
        return;
    }
    emit backRequested();
}

BlackjackWindow::~BlackjackWindow()
{
    delete ui;
}