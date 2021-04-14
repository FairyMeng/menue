#include "impulse_second_form.h"
#include "ui_impulse_second_form.h"

impulse_second_form::impulse_second_form(QWidget *parent) :
    BaseWindow(parent),
    ui(new Ui::impulse_second_form)
{
    ui->setupUi(this);
    //设置qss样式
        {
        QFile file(":/qss/impulse_second_form.css");
        file.open(QFile::ReadOnly);
        QTextStream filetext(&file);
        QString stylesheet = filetext.readAll();
        this->setStyleSheet(stylesheet);
        file.close();
        }

        // 初始化标题栏;
        initTitleBar();
        setSupportStretch(true);
        //脉冲聚类列表布局
        ui->tableWidget_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->tableWidget_4->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

impulse_second_form::~impulse_second_form()
{
    delete ui;
}
void impulse_second_form::initTitleBar()
{
    // 设置标题栏跑马灯效果，可以不设置;
    m_titleBar->setTitleRoll();

    m_titleBar->setTitleIcon(":/new/icon/images/app_icon.jpg");
    m_titleBar->setTitleContent(QString("  脉冲次窗"));
    m_titleBar->setButtonType(MIN_MAX_BUTTON);
    m_titleBar->setTitleWidth(this->width());
}
