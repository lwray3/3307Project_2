#include "blackjackwindow.h"
#include "ui_blackjackwindow.h" // CORRECT

BlackjackWindow::BlackjackWindow(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::BlackjackWindow)
{
    ui->setupUi(this);

    connect(ui->Back_Button, &QPushButton::clicked,
        this, &BlackjackWindow::backRequested);

    connect(ui->HitButton, &QPushButton::clicked,
        this, &BlackjackWindow::onHit);
    
}


void BlackjackWindow::onHit()
{

    game.hit();

    Player player = game.getPlayer();
    const auto& hand = player.getHand();

    int score = game.calcScore(hand);

    qDebug() << "Player received another card";
    qDebug() << "Number of cards:" << hand.size();
    qDebug() << "Player score:" << score;

    if (score > 21) {
        qDebug() << "Player busted!";
        ui->HitButton->setEnabled(false);
    }
}


BlackjackWindow::~BlackjackWindow()
{
    delete ui;
}
