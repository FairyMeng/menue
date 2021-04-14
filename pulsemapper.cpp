#include "pulsemapper.h"

PulseMapper::PulseMapper()
{

}
//对指定段的数据求平均值
QVector<InterPulse> PulseMapper::MapPulseTobePlotted(QVector<InterPulse> million_data_vector, int axisx_start, int axisx_end){
         QTime time;
         time.start();
         QVector<InterPulse> average_data_vector;
         InterPulse temp_pulse;
         double sum_frequency = 0;
         int sum_dtoa = 0;
         double sum_pulse_width = 0;
         double sum_pulse_amp = 0;

         //坐标轴起止小于POINTNUMBER(宏定义为1000)，添加原始数据；否则计算平均值数据,axis_start:0,axis_end:million.size,但会随着放大缩小图像时改变
         if(axisx_end - axisx_start <= POINTNUMBER){
             for(int i = axisx_start; i <= axisx_end; i++){
                 average_data_vector.append(million_data_vector[i]);
             }
         }else{
             //文件中数据miliomData的一段，截止位置axisx_end与起始位置axisx_start只差除以屏幕中的点数，求出每多少条数据求一次平均值
             int average_number = (axisx_end - axisx_start) / POINTNUMBER + 1;
             for(int i = 0; i < POINTNUMBER ; i++){
                 sum_frequency = 0;
                 sum_dtoa = 0;
                 sum_pulse_width = 0;
                 sum_pulse_amp = 0;
                 if(i * average_number + average_number <= million_data_vector.size()){
                     for(int j = 0; j < average_number; j++){
                         sum_frequency += million_data_vector[axisx_start + j + i * average_number].frequency;
                         sum_dtoa += million_data_vector[axisx_start + j + i * average_number].dtoa;
                         sum_pulse_width += million_data_vector[axisx_start + j + i * average_number].pulse_width;
                         sum_pulse_amp += million_data_vector[axisx_start + j + i * average_number].pulse_amp;
                     }
                     //点数>1000或者<1000时有不同的情况
                     if(average_number>0){
                         temp_pulse.frequency = sum_frequency / average_number;
                         temp_pulse.dtoa = sum_dtoa / average_number;
                         temp_pulse.pulse_width = sum_pulse_width / average_number;
                         temp_pulse.pulse_amp = sum_pulse_amp / average_number;
                     }else{
                         average_number=1;
                         temp_pulse.frequency = sum_frequency / average_number;
                         temp_pulse.dtoa = sum_dtoa / average_number;
                         temp_pulse.pulse_width = sum_pulse_width / average_number;
                         temp_pulse.pulse_amp = sum_pulse_amp / average_number;
                     }
                    average_data_vector.append(temp_pulse);
                    //qDebug()<<average_data_vector.at(i).frequency;
                 }else{
                     for(int k = i * average_number; k < million_data_vector.size(); k++){
                         sum_frequency += million_data_vector[axisx_start + k].frequency;
                         sum_dtoa += million_data_vector[axisx_start + k].dtoa;
                         sum_pulse_width += million_data_vector[axisx_start + k].pulse_width;
                         sum_pulse_amp += million_data_vector[axisx_start + k].pulse_amp;
                     }
                     temp_pulse.frequency = sum_frequency / ((axisx_end - axisx_start) - i * average_number);
                     temp_pulse.dtoa = sum_dtoa / ((axisx_end - axisx_start) - i * average_number);
                     temp_pulse.pulse_width = sum_pulse_width / ((axisx_end - axisx_start) - i * average_number);
                     temp_pulse.pulse_amp = sum_pulse_amp / ((axisx_end - axisx_start) - i * average_number);
                     average_data_vector.append(temp_pulse);
                     //qDebug()<<average_data_vector.at(i).frequency;
                 }
             }
         }
         qDebug()<<"PulseMapper-calculate Avg:"<<time.elapsed()/1000<<"s";
         return average_data_vector;
}

