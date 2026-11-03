
#include "baccaratwindow.h"
#include "ui_baccaratwindow.h"

BaccaratWindow::BaccaratWindow(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

BaccaratWindow::~BaccaratWindow()
{
    delete ui;
}
