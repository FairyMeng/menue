#include "message_cancel.h"
#include "ui_message_cancel.h"

Message_cancel::Message_cancel(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Message_cancel)
{
    ui->setupUi(this);
    QObject::connect(ui->pushButton_enter,SIGNAL(clicked()),this,SLOT(pushButton_enterSlot()));
    QObject::connect(ui->pushButton_enter,SIGNAL(clicked()),this,SLOT(pushButton_cancelSlot()));
}

Message_cancel::~Message_cancel()
{
    delete ui;
}
//取消按钮
void Message_cancel::pushButton_cancelSlot(){
    this->close();
}
//确定按钮
void Message_cancel::pushButton_enterSlot(){
    isClicked=true;
    this->close();
    isClose=true;
}
