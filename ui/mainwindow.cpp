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



void MainWindow::openBlackjack()
{
    qDebug() << "Play Blackjack button clicked!";

    // Only create the game and window if they don't exist
    if (!blackjackWindow) {

        qDebug() << "Creating blackjack game";

        // Step 1: Factory creates the Blackjack object
        auto blackjack = GameFactory::createBlackjack();

        // Step 2: Transfer ownership to GameManager
        manager.setGame(std::move(blackjack));

        // Step 3: GameManager starts the Blackjack round
        manager.startRound();

        qDebug() << "Creating blackjack window";

        // Step 4: Pass GameManager to BlackjackWindow
        blackjackWindow = new BlackjackWindow(
            manager,
            this
            );
        // Make BlackjackWindow a separate window
        blackjackWindow->setWindowFlag(Qt::Window, true);
        // Step 5: Handle returning to the main menu
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

        // Step 6: Reset pointer if window is destroyed
        connect(
            blackjackWindow,
            &QObject::destroyed,
            this,
            [this]() {
                blackjackWindow = nullptr;
            }
            );
    }

    // Step 7: Show Blackjack window
    blackjackWindow->show();
    blackjackWindow->raise();
    blackjackWindow->activateWindow();

    // Step 8: Hide main menu
    this->hide();
}



void MainWindow::openPoker()
{
    qDebug() << "Play Poker button clicked!";

    // Only create the game and window if they don't exist
    if (!pokerWindow) {

        qDebug() << "Creating Poker game";

        // Step 1: GameFactory creates the Poker object
        auto poker = GameFactory::createPoker();

        // Step 2: GameManager takes ownership
        manager.setGame(std::move(poker));

        // Step 3: Start the Poker round
        // Only if the Poker constructor hasn't started it
        manager.startRound();

        // Step 4: Create PokerWindow using GameManager
        pokerWindow = new PokerWindow(manager, this);

        // Make PokerWindow a separate window
        pokerWindow->setWindowFlag(Qt::Window, true);

        // Step 5: Handle returning to main menu
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

        // Step 6: Reset pointer if window is destroyed
        connect(
            pokerWindow,
            &QObject::destroyed,
            this,
            [this]() {
                pokerWindow = nullptr;
            }
            );
    }

    // Step 7: Display PokerWindow
    pokerWindow->show();
    pokerWindow->raise();
    pokerWindow->activateWindow();

    // Step 8: Hide MainWindow
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
