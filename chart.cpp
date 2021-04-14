#include "chart.h"
#include "pulsegraphoperator.h"//该头文件不可放到chart.h中，否则该头文件会发生与displayer.h相互引用现象
Chart::Chart()
{
    doubleclick_ = false;//双击标签初始化为假
    client_rect = QApplication::desktop()->availableGeometry(); //获取用户可用窗口大小
    series1_ = new QScatterSeries();
    series2_ = new QScatterSeries();
    series1_->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    series1_->setMarkerSize(4.0);
    series1_->setColor(Qt::green);
    series1_->setBorderColor(Qt::green);
    series2_->setBorderColor(Qt::red);
    series2_->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    series2_->setMarkerSize(4.0);
    series2_->setColor(Qt::red);

    axisx1_ = new QValueAxis();
    axisx1_->setLabelsColor(QColor::fromRgb(0, 204, 204));
    axisx1_->setGridLineColor(QColor::fromRgb(0, 204, 204));
    axisx1_->setLinePenColor(QColor::fromRgb(0, 204, 204));
    this->addSeries(series1_);
    this->addSeries(series2_);
    this->setAxisX(axisx1_, series1_);
//    this->setAxisY(axisY1, series1);
    this->setAxisX(axisx1_, series2_);
//    this->setAxisY(axisY1, series2);
    this->legend()->hide();
    this->setAcceptHoverEvents(true);
    this->setFlag(ItemIsFocusable,true);
    //connect(this,SIGNAL(UnselectAllPointsInChartsSignal(QList<Chart *>,QGraphicsSceneMouseEvent*)),this,SLOT(UnselectAllPointsInChartsSlots(QList<Chart *>,QGraphicsSceneMouseEvent*)));

}
/*键盘事件
* 1.已选中点的情况下，按下键盘左键，向左单选
* 2.按下键盘左键，向左单选
* 3.按下键盘右键，向右单选
* 4.按下键盘Ctrl+左键，向左多选
* 5.按下键盘Ctrl+右键，向右多选
* 6.按下键盘Ctrl+A键，全选
*/

