#include "pulsegraphoperator.h"

PulseGraphOperator::PulseGraphOperator()
{

}

//鼠标单选
void PulseGraphOperator::SelectSinglePointWithMouse(Chart *chart,qreal xpos){
    QPointF clicked_point;
    foreach(QPointF series1_point, chart->series1_->points()){
        if(series1_point.x() ==xpos){
            clicked_point = series1_point;
            break;
        }
    }
    //未按下Ctrl，鼠标左键点击选中
    chart->series1_->append(chart->series2_->points());
    chart->series2_->clear();
    chart->series2_->append(clicked_point);
    chart->series1_->remove(clicked_point);
    qDebug()<<"PulseGraphOperator-SelectSinglePointWithMouse:MouseClicked";
}

//反选一个点
void PulseGraphOperator::UnSelectPoint(Chart *chart,QPointF Unselect_point){
    QPointF clicked_point;
    QList <double> selected_points;
    //for 循环将鼠标点击点的x坐标与series2的点每个作比较,并将所有点添加到list中
    for(int i=0;i<chart->series2_->points().size();i++){
        selected_points.append(qAbs(Unselect_point.x()-chart->series2_->points().at(i).x()));
     }
     //在list中，将距离鼠标点最近距离求出，并找出对应的点
    if(!selected_points.empty()){
       int index=0;
       // 假设list中第一个点最近
        double MIN_distance=qAbs(Unselect_point.x()-chart->series2_->points().at(0).x());
        for(int i=1;i<selected_points.count();i++){
            if(selected_points.at(i)<MIN_distance){
                MIN_distance=selected_points.at(i);
                index=i;
                break;
            }
        }
        qDebug()<<"PulseGraphOperator::UnselectPoint:Min distance"<<MIN_distance<<";index:"<<index;
        clicked_point=chart->series2_->at(index);
        chart->series1_->append(clicked_point);
        chart->series2_->remove(clicked_point);
    }
}
//反选时至少有2个点
void PulseGraphOperator::UnselectAtLeastTwoPoint(Chart *chart, QPointF Unselect_point){
    QPointF Clicked_Point;
    for(int i=0;i<chart->series2_->points().size();i++){
        if(chart->series2_->at(i).x()==Unselect_point.x()){
            Clicked_Point=chart->series2_->at(i);
            break;
        }
    }
    chart->series1_->append(Clicked_Point);
    chart->series2_->remove(Clicked_Point);
}

//键盘左右键单选
void PulseGraphOperator::SelectSinglePointWithKeyPress(Chart* chart, qreal xpos){
    QPointF next_point;
    foreach(QPointF series1_point, chart->series1_->points()){
        if(series1_point.x() == xpos){
            next_point = series1_point;
            break;
        }
    }
    chart->series1_->append(chart->series2_->points());
    chart->series2_->clear();
    chart->series2_->append(next_point);
    chart->series1_->remove(next_point);
    qDebug()<<"PulseGraphOperator::SelectSinglePointWithKeyPress()-select next";
}

//鼠标+Ctrl多选
void PulseGraphOperator::SelectMultiPointsWithMouse(Chart *chart, qreal xpos){
  QPointF clicked_point;
  foreach(QPointF series1_point, chart->series1_->points()){
      if(series1_point.x() == xpos)
          clicked_point = series1_point;
  }
  //按下Ctrl+鼠标左键点击选中
  chart->series2_->append(clicked_point);
  chart->series1_->remove(clicked_point);
  qDebug()<<"Ctrl+MouseClicked";
}

//键盘左右键+Ctrl多选、Ctrl+A全选
void PulseGraphOperator::SelectMultiPointsWithKeyPress(Chart* chart, qreal xpos){
    QPointF next_point;
    foreach(QPointF series1_point, chart->series1_->points()){
        if(series1_point.x() == xpos){
            next_point = series1_point;
            break;
        }
    }
    chart->series2_->append(next_point);
    chart->series1_->remove(next_point);
    qDebug()<<"PulseGraphOperator::SelectMultiPointsWithKeyPress()-select next";
}


//全选
void PulseGraphOperator::SelectAllPoints(Chart* chart, QKeyEvent *event){
  if(event->modifiers() == Qt::ControlModifier){
      chart->series2_->append(chart->series1_->points());
      chart->series1_->clear();
      qDebug()<<"Select All";
  }
}

//框选
void PulseGraphOperator::SelectMultiPointsWithRubberBand(Chart* chart, QVector<int> rubberband_points_x){
  //将图中X坐标值等于数组rubberband_points_x中元素的点选中
  foreach(QPointF series1_point, chart->series1_->points()){
      for(int i = 0; i < rubberband_points_x.size(); i++){
          if(series1_point.x() == rubberband_points_x[i]){
              chart->series2_->append(series1_point);
              chart->series1_->remove(series1_point);
          }
      }
  }
}

