#include "piontlinemeasuring.h"
#include "ui_piontlinemeasuring.h"

PiontLineMeasuring::PiontLineMeasuring(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PiontLineMeasuring)
{
    ui->setupUi(this);
}

PiontLineMeasuring::~PiontLineMeasuring()
{
    delete ui;
}