void Chart::keyPressEvent(QKeyEvent *event){
    qDebug()<<"chart--keyPressEvent";
    setFocus();
    switch (event->key()) {
    case Qt::Key_A:{
        PulseGraphOperator::SelectAllPointsInAllCharts(this->charts_list_, event);
        //发射信号，将所有脉冲添加到脉冲列表中
        int axisX_start = this->axisx1_->min();
        int axisX_end = this->axisx1_->max();
        QVector<InterPulse> allPulseVector;
        for(int i = axisX_start; i < axisX_end; i++){
            allPulseVector.append(this->million_data_vector_[i]);
        }
        qDebug()<<"axisX_end - axisX_start = "<<axisX_end - axisX_start;
        qDebug()<<"allPulseVector.size = "<<allPulseVector.size();
        emit AddAllPulseInfoIntoList(allPulseVector);
        break;
    }
    case Qt::Key_Left:
        if(this->series2_->count() != 0){
            //键盘按下Left,选中已选点的左边的一个点next_point
            QPointF left_point(INT_MAX, 0);//series2最左端的点
            QPointF next_point;//下一个要选中的点
            int average_number = (this->axisx1_->max() - this->axisx1_->min()) / POINTNUMBER + 1;//横坐标每两个点相距average_number
            foreach(QPointF series1_point, this->series1_->points()){
                //找出series2中最左端的点
                foreach(QPointF series2_point, this->series2_->points()){
                    if(series2_point.x() < left_point.x())
                        left_point = series2_point;
                }
                if(series1_point.x() == left_point.x() - average_number)
                    next_point = series1_point;
            }
            InterPulse pulse;
            if(average_number == 1){
                foreach(InterPulse one_pulse, this->million_data_vector_){
                    if(one_pulse.pulse_id == next_point.x()){
                        pulse = one_pulse;
                        break;
                    }
                }
            }else{
                qDebug()<<"Chart::keyPressEvent- chart.title"<<this->title();
                pulse = PulseMapper::GetProximalOriginalPulseData(this, next_point.x(), next_point.y(), average_number);
            }
            int startNum = pulse.midFrequencyInitialPoint;
            int endNum = pulse.midFrequencyEndPoint;

            if(QApplication::keyboardModifiers()&Qt::ControlModifier){
                /*TODO:如果next_point.x()超出坐标轴的范围，需要进行判断*
                 *
                 */
                PulseGraphOperator::SelectMultiPointsInAllChartsWithKeyPress(charts_list_, next_point.x());
                //发送信号，槽函数将Ctrl+键盘左键选中的脉冲添加进列表
                emit AddKeyPressSelectedPulseInfoIntoList(pulse);
                //发送信号，槽函数将Ctrl+键盘左键选中的脉冲的脉内图像显示
                emit UpdateInternalPulseChartWithKeyPressPulse(startNum, endNum);
            }else{
                /*TODO:如果next_point.x()超出坐标轴的范围，需要进行判断*
                 *
                 */
                PulseGraphOperator::SelectSinglePointInAllChartsWithKeyPress(charts_list_, next_point.x());
                //发送信号，槽函数将键盘左键选中的脉冲添加进列表
                emit AddKeyPressSelectedPulseInfoIntoList(pulse);
                //发送信号，槽函数将键盘左键选中的脉冲的脉内图像显示
                emit UpdateInternalPulseChartWithKeyPressPulse(startNum, endNum);
            }
        }else{
            PulseGraphOperator::TransformAllChartsLeft(charts_list_);
        }
        break;
    case Qt::Key_Right:
        if(this->series2_->count() != 0){
            //键盘按下Right,选中已选点的右边的一个点next_point
            QPointF right_point(-1, 0);//series2最右端的点
            QPointF next_point;//下一个要选中的点
            int average_number = (this->axisx1_->max() - this->axisx1_->min()) / POINTNUMBER + 1;//横坐标每两个点相距average_number
            foreach(QPointF series1_point, this->series1_->points()){
                //找出series2中最右端的点
                foreach(QPointF series2_point, this->series2_->points()){
                    if(series2_point.x() > right_point.x())
                        right_point = series2_point;
                }
                if(series1_point.x() == right_point.x() + average_number)
                    next_point = series1_point;
            }
            InterPulse pulse;
            if(average_number == 1){
                foreach(InterPulse one_pulse, this->million_data_vector_){
                    if(one_pulse.pulse_id == next_point.x()){
                        pulse = one_pulse;
                        break;
                    }
                }
            }else{
                pulse = PulseMapper::GetProximalOriginalPulseData(this, next_point.x(), next_point.y(), average_number);
            }
            int startNum = pulse.midFrequencyInitialPoint;
            int endNum = pulse.midFrequencyEndPoint;

            if(QApplication::keyboardModifiers()&Qt::ControlModifier){
                /*TODO:如果next_point.x()超出坐标轴的范围，需要进行判断*
                 *
                 */
                PulseGraphOperator::SelectMultiPointsInAllChartsWithKeyPress(charts_list_, next_point.x());
                //发送信号，槽函数将Ctrl+键盘右键选中的脉冲添加进列表
                emit AddKeyPressSelectedPulseInfoIntoList(pulse);
                //发送信号，槽函数将Ctrl+键盘右键选中的脉冲的脉内图像显示
                emit UpdateInternalPulseChartWithKeyPressPulse(startNum, endNum);
            }else{
                /*TODO:如果next_point.x()超出坐标轴的范围，需要进行判断*
                 *
                 */
                PulseGraphOperator::SelectSinglePointInAllChartsWithKeyPress(charts_list_, next_point.x());
                //发送信号，槽函数将键盘右键选中的脉冲添加进列表
                emit AddKeyPressSelectedPulseInfoIntoList(pulse);
                //发送信号，槽函数将键盘右键选中的脉冲的脉内图像显示
                emit UpdateInternalPulseChartWithKeyPressPulse(startNum, endNum);
            }
        }else{
            PulseGraphOperator::TransformAllChartsRight(charts_list_);
        }
        break;
    default:
        QChart::keyPressEvent(event);
        break;
    }
}
void Chart::mousePressEvent(QGraphicsSceneMouseEvent *event){
    setFocus();
    qDebug()<<"chart--mousePressEvent";
    //鼠标左键点击chart任意空白处，取消所有选中图像
    if(QApplication::keyboardModifiers()&Qt::ControlModifier){
        //鼠标点击时按下Ctrl键，可能执行Ctrl+框选，所以这里不进行全部反选的操作
    }else{
        if(event->button() == Qt::LeftButton){
            PulseGraphOperator::UnSelectAllPointsInAllCharts(charts_list_, event);
            //发射信号，清空脉冲列表
            emit ClearPulseList();
        }
    }
    zoom_press_point_ = event->pos();
    zoom_press_point_inseries_ = mapToValue(zoom_press_point_, series1_);

    rubberband_press_point_ = event->pos();
    rubberband_press_point_inseries_ = mapToValue(rubberband_press_point_, series1_);
}
//根据鼠标滑动的距离放大缩小
void Chart::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    setFocus();
    qDebug()<<"chart-mouseReleaseEvent";
    if(event->button()==Qt::RightButton){
        zoom_release_point_ = event->pos();
        zoom_release_point_inseries_ = mapToValue(zoom_release_point_, series1_);
        move_length_ = zoom_release_point_.x() - zoom_press_point_.x();
        if(move_length_ >5){
            PulseGraphOperator::ZoomInAllCharts(charts_list_, zoom_press_point_inseries_.x(), zoom_release_point_inseries_.x());
            emit SettingParameterForScrollBar(charts_list_);
        }
        else if(move_length_ < -5){
            PulseGraphOperator::ZoomOutAllCharts(charts_list_, zoom_press_point_inseries_.x(), zoom_release_point_inseries_.x());
            emit SettingParameterForScrollBar(charts_list_);
        }
    }else if(event->button() == Qt::LeftButton){
        rubberband_release_point_ = event->pos();
        rubberband_release_point_inseries_ = mapToValue(rubberband_release_point_, series1_);
        //计算出框选中点的X坐标，存入RubberBandPoints_X数组中
        QVector<int> rubberband_points_x;
        foreach(QPointF series1_point, this->series1_->points()){
            if(series1_point.x() > qMin(rubberband_press_point_inseries_.x(), rubberband_release_point_inseries_.x()) &&
                    series1_point.x() < qMax(rubberband_press_point_inseries_.x(), rubberband_release_point_inseries_.x()) &&
                    series1_point.y() > qMin(rubberband_press_point_inseries_.y(), rubberband_release_point_inseries_.y()) &&
                    series1_point.y() < qMax(rubberband_press_point_inseries_.y(), rubberband_release_point_inseries_.y())){
                rubberband_points_x.append(series1_point.x());
            }
        }
        if(rubberband_points_x.size() != 0){
            PulseGraphOperator::SelectMultiPointsinAllChartsWithRubberBand(charts_list_, rubberband_points_x);
            //发送信号，槽函数将框选脉冲添加入列表
            emit AddRubberBandSelectedPulseInfoIntoList(this, rubberband_points_x);
            //发送信号，槽函数将框选的所有脉冲中的第一条脉内图像显示
            int minIndex = rubberband_points_x[0];
            foreach(int x, rubberband_points_x){
                if(x < minIndex)
                    minIndex = x;
            }
            int startNum = this->million_data_vector_[minIndex].midFrequencyInitialPoint;
            int endNum = this->million_data_vector_[minIndex].midFrequencyEndPoint;
            emit UpdateInternalPulseChartWithRBFirstPulse(startNum, endNum);
        }
    }

}
void Chart::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event){
    //setFocus();
    if(event->button() == Qt::RightButton){
        if(!doubleclick_){
            doubleclick_ = true;
            emit FullScreen();
            qDebug() << "Chart-doubleClick-fullScreen";
        }else{
            doubleclick_ = false;
            emit QuitFullScreen();
            qDebug() <<"Chart-doubleClick-quitFullScreen";
        }
    }
}
void Chart::wheelEvent(QGraphicsSceneWheelEvent *event){
    qreal chart_x_min = charts_list_->at(0)->axisx1_->min();
    qreal chart_x_max = charts_list_->at(0)->axisx1_->max();
    qreal new_chart_x_min=0;
    qreal new_chart_x_max=0;
    qreal mouse_pos_x = mapToValue(event->pos(), series1_).x();
    //根据滚轮滚动的方向判断是放大还是缩小
    if(event->delta() > 0){
        int length = chart_x_max - chart_x_min;

        if(length > 0 && length < 10000){
            new_chart_x_min = mouse_pos_x - length * 0.4;
            new_chart_x_max = mouse_pos_x + length * 0.4;
        }else if(length >= 10000 && length < 100000){
            new_chart_x_min = mouse_pos_x - length * 0.3;
            new_chart_x_max = mouse_pos_x + length * 0.3;
        }else if(length >= 100000 && length < 500000){
            new_chart_x_min = mouse_pos_x - length * 0.2;
            new_chart_x_max = mouse_pos_x + length * 0.2;
        }else if(length >= 500000){
            new_chart_x_min = mouse_pos_x - length * 0.1;
            new_chart_x_max = mouse_pos_x + length * 0.1;
        }
        //new_chart_x_min = mouse_pos_x + (chart_x_max - chart_x_min) * 0.1;
        //new_chart_x_max = mouse_pos_x - (chart_x_max - chart_x_min) * 0.1;
        PulseGraphOperator::ZoomInAllCharts(charts_list_, new_chart_x_min, new_chart_x_max);
        emit SettingParameterForScrollBar(charts_list_);
    }else{
//        new_chart_x_min = chart_x_min - (chart_x_max - chart_x_min) * 0.05;
//        new_chart_x_max = chart_x_max + (chart_x_max - chart_x_min) * 0.05;
        PulseGraphOperator::ZoomOutAllCharts(charts_list_, new_chart_x_min, new_chart_x_max);
        emit SettingParameterForScrollBar(charts_list_);
    }
}
