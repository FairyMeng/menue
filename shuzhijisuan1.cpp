#include "shuzhijisuan1.h"
#include "ui_shuzhijisuan1.h"

shuzhijisuan1::shuzhijisuan1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::shuzhijisuan1)
{
    ui->setupUi(this);
}

shuzhijisuan1::~shuzhijisuan1()
{
    delete ui;
}