//取消全部选择
void PulseGraphOperator::UnSelectAllPoints(Chart* chart, QGraphicsSceneMouseEvent *event){
  if(event->button()==Qt::LeftButton)
  {
      chart->series1_->append(chart->series2_->points());
      chart->series2_->clear();
  }
}


//鼠标悬停点的坐标信息
QString PulseGraphOperator::GetHoveredPointInfo(QPointF point, QList<Chart*> *list,int chartFocusNumberUnderHover){
  QString point_info;
  point_info.append("x=");
  point_info.append(QString::number(point.x(),'g',6));
  point_info.append(" ");
  point_info.append("y=");
//  if(list->size()>0){
  point_info.append(QString::number(PulseMapper::DisplayOriginalOrdinateValue(point.x(),point.y(),list,chartFocusNumberUnderHover),'g',8));
//  }
  return point_info;
}
//放大图像
void PulseGraphOperator::ZoomInChart(Chart *chart, qreal axisx_start, qreal axisx_end){
  chart->axisx1_->setMax(axisx_end);
  chart->axisx1_->setMin(axisx_start);
  chart->removeAxis(chart->axisX());
  chart->setAxisX(chart->axisx1_, chart->series1_);
  chart->setAxisX(chart->axisx1_, chart->series2_);
  //qDebug()<<"zoom in min:"<<axisx_start<<"zoom in max:"<<axisx_end<<axisx_end-axisx_start;
}
//缩小
void PulseGraphOperator::ZoomOutChart(Chart *chart, qreal axisx_start, qreal axisx_end){
  chart->axisx1_->setMax(axisx_end);
  chart->axisx1_->setMin(axisx_start);
  chart->removeAxis(chart->axisX());
  chart->setAxisX(chart->axisx1_, chart->series1_);
  chart->setAxisX(chart->axisx1_, chart->series2_);
  //qDebug()<<"zoom out min:"<<axisx_start<<"zoom out max:"<<axisx_end<<axisx_end-axisx_start;
}

void PulseGraphOperator::TransformChartLeft(Chart *chart, qreal axisx_start, qreal axisx_end){
  chart->axisx1_->setMax(axisx_end);
  chart->axisx1_->setMin(axisx_start);
  chart->removeAxis(chart->axisX());
  chart->setAxisX(chart->axisx1_, chart->series1_);
  chart->setAxisX(chart->axisx1_, chart->series2_);
}

void PulseGraphOperator::TransformChartRight(Chart *chart, qreal axisx_start, qreal axisx_end){
  //chart->scroll(50, 0);
  chart->axisx1_->setMax(axisx_end);
  chart->axisx1_->setMin(axisx_start);
  chart->removeAxis(chart->axisX());
  chart->setAxisX(chart->axisx1_, chart->series1_);
  chart->setAxisX(chart->axisx1_, chart->series2_);
}

//针对五图联动进行二次封装
void PulseGraphOperator::SelectSinglePointInAllChartsWithMouse(QList<Chart*> *list,qreal xpos){
  for(int i = 0; i < list->count(); i++){
      PulseGraphOperator::SelectSinglePointWithMouse(list->at(i), xpos);
  }
}

void PulseGraphOperator::SelectSinglePointInAllChartsWithKeyPress(QList<Chart*> *list, qreal xpos){
  for(int i = 0; i < list->count(); i++){
      PulseGraphOperator::SelectSinglePointWithKeyPress(list->at(i), xpos);
  }
}

void PulseGraphOperator::SelectMultiPointsInAllChartsWithMouse(QList<Chart*> *list, qreal xpos){
  for(int i = 0; i < list->count(); i++){
      PulseGraphOperator::SelectMultiPointsWithMouse(list->at(i), xpos);
  }
}

void PulseGraphOperator::SelectMultiPointsInAllChartsWithKeyPress(QList<Chart*> *list, qreal xpos){
  for(int i = 0; i < list->count(); i++){
      PulseGraphOperator::SelectMultiPointsWithKeyPress(list->at(i), xpos);
  }
}

void PulseGraphOperator::SelectAllPointsInAllCharts(QList<Chart*> *list, QKeyEvent *event){
  for(int i = 0; i < list->count(); i++){
      PulseGraphOperator::SelectAllPoints(list->at(i), event);
  }
}

void PulseGraphOperator::SelectMultiPointsinAllChartsWithRubberBand(QList<Chart*> *list, QVector<int> rubberband_points_x){
  for(int i = 0; i < list->count(); i++){
      PulseGraphOperator::SelectMultiPointsWithRubberBand(list->at(i), rubberband_points_x);
  }
}

