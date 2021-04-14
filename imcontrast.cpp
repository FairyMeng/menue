#include "imcontrast.h"
#include "ui_imcontrast.h"

Imcontrast::Imcontrast(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Imcontrast)
{
    ui->setupUi(this);
}

Imcontrast::~Imcontrast()
{
    delete ui;
}
