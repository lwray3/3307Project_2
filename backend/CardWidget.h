
#ifndef CARDWIDGET_H
#define CARDWIDGET_H

#include <QWidget>
#include <QLabel>

#include "Card.h"

class CardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CardWidget(const Card& card,
                        QWidget *parent = nullptr);

    void setFaceUp(bool faceUp);

private:
    Card card;

    QLabel *rankLabel;
    QLabel *suitLabel;

    void updateAppearance();
};

#endif // CARDWIDGET_H
