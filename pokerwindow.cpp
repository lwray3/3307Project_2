#include "pokerwindow.h"
#include "ui_pokerwindow.h"

#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>

PokerWindow::PokerWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::PokerWindow) {
    ui->setupUi(this);
    setWindowTitle("Texas Hold'em Poker");
    
    connect(ui->FoldButton, &QPushButton::clicked, this, &PokerWindow::onFold);
    connect(ui->CheckButton, &QPushButton::clicked, this, &PokerWindow::onCheckCall);
    connect(ui->RaiseButton, &QPushButton::clicked, this, &PokerWindow::onRaise);
    connect(ui->RulesButton, &QPushButton::clicked, this, &PokerWindow::onShowRules);

    connect(ui->Back, &QPushButton::clicked, this, &PokerWindow::backRequested);

    // Poker::play() currently reads from std::cin and runs a whole match.
    // Do NOT call it from a Qt button: it would block the UI event loop.
    setBettingEnabled(false);
}

PokerWindow::~PokerWindow() { delete ui; }

void PokerWindow::setBettingEnabled(bool enabled) {
    ui->FoldButton->setEnabled(enabled);
    ui->CheckButton->setEnabled(enabled);
    ui->RaiseButton->setEnabled(enabled);
    ui->RaiseSpinBox->setEnabled(enabled);
}

void PokerWindow::onStartHand() {
    emit startHandRequested();
    // Once the backend starts a hand, update labels/cards and enable legal actions.
}

void PokerWindow::onFold() { emit foldRequested(); }
void PokerWindow::onCheckCall() { emit checkCallRequested(); }
void PokerWindow::onRaise() { emit raiseRequested(ui->RaiseSpinBox->value()); }

void PokerWindow::onShowRules() {
    QMessageBox::information(this, "Texas Hold'em Rules",
        "Heads-up Texas Hold'em: you versus the computer dealer.\n\n"
        "Each player receives two private cards. Five community cards are "
        "revealed over the flop (3), turn (1), and river (1).\n\n"
        "Choose fold, check/call, or raise when it is your turn. "
        "Make the strongest five-card hand from your two cards and the board.\n\n"
        "Hand rankings (strongest first): straight flush, four of a kind, "
        "full house, flush, straight, three of a kind, two pair, pair, high card.\n\n"
        "Starting stacks: 1000 chips each. Blinds: 10 / 20.");
}