//取得脉间图Y轴的取值范围
QVector<double> PulseMapper::GetMaxMinValueOfPulseTobePlotted(QVector<InterPulse> data_vector){
    QTime time;
    time.start();
    QVector<double> extreme_value;
    int fre_max = 0;
    int fre_min = 0;
    int dtoa_max = 0;
    int dtoa_min = 0;
    int puw_max = 0;
    int puw_min = 0;
    int pua_max = 0;
    int pua_min = 0;
        for(int i=0; i < data_vector.size(); i++)
        {
            if(data_vector[i].frequency > data_vector[fre_max].frequency)
                fre_max = i;
            else if(data_vector[i].frequency < data_vector[fre_min].frequency)
                fre_min = i;

            if(data_vector[i].dtoa > data_vector[dtoa_max].dtoa)
                dtoa_max = i;
            else if(data_vector[i].dtoa < data_vector[dtoa_min].dtoa)
                dtoa_min = i;

            if(data_vector[i].pulse_width > data_vector[puw_max].pulse_width)
                puw_max = i;
            else if(data_vector[i].pulse_width < data_vector[puw_min].pulse_width)
                puw_min = i;
            if(data_vector[i].pulse_amp > data_vector[pua_max].pulse_amp)
                pua_max = i;
            else if(data_vector[i].pulse_amp < data_vector[pua_min].pulse_amp)
                pua_min = i;
        }
    extreme_value.append(data_vector[fre_min].frequency);
    extreme_value.append(data_vector[fre_max].frequency);
    extreme_value.append(data_vector[dtoa_min].dtoa);
    extreme_value.append(data_vector[dtoa_max].dtoa);
    extreme_value.append(data_vector[puw_min].pulse_width);
    extreme_value.append(data_vector[puw_max].pulse_width);
    extreme_value.append(data_vector[pua_min].pulse_amp);
    extreme_value.append(data_vector[pua_max].pulse_amp);
//    qDebug()<<"PulseMapper-calculate MaxMinValue time is:"<<time.elapsed()/1000<<"s";
    return extreme_value;

}
//取得脉内图Y轴的取值范围，最大值和最小值
QVector<int> PulseMapper::GetMaxMinValueOfInternalPulseTobePlotted(QVector<int> internalVector){
    //qDebug()<<"PulseMapper-";
    QTime time1;
    time1.start();
    QVector<int> extreme_value;
    int max=internalVector.at(0);
    int min=internalVector.at(0);
    for(int i=0;i<internalVector.size();i++){
        if(internalVector.at(i)>max)
             max=internalVector.at(i);
        if(internalVector.at(i)<min)
             min=internalVector.at(i);
    }
    extreme_value.append(min);
    extreme_value.append(max);
   // qDebug()<<"min:"<<min<<"max"<<max;
   // qDebug()<<"PulseMapper:脉内数据，计算最大值最小值时间为："<<time1.elapsed()/1000<<"s";
    return extreme_value;
}

