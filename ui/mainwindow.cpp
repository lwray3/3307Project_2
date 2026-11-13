#include "mainwindow.h"
#include "core/GameFactory.h"
#include "blackjackwindow.h"
#include "pokerwindow.h"
#include "baccaratwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPushButton>
#include <memory>
#include <utility>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->playBlackjack, &QPushButton::clicked, this, &MainWindow::openBlackjack);
    connect(ui->playPoker, &QPushButton::clicked, this, &MainWindow::openPoker);
    connect(ui->playBaccarat, &QPushButton::clicked, this, &MainWindow::openBaccarat);
}
void MainWindow::openBlackjack() //checks whether a blackjack window already exists, and calls GameFactory to create Blackjack game
{
    qDebug() << "Play Blackjack button clicked!";
    if (!blackjackWindow) { //only creates a new Blackjack window if one doesn't exist
        qDebug() << "Creating blackjack game";
        auto blackjack = GameFactory::createBlackjack();
        manager.setGame(std::move(blackjack));
        manager.startRound(); //gives the game to the object to be managed
        qDebug() << "Creating blackjack window";
        blackjackWindow = new BlackjackWindow(
            manager,
            this
            );
        blackjackWindow->setWindowFlag(Qt::Window, true);
        connect(
            blackjackWindow,
            &GameWindow::backRequested,
            this,
            [this]() {
                qDebug() << "Returning to main menu";

                blackjackWindow->hide();

                this->show();
                this->raise();
                this->activateWindow();
            }
            );
        connect(
            blackjackWindow,
            &QObject::destroyed,
            this,
            [this]() {
                blackjackWindow = nullptr;
            }
            );
    }
    blackjackWindow->show();
    blackjackWindow->raise();
    blackjackWindow->activateWindow();
    this->hide();
}
void MainWindow::openPoker()
{
    qDebug() << "Play Poker button clicked!";
    if (!pokerWindow) { // creates Poker windo w if it doesn't exist already
        qDebug() << "Creating Poker game";
        auto poker = GameFactory::createPoker(); //create the poker game
        manager.setGame(std::move(poker));
        manager.startRound();
        pokerWindow = new PokerWindow(manager, this);


        pokerWindow->setWindowFlag(Qt::Window, true); // Make PokerWindow a separate window
        connect(
            pokerWindow,
            &GameWindow::backRequested,
            this,
            [this]() {
                qDebug() << "Returning to main menu";

                pokerWindow->close();
                pokerWindow->deleteLater();
                pokerWindow = nullptr;

                manager.clearGame();

                this->show();
                this->raise();
                this->activateWindow();
            }
            );
        connect(
            pokerWindow,
            &QObject::destroyed,
            this,
            [this]() {
                pokerWindow = nullptr;
            }
            );
    }
    pokerWindow->show();
    pokerWindow->raise();
    pokerWindow->activateWindow();
    this->hide();
}

void MainWindow::openBaccarat()
{
    if (!baccaratWindow) {
        baccaratWindow = new BaccaratWindow(this);
        baccaratWindow->setWindowFlags(Qt::Window);

        connect(baccaratWindow, &BaccaratWindow::backRequested,
                this, [this]() {
                    baccaratWindow->hide();

                    this->show();
                    this->raise();
                    this->activateWindow();
                });
    }

    baccaratWindow->show();
    baccaratWindow->raise();
    baccaratWindow->activateWindow();

    this->hide();
}


MainWindow::~MainWindow()
{
    delete ui;
}
