#include "pulsegraphdisplayer.h"

PulseGraphDisplayer::PulseGraphDisplayer()
{

}

void PulseGraphDisplayer::displayInterPulseGraph(QVector<InterPulse> average_data_vector, int average_number, int axisx_start, QList<Chart*> *list){
    for(int i = 0; i < list->count(); i++){
        list->at(i)->series1_->clear();
        list->at(i)->series2_->clear();
    }
    for(int j = 0; j < average_data_vector.size(); j++){
        for(int i = 0; i < list->count(); i++){
            switch(i){
            case 0:
                list->at(i)->series1_->append(axisx_start + j * average_number, average_data_vector.at(j).frequency);
                break;
            case 1:
                list->at(i)->series1_->append(axisx_start + j * average_number, average_data_vector.at(j).dtoa);
                break;
            case 2:
                list->at(i)->series1_->append(axisx_start + j * average_number, average_data_vector.at(j).pulse_width);
                break;
            case 3:
                list->at(i)->series1_->append(axisx_start + j * average_number, average_data_vector.at(j).pulse_amp);
                break;
            case 4:
                break;
            }
        }
    }
    if(average_data_vector.size()>0)
        PulseGraphDisplayer::setChartAxisY(average_data_vector,list);
    else
        qDebug()<<"PulseGraphDisplayer::displayInterPulseGraph--The array is null and does not determine the ordinate value.";
}

void PulseGraphDisplayer::displayInterPulseGraphWithScrollBar(QVector<InterPulse> average_data_vector, int average_number, int axisx_start, QList<Chart *> *list){
    for(int i = 0; i < list->count(); i++){
        list->at(i)->series1_->clear();
    }
//    int num = 0;
//    qDebug()<<"series2.points() = "<<list->at(0)->series2_->points().size();
    for(int j = 0; j < average_data_vector.size(); j++){
        bool flag = false;
        foreach(QPointF series2_point,  list->at(0)->series2_->points()){
            if(axisx_start + j * average_number == series2_point.x()){
                flag = true;
//                num++;
            }
        }
        if(flag == false){
            for(int i = 0; i < list->count(); i++){
                switch(i){
                case 0:
                    list->at(i)->series1_->append(axisx_start + j * average_number, average_data_vector.at(j).frequency);
                    break;
                case 1:
                    list->at(i)->series1_->append(axisx_start + j * average_number, average_data_vector.at(j).dtoa);
                    break;
                case 2:
                    list->at(i)->series1_->append(axisx_start + j * average_number, average_data_vector.at(j).pulse_width);
                    break;
                case 3:
                    list->at(i)->series1_->append(axisx_start + j * average_number, average_data_vector.at(j).pulse_amp);
                    break;
                case 4:
                    break;
                }
            }
        }
    }
//    qDebug()<<"series1 not paint point size = "<<num;
    if(average_data_vector.size()>0)
        PulseGraphDisplayer::setChartAxisY(average_data_vector,list);
    else
        qDebug()<<"PulseGraphDisplayer::displayInterPulseGraph--The array is null and does not determine the ordinate value.";
}