//显示点的原始纵坐标值,将屏幕上纵坐标显示的平均值与数组中的值作比较，选出最近的点的纵坐标
double PulseMapper::DisplayOriginalOrdinateValue(int x_point,double y_point,QList<Chart*> *list,int chartFocusNumber){
   /*1.获得点的坐标在所属平均值数组的下标，比如是第30点
     2.根据确定的点的下标，在million_data中确定求平均值的范围，比如第平均值数组中第0个点，就是million_data中第0-10000个数
     3.将这个平均值与0-1000个数做比较，距离最近的纵坐标的值显示出来 */

    QVector<InterPulse> originalPulseData;//用来存储原始脉冲数据，从start到end这段
    InterPulse temp_Pulse;

    int start=x_point;//所求平均值数组范围的起点,对应million_data_vector中的x坐标
//list是什么？x_point是代表谁的横坐标？
    int end=x_point+list->at(0)->average_number;//所求平均值数组范围的终点

    if(end<list->at(0)->million_data_vector_.size()){
        for(int i=start;i<end;i++){
              temp_Pulse.frequency=list->at(0)->million_data_vector_.at(i).frequency;
              temp_Pulse.dtoa=list->at(0)->million_data_vector_.at(i).dtoa;
              temp_Pulse.pulse_width=list->at(0)->million_data_vector_.at(i).pulse_width;
              temp_Pulse.pulse_amp=list->at(0)->million_data_vector_.at(i).pulse_amp;
        }
        originalPulseData.append(temp_Pulse);
    }else{
        qDebug()<<"PulseMapper-DisplayOriginalOrdinateValue:the sum of x-corrdinate of point and averageNumber beyond array";
    }
    //qDebug()<<"PulseMapper-the originalPulseData size :"<<originalPulseData.size();
    switch (chartFocusNumber) {
    case 0:
    {
       // qDebug()<<"pulsemapper-Frequency-chartFocusNumber"<<chartFocusNumber;
        int index=0;
        QVector<double> selected_points;
        for(int i=0;i<originalPulseData.size();i++){
            //比较frequency
            if(y_point==originalPulseData.at(i).frequency){
                y_point=originalPulseData.at(i).frequency;
                qDebug()<<"pulseMapper-DisplayOriginalOrdinateValue:"<<"find accurate  frequency value successfully!";
                break;
            }else{
                //将y_point与originalPulseDatta中frequency的每个点比较，最终将比较结果存入QVector中
                selected_points.append(qAbs(y_point-originalPulseData.at(i).frequency));
            }
        }
        if(!selected_points.empty()){
//            qDebug()<<"PulseMapper- sizeof selectedPoints:"<<selected_points.size();
            //假设list中第一个点最近
            double MIN_distance=selected_points.at(0);
            for(int j=1;j<selected_points.size();j++){
                if(selected_points.at(j)<MIN_distance){
                    MIN_distance=selected_points.at(j);
                    index=j;
                    y_point=originalPulseData.at(index).frequency;
                    break;
                }
            }
//            qDebug()<<"PulseMapper-DisplayOriginalOrdinateValue-dtoa"<<"y_point:"<<y_point<<";MIN_distance"<<QString("%1").arg(MIN_distance,0,'f',6)<<";index"<<index;
        }
    }
        break;
    case 1:
    {
        //qDebug()<<"pulsemapper-DTOA-chartFocusNumber"<<chartFocusNumber;
        int index=0;
        QVector<double> selected_points;

        for(int i=0;i<originalPulseData.size();i++){
            //比较dtoa
            if(y_point==originalPulseData.at(i).dtoa){
                y_point=originalPulseData.at(i).dtoa;
                qDebug()<<"pulseMapper-DisplayOriginalOrdinateValue:"<<"find accurate  dtoa value successfully!";
                break;
            }else{
                //将y_point与originalPulseDatta中dtoa的每个点比较，最终将比较结果存入QList中
                selected_points.append(qAbs(y_point-originalPulseData.at(i).dtoa));
            }
        }
        if(!selected_points.empty()){
//            qDebug()<<"PulseMapper- sizeof selectedPoints:"<<selected_points.size();

            //假设list中第一个点最近
            double MIN_distance=selected_points.at(0);
            for(int j=1;j<selected_points.size();j++){
                if(selected_points.at(j)<MIN_distance){
                    MIN_distance=selected_points.at(j);
                    index=j;
                    y_point=originalPulseData.at(index).dtoa;
                    break;
                }
            }
//           qDebug()<<"PulseMapper-DisplayOriginalOrdinateValue-dtoa"<<"y_point:"<<y_point<<";MIN_distance"<<QString("%1").arg(MIN_distance,0,'f',6)<<";index"<<index;
        }


    }
        break;
    case 2:
    {
        //qDebug()<<"pulsemapper-Width-chartFocusNumber"<<chartFocusNumber;
        int index=0;
        QVector<double> selected_points;
        for(int i=0;i<originalPulseData.size();i++){
            //比较pulse_width
            if(y_point==originalPulseData.at(i).pulse_width){
                y_point=originalPulseData.at(i).pulse_width;
                qDebug()<<"pulseMapper-DisplayOriginalOrdinateValue:"<<"find accurate  pulse_width value successfully!";
                break;
            }else{
                //将y_point与originalPulseDatta中dtoa的每个点比较，最终将比较结果存入QList中
                selected_points.append(qAbs(y_point-originalPulseData.at(i).pulse_width));
            }
        }
        if(!selected_points.empty()){
//            qDebug()<<"PulseMapper- sizeof selectedPoints:"<<selected_points.size();
            //假设list中第一个点最近
            double MIN_distance=selected_points.at(0);
            for(int j=1;j<selected_points.size();j++){
                if(selected_points.at(j)<MIN_distance){
                    MIN_distance=selected_points.at(j);
                    index=j;
                    y_point=originalPulseData.at(index).pulse_width;
                    break;
                }
            }
//           qDebug()<<"PulseMapper-DisplayOriginalOrdinateValue-pulse_width"<<"y_point:"<<y_point<<";MIN_distance"<<QString("%1").arg(MIN_distance,0,'f',6)<<";index"<<index;
        }
    }
        break;
    case 3:
    {
//        qDebug()<<"pulsemapper-Amp-chartFocusNumber"<<chartFocusNumber;
        int index=0;
        QVector<double> selected_points;
        for(int i=0;i<originalPulseData.size();i++){
            //比较pulse_width
            if(y_point==originalPulseData.at(i).pulse_amp){
                y_point=originalPulseData.at(i).pulse_amp;
                qDebug()<<"pulseMapper-DisplayOriginalOrdinateValue:"<<"find accurate pulse_amp value successfully!";
                break;
            }else{
                //将y_point与originalPulseDatta中dtoa的每个点比较，最终将比较结果存入QList中
                selected_points.append(qAbs(y_point-originalPulseData.at(i).pulse_amp));
            }
        }
        if(!selected_points.empty()){
//            qDebug()<<"PulseMapper- sizeof selectedPoints:"<<selected_points.size();
            //假设list中第一个点最近
            double MIN_distance=selected_points.at(0);
            for(int j=1;j<selected_points.size();j++){
                if(selected_points.at(j)<MIN_distance){
                    MIN_distance=selected_points.at(j);
                    index=j;
                    y_point=originalPulseData.at(index).pulse_amp;
                    break;
                }
            }
//           qDebug()<<"PulseMapper-DisplayOriginalOrdinateValue-pulse_amp"<<"y_point:"<<y_point<<";MIN_distance"<<QString("%1").arg(MIN_distance,0,'f',6)<<";index"<<index;
        }
    }
        break;
    default:
        break;
    }
    return y_point;
}

