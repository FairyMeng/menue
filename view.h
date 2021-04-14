#ifndef VIEW_H
#define VIEW_H


#include <QWidget>
#include <QProxyStyle>
#include <QChartView>
#include <QGraphicsView>
#include <QScatterSeries>
#include <QPointF>
#include <QKeyEvent>
#include <QDesktopWidget>
#include <QApplication>
#include <QTime>
#include <QDebug>
#include <QList>
#include <QRubberBand>
#include <QScrollBar>

#include "chart.h"
#include "internalpulsechart.h"
#include "scene.h"
#include "basewindow.h"
#include "pulsegraphoperator.h"
#include "structdefine.h"
#include "pulsemapper.h"

QT_CHARTS_USE_NAMESPACE
//解决中文乱码
#pragma execution_character_set("utf-8")
class View : public QGraphicsView
{
    Q_OBJECT

public:
    View(QWidget *parent = 0);
    ~View();
    QRect client_rect;//设置一个存放屏幕分辨率的矩形框
    Scene *scene_;
    QList<Chart*> *charts_list_;
    QList<QLabel*> *label_list_;//存放chart标题的列表
    int chartFocusNumberUnderHover;//在鼠标悬浮的状态下，获得聚焦的chart



protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *mevent);
    void resizeEvent(QResizeEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
signals:
    void FullScreen1();
    void QuitFullScreen1();
    void resetPulseGraph();
    void Series1Clicked(int startNum, int endNum);
    void Series1Clicked_List(InterPulse proximalOriginalPulse);
private Q_SLOTS:
    void Series1HandleClickedPoint(const QPointF &point);
    void Series2HandleClickedPoint(const QPointF &point);
    void Series1HandleHoverEnterPoint(const QPointF &point,bool);
    void Series1HandleHoverLeavePoint(const QPointF &point,bool);
    void Series2HandleHoverEnterPoint(const QPointF &point,bool);
    void Series2HandleHoverLeavePoint(const QPointF &point,bool);
    void FullScreenSlot();
    void QuitFullScreenSlot();
    void on_button_reset_clicked();//reset

    void SettingParameterForScrollBar(QList<Chart*> *list);
    void TransformAllChartsWithScrollBar();
private:
     int focuschartnumber=0; //子图凸显后判断哪一个chart持有焦点
     QPushButton *button_reset;//用于chart数据还原的按钮
     //用于框选显示框线
     QRubberBand *rubberband;
     QPoint origin;

     QScrollBar *pulseview_sb;

     //判断鼠标点击还是鼠标长按
     //QTimer *timer;
};

#endif // VIEW_H
