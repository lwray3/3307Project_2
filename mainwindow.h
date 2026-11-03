
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class BlackjackWindow;
class PokerWindow;
class BaccaratWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openBlackjack();
    void openPoker();
    void openBaccarat();

private:
    Ui::MainWindow *ui;
    BlackjackWindow *blackjackWindow = nullptr;
    PokerWindow *pokerWindow = nullptr;
    BaccaratWindow *baccaratWindow = nullptr;
};

#endif