//获取序号从point_x到point_x+average_number范围内的原始数据
//这个函数的适用场景是什么？point_x如何获得？
QVector<InterPulse> PulseMapper::GetOriginalPulseData(Chart *chart, int point_x, int average_number){
    QVector<InterPulse> originalPulseVector;
    InterPulse temp_Pulse;
    int start = point_x;//所求平均值数组范围的起点,对应million_data_vector中的x坐标
    int end = point_x + average_number;//所求平均值数组范围的终点
    if(end < chart->charts_list_->at(0)->million_data_vector_.size()){
        for(int i = start; i < end; i++){
            temp_Pulse.pulse_id = chart->charts_list_->at(0)->million_data_vector_.at(i).pulse_id;
            temp_Pulse.toa = chart->charts_list_->at(0)->million_data_vector_.at(i).toa;
            temp_Pulse.frequency = chart->charts_list_->at(0)->million_data_vector_.at(i).frequency;
            temp_Pulse.dtoa = chart->charts_list_->at(0)->million_data_vector_.at(i).dtoa;
            temp_Pulse.pulse_width = chart->charts_list_->at(0)->million_data_vector_.at(i).pulse_width;
            temp_Pulse.pulse_amp = chart->charts_list_->at(0)->million_data_vector_.at(i).pulse_amp;
            originalPulseVector.append(temp_Pulse);
        }
    }else{
        qDebug()<<"PulseMapper-DisplayOriginalOrdinateValue:the sum of x-corrdinate of point and averageNumber beyond array";
    }
    return originalPulseVector;
}

