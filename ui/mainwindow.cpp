#include "mainwindow.h"
#include "core/GameFactory.h"
#include "blackjackwindow.h"
#include "pokerwindow.h"
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
}
void MainWindow::openBlackjack()
{
    qDebug() << "Play Blackjack button clicked!";
    if (!blackjackWindow) {
        qDebug() << "Creating blackjack game";
        auto blackjack = GameFactory::createBlackjack();
        manager.setGame(std::move(blackjack));
        manager.startRound();
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
    if (!pokerWindow) {
        qDebug() << "Creating Poker game";
        auto poker = GameFactory::createPoker();
        manager.setGame(std::move(poker));
        manager.startRound();
        pokerWindow = new PokerWindow(manager, this);

        // Make PokerWindow a separate window
        pokerWindow->setWindowFlag(Qt::Window, true);
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

MainWindow::~MainWindow()
{
    delete ui;
}