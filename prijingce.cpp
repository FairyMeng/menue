#include "prijingce.h"
#include "ui_prijingce.h"

PRIjingce::PRIjingce(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PRIjingce)
{
    ui->setupUi(this);
}

PRIjingce::~PRIjingce()
{
    delete ui;
}
