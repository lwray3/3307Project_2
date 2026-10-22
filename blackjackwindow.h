
#ifndef BLACKJACKWINDOW_H
#define BLACKJACKWINDOW_H

#include <QWidget>
#include "backend/Blackjack.h"

namespace Ui {
class BlackjackWindow;
}

class BlackjackWindow : public QWidget
{
    Q_OBJECT

public:
    explicit BlackjackWindow(QWidget *parent = nullptr);
    ~BlackjackWindow();

private slots:
    void onHit();

private:
    Ui::BlackjackWindow *ui;

    Blackjack game;
};

#endif