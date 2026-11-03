
#include "GameWindow.h"
#include "QtCardAnimation.h"

#include <QVBoxLayout>
#include <QLayoutItem>
#include <QPointer>
#include <QTimer>
#include <QRect>
#include <QDebug>
#include <utility>

GameWindow::GameWindow(QWidget* parent)
    : QWidget(parent),
    actionLabel(new QLabel(this)),
    resultLabel(new QLabel(this)),
    animation(std::make_unique<QtCardAnimation>())
{
    actionLabel->setAlignment(Qt::AlignCenter);
    resultLabel->setAlignment(Qt::AlignCenter);

    actionLabel->setText("Welcome!");
    resultLabel->clear();
}

void GameWindow::setAnimation(
    std::unique_ptr<IGameAnimation> newAnimation)
{
    if (animationRunning || !newAnimation)
        return;

    animation = std::move(newAnimation);
}

void GameWindow::showAction(
    GameAction action,
    const QString& actor)
{
    showMessage(actionMessage(action, actor));
}

void GameWindow::showMessage(
    const QString& message)
{
    actionLabel->setText(message);
}

void GameWindow::showResult(
    const QString& result)
{
    resultLabel->setText(result);
}

bool GameWindow::isAnimationRunning() const
{
    return animationRunning;
}

void GameWindow::dealCardAnimated(
    const Card& card,
    QLayout* targetLayout,
    const QPoint& deckPosition,
    bool faceUp,
    std::function<void()> onFinished)
{
    if (!targetLayout || !animation ||
        animationRunning)
        return;

    QWidget* container = targetLayout->parentWidget();

    if (!container)
        return;

    // Create a card on the same coordinate system
    // as the destination hand.
    auto* cardWidget = new CardWidget(card, container);
    cardWidget->setFaceUp(faceUp);

    auto* placeholder = new QWidget(container);
    placeholder->setFixedSize(cardWidget->size());
    targetLayout->addWidget(placeholder);

    container->layout()->activate();

    QPoint endPosition =
        placeholder->mapTo(container, QPoint(0, 0));

    cardWidget->move(deckPosition);
    cardWidget->show();
    cardWidget->raise();

    animationRunning = true;

    QPointer<GameWindow> safeWindow(this);
    QPointer<QWidget> safePlaceholder(placeholder);
    QPointer<CardWidget> safeCard(cardWidget);

    animation->animateDeal(
        cardWidget,
        deckPosition,
        endPosition,
        [safeWindow, safePlaceholder, safeCard,
         targetLayout,
         onFinished = std::move(onFinished)]() mutable
        {
            if (!safeWindow || !safeCard ||
                !safePlaceholder)
                return;

            int index =
                targetLayout->indexOf(safePlaceholder);

            if (index < 0) {
                safeWindow->animationRunning = false;
                return;
            }

            targetLayout->removeWidget(safePlaceholder);
            safePlaceholder->deleteLater();

            targetLayout->insertWidget(index, safeCard);

            safeWindow->animationRunning = false;

            if (onFinished)
                onFinished();
        }
        );
}

void GameWindow::clearCards(QLayout* layout)
{
    if (!layout || animationRunning)
        return;

    while (QLayoutItem* item = layout->takeAt(0)) {
        if (QWidget* widget = item->widget())
            delete widget;

        delete item;
    }
}
++