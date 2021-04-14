#include "view.h"
View::View(QWidget *parent)
    : QGraphicsView(parent)

{
    client_rect = QApplication::desktop()->availableGeometry(); //用户可用窗口大小
    scene_ = new Scene();
    scene_->setBackgroundBrush(QColor::fromRgb(69,69,69));
    scene_->setSceneRect(0,0,client_rect.width()*0.4995,client_rect.height()*0.63);

    scene_->addItem(scene_->chart1_);
    scene_->addItem(scene_->chart2_);
    scene_->addItem(scene_->chart3_);
    scene_->addItem(scene_->chart4_);
    scene_->addItem(scene_->chart5_);
    scene_->setFocusItem(scene_->chart1_);

    charts_list_ = new QList<Chart*>;
    charts_list_->append(scene_->chart1_) ;
    charts_list_->append(scene_->chart2_);
    charts_list_->append(scene_->chart3_);
    charts_list_->append(scene_->chart4_);
    charts_list_->append(scene_->chart5_);

    //对图像重置按钮的设置
    button_reset = new QPushButton();
    button_reset->setText(tr("重置"));
    scene_->addWidget(button_reset);
    button_reset->setFixedSize(45,20);
    button_reset->setStyleSheet("QPushButton{background-color:#373737;color:#00CCCC;border: 0px;}"
                                  "QPushButton:hover{border: 0px;background-color:#00FFFF;color:#000000;}");
    this->setWindowTitle("PulseCharacter");
    this->setScene(scene_);
    //去掉view的滚动条
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //设置view中的item左上角对齐,保持scene一直在视野中
    this->setAlignment(Qt::AlignLeft|Qt::AlignTop);
    //与mousemoveEvent搭配使用，用来确定那个脉间chart被聚焦，在hover情况下
    this->setMouseTracking(true);

    //框选时显示框线
    rubberband=NULL;
    //定时器初始化
    //timer=new QTimer(this);

    //设置一个scrollbar
    pulseview_sb = new QScrollBar();
    pulseview_sb->setOrientation(Qt::Horizontal);
    scene_->addWidget(pulseview_sb);
    pulseview_sb->setTracking(false);
    pulseview_sb->setRange(0,0);
//    pulseview_sb->setStyleSheet("QScrollBar:horizontal{background:#484848;background:#484848;border-radius:6px;max-height:12px;}"
//                                "QScrollBar::handle:horizontal{background:#525252;min-width:50px;border-radius:6px;}"
//                                "QScrollBar::handle:horizontal:hover{background:#242424;}"
//                                "QScrollBar::handle:horizontal:pressed{background:#242424;}");
    pulseview_sb->setStyleSheet("QScrollBar:horizontal{background:#484848;background:#484848;}"
                                "QScrollBar::handle:horizontal{background:#525252;}"
                                "QScrollBar::handle:horizontal:hover{background:#242424;}"
                                "QScrollBar::handle:horizontal:pressed{background:#242424;}");
//    pulseview_sb->setRange(0,1);
//    pulseview_sb->setValue(1);
//    pulseview_sb->setPageStep(1);



    for(int i = 0; i < charts_list_->count(); i++){
        charts_list_->at(i)->charts_list_ = charts_list_;//初始化每一个chart的list
        connect(charts_list_->at(i)->series1_,SIGNAL(clicked(QPointF)),this,SLOT(Series1HandleClickedPoint(QPointF)));
        connect(charts_list_->at(i)->series2_,SIGNAL(clicked(QPointF)),this,SLOT(Series2HandleClickedPoint(QPointF)));
        connect(charts_list_->at(i)->series1_,SIGNAL(hovered(QPointF,bool)),this,SLOT(Series1HandleHoverEnterPoint(QPointF,bool)));
        connect(charts_list_->at(i)->series1_,SIGNAL(hovered(QPointF,bool)),this,SLOT(Series1HandleHoverLeavePoint(QPointF,bool)));
        connect(charts_list_->at(i)->series2_,SIGNAL(hovered(QPointF,bool)),this,SLOT(Series2HandleHoverEnterPoint(QPointF,bool)));
        connect(charts_list_->at(i)->series2_,SIGNAL(hovered(QPointF,bool)),this,SLOT(Series2HandleHoverLeavePoint(QPointF,bool)));
        connect(charts_list_->at(i),SIGNAL(FullScreen()),this,SLOT(FullScreenSlot()));
        connect(charts_list_->at(i),SIGNAL(QuitFullScreen()),this,SLOT(QuitFullScreenSlot()));
        //放大缩小图像后设置滚动条参数
        connect(charts_list_->at(i),SIGNAL(SettingParameterForScrollBar(QList<Chart*>*)),this,SLOT(SettingParameterForScrollBar(QList<Chart*>*)));
    }
    //reset重置图像
    connect(button_reset,SIGNAL(pressed()),this,SLOT(on_button_reset_clicked()));
    //connect(pulseview_sb,SIGNAL(sliderReleased()),this,SLOT(TransformAllChartsWithScrollBar()));
    connect(pulseview_sb,SIGNAL(valueChanged(int)),this,SLOT(TransformAllChartsWithScrollBar()));

    //scrollbar信号关联
    //connect(pulseview_sb,SIGNAL(valueChanged(int)),pulseview_sb,SIGNAL(sliderReleased()));

}

