#include "radarperform.h"
#include "ui_radarperform.h"

RadarPerform::RadarPerform(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RadarPerform)
{
    ui->setupUi(this);
}

RadarPerform::~RadarPerform()
{
    delete ui;
}
