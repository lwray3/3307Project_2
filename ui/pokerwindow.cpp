#include "pokerwindow.h"
#include "ui_pokerwindow.h"
#include "core/Poker.h"
#include <QPushButton>
#include <QDebug>
#include <QString>
#include <stdexcept>
#include <utility>

PokerWindow::PokerWindow(
    GameManager& manager,
    QWidget* parent)
    : GameWindow(parent),
    ui(new Ui::PokerWindow),
    manager(manager)
{
    ui->setupUi(this);

    setActionLabel(ui->actionLabel);

    connect(ui->CheckButton, &QPushButton::clicked,
            this, &PokerWindow::onCheck);

    connect(ui->CallButton, &QPushButton::clicked,
            this, &PokerWindow::onCall);

    connect(ui->RaiseButton, &QPushButton::clicked,
            this, &PokerWindow::onRaise);

    connect(ui->BetButton, &QPushButton::clicked,
            this, &PokerWindow::onBet);

    connect(ui->FoldButton, &QPushButton::clicked,
            this, &PokerWindow::onFold);

    connect(ui->playAgainButton, &QPushButton::clicked,
            this, &PokerWindow::onPlayAgain);

    connect(ui->mainMenuButton, &QPushButton::clicked,
            this, &PokerWindow::onMainMenu);

    connect(ui->Back_Button, &QPushButton::clicked,
            this, &PokerWindow::onMainMenu);
    ui->playAgainButton->hide();
    ui->mainMenuButton->hide();
    ui->playerHandLabel->clear();
    ui->dealerHandLabel->clear();
    displayInitialCards();
    updateLabels();
    updateButtons();
}

void PokerWindow::animateCard( //animate the card
    const CardPtr& card,
    QVector<CardWidget*>& collection,
    const QPoint& target,
    bool faceUp,
    std::function<void()> onFinished)
{
    CardWidget* widget = new CardWidget(card, this);
    widget->setFaceUp(faceUp);
    collection.append(widget);
    QPoint start(width() - 110, 80);
    ++activeAnimations;
    animationRunning = true;
    updateButtons();
    animateDeal(
        widget,
        start,
        target,
        [this, onFinished = std::move(onFinished)]() mutable {
            --activeAnimations;

            animationRunning = (activeAnimations > 0);
            updateButtons();

            if (onFinished) {
                onFinished();
            }
        }
        );
}
void PokerWindow::displayInitialCards() //same as the Blackjack methods
{
    const auto& playerHand = game().getPlayer().getHand();
    const auto& dealerHand = game().getDealer().getHand();

    const int spacing = 82;
    const int startX = (width() - 2 * 72 - 10) / 2;

    for (int i = 0; i < 2; ++i) {
        animateCard(
            playerHand[i],
            playerCards,
            QPoint(startX + i * spacing, 390),
            true
            );

        animateCard(
            dealerHand[i],
            dealerCards,
            QPoint(startX + i * spacing, 85),
            false
            );
    }
}

void PokerWindow::onCall() //check if the game can call and if so call if the call button
{
    if (!game().canCall()) {
        return;
    }
    game().call();
    updateButtons();
    updatePlayerStats();
}

void PokerWindow::onRaise() //same as the call method but raising
{
    int raiseAmount = ui->raiseAmountSpinBox->value();
    if (!game().canRaise(raiseAmount)) {
        return;
    }
    game().raise(raiseAmount);
    updatePlayerStats();
    updateButtons();
}
void PokerWindow::updatePlayerStats()
{
    ui->playerChipsLabel->setText(
        QString("Your Chips: $%1")
            .arg(game().getPlayerChips())
        );
    ui->dealerChipsLabel->setText(
        QString("Computer Chips: $%1")
            .arg(game().getDealerChips())
        );
    ui->potLabel->setText(
        QString("Pot: $%1")
            .arg(game().getPot())
        );
}

void PokerWindow::displayNewCommunityCards() //
{
    const auto& community = game().getCommunityCards();
    const int spacing = 82;
    const int totalWidth = 5 * 72 + 4 * 10;
    const int startX = (width() - totalWidth) / 2;
    int displayed = communityCardWidgets.size();
    for (int i = displayed;
         i < static_cast<int>(community.size());
         ++i) {
        animateCard(
            community[i],
            communityCardWidgets,
            QPoint(startX + i * spacing, 240),
            true
            );
    }
}

