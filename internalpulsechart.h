#ifndef INTERPULSECHART_H
#define INTERPULSECHART_H

#include <QChart>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QScatterSeries>
#include <QValueAxis>

QT_CHARTS_USE_NAMESPACE
class InternalPulseChart : public QChart
{
    Q_OBJECT
public:
    InternalPulseChart();
    QScatterSeries *series1_;
    QScatterSeries *series2_;
    QValueAxis *axisx1_;
    QList<InternalPulseChart *> *internalChartsList;
    QVector<int> internalVector;
    //子图凸显中判断窗口大小的双击标签
    bool doubleclick_;
protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
signals:
    void InternalChartFullScreen();
    void InternalChartQuitFullScreen();
private:
};

#endif // INTERPULSECHART_H
