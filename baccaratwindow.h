
#ifndef BACCARATWINDOW_H
#define BACCARATWINDOW_H

#include <QDialog>
#include <memory>
#include "core/Baccarat.h"

namespace Ui {
class Dialog;
}
class QHBoxLayout;
class BaccaratWindow : public QDialog
{
    Q_OBJECT

public:
    explicit BaccaratWindow(QWidget *parent = nullptr);
    ~BaccaratWindow();

signals:
    void backRequested();

private slots:
    void onDeal();

private:
    Ui::Dialog *ui;
    std::unique_ptr<Baccarat> game;
    QHBoxLayout* playerCardsLayout = nullptr;
    QHBoxLayout* bankerCardsLayout = nullptr;
};

#endif
