#include "maichongcunchu.h"
#include "ui_maichongcunchu.h"

maichongcunchu::maichongcunchu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::maichongcunchu)
{
    ui->setupUi(this);
}

maichongcunchu::~maichongcunchu()
{
    delete ui;
}