void PulseGraphDisplayer::setChartAxisY(QVector<InterPulse> million_data_vector, QList<Chart *> *list){
    QVector<double> extreme_value = PulseMapper::GetMaxMinValueOfPulseTobePlotted(million_data_vector);//million_data_vector.size=1000
    for(int i = 1; i <= 4; i++){
        switch(i){
        case 1:{
            QValueAxis *axisy1 = new QValueAxis();
            axisy1->setLabelsColor(QColor::fromRgb(0, 204, 204));
            axisy1->setGridLineColor(QColor::fromRgb(0, 204, 204));
            axisy1->setLabelFormat("%013.4f");
            axisy1->setLinePenColor(QColor::fromRgb(0, 204, 204));
            axisy1->setRange(extreme_value.at(0), extreme_value.at(1));
            list->at(i-1)->setAxisY(axisy1, list->at(i-1)->series1_);
            list->at(i-1)->setAxisY(axisy1, list->at(i-1)->series2_);
        }
            break;
        case 2:{
            QValueAxis *axisy2 = new QValueAxis();
            axisy2->setLabelsColor(QColor::fromRgb(0, 204, 204));
            axisy2->setGridLineColor(QColor::fromRgb(0, 204, 204));
            axisy2->setLabelFormat("%013.4f");
            axisy2->setLinePenColor(QColor::fromRgb(0, 204, 204));
            axisy2->setRange(extreme_value.at(2), extreme_value.at(3));
            list->at(i-1)->setAxisY(axisy2, list->at(i-1)->series1_);
            list->at(i-1)->setAxisY(axisy2, list->at(i-1)->series2_);
        }
            break;
        case 3:{
            QValueAxis *axisy3 = new QValueAxis();
            //设置坐标数值颜色
            axisy3->setLabelsColor(QColor::fromRgb(0, 204, 204));
            //设置左边网格线的颜色
            axisy3->setGridLineColor(QColor::fromRgb(0, 204, 204));
            //设置坐标轴颜色
            axisy3->setLinePenColor(QColor::fromRgb(0, 204, 204));
            //用于多个chart纵坐标对齐
            axisy3->setLabelFormat("&nbsp;&nbsp;%09.4f");
            axisy3->setRange(extreme_value.at(4), extreme_value.at(5));
            list->at(i-1)->setAxisY(axisy3, list->at(i-1)->series1_);
            list->at(i-1)->setAxisY(axisy3, list->at(i-1)->series2_);
        }
            break;
        case 4:{
            QValueAxis *axisy4 = new QValueAxis();
            axisy4->setLabelsColor(QColor::fromRgb(0, 204, 204));
            axisy4->setGridLineColor(QColor::fromRgb(0, 204, 204));
            axisy4->setLinePenColor(QColor::fromRgb(0, 204, 204));
            axisy4->setLabelFormat("&nbsp;%011.4f");
            axisy4->setRange(extreme_value.at(6), extreme_value.at(7));
            list->at(i-1)->setAxisY(axisy4, list->at(i-1)->series1_);
            list->at(i-1)->setAxisY(axisy4, list->at(i-1)->series2_);
        }
            break;
        default:
            break;
        }
    }
}

//显示脉内图像
void PulseGraphDisplayer::disPlayInternalPulseGraph(int startNum, int endNum, QVector<int> internalVector, InternalPulseChart *internalChart){
//    for(int i=1091;i<2268;i++){
//        internalChart->series1_->append(i,internalVector.at(i));
//    }
    internalChart->series1_->clear();
    QTime time1,time2;
    time1.start();
    time2.start();
    for(int i = startNum; i <= endNum; i++){
        internalChart->series1_->append(i, internalVector.at(i - startNum));
    }
    qDebug()<<"internalChart add point use time:"<<time1.elapsed() / 1000<<"s";
    PulseGraphDisplayer::setInternalChartAxisY(internalVector,internalChart);
    qDebug()<<"internalChart set chart axisY use time:"<<time2.elapsed() / 1000<<"s";
   // qDebug()<<"PulseGraphDisplayer- finish internal graph";
}
void PulseGraphDisplayer::setInternalChartAxisY(QVector<int> internalVector, InternalPulseChart *internalChart){
    QVector<int> extremeValue=PulseMapper::GetMaxMinValueOfInternalPulseTobePlotted(internalVector);
    QValueAxis *axisy1 = new QValueAxis();

    axisy1->setLabelsColor(QColor::fromRgb(0, 204, 204));
    axisy1->setGridLineColor(QColor::fromRgb(0, 204, 204));
    axisy1->setLinePenColor(QColor::fromRgb(0, 204, 204));
    axisy1->setRange(extremeValue.at(0),extremeValue.at(1));
    internalChart->setAxisY(axisy1,internalChart->series1_);
}
