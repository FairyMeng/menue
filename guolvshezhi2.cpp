#include "guolvshezhi2.h"
#include "ui_guolvshezhi2.h"

guolvshezhi2::guolvshezhi2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::guolvshezhi2)
{
    ui->setupUi(this);
}

guolvshezhi2::~guolvshezhi2()
{
    delete ui;
}
