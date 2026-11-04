#include "mainwindow.h"
#include "backend/GameFactory.h"
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

void MainWindow::openPoker()
{
    // Print a message to confirm the button works
    qDebug() << "Play Poker button clicked!";

    // Create the Blackjack window if necessary
    if (!pokerWindow) {
        pokerWindow = new PokerWindow(this);
        pokerWindow->setWindowFlags(Qt::Window);

        connect(pokerWindow, &PokerWindow::backRequested, this, [this]() {
            pokerWindow->hide();
            this->show();
        });
    }

    pokerWindow->show();
    pokerWindow->raise();
    pokerWindow->activateWindow();

    // Hide the main menu
    this->hide();
}

void MainWindow::openBlackjack()
{
    // Print a message to confirm the button works
    qDebug() << "Play Blackjack button clicked!";

    // Create the Blackjack window if necessary
    if (!blackjackWindow) {
        qDebug() << "Creating blackjack game";
        auto game = GameFactory::createBlackjack();
        qDebug() << "Creating blackjack window";
        blackjackWindow = new BlackjackWindow(std::move(game), this);
        blackjackWindow->setWindowFlag(Qt::Window, true);
        connect(blackjackWindow, &GameWindow::backRequested, this, [this]() {
            qDebug() << "Returning to main menu";

            // Hide Blackjack window
            blackjackWindow->hide();

            // Show main menu
            this->show();
            this->raise();
            this->activateWindow();
        });

        // Reset pointer if Blackjack window is destroyed
        connect(blackjackWindow, &QObject::destroyed, this, [this]() { blackjackWindow = nullptr; });
    }

    // Show Blackjack
    blackjackWindow->show();
    blackjackWindow->raise();
    blackjackWindow->activateWindow();

    // Hide the main menu
    this->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}