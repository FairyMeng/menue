#include "shujuchuli.h"
#include "ui_shujuchuli.h"

shujuchuli::shujuchuli(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::shujuchuli)
{
    ui->setupUi(this);
}

shujuchuli::~shujuchuli()
{
    delete ui;
}
