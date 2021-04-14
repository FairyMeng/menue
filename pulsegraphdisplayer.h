#ifndef GRAPHDISPLAYER_H
#define GRAPHDISPLAYER_H

#include <QDebug>
#include "structdefine.h"
#include "view.h"
#include "PulseMapper.h"

class PulseGraphDisplayer
{
public:
    PulseGraphDisplayer();
    //显示脉间图像
    static void displayInterPulseGraph(QVector<InterPulse> average_data_vector, int average_number, int axisx_start, QList<Chart*> *list);
    //滚动条移动图像时显示脉间图像，同时显示series1_和series2_
    static void displayInterPulseGraphWithScrollBar(QVector<InterPulse> average_data_vector, int average_number, int axisx_start, QList<Chart*> *list);
    //显示脉内图像
    static void disPlayInternalPulseGraph(int startNum, int endNum, QVector<int> internalVector,InternalPulseChart *internalChart);
private:
    //脉间图像设置Y轴
    static void setChartAxisY(QVector<InterPulse> million_data_vector, QList<Chart*> *list);
    //脉内图像设置Y轴
    static void setInternalChartAxisY(QVector<int >internalVector,InternalPulseChart *internalChart);
};

#endif // GRAPHDISPLAYER_H