//SLOT:处理series1的clicked信号
void View::Series1HandleClickedPoint(const QPointF &point){
    qreal xpos = point.x();//这里定义xpos作为传入参数，直接传入point.x()会出现点不能对应的错误
    double ypos = point.y();
    if(QApplication::keyboardModifiers()&&Qt::ControlModifier){
        PulseGraphOperator::SelectMultiPointsInAllChartsWithMouse(charts_list_, xpos);
    }else{
        PulseGraphOperator::SelectSinglePointInAllChartsWithMouse(charts_list_, xpos);
    }
    qDebug()<<"View::Series1HandleClickedPoint-clickedpoint.x = "<<xpos;
    qDebug()<<"View::Series1HandleClickedPoint-chart.title = "<<this->charts_list_->at(this->chartFocusNumberUnderHover)->title();
    int average_number = (charts_list_->at(0)->axisx1_->max() - charts_list_->at(0)->axisx1_->min()) / POINTNUMBER + 1;
    qDebug()<<"View::Series1HandleClickedPoint-average_number = "<<average_number;

    InterPulse proximalOriginalPulse;
    if(average_number == 1){
        foreach(InterPulse one_pulse, this->charts_list_->at(0)->million_data_vector_){
            if(one_pulse.pulse_id == xpos){
                proximalOriginalPulse = one_pulse;
                break;
            }
        }
    }else{
        proximalOriginalPulse = PulseMapper::GetProximalOriginalPulseData(this->charts_list_->at(this->chartFocusNumberUnderHover), xpos, ypos, average_number);
    }
    int startNum = proximalOriginalPulse.midFrequencyInitialPoint;
    int endNum = proximalOriginalPulse.midFrequencyEndPoint;
    emit Series1Clicked(startNum, endNum);
    emit Series1Clicked_List(proximalOriginalPulse);
    qDebug()<<this->scene_->focusItem();
}
//SLOT:处理series2的clicked信号
void View::Series2HandleClickedPoint(const QPointF &point){
    if(charts_list_->at(0)->series2_->points().count()>1){
        PulseGraphOperator::UnselectAtLeastTwoPointInAllCharts(charts_list_,point);
    }else{
        qDebug()<<"dian shu <=1";
    }
}
//SLOT:处理series1的hovered=true信号
void View::Series1HandleHoverEnterPoint(const QPointF &point, bool state){
    qDebug()<<"sereis1 hover enter";
    if(state){
        setCursor(Qt::CrossCursor);
        setToolTip(PulseGraphOperator::GetHoveredPointInfo(point,charts_list_,chartFocusNumberUnderHover));
        //qDebug()<<"sereis1 hover enter state = true";
    }
}

//SLOT:处理series1的hovered=false信号
void View::Series1HandleHoverLeavePoint(const QPointF &point, bool state){
    //QPointF hoveredPoint = point;
    if(!state){
        setCursor(Qt::ArrowCursor);
        setToolTip("");
       // qDebug()<<"series1 hover leave";
    }
}

