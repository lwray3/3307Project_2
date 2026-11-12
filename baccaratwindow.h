
#ifndef BACCARATWINDOW_H
#define BACCARATWINDOW_H

#include <memory>
#include "ui/GameWindow.h"
#include "core/Baccarat.h"

namespace Ui {
class BaccaratForm;
}

class QHBoxLayout;

class BaccaratWindow : public GameWindow
{
    Q_OBJECT

public:
    explicit BaccaratWindow(QWidget *parent = nullptr);
    ~BaccaratWindow();

private slots:
    void onDeal();

private:
    Ui::BaccaratForm *ui;
    std::unique_ptr<Baccarat> game;
    QHBoxLayout* playerCardsLayout = nullptr;
    QHBoxLayout* bankerCardsLayout = nullptr;
};

#endif
