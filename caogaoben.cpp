#include "caogaoben.h"
#include "ui_caogaoben.h"

caogaoben::caogaoben(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::caogaoben)
{
    ui->setupUi(this);
}

caogaoben::~caogaoben()
{
    delete ui;
}
