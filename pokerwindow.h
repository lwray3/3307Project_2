#ifndef POKERWINDOW_H
#define POKERWINDOW_H

#include <QWidget>
#include "backend/Poker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class PokerWindow; }
QT_END_NAMESPACE

class PokerWindow : public QWidget {
    Q_OBJECT
public:
    explicit PokerWindow(QWidget *parent = nullptr);
    ~PokerWindow() override;

signals:
    // Connect these to a future non-blocking, turn-based Poker backend API.
    void startHandRequested();
    void foldRequested();
    void checkCallRequested();
    void raiseRequested(int totalStreetBet);

private slots:
    void onStartHand();
    void onFold();
    void onCheckCall();
    void onRaise();
    void onShowRules();

private:
    Ui::PokerWindow *ui;
    Poker game;
    void setBettingEnabled(bool enabled);
};

#endif 
