#include "QtCardAnimation.h"
#include "core/CardWidget.h"
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
    if (!card) {
        if (onFinished) {
            onFinished();
        }
        return;
    }

    card->move(start);
    card->show();
    card->raise();

    auto* animation = new QPropertyAnimation(card, "pos", card);

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
            if (safeCard && onFinished) {
                onFinished();
            }
        }
        );

    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void QtCardAnimation::animateFlip(
    QWidget* card,
    std::function<void()> onFinished)
{
auto* cardWidget = dynamic_cast<CardWidget*>(card);

    if (!cardWidget) {
        if (onFinished) onFinished();
        return;
    }

    const QRect original = cardWidget->geometry();

    QRect collapsed = original;
    collapsed.setLeft(original.center().x());
    collapsed.setRight(original.center().x());

    auto* group = new QSequentialAnimationGroup(cardWidget);

    auto* shrink = new QPropertyAnimation(cardWidget, "geometry");
    shrink->setDuration(120);
    shrink->setStartValue(original);
    shrink->setEndValue(collapsed);

    auto* expand = new QPropertyAnimation(cardWidget, "geometry");
    expand->setDuration(120);
    expand->setStartValue(collapsed);
    expand->setEndValue(original);

    group->addAnimation(shrink);

    auto* pause = new QPauseAnimation(1);
    group->addAnimation(pause);

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
        [safeCard, original, onFinished = std::move(onFinished)]() {
            if (safeCard)
                safeCard->setGeometry(original);

            if (onFinished)
                onFinished();
        }
        );

    group->start(QAbstractAnimation::DeleteWhenStopped);
}
