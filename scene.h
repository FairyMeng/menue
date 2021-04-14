#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QKeyEvent>
#include <QDebug>
#include "chart.h"
#include "internalpulsechart.h"
class Scene : public QGraphicsScene
{
public:
    Scene(QGraphicsScene *parent = 0);
   // ~Scene();
    //脉间charts
    Chart *chart1_;
    Chart *chart2_;
    Chart *chart3_;
    Chart *chart4_;
    Chart *chart5_;
    //脉内charts
    InternalPulseChart *interChart1_;
    InternalPulseChart *interChart2_;
    InternalPulseChart *interChart3_;
    InternalPulseChart *interChart4_;
protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void wheelEvent(QGraphicsSceneWheelEvent *event);
private:
    void setAllChartProperties();//设置脉间，脉内chart属性
    void setAllChartsMargins();//给所有chart(脉间，脉内)设置边距，增大显示面积
};

#endif // SCENE_H
