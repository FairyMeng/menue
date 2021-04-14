#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGuiApplication>
#include <QMenu>
#include <QWidget>
#include <QProxyStyle>
#include <QFileInfo>
#include "basewindow.h"

#include "mywindow.h"
#include "sequencefrequency.h"
#include "radarperform.h"
#include "datafit.h"
#include "piontlinemeasuring.h"
#include "shuzhijisuan1.h"
#include "xvliepingpufenxi.h"
#include "shujuchuli.h"
#include "maichongcunchu.h"
#include "maijianjujingce.h"
#include "prijingce.h"
#include "guolvshezhi.h"
#include "caogaoben.h"
#include "imcontrast.h"
#include "matchsearch.h"
#include "impulse_clustering.h"
#include "view.h"
#include "internalpulseview.h"
#include "pulsewindow.h"
#include "internalpulsewindow.h"
#include "impulse_second_form.h"


#include "internalpulsewindow.h"

#include "pulsefilemanager.h"
#include "pulsegraphdisplayer.h"

#include "signalworker.h"
#include "slotworker.h"
#include "pulsemapper.h"

namespace Ui {
class MainWindow;
}

class MainWindow :public BaseWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QMenu  menu_;
    PulseWindow imp_s;//脉间窗口
    InternalPulseWindow interWindow;//脉内窗口
    impulse_Clustering imp_Clu;//脉冲聚类列表
    impulse_second_form imp_form;//脉冲次窗列表
    //设置子窗口并显示
    void SetAllWindowsSizeAndShow();
private:
    Ui::MainWindow *ui;
    mywindow *mywin;
    SequenceFrequency * sequence;
    RadarPerform * radarperform;
    Datafit * datafit;
    PiontLineMeasuring * pointline;
    shuzhijisuan1 *shuzhi1;
    xvliepingpufenxi *pingpufenxi;
    shujuchuli *datahanding;
    maichongcunchu *pulse_storage;
    maijianjujingce *pulse_spacing_measurement;
    PRIjingce *PRI_measurement;
    guolvshezhi *guolv;
    caogaoben *caogao;
    Imcontrast * imcon;
    matchsearch *match;

    QVector<InterPulse> million_data_vector;//用于存储百万结构体数组,第一次给定的数据
    QVector<InterPulse> AverageVector;//用于存放求平均之后的结构体数据
    QVector<int> internalVector;//存放脉内数据

    QRect clientRect;//用于窗户拼接显示
    //判断窗口是否关闭
    bool pulse_window_flag;
    bool internal_pulse_window_flag;
    bool impulse_clustering_flag;

    //双击tab隐藏工具栏
    bool tab_tool_hide_flag;

    //脉冲返回次窗标签
    bool imp_form_show_flag;

    QTimer *timerForLoadInternalPulseFile;

    bool InterPulseFileIsLoadedFinish;//判断脉间文件是否被加载完成

    //存放4个窗口的堆叠层次
    QObject* windowStackedLayer[4];

    void initTitleBar();
signals:
    void openFile(QFileInfo fileInfo);//加载文件
    void openInternalPulseFile();//加载脉内文件
    void AllWindowActivate(QObject *obj);
protected:
    void changeEvent(QEvent *event);

private slots:
    void openwindow_childSlots();//打开文件载入窗口
    void openSequenceFrequency();//打开同行序列载频精测窗口
    void openRadarPerformSlots();//打开雷达性能估计窗口
    void openDataFitting();//打开数据整合窗口
    void openPointLineMeasuring();//打开测量点线界面
    void openshuzhijisuan1();//打开数值计算界面
    void openxvliepingpufenxiSlots();//打开序列频谱分析界面
    void openshujuchuliSlots();//打开数据处理
    void openmaichongcunchuSlots();//脉冲存储
    void openmaijianjujingceSlots();//脉间距精测
    void openprijingceSlots();//序列固定PRI精测
    void openguolvshezhiSlots();//过滤设置
    void openCaoGaoBen();//打开草稿本
    void on_toolButton_PulseWindow_clicked();
    void openImpluseContrast();//打开脉冲对比
    void openMatchSearch();//打开匹配搜索
    void openFileSlots(QFileInfo fileInfo);
    void on_toolButton_InternalPulseWindow_clicked();//点击脉间窗口按钮
    void on_toolButton_Impulse_Clustering_clicked();//点击脉冲聚类窗口按钮

    void PulseWindow_closeSlots();//关闭脉间窗口
    void InternalPulseWindow_closeSlots();//关闭脉内窗口
    void impulse_Clustering_closeSlots();//关闭脉冲聚类窗口

    void mywindow_CloseSlot();//载入文件窗口关闭
    void spinBox_setValue(int value);
    void pushButton_QuickLoadFileSlot();//从mainwindow界面快速载入文件

    void chart_data_reset();//用于还原chart中的数据
    void openInternalPulseFileSlot();//加载脉内文件

    void on_tabWidget_tabBarDoubleClicked(int index);

	void on_toolButton_cfxc_clicked();

    //脉间窗口全屏、退出全屏
    void PulseWindowFullScreenSlot();
    void PulseWindowQuitFullScreenSlot();
    //脉内窗口全屏、退出
    void InternalPulseWindowFullScreenSlot();//脉内窗口全屏
    void InternalPulseWindowQuitFullScreenSlot();//脉内窗口退出全屏

    //当一个窗口激活时，所有窗口提升到堆叠层次最上面
    void AllWindowRaiseSlot(QObject *obj);
    //移动窗口到默认位置时自动设置窗口位置对齐
    void AutoSetWindowPosSlot(QObject *obj);
protected:
    bool eventFilter(QObject *watched, QEvent *event);
};

#endif // MAINWINDOW_H
