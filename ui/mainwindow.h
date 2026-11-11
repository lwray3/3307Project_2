#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "core/GameManager.h"
namespace Ui {
class MainWindow;
}

class BlackjackWindow;
class PokerWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    GameManager manager;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openBlackjack();
    void openPoker();

private:
    Ui::MainWindow *ui;
    BlackjackWindow *blackjackWindow = nullptr;
    PokerWindow *pokerWindow = nullptr;
};

#endif