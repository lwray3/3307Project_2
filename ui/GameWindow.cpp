#include "QtCardAnimation.h"
#include "GameWindow.h"
#include <QDebug>
#include <utility>

GameWindow::GameWindow(QWidget* parent)
    : QWidget(parent),
    cardAnimation(std::make_unique<QtCardAnimation>())
{
}

void GameWindow::setActionLabel(QLabel* label)
{
    actionLabel = label;
}

void GameWindow::showActionMessage(const QString& message)
{
    if (actionLabel) {
        actionLabel->setText(message);
    }

    qDebug() << message;
}

void GameWindow::showResultMessage(const QString& message)
{
    showActionMessage(message);
}

void GameWindow::animateDeal(
    QWidget* card,
    const QPoint& start,
    const QPoint& end,
    std::function<void()> onFinished)
{
    cardAnimation->animateDeal(
        card,
        start,
        end,
        std::move(onFinished)
        );
}

void GameWindow::animateFlip(
    QWidget* card,
    std::function<void()> onFinished)
{
    cardAnimation->animateFlip(
        card,
        std::move(onFinished)
        );
}