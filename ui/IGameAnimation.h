#ifndef IGAMEANIMATION_H
#define IGAMEANIMATION_H

#include <QPoint>
#include <QWidget>
#include <functional>

class IGameAnimation {
public:
    virtual ~IGameAnimation() = default;

    virtual void animateDeal(
        QWidget* card,
        const QPoint& start,
        const QPoint& end,
        std::function<void()> onFinished = {}
        ) = 0;

    virtual void animateFlip(
        QWidget* card,
        std::function<void()> onFinished = {}
        ) = 0;
};

#endif
