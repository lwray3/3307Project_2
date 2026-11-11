#ifndef QTCARDANIMATION_H
#define QTCARDANIMATION_H
#include "IGameAnimation.h"

class QtCardAnimation : public IGameAnimation {
public:
    void animateDeal(
        QWidget* card,
        const QPoint& start,
        const QPoint& end,
        std::function<void()> onFinished = {}
        ) override;

    void animateFlip(
        QWidget* card,
        std::function<void()> onFinished = {}
        ) override;
};

#endif
