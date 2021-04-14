#ifndef DATACALCULATOR_H
#define DATACALCULATOR_H
#include <QScatterSeries>
#include <databaseoperator.h>
#include <QVector>
#include <qmath.h>
#include <QTime>

#include "structdefine.h"
#include "chart.h"
QT_CHARTS_USE_NAMESPACE

class PulseMapper
{
public:
//    QVector<InterPulse> million_data_vector;
//    QVector<InterPulse> AverageVector;
    PulseMapper();
    //对指定段的数据求平均值
    static QVector<InterPulse> MapPulseTobePlotted(QVector<InterPulse> million_data_vector, int axisx_start, int axisx_end);
    //取得脉间图Y轴的取值范围
    static QVector<double> GetMaxMinValueOfPulseTobePlotted(QVector<InterPulse> data_vector);
    //取得脉内图Y轴的取值范围
    static QVector<int> GetMaxMinValueOfInternalPulseTobePlotted(QVector<int> internalVector);
    //显示Y轴的原始坐标序列
    static double DisplayOriginalOrdinateValue(int x_point,double y_point,QList<Chart*> *list,int chartFocusNumber);
    //取得脉间原始数据
    static QVector<InterPulse> GetOriginalPulseData(Chart *chart, int point_x, int average_number);
    //取得脉间原始数据中最接近的一条数据
    static InterPulse GetProximalOriginalPulseData(Chart *chart, int point_x, double point_y, int average_number);

};
#endif // DATACALCULATOR_H
