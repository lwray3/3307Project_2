
#include "QtCardAnimation.h"
#include "CardWidget.h"

#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QSequentialAnimationGroup>
#include <QPauseAnimation>
#include <QObject>
#include <QPointer>
#include <QAbstractAnimation>
#include <utility>

void QtCardAnimation::animateDeal(
    QWidget* card,
    const QPoint& start,
    const QPoint& end,
    std::function<void()> onFinished)
{
    if (!card) return;

    card->move(start);
    card->show();
    card->raise();

    auto* animation =
        new QPropertyAnimation(card, "pos", card);

    animation->setDuration(350);
    animation->setStartValue(start);
    animation->setEndValue(end);
    animation->setEasingCurve(QEasingCurve::OutCubic);

    QPointer<QWidget> safeCard(card);

    QObject::connect(
        animation,
        &QPropertyAnimation::finished,
        card,
        [safeCard, onFinished = std::move(onFinished)]() {
            if (safeCard && onFinished)
                onFinished();
        }
        );

    animation->start(
        QAbstractAnimation::DeleteWhenStopped
        );
}

void QtCardAnimation::animateFlip(
    QWidget* card,
    std::function<void()> onFinished)
{
    auto* cardWidget = qobject_cast<CardWidget*>(card);
    if (!cardWidget) return;

    auto* group =
        new QSequentialAnimationGroup(cardWidget);

    auto* shrink =
        new QPropertyAnimation(cardWidget, "maximumWidth");

    shrink->setDuration(120);
    shrink->setStartValue(cardWidget->width());
    shrink->setEndValue(1);

    group->addAnimation(shrink);

    auto* pause = new QPauseAnimation(1);
    group->addAnimation(pause);

    auto* expand =
        new QPropertyAnimation(cardWidget, "maximumWidth");

    expand->setDuration(120);
    expand->setStartValue(1);
    expand->setEndValue(cardWidget->width());

    group->addAnimation(expand);

    QPointer<CardWidget> safeCard(cardWidget);

    QObject::connect(
        pause,
        &QPauseAnimation::finished,
        cardWidget,
        [safeCard]() {
            if (safeCard)
                safeCard->setFaceUp(true);
        }
        );

    QObject::connect(
        group,
        &QSequentialAnimationGroup::finished,
        cardWidget,
        [safeCard, onFinished = std::move(onFinished)]() {
            if (safeCard)
                safeCard->setMaximumWidth(QWIDGETSIZE_MAX);

            if (safeCard && onFinished)
                onFinished();
        }
        );

    group->start(
        QAbstractAnimation::DeleteWhenStopped
        );
}
