#include "maijianjujingce.h"
#include "ui_maijianjujingce.h"

maijianjujingce::maijianjujingce(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::maijianjujingce)
{
    ui->setupUi(this);
}

maijianjujingce::~maijianjujingce()
{
    delete ui;
}
