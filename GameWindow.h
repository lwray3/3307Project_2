
#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QString>
#include "IGameAnimation.h"

class GameWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget* parent = nullptr);
    ~GameWindow() override = default;

    void setActionLabel(QLabel* label);
    void animateFlip(QWidget* card, std::function<void()> onFinished);
    void animateDeal(QWidget* card,
        const QPoint& start,
        const QPoint& end,
        std::function<void()> onFinished);

    void showActionMessage(const QString& message);
    void showResultMessage(const QString& message);

signals:
    void backRequested();

private:
    QLabel* actionLabel = nullptr;
    std::unique_ptr<IGameAnimation> cardAnimation;

};

#endif
