
#include "baccaratwindow.h"
#include "ui_baccaratwindow.h"
#include "backend/GameFactory.h"

#include <QPushButton>
#include <QMessageBox>
#include <QStringList>

#include "backend/CardWidget.h"
#include <QHBoxLayout>

static QString handToText(const std::vector<Card>& hand)
{
    QStringList cards;

    for (const Card& card : hand) {
        int value = static_cast<int>(card.getRank());
        QString rank = QString::number(value);

        if (value == 11) rank = "J";
        else if (value == 12) rank = "Q";
        else if (value == 13) rank = "K";
        else if (value == 14) rank = "A";

        QChar suit;
        switch (card.getSuit()) {
        case Suit::hearts:   suit = QChar(0x2665); break;
        case Suit::diamonds: suit = QChar(0x2666); break;
        case Suit::clubs:    suit = QChar(0x2663); break;
        case Suit::spades:   suit = QChar(0x2660); break;
        }

        cards << (rank + suit);
    }

    return cards.join("\n");
}


static void displayHand(QHBoxLayout* layout,const std::vector<Card>& hand)
{

    while (QLayoutItem* item = layout->takeAt(0)) {
        delete item->widget();
        delete item;
    }

    // show card animation
    for (const Card& card : hand) {
        CardWidget* cardWidget =
            new CardWidget(card, layout->parentWidget());

        layout->addWidget(cardWidget);
    }
}


BaccaratWindow::BaccaratWindow(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::Dialog),
    game(GameFactory::createBaccarat())
{
    ui->setupUi(this);


    playerCardsLayout = new QHBoxLayout(ui->PlayerCards);
    bankerCardsLayout = new QHBoxLayout(ui->BankerCards);

    playerCardsLayout->setContentsMargins(0, 0, 0, 0);
    bankerCardsLayout->setContentsMargins(0, 0, 0, 0);

    playerCardsLayout->setSpacing(8);
    bankerCardsLayout->setSpacing(8);

    playerCardsLayout->setAlignment(Qt::AlignCenter);
    bankerCardsLayout->setAlignment(Qt::AlignCenter);


    // back
    connect(ui->Back_Button, &QPushButton::clicked, this, &BaccaratWindow::backRequested);

    // deal
    connect(ui->pushButton_2, &QPushButton::clicked, this, &BaccaratWindow::onDeal);
}

void BaccaratWindow::onDeal()
{
    game->startRound();
    game->playRound();

    // Display the cards
    displayHand(
        playerCardsLayout,
        game->getPlayer().getHand()
        );

    displayHand(
        bankerCardsLayout,
        game->getBanker().getHand()
        );

    int playerScore = game->getPlayerScore();
    int bankerScore = game->getBankerScore();

    ui->label_6->setText(QString::number(playerScore));
    ui->label_7->setText(QString::number(bankerScore));

    QString result;

    if (playerScore > bankerScore) {
        result = "Player wins!";
    }
    else if (bankerScore > playerScore) {
        result = "Banker wins!";
    }
    else {
        result = "It's a tie!";
    }

    ui->result_label->setText(result);
}
BaccaratWindow::~BaccaratWindow()
{

    delete ui;
}