void PokerWindow::onCheck()
{
    if (animationRunning || !game().canCheck()) {
        return;
    }

    try {
        showActionMessage("Player checks. Computer checks.");

        game().check();
        afterBettingAction();
    }
    catch (const std::exception& e) {
        qWarning() << "Poker check error:" << e.what();
    }
}

void PokerWindow::onBet()
{
    if (animationRunning || !game().canBet()) {
        return;
    }
    try {
        showActionMessage("Player bets 20. Computer calls.");

        game().bet();
        afterBettingAction();
    }
    catch (const std::exception& e) {
        qWarning() << "Poker bet error:" << e.what();
    }
}

void PokerWindow::onFold()
{
    if (animationRunning || game().isRoundOver()) {
        return;
    }
    try {
        game().fold();

        showActionMessage("Player folds!");
        finishRoundDisplay();
    }
    catch (const std::exception& e) {
        qWarning() << "Poker fold error:" << e.what();
    }
}

void PokerWindow::afterBettingAction()
{
    updateLabels();

    if (game().isRoundOver()) {
        finishRoundDisplay();
        return;
    }
    displayNewCommunityCards();
    updateButtons();
}

void PokerWindow::updateLabels()
{
    updatePlayerStats();
}


void PokerWindow::updateButtons()
{
    bool canInteract =
        !animationRunning && !game().isRoundOver();

    ui->FoldButton->setEnabled(
        canInteract && game().canFold()
        );

    ui->CheckButton->setEnabled(
        canInteract && game().canCheck()
        );

    ui->CallButton->setEnabled(
        canInteract && game().canCall()
        );

    ui->RaiseButton->setEnabled(
        canInteract &&
        game().canRaise(ui->raiseAmountSpinBox->value())
        );


    ui->BetButton->setEnabled(
        canInteract && game().canBet()
        );

    if (game().getCurrentBet() == 0) {
        ui->RaiseButton->setText("Bet");
    } else {
        ui->RaiseButton->setText("Raise");
    }
}



void PokerWindow::finishRoundDisplay()
{
    if (!game().isRoundOver()) {
        return;
    }

    updateLabels();
    QString message;

    if (game().getResult() == PokerResult::PlayerWins) {
        message = "You win!";
    }
    else if (game().getResult() == PokerResult::DealerWins) {
        message = "Computer wins!";
    }
    else {
        message = "It's a tie!";
    }
    if (game().getCommunityCards().size() == 5) {
        const auto playerRank = game().getPlayerHandRank();
        const auto dealerRank = game().getDealerHandRank();
        ui->playerHandLabel->setText(
            QString::fromStdString(
                PokerHandEvaluator::categoryName(
                    playerRank.category
                    )
                )
            );

        ui->dealerHandLabel->setText(
            QString::fromStdString(
                PokerHandEvaluator::categoryName(
                    dealerRank.category
                    )
                )
            );
    }

    showResultMessage(message);
    updateButtons();
    ui->playAgainButton->show();
    ui->mainMenuButton->show();
    for (CardWidget* card : dealerCards) {
        if (card) {
            card->setFaceUp(true);
        }
    }
}

void PokerWindow::clearDisplayedCards()
{
    for (CardWidget* card : playerCards) {
        delete card;
    }

    for (CardWidget* card : dealerCards) {
        delete card;
    }

    for (CardWidget* card : communityCardWidgets) {
        delete card;
    }
    playerCards.clear();
    dealerCards.clear();
    communityCardWidgets.clear();
    activeAnimations = 0;
    animationRunning = false;
}

void PokerWindow::onPlayAgain()
{
    if (animationRunning || !game().isRoundOver()) {
        return;
    }

    clearDisplayedCards();
    ui->playAgainButton->hide();
    ui->mainMenuButton->hide();
    ui->playerHandLabel->clear();
    ui->dealerHandLabel->clear();
    manager.startRound();
    displayInitialCards();
    updateLabels();
    updateButtons();

    showActionMessage("New Poker round started!");
}

void PokerWindow::onMainMenu()
{
    if (animationRunning) {
        return;
    }
    emit backRequested();
}


PokerWindow::~PokerWindow()
{
    delete ui;
}