//SLOT:处理series2的hovered=true信号
void View::Series2HandleHoverEnterPoint(const QPointF &point, bool state){
    if(state){
        setCursor(Qt::CrossCursor);
        setToolTip(PulseGraphOperator::GetHoveredPointInfo(point,charts_list_,chartFocusNumberUnderHover));
        //qDebug()<<"series2 hover enter";
    }
}

//SLOT:处理series2的hovered=false信号
void View::Series2HandleHoverLeavePoint(const QPointF &point, bool state){
    if(!state){
        setCursor(Qt::ArrowCursor);
        setToolTip("");
       // qDebug()<<"series2 hover leave";
    }
}
void View::keyPressEvent(QKeyEvent *event){
    qDebug()<<"view--keyPressEvent";
    QGraphicsView::keyPressEvent(event);
    Chart* currentFocusChart = dynamic_cast<Chart*>(scene_->focusItem());//获取s中当前具有焦点的chart
    if(currentFocusChart)
        qDebug()<<"currentFocusChart.title"<<currentFocusChart->title();
    else
       qDebug()<<"currentFocusChart.title = NULL";
}
void View::mousePressEvent(QMouseEvent *event){
    qDebug()<<"view--mousePressEvent";
    QGraphicsView::mousePressEvent(event);
    scene_->setFocusItem(charts_list_->at(focuschartnumber));
    //控制ctrl多选点
    if(QApplication::keyboardModifiers()&&Qt::ControlModifier){
        qDebug()<<"carry ctrl +clicked";
    }else{
        /*判断点的个数来选择执行不同方法，如果点数大于1则按照视图传递模式去执行view中的信号与槽
           如果点数==1，则去执行反选一个点的方法*/
        if(charts_list_->at(0)->series2_->points().count()>1){
            qDebug()<<"go to Series2HandleClicked";
        }else if(charts_list_->at(0)->series2_->points().count()==1){
            QPointF point_scene=mapToScene(event->pos());
            //转化为chart的x坐标，保持每个chart的x坐标都相同，已达到五图联动的效果
            QPointF point_scene_series=charts_list_->at(0)->mapToValue(point_scene,charts_list_->at(0)->series2_);
            //qDebug()<<"point scene series"<<point_scene_series;
            PulseGraphOperator::UnSelectPointInAllCharts(charts_list_,point_scene_series);
        }
    }
    //用于框选显示框线
     origin=event->pos();

}
void View::mouseMoveEvent(QMouseEvent *event){
       QGraphicsView::mouseMoveEvent(event);
       qreal yPoint=event->pos().y();
       //判断鼠标Hover状态下，chart的序号，用作chart中鼠标悬浮显示精确值
       if(yPoint>=(-5)&&yPoint<(this->height()*0.20)){
                   this->chartFocusNumberUnderHover=0;
                   charts_list_->at(0)->chartFocusNumberUnderHover=this->chartFocusNumberUnderHover;
                   //qDebug()<<"View-mouseMoveEvent:chart0";
               }else if(yPoint>=(this->height()*0.20)&&yPoint<(this->height()*0.20*2-5)){
                   this->chartFocusNumberUnderHover=1;
                   charts_list_->at(1)->chartFocusNumberUnderHover=this->chartFocusNumberUnderHover;
                   //qDebug()<<"View-mouseMoveEvent:chart1";
               }else if(yPoint>=(this->height()*0.20-5)&&yPoint<(this->height()*0.20*3-5)){
                   this->chartFocusNumberUnderHover=2;
                   charts_list_->at(2)->chartFocusNumberUnderHover=this->chartFocusNumberUnderHover;
                   //qDebug()<<"View-mouseMoveEvent:chart2";
               }else if(yPoint>=(this->height()*0.20*3-5)&&yPoint<(this->height()*0.20*4-5)){
                   this->chartFocusNumberUnderHover=3;
                   charts_list_->at(3)->chartFocusNumberUnderHover=this->chartFocusNumberUnderHover;
                   //qDebug()<<"View-mouseMoveEvent:chart3";
               }else{
                   this->chartFocusNumberUnderHover=4;
                   charts_list_->at(4)->chartFocusNumberUnderHover=this->chartFocusNumberUnderHover;
                   //qDebug()<<"View-mouseMoveEvent:chart4";
               }
       //显示框线
       if(event->buttons()){
           if(event->pos().y()<this->height()*0.975)
           {
              if(origin!=event->pos()){
                  if (!rubberband)
                      rubberband = new QRubberBand(QRubberBand::Rectangle, this);
                  rubberband->setGeometry(QRect(origin,QSize(200,80)));
                  rubberband->show();
                 }
              if(rubberband){
                  rubberband->setGeometry(QRect(origin,event->pos()).normalized());
              }else{
                  qDebug()<<"View-mouseMoveEvent:没有任何操作需要显示框线";
                   }
           }else
           {
               qDebug()<<"View-mouseMoveEvent:点在滚动条上了不显示框线";
           }
       }
}
void View::mouseReleaseEvent(QMouseEvent *event){
     qDebug()<<"view--mouseReleaseEvent";
     QGraphicsView::mouseReleaseEvent(event);
     if(rubberband){
         rubberband->hide();
     }
    // rubberband->hide();

}
void View::mouseDoubleClickEvent(QMouseEvent *event){
    QGraphicsView::mouseDoubleClickEvent(event);
}
//SLOT:处理chart发出的fullScreen()信号
void View::FullScreenSlot(){
    //this->setGeometry(0,0,client_rect.width()*0.998,client_rect.height()*0.998);
    qDebug()<<"View-FullSCreenSlot:receive full screen";
    emit FullScreen1();
}
//SLOT:处理chart发出的quitFullScreen()信号
void View::QuitFullScreenSlot(){
    //this->setGeometry(0,client_rect.height()*0.03,client_rect.width()*0.498,client_rect.height()*0.69);
    qDebug()<<"View-FullSCreenSlot:receive screen quit";
    emit QuitFullScreen1();
    //利用重新显示图像解决双击放大时，再次复原，只显示了点，无法正常显示坐标系的问题。
    if(this->charts_list_->at(0)->million_data_vector_.size() != 0){
        int axisx_start = charts_list_->at(0)->axisx1_->min();
        int axisx_end = charts_list_->at(0)->axisx1_->max();
        QVector<InterPulse> average_data_vector = PulseMapper::MapPulseTobePlotted(this->charts_list_->at(0)->million_data_vector_, axisx_start, axisx_end);
        int average_number = (axisx_end - axisx_start) / POINTNUMBER + 1;
        PulseGraphDisplayer::displayInterPulseGraph(average_data_vector, average_number, axisx_start, this->charts_list_);
    }
}
void View::resizeEvent(QResizeEvent *event){
        //判断一下chart_list.count()的大小，如果等于4则四图平分没有方位图，如果
              //等于5则五图平分有方位图
    if(charts_list_->count()==5){
        //has_double_clicked的真假表示是否有chart被右键双击要求放大
        bool has_double_clicked = false;
        for(int i = 0; i < charts_list_->count(); i++){
            if(charts_list_->at(i)->doubleclick_ == true){
                //将具有焦点的chart下标存储。
                 focuschartnumber=i;
                 has_double_clicked = true;
            }
        }
        if(has_double_clicked){
            //has_double_clicked=true，将被双击的chart放大，其余隐藏
            for(int i = 0; i < charts_list_->count(); i++){
                if(charts_list_->at(i)->doubleclick_ == true){
                    charts_list_->at(i)->setGeometry(0, -5, client_rect.width(), this->height());
                    //label_list_->at(i)->moe(23,10);//让对应的label在对应位置显示
                    button_reset->move(client_rect.width()-50,4);
                    pulseview_sb->setGeometry(0,this->height()-15,this->width(),15);

                }else{
                    charts_list_->at(i)->hide();

                }
            }
        }else{

            //has_double_clicked=false,没有chart将要被放大，所有chart正常显示，且宽度随view的改变而改变
            for(int i = 0; i < charts_list_->count(); i++){

                charts_list_->at(i)->show();
                charts_list_->at(i)->setGeometry(0, i * this->height()*0.196-5, this->width(), this->height()*0.216);
                button_reset->move(this->width()-50,4);
                pulseview_sb->setGeometry(0,this->height()-15,this->width(),15);
            }

        }
    }
    else{     //等于4的情况
        //has_double_clicked的真假表示是否有chart被右键双击要求放大
        bool has_double_clicked = false;
        for(int i = 0; i < charts_list_->count(); i++){
            if(charts_list_->at(i)->doubleclick_ == true){
                //将具有焦点的chart下标存储。
                 focuschartnumber=i;
                 has_double_clicked = true;
            }
        }
        if(has_double_clicked){
            //has_double_clicked=true，将被双击的chart放大，其余隐藏
            for(int i = 0; i < charts_list_->count(); i++){
                if(charts_list_->at(i)->doubleclick_ == true){
                    charts_list_->at(i)->setGeometry(0, -5, client_rect.width(), this->height());
                    //label_list_->at(i)->moe(23,10);//让对应的label在对应位置显示
                    button_reset->move(client_rect.width()-50,4);
                    pulseview_sb->setGeometry(0,this->height()-15,this->width(),15);

                }else{
                    charts_list_->at(i)->hide();

                }
            }
        }else{

            //has_double_clicked=false,没有chart将要被放大，所有chart正常显示，且宽度随view的改变而改变
            for(int i = 0; i < charts_list_->count(); i++){

                charts_list_->at(i)->show();
                charts_list_->at(i)->setGeometry(0, i * this->height()*0.245-5, this->width(), this->height()*0.265);
                button_reset->move(this->width()-50,4);
                pulseview_sb->setGeometry(0,this->height()-15,this->width(),15);
            }

        }

    }
}

