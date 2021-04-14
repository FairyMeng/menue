#include "datafit.h"
#include "ui_datafit.h"

Datafit::Datafit(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Datafit)
{
    ui->setupUi(this);
}

Datafit::~Datafit()
{
    delete ui;
}
