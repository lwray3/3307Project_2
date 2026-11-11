#ifndef CARDWIDGET_H
#define CARDWIDGET_H

#include <QWidget>
#include <QLabel>
#include "Card.h"

class CardWidget : public QWidget
{
public:
    explicit CardWidget(CardPtr card, QWidget *parent = nullptr);

    void updateAppearance();
    void setFaceUp(bool faceUp);

private:
    CardPtr card;

    QLabel *rankLabel;
    QLabel *suitLabel;
};

#endif