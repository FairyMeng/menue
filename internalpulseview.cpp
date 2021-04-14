#include "internalpulseview.h"
InternalPulseView::InternalPulseView(QWidget *parent) :
     QGraphicsView(parent)
{
    clientRect = QApplication::desktop()->availableGeometry(); //用户可用窗口大小
    internalChartsList= new QList<InternalPulseChart *>;
    internalScene=new Scene();
    internalScene->setSceneRect(0,0,clientRect.width()*0.4995,clientRect.height()*0.59);
    internalScene->setBackgroundBrush(QColor::fromRgb(69,69,69));

    internalScene->addItem(internalScene->interChart1_);
    internalScene->addItem(internalScene->interChart2_);
    internalScene->addItem(internalScene->interChart3_);
    internalScene->addItem(internalScene->interChart4_);

    internalChartsList->append(internalScene->interChart1_);
    internalChartsList->append(internalScene->interChart2_);
    internalChartsList->append(internalScene->interChart3_);
    internalChartsList->append(internalScene->interChart4_);
    this->setScene(internalScene);
    //设置internalview中的item左上角对齐,保持scene一直在视野中
    this->setAlignment(Qt::AlignLeft|Qt::AlignTop);
    //对InternalChart类中charts初始化
    for(int i=0;i<internalChartsList->size();i++){
        connect(internalChartsList->at(i),SIGNAL(InternalChartFullScreen()),this,SLOT(InternalChartFullScreenSlot()));
        connect(internalChartsList->at(i),SIGNAL(InternalChartQuitFullScreen()),this,SLOT(InternalChartQuitFullScreenSlot()));
    }
    //去掉脉内view的滚动条
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void InternalPulseView::mousePressEvent(QMouseEvent *event){
    qDebug()<<"internalPulseView--mousePressEvent";
    QGraphicsView::mousePressEvent(event);
}
void InternalPulseView::mouseReleaseEvent(QMouseEvent *event){
    qDebug()<<"internalPulseView--mouseRealseEvent";
    QGraphicsView::mouseReleaseEvent(event);
}
void InternalPulseView::mouseDoubleClickEvent(QMouseEvent *event){
    qDebug()<<"internalPulseVie-mousedoubleclickEvent";
    QGraphicsView::mouseDoubleClickEvent(event);
}
//处理InternalChart发出的信号
void InternalPulseView::InternalChartFullScreenSlot(){
    emit InternalPulseWindowFullScreen();
    qDebug()<<"InternalPulseView receive signal full";
}
void InternalPulseView::InternalChartQuitFullScreenSlot(){
    emit InternalPulseWindowQuitFullScreen();
    qDebug()<<"InternalPulseView receive signal quit";
}

void InternalPulseView::UpdateInternalChart(int startNum, int endNum){
    int x1 = 0;
    int num = 0;
    QVector<int> internalVector;
//    uchar *fpr = InternalPulseView::file->map(0, file->size());
//    if(fpr){

//    }else{
//        qDebug()<<"fpr = 0";
//    }
    char *pEnd = (char *)fpr;
    QTime time;
    time.start();
    for (int i = 0 ; ; i++)
    {
        //qDebug()<<"fpr["<<i<<"] = "<<fpr[i];
        if(fpr[i] == 10){
            //qDebug()<<"i = "<<i;
            num++;
            if(num < startNum){
               pEnd = (char *)fpr + i;
            }
            if(num > endNum){
                break;
            }
            if(num >= startNum){
                x1 = strtod(pEnd, &pEnd);
                internalVector.append(x1);
                //qDebug()<<x1;
            }
        }
    }
    qDebug()<<time.elapsed() / 1000<<"s";
    qDebug()<<"startNum and endNum:"<<startNum<<"、"<<endNum<<endl<<"internalVector.size = "<<internalVector.size();

    this->internalScene->interChart1_->axisx1_->setRange(startNum, endNum);
    PulseGraphDisplayer::disPlayInternalPulseGraph(startNum, endNum, internalVector, this->internalChartsList->at(0));
    qDebug()<<"InternalView update";

}

void InternalPulseView::resizeEvent(QResizeEvent *event){
    //has_double_clicked的真假表示是否有chart被右键双击要求放大
    bool has_double_clicked = false;
    for(int i = 0; i < internalChartsList->count(); i++){
        if(internalChartsList->at(i)->doubleclick_ == true){
            //将具有焦点的chart下标存储。
             focuschartnumber=i;
             has_double_clicked = true;
        }
    }
    if(has_double_clicked){
        //has_double_clicked=true，将被双击的chart放大，其余隐藏
        for(int i = 0; i < internalChartsList->count(); i++){
            if(internalChartsList->at(i)->doubleclick_ == true){
                internalChartsList->at(i)->setGeometry(0,-5, clientRect.width(), clientRect.height()*0.835);
            }else{
                internalChartsList->at(i)->hide();
            }
        }
    }else{
        //has_double_clicked=false,没有chart将要被放大，所有chart正常显示，且宽度随view的改变而改变
        for(int i = 0; i < internalChartsList->count(); i++){
            internalChartsList->at(i)->show();
            internalChartsList->at(i)->setGeometry(0, i*this->height()*0.248-5, this->width()*0.998, this->height()*0.268);
        }
    }
}
InternalPulseView::~InternalPulseView()
{
    delete internalScene;
}








