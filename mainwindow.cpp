
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "blackjackwindow.h"
#include "pokerwindow.h"

#include <QPushButton>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connect the button to the function
    connect(
        ui->playBlackjack,
        &QPushButton::clicked,
        this,
        &MainWindow::openBlackjack
    );

    connect(
        ui->playPoker,
        &QPushButton::clicked,
        this,
        &MainWindow::openPoker
    );
    
}

void MainWindow::openPoker()
{
    // Print a message to confirm the button works
    qDebug() << "Play Poker button clicked!";

    // Create the Blackjack window if necessary
    if (!pokerWindow) {
        pokerWindow = new PokerWindow(this);
        pokerWindow->setWindowFlags(Qt::Window);
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
        blackjackWindow = new BlackjackWindow(this);
        blackjackWindow->setWindowFlags(Qt::Window);
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
