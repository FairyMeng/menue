#include "internalpulsewindow.h"
#include "ui_internalpulsewindow.h"

InternalPulseWindow::InternalPulseWindow(QWidget *parent) :
    BaseWindow(parent),
    ui(new Ui::InternalPulseWindow)
{
    ui->setupUi(this);
    // 初始化标题栏;
    initTitleBar();
    setSupportStretch(true);
    clientRect = QApplication::desktop()->availableGeometry(); //用户可用窗口大小

    InternalPulseWindow::installEventFilter(this);//为这个窗口安装过滤器

    //设置qss样式
        {
        QFile file(":/qss/impulse.css");
        file.open(QFile::ReadOnly);
        QTextStream filetext(&file);
        QString stylesheet = filetext.readAll();
        this->setStyleSheet(stylesheet);
        file.close();
        }
    ui->verticalLayout->addWidget(&internalView);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


}
void InternalPulseWindow::initTitleBar()
{
    // 设置标题栏跑马灯效果，可以不设置;
    m_titleBar->setTitleRoll();
    m_titleBar->setTitleIcon(":/new/icon/images/app_icon.png");
    //m_titleBar->setTitleContent(QStringLiteral("春水初生，春林初盛,春风十里，不如你。春水初生，春林初盛,春风十里，不如你。"));
    m_titleBar->setButtonType(MIN_MAX_BUTTON);
    m_titleBar->setTitleWidth(this->width());

}

void InternalPulseWindow::closeEvent(QCloseEvent *event)
{
    emit InternalPulseWindow_close();
}

bool InternalPulseWindow::eventFilter(QObject *watched, QEvent *event){
    //当窗口状态变为激活状态时，发射信号，槽函数将所有窗口提升到堆叠层次最上面
    if(watched == this){
        //过滤窗口激活事件
        if(event->type() == QEvent::WindowActivate){
            qDebug()<<"InterWindow activate";
            emit AllWindowActivate(this);
        }
        //过滤窗口的鼠标松开事件
        if(QEvent::MouseButtonRelease == event->type()){
            emit MouseRelease(this);
        }
    }
    return BaseWindow::eventFilter(watched, event);
}

InternalPulseWindow::~InternalPulseWindow()
{
    delete ui;
}