//在序号从point_x到point_x+average_number范围内的原始数据中寻找（chart纵轴属性）最接近point_y的一条脉冲
InterPulse PulseMapper::GetProximalOriginalPulseData(Chart *chart, int point_x, double point_y, int average_number){
    //InterPulse proximalOriginalPulse;
    QVector<InterPulse> originalPulseData;//用来存储原始脉冲数据，从start到end这段
    InterPulse temp_Pulse;
    int start = point_x;//所求平均值数组范围的起点,对应million_data_vector中的x坐标
    int end = point_x + average_number;//所求平均值数组范围的终点
    if(end < chart->charts_list_->at(0)->million_data_vector_.size()){
        for(int i = start; i < end; i++){
            temp_Pulse.pulse_id = chart->charts_list_->at(0)->million_data_vector_.at(i).pulse_id;
            temp_Pulse.toa = chart->charts_list_->at(0)->million_data_vector_.at(i).toa;
            temp_Pulse.frequency = chart->charts_list_->at(0)->million_data_vector_.at(i).frequency;
            temp_Pulse.dtoa = chart->charts_list_->at(0)->million_data_vector_.at(i).dtoa;
            temp_Pulse.pulse_width = chart->charts_list_->at(0)->million_data_vector_.at(i).pulse_width;
            temp_Pulse.pulse_amp = chart->charts_list_->at(0)->million_data_vector_.at(i).pulse_amp;
            originalPulseData.append(temp_Pulse);
        }
    }else{
        qDebug()<<"PulseMapper-DisplayOriginalOrdinateValue:the sum of x-corrdinate of point and averageNumber beyond array";
    }
    qDebug()<<"PulseMapper-GetProximalOriginalPulse-originalPulseData.size = "<<originalPulseData.size();
    int index = start;
    QString chartTitle = chart->title();
    if(chartTitle == "Frequency"){
        double tempDistance;
        //double c=qAbs(a)：数qAbs()返回double型数值a的绝对值；
        double minDistance = qAbs(point_y - originalPulseData.at(0).frequency);
//        qDebug()<<"PulseMapper::GetProximalOriginalPulse-Frequency-distance = "<<minDistance;
//        qDebug()<<"PulseMapper::GetProximalOriginalPulse-Frequency-point_y = "<<point_y;
        for(int i = 0; i < originalPulseData.size(); i++){
            //比较frequency
            //将point_x处的frequency与originalPulseData中每个点的frequency比较，最终比较结果为最近的赋值其脉冲序号给index
            tempDistance = qAbs(point_y - originalPulseData.at(i).frequency);
//            qDebug()<<"PulseMapper::GetProximalOriginalPulse-Frequency-i.frequency = "<<originalPulseData.at(i).frequency;
//            qDebug()<<"PulseMapper::GetProximalOriginalPulse-Frequency-tempDistance = "<<tempDistance;
            if(minDistance > tempDistance){
                minDistance = tempDistance;
                index = start + i;
            }
        }
        qDebug()<<"PulseMapper::GetProximalOriginalPulse-Frequency-index = "<<index;
        return chart->charts_list_->at(0)->million_data_vector_.at(index);
    }else if(chartTitle == "DTOA"){
        double tempDistance;
        double minDistance = qAbs(point_y - originalPulseData.at(0).dtoa);
//        qDebug()<<"PulseMapper::GetProximalOriginalPulse-Frequency-distance = "<<minDistance;
//        qDebug()<<"PulseMapper::GetProximalOriginalPulse-Frequency-point_y = "<<point_y;
        for(int i = 0; i < originalPulseData.size(); i++){
            //DTOA
            //将point_x处的dtoa与originalPulseData中每个点的dtoa比较，最终比较结果为最近的赋值其脉冲序号给index
            tempDistance = qAbs(point_y - originalPulseData.at(i).dtoa);
//            qDebug()<<"PulseMapper::GetProximalOriginalPulse-Frequency-i.frequency = "<<originalPulseData.at(i).dtoa;
//            qDebug()<<"PulseMapper::GetProximalOriginalPulse-Frequency-tempDistance = "<<tempDistance;
            if(minDistance > tempDistance){
                minDistance = tempDistance;
                index = start + i;
            }
        }
        qDebug()<<"PulseMapper::GetProximalOriginalPulse-DTOA-index = "<<index;
        return chart->charts_list_->at(0)->million_data_vector_.at(index);
    }else if(chartTitle == "Width"){
        double tempDistance;
        double minDistance = qAbs(point_y - originalPulseData.at(0).pulse_width);
        for(int i = 0; i < originalPulseData.size(); i++){
            //Width
            //将point_x处的width与originalPulseData中每个点的width比较，最终比较结果为最近的赋值其脉冲序号给index
            tempDistance = qAbs(point_y - originalPulseData.at(i).pulse_width);
            if(minDistance > tempDistance){
                minDistance = tempDistance;
                index = start + i;
            }
        }
        qDebug()<<"PulseMapper::GetProximalOriginalPulse-Width-index = "<<index;
        return chart->charts_list_->at(0)->million_data_vector_.at(index);
    }else if(chartTitle == "Amp"){
        double tempDistance;
        double minDistance = qAbs(point_y - originalPulseData.at(0).pulse_amp);
        for(int i = 0; i < originalPulseData.size(); i++){
            //Amp
            //将point_x处的Amp与originalPulseData中每个点的Amp比较，最终比较结果为最近的赋值其脉冲序号给index
            tempDistance = qAbs(point_y - originalPulseData.at(i).pulse_amp);
            if(minDistance > tempDistance){
                minDistance = tempDistance;
                index = start + i;
            }
        }
        qDebug()<<"PulseMapper::GetProximalOriginalPulse-Amp-index = "<<index;
        return chart->charts_list_->at(0)->million_data_vector_.at(index);
    }
    //return proximalOriginalPulse;
}