void View::wheelEvent(QWheelEvent *event){
    QGraphicsView::wheelEvent(event);
}
void View::on_button_reset_clicked(){
    qDebug()<<"View-on_button_reset_clicked:resetPulseGraph";
    emit resetPulseGraph();
}

//设置平移滚动条参数
void View::SettingParameterForScrollBar(QList<Chart *> *list){
    int axisx_start =list->at(0)->axisx1_->min();
    int axisx_end =list->at(0)->axisx1_->max();
    pulseview_sb->setRange(0,list->at(0)->million_data_vector_.size()/(axisx_end-axisx_start));
    pulseview_sb->setValue(axisx_start/(axisx_end-axisx_start));

    pulseview_sb->setPageStep(10);
    pulseview_sb->setSingleStep(2);
 }
//滚动条平移图像
void View::TransformAllChartsWithScrollBar(){

    int axisx_start = charts_list_->at(0)->axisx1_->min();
    int axisx_end = charts_list_->at(0)->axisx1_->max();
    int length = axisx_end-axisx_start;
    float last_data = charts_list_->at(0)->million_data_vector_.size()/length;//为了获取因取整丢失掉的小数部分

    if(pulseview_sb->value()<pulseview_sb->maximum())
    {
        axisx_start = pulseview_sb->value()*length;
        axisx_end = axisx_start+length;
    }
    else
    {
        axisx_start = (pulseview_sb->value()-1)*length;
        axisx_end = axisx_start+length*(1+last_data-(int)last_data);//当滚动条移动到最后时补全丢失的小数部分数据
    }

    int average_number;
    if(axisx_end - axisx_start <= POINTNUMBER){
        average_number= 1;
    }else{
        average_number = (axisx_end - axisx_start) / POINTNUMBER+1;
    }
    if(axisx_end - axisx_start > 0 && axisx_start > 0){
         //求平均值
         QVector<InterPulse> average_data_vector = PulseMapper::MapPulseTobePlotted(charts_list_->at(0)->million_data_vector_, axisx_start, axisx_end);
         for(int i = 0; i < charts_list_->count(); i++){
             PulseGraphOperator::TransformChartLeft(charts_list_->at(i), axisx_start, axisx_end);
         }
         PulseGraphDisplayer::displayInterPulseGraphWithScrollBar(average_data_vector, average_number, axisx_start, charts_list_);
    }

}
View::~View()
{
     delete charts_list_;
     delete scene_;
}