void PulseGraphOperator::UnSelectAllPointsInAllCharts(QList<Chart*> *list, QGraphicsSceneMouseEvent *event){
  for(int i = 0; i < list->count(); i++){
      PulseGraphOperator::UnSelectAllPoints(list->at(i), event);
  }
}

void PulseGraphOperator::ZoomInAllCharts(QList<Chart*> *list, qreal zoom_press_point_inseries_x, qreal zoom_release_point_inseries_x){
      int axisx_start = zoom_press_point_inseries_x;
      int axisx_end = zoom_release_point_inseries_x;
      //根据鼠标press和release的位置重新计算平均值
      int average_number = (zoom_release_point_inseries_x - zoom_press_point_inseries_x) / POINTNUMBER+1;
      if(zoom_release_point_inseries_x - zoom_press_point_inseries_x >= POINTNUMBER){
          //求平均值
          QVector<InterPulse> average_data_vector = PulseMapper::MapPulseTobePlotted(list->at(0)->million_data_vector_, zoom_press_point_inseries_x, zoom_release_point_inseries_x);
          for(int i = 0; i < list->count(); i++){
              PulseGraphOperator::ZoomInChart(list->at(i), axisx_start, axisx_end);
          }
          PulseGraphDisplayer::displayInterPulseGraph(average_data_vector, average_number, axisx_start, list);
      }else if(zoom_release_point_inseries_x - zoom_press_point_inseries_x >1&&zoom_release_point_inseries_x-zoom_press_point_inseries_x<1000){
          //横坐标跨度小于POINTNUMBER，大于1，显示原始数据
          QVector<InterPulse> original_data_vector = PulseMapper::MapPulseTobePlotted(list->at(0)->million_data_vector_, zoom_press_point_inseries_x, zoom_release_point_inseries_x);
          for(int i = 0; i < list->count(); i++){
              PulseGraphOperator::ZoomInChart(list->at(i), axisx_start, axisx_end);
          }
          PulseGraphDisplayer::displayInterPulseGraph(original_data_vector, 1 ,axisx_start, list);
      }
}
void PulseGraphOperator::ZoomOutAllCharts(QList<Chart*> *list, qreal zoom_press_point_inseries_x, qreal zoom_release_point_inseries_x){
      int length = list->at(0)->axisx1_->max() - list->at(0)->axisx1_->min();
      //计算缩小后的坐标轴起始与结束值
      int axisx_start = list->at(0)->axisx1_->min();
      int axisx_end = list->at(0)->axisx1_->max();
      if(length > 0 && length < 10000){
          axisx_start = axisx_start - length * 1;
          axisx_end = axisx_end + length * 1;
      }else if(length >= 10000 && length < 100000){
          axisx_start = axisx_start - length * 0.5;
          axisx_end = axisx_end + length * 0.5;
      }else if(length >= 100000 && length < 500000){
          axisx_start = axisx_start - length * 0.1;
          axisx_end = axisx_end + length * 0.1;
      }else if(length >= 500000){
          axisx_start = axisx_start - length * 0.05;
          axisx_end = axisx_end + length * 0.05;
      }
      int average_number;
      //坐标轴起止小于POINTNUMBER时，不用求平均值，因此average_number = 1
      if(axisx_end - axisx_start <= POINTNUMBER){
          average_number= 1;
      }else{
          average_number = (axisx_end - axisx_start) / POINTNUMBER+1;
      }

      if(axisx_start >= 0 && axisx_end <= list->at(0)->million_data_vector_.size() - 1){
          //求平均值
    //        qDebug()<<"start = "<<start<<endl<<"end = "<<end;
    //        qDebug()<<"axisX.min = "<<list->at(0)->axisX1->min()<<endl<<"axisX.max = "<<list->at(0)->axisX1->max();
          QVector<InterPulse> average_data_vector = PulseMapper::MapPulseTobePlotted(list->at(0)->million_data_vector_, axisx_start, axisx_end);
          for(int i = 0; i < list->count(); i++){
             PulseGraphOperator::ZoomOutChart(list->at(i), axisx_start, axisx_end);
          }
          PulseGraphDisplayer::displayInterPulseGraph(average_data_vector, average_number, axisx_start, list);
      }else{
          axisx_start = 0;
          axisx_end = list->at(0)->million_data_vector_.size() - 1;
          average_number = (axisx_end - axisx_start) / POINTNUMBER+1;
          QVector<InterPulse> average_data_vector = PulseMapper::MapPulseTobePlotted(list->at(0)->million_data_vector_, axisx_start, axisx_end);
          for(int i = 0; i < list->count(); i++){
             PulseGraphOperator::ZoomOutChart(list->at(i), axisx_start, axisx_end);
          }
          PulseGraphDisplayer::displayInterPulseGraph(average_data_vector, average_number, axisx_start, list);
      }
}
void PulseGraphOperator::TransformAllChartsLeft(QList<Chart*> *list){
     int length = list->at(0)->axisx1_->max() - list->at(0)->axisx1_->min();
      //计算平移后的坐标轴起始与结束值
      int axisx_start = list->at(0)->axisx1_->min();
      int axisx_end = list->at(0)->axisx1_->max();
      if(length > 0 && length < 10000){
          axisx_start = axisx_start - length * 1;
          axisx_end = axisx_end - length * 1;
      }else if(length >= 10000 && length < 100000){
          axisx_start = axisx_start - length * 0.5;
          axisx_end = axisx_end - length * 0.5;
      }else if(length >= 100000 && length < 500000){
          axisx_start = axisx_start - length * 0.1;
          axisx_end = axisx_end - length * 0.1;
      }else if(length >= 500000){
          axisx_start = axisx_start - length * 0.05;
          axisx_end = axisx_end - length * 0.05;
      }
      int average_number;
      if(axisx_end - axisx_start <= POINTNUMBER){
          average_number= 1;
      }else{
          average_number = (axisx_end - axisx_start) / POINTNUMBER+1;
      }
      if(axisx_end - axisx_start > 0 && axisx_start > 0){
           //左移求平均值
           QVector<InterPulse> average_data_vector = PulseMapper::MapPulseTobePlotted(list->at(0)->million_data_vector_, axisx_start, axisx_end);
           for(int i = 0; i < list->count(); i++){
               PulseGraphOperator::TransformChartLeft(list->at(i), axisx_start, axisx_end);
           }
           PulseGraphDisplayer::displayInterPulseGraph(average_data_vector, average_number, axisx_start, list);
      }
      //qDebug()<<"axisX.min = "<<list->at(0)->axisx1_->min()<<endl<<"axisX.max = "<<list->at(0)->axisx1_->max();
}
void PulseGraphOperator::TransformAllChartsRight(QList<Chart*> *list){
      int length = list->at(0)->axisx1_->max() - list->at(0)->axisx1_->min();
      //计算平移后的坐标轴起始与结束值
      int axisx_start = list->at(0)->axisx1_->min();
      int axisx_end = list->at(0)->axisx1_->max();
      if(length > 0 && length < 10000){
          axisx_start = axisx_start + length * 1;
          axisx_end = axisx_end + length * 1;
      }else if(length >= 10000 && length < 100000){
          axisx_start = axisx_start + length * 0.5;
          axisx_end = axisx_end + length * 0.5;
      }else if(length >= 100000 && length < 500000){
          axisx_start = axisx_start + length * 0.1;
          axisx_end = axisx_end + length * 0.1;
      }else if(length >= 500000){
          axisx_start = axisx_start + length * 0.05;
          axisx_end = axisx_end + length * 0.05;
      }
      int average_number;
      if(axisx_end - axisx_start <= POINTNUMBER){
          average_number= 1;
      }else{
          average_number = (axisx_end - axisx_start) / POINTNUMBER+1;
      }
      if((axisx_end - axisx_start > 0) && (axisx_end < list->at(0)->million_data_vector_.size())){
          //求平均值
           QVector<InterPulse> average_data_vector = PulseMapper::MapPulseTobePlotted(list->at(0)->million_data_vector_, axisx_start, axisx_end);
           for(int i = 0; i < list->count(); i++){
               PulseGraphOperator::TransformChartRight(list->at(i), axisx_start, axisx_end);
           }
           PulseGraphDisplayer::displayInterPulseGraph(average_data_vector, average_number, axisx_start, list);
      }
      qDebug()<<"axisX.min = "<<list->at(0)->axisx1_->min()<<endl<<"axisX.max = "<<list->at(0)->axisx1_->max();
}
//反选时只有1个点
void PulseGraphOperator::UnSelectPointInAllCharts(QList<Chart *>*list,QPointF Unselect_point){
  for(int i=0;i<list->count();i++){
      PulseGraphOperator::UnSelectPoint(list->at(i),Unselect_point);
  }
}
//反选时至少有2个点
void PulseGraphOperator::UnselectAtLeastTwoPointInAllCharts(QList<Chart *>*list,QPointF Unselect_point){
    for(int i=0;i<list->count();i++){
        PulseGraphOperator::UnselectAtLeastTwoPoint(list->at(i),Unselect_point);
    }
}
