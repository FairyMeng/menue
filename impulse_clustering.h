#ifndef IMPULSE_CLUSTERING_H
#define IMPULSE_CLUSTERING_H

#include <QWidget>
#include <QDebug>
#include <QProxyStyle>
#include "view.h"
#include "basewindow.h"

namespace Ui {
class impulse_Clustering;
}

class impulse_Clustering : public BaseWindow
{
    Q_OBJECT

public:
    explicit impulse_Clustering(QWidget *parent = 0);
    ~impulse_Clustering();
    QVector<InterPulse> million_data_vector_;
    QRect clientRect;//设置一个存放屏幕分辨率的矩形框
private:
    Ui::impulse_Clustering *ui;
    void initTitleBar();
    void closeEvent(QCloseEvent *event);
signals:
    void impulse_Clustering_close();
    //当窗口状态变为激活状态时，发射信号
    void AllWindowActivate(QObject *obj);
    //鼠标在窗口松开时，发射信号
    void MouseRelease(QObject *obj);
private slots:
    void AddMouseSelectedPulseInfoIntoListSlots(InterPulse proximalOriginalPulse);
    void AddKeyPressSelectedPulseInfoIntoListSlots(InterPulse pulse);
    void AddRubberBandSelectedPulseInfoIntoListSlots(Chart *chart, QVector<int> rubberband_points_x);
    void ClearPulseListSlot();
    void AddAllPulseInfoIntoListSlot(QVector<InterPulse> allPulseVector);
protected:
    bool eventFilter(QObject *watched, QEvent *event);
};

#endif // IMPULSE_CLUSTERING_H
