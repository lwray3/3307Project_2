
#ifndef BACCARATWINDOW_H
#define BACCARATWINDOW_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class BaccaratWindow : public QDialog
{
    Q_OBJECT

public:
    explicit BaccaratWindow(QWidget *parent = nullptr);
    ~BaccaratWindow();

private:
    Ui::Dialog *ui;
};

#endif
