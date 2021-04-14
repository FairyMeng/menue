#ifndef CHART_H
#define CHART_H

#include <QChart>
#include <QScatterSeries>
#include <QValueAxis>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QCursor>
#include <QDesktopWidget>
#include <QApplication>
#include <QLineSeries>
#include <QList>
#include <QValueAxis>
#include <QDebug>

#include "structdefine.h"
#include "databaseoperator.h"

QT_CHARTS_USE_NAMESPACE

class Chart : public QChart
{
    Q_OBJECT
public:
    Chart();
    QScatterSeries *series1_;
    QScatterSeries *series2_;
    QValueAxis *axisx1_;
    QList<Chart*> *charts_list_;
    QVector<InterPulse> million_data_vector_;
    //子图凸显中判断窗口大小的双击标签
    bool doubleclick_;
    //用来表示求完平均值的数组
    QVector<InterPulse> average_data_vector;
    //表示每几个数需要求一次平均值
    int average_number;
    //鼠标在悬浮状态下获得chart的焦点
    int chartFocusNumberUnderHover;
protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void wheelEvent(QGraphicsSceneWheelEvent *event);
signals:
    void FullScreen();
    void QuitFullScreen();
    //键盘左右键选中脉冲后发射以下信号
    void AddKeyPressSelectedPulseInfoIntoList(InterPulse pulse);
    void UpdateInternalPulseChartWithKeyPressPulse(int startNum, int endNum);
    //框选脉冲后发射以下信号
    void AddRubberBandSelectedPulseInfoIntoList(Chart *chart, QVector<int> rubberband_points_x);
    void UpdateInternalPulseChartWithRBFirstPulse(int startNum, int endNum);
    //全选时发射以下信号，槽函数将所有脉冲添加到脉冲列表中
    void AddAllPulseInfoIntoList(QVector<InterPulse> allPulseVector);
    //全部反选时发射以下信号，槽函数将清空脉冲聚类列表
    void ClearPulseList();
    //图像缩放后发送设置滚动条信号
    void SettingParameterForScrollBar(QList<Chart *> *list);
private:
    QRect client_rect;
    //用于放大缩小
    QPointF zoom_press_point_;
    QPointF zoom_release_point_;
    QPointF zoom_press_point_inseries_;
    QPointF zoom_release_point_inseries_;
    qreal move_length_;

    //用于框选
    QPointF rubberband_press_point_;
    QPointF rubberband_release_point_;
    QPointF rubberband_press_point_inseries_;
    QPointF rubberband_release_point_inseries_;
};

#endif // CHART_H
