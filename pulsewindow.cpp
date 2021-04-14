#include "pulsewindow.h"

PulseWindow::PulseWindow(QWidget *parent) :
    BaseWindow(parent),
    ui(new Ui::PulseWindow)
{
    ui->setupUi(this);
    // 初始化标题栏;
    initTitleBar();
    setSupportStretch(true);
    clientRect = QApplication::desktop()->availableGeometry(); //用户可用窗口大小
    //设置qss样式
        {
        QFile file(":/qss/impulse_separation.css");
        file.open(QFile::ReadOnly);
        QTextStream filetext(&file);
        QString stylesheet = filetext.readAll();
        this->setStyleSheet(stylesheet);
        file.close();
        }

    ui->verticalLayout->addWidget(&view_);

    installEventFilter(this);
}

void PulseWindow::initTitleBar()
{
    // 设置标题栏跑马灯效果，可以不设置;
    m_titleBar->setTitleRoll();
    m_titleBar->setTitleIcon(":/new/icon/images/app_icon.png");
    //m_titleBar->setTitleContent(QStringLiteral("春水初生，春林初盛,春风十里，不如你。春水初生，春林初盛,春风十里，不如你。"));
    m_titleBar->setButtonType(MIN_MAX_BUTTON);
    m_titleBar->setTitleWidth(this->width());

}
void PulseWindow::closeEvent(QCloseEvent *event)
{
    emit PulseWindow_close();
}

bool PulseWindow::eventFilter(QObject *watched, QEvent *event){
    //当窗口状态变为激活状态时，发射信号，槽函数将所有窗口提升到堆叠层次最上面
    if(watched == this){
        //过滤窗口激活事件
        if(event->type() == QEvent::WindowActivate){
            qDebug()<<"PulseWindow activate";
            emit AllWindowActivate(this);
        }

        //过滤窗口的鼠标松开事件
        if(QEvent::MouseButtonRelease == event->type()){
            emit MouseRelease(this);
        }
    }
    return BaseWindow::eventFilter(watched, event);
}

PulseWindow::~PulseWindow()
{
    //delete ui;
}
