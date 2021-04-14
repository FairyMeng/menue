#ifndef GRAPHOPERATOR_H
#define GRAPHOPERATOR_H

#include <QScatterSeries>
#include <QPointF>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QApplication>
#include <QDebug>
#include <qmath.h>
#include <QVector>
#include <QList>


#include "pulsemapper.h"
#include "pulsegraphdisplayer.h"
#include "structdefine.h"

QT_CHARTS_USE_NAMESPACE

class PulseGraphOperator
{
public:
    PulseGraphOperator();
    static QString GetHoveredPointInfo(QPointF point,QList<Chart*> *list,int chartFocusNumberUnderHover);
    //针对五图联动进行二次封装
    static void SelectSinglePointInAllChartsWithMouse(QList<Chart*> *list, qreal xpos);
    static void SelectSinglePointInAllChartsWithKeyPress(QList<Chart*> *list, qreal xpos);
    static void SelectMultiPointsInAllChartsWithMouse(QList<Chart*> *list, qreal xpos);
    static void SelectMultiPointsInAllChartsWithKeyPress(QList<Chart*> *list, qreal xpos);

    static void SelectAllPointsInAllCharts(QList<Chart*> *list, QKeyEvent *event);
    static void SelectMultiPointsinAllChartsWithRubberBand(QList<Chart*> *list, QVector<int> rubberband_points_x);
    static void UnSelectAllPointsInAllCharts(QList<Chart*> *list, QGraphicsSceneMouseEvent *event);
    static void UnSelectPointInAllCharts(QList<Chart *>*list,QPointF Unselect_point);
    //至少处理2个点
    static void UnselectAtLeastTwoPointInAllCharts(QList<Chart *>*list,QPointF Unselect_point);

    static void ZoomInAllCharts(QList<Chart*> *list, qreal zoom_press_point_inseries_x, qreal zoom_release_point_inseries_x);
    static void ZoomOutAllCharts(QList<Chart*> *list, qreal zoom_press_point_inseries_x, qreal zoom_release_point_inseries_x);
    static void TransformAllChartsLeft(QList<Chart*> *list);
    static void TransformAllChartsRight(QList<Chart*> *list);

    static void TransformChartLeft(Chart* chart, qreal axisx_start, qreal axisx_end);
    static void TransformChartRight(Chart* chart, qreal axisx_start, qreal axisx_end);


private:
    static void SelectSinglePointWithMouse(Chart *chart,qreal xpos);
    static void SelectSinglePointWithKeyPress(Chart *chart, qreal xpos);
    static void SelectMultiPointsWithMouse(Chart *chart, qreal xpos);
    static void SelectMultiPointsWithKeyPress(Chart *chart, qreal xpos);
    static void SelectAllPoints(Chart* chart, QKeyEvent *event);
    static void SelectMultiPointsWithRubberBand(Chart* chart, QVector<int> rubberband_points_x);
    static void UnSelectAllPoints(Chart* chart, QGraphicsSceneMouseEvent *event);
    static void UnSelectPoint(Chart *chart,QPointF Unselect_point);
    //至少处理2个点
    static void UnselectAtLeastTwoPoint(Chart *chart,QPointF Unselect_point);

    static void ZoomInChart(Chart* chart, qreal axisx_start, qreal axisx_end);
    static void ZoomOutChart(Chart* chart, qreal axisx_start, qreal axisx_end);

};

#endif // GRAPHOPERATOR_H
