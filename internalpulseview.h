#ifndef IMPLUSE_H
#define IMPLUSE_H

#include <QtWidgets>
#include <QWidget>
#include <QProxyStyle>
#include <QGraphicsView>
#include "scene.h"
#include "pulsegraphdisplayer.h"

QT_CHARTS_USE_NAMESPACE

class InternalPulseView :public QGraphicsView
{
    Q_OBJECT
public:
     InternalPulseView(QWidget *parent = 0);
    ~InternalPulseView();

     QRect clientRect;//设置一个存放屏幕分辨率的矩形框
     QList<InternalPulseChart*> *internalChartsList;
     Scene *internalScene;
     uchar *fpr;
     //QFile *file;
protected:
     void mousePressEvent(QMouseEvent *event);
     void mouseReleaseEvent(QMouseEvent *event);
     void mouseDoubleClickEvent(QMouseEvent *mevent);
     void resizeEvent(QResizeEvent *event);
private:
     int focuschartnumber=0;  //子图凸显后判断哪一个chart持有焦点
signals:
     void InternalPulseWindowFullScreen();
     void InternalPulseWindowQuitFullScreen();
private slots:
     void InternalChartFullScreenSlot();
     void InternalChartQuitFullScreenSlot();
     void UpdateInternalChart(int startNum, int endNum);
};

#endif // IMPLUSE_H
