#include "guolvshezhi.h"
#include "ui_guolvshezhi.h"

guolvshezhi::guolvshezhi(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::guolvshezhi)
{
    ui->setupUi(this);
    guolv2=new guolvshezhi2();
    QObject::connect(ui->radioButton_guolv,SIGNAL(clicked()),this,SLOT(openguolvshezhi2Slots()));
   // mywin=new mywindow();
   //QObject::connect(ui->pushButton_loading,SIGNAL(clicked()),this,SLOT(openwindow_childSlots()));
}

guolvshezhi::~guolvshezhi()
{
    delete ui;
}
void guolvshezhi::openguolvshezhi2Slots(){
    guolv2->show();
}
