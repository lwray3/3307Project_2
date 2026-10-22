
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class BlackjackWindow;
class PokerWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

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
