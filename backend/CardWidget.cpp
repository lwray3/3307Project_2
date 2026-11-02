#include "CardWidget.h"

#include <QVBoxLayout>
#include <QString>
#include <QStyle>

CardWidget::CardWidget(const Card& card, QWidget *parent)
    : QWidget(parent), card(card)
{
    setFixedSize(72, 102);
    setObjectName("playingCard");
    setAttribute(Qt::WA_StyledBackground, true);
    rankLabel = new QLabel(this);
    suitLabel = new QLabel(this);

    rankLabel->setAlignment(Qt::AlignLeft);
    suitLabel->setAlignment(Qt::AlignCenter);

    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->setContentsMargins(9, 8, 9, 8);
    layout->addWidget(rankLabel);
    layout->addWidget(suitLabel);

    updateAppearance();
}

void CardWidget::updateAppearance()
{
    QString rank;

    switch (card.getRank()) {
    case Rank::two: rank = "2"; break;
    case Rank::three: rank = "3"; break;
    case Rank::four: rank = "4"; break;
    case Rank::five: rank = "5"; break;
    case Rank::six: rank = "6"; break;
    case Rank::seven: rank = "7"; break;
    case Rank::eight: rank = "8"; break;
    case Rank::nine: rank = "9"; break;
    case Rank::ten: rank = "10"; break;
    case Rank::jack: rank = "J"; break;
    case Rank::queen: rank = "Q"; break;
    case Rank::king: rank = "K"; break;
    case Rank::ace: rank = "A"; break;
    }

    QString suit;

    switch (card.getSuit()) {
    case Suit::hearts: suit = "♥"; break;
    case Suit::diamonds: suit = "♦"; break;
    case Suit::clubs: suit = "♣"; break;
    case Suit::spades: suit = "♠"; break;
    }

    QString color =
        (card.getSuit() == Suit::hearts ||
         card.getSuit() == Suit::diamonds)
            ? "#d81e3f" : "#203044";

    // Style the card itself
    setStyleSheet(
        "QWidget#playingCard {"
        " background-color: #f5f0e6;"
        " border: 1px solid #d5c9b7;"
        " border-radius: 10px;"
        "}"
        "QWidget#playingCard QLabel {"
        " background-color: transparent;"
        " border: none;"
        "}"
        );

    // Display rank and suit
    rankLabel->setText(rank + " " + suit);
    suitLabel->setText(suit);

    // Style rank label
    rankLabel->setStyleSheet(
        "QLabel {"
        " color: " + color + ";"
                  " background-color: transparent;"
                  " font-size: 17px;"
                  " font-weight: bold;"
                  " border: none;"
                  "}"
        );

    // Style suit label
    suitLabel->setStyleSheet(
        "QLabel {"
        " color: " + color + ";"
                  " background-color: transparent;"
                  " font-size: 30px;"
                  " border: none;"
                  "}"
        );
}

void CardWidget::setFaceUp(bool faceUp)
{
    rankLabel->setVisible(faceUp);
    suitLabel->setVisible(faceUp);

    if (faceUp) {
        updateAppearance();
    } else {
        setStyleSheet(
            "QWidget#playingCard {"
            " background-color: #203c55;"
            " border: 2px solid #d6ae60;"
            " border-radius: 10px;"
            "}"
            );
    }
}