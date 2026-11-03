
#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QLayout>
#include <QString>
#include <QPoint>
#include <QPointer>

#include <memory>
#include <functional>

#include "Card.h"
#include "CardWidget.h"
#include "IGameAnimation.h"
#include "GameAction.h"

class GameWindow : public QWidget {
    Q_OBJECT

public:
    explicit GameWindow(QWidget* parent = nullptr);
    ~GameWindow() override = default;

    void setAnimation(
        std::unique_ptr<IGameAnimation> animation
        );

    void showAction(
        GameAction action,
        const QString& actor
        );

    void showMessage(const QString& message);
    void showResult(const QString& result);
    void showActionMessage(const QString& message);
    void showResultMessage(const QString& message);

    void dealCardAnimated(
        const Card& card,
        QLayout* targetLayout,
        const QPoint& deckPosition,
        bool faceUp = true,
        std::function<void()> onFinished = {}
        );

    void clearCards(QLayout* layout);

    bool isAnimationRunning() const;

signals:
    void backRequested();

protected:
    QLabel* actionLabel;
    QLabel* resultLabel;

    std::unique_ptr<IGameAnimation> animation;

    bool animationRunning = false;
};

#endif
