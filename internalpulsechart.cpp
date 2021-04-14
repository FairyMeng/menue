#include "internalpulsechart.h"

InternalPulseChart::InternalPulseChart(){
    doubleclick_=false;
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
    this->setAxisX(axisx1_, series2_);

    this->legend()->hide();
}
void InternalPulseChart::keyPressEvent(QKeyEvent *event){
    QChart::keyPressEvent(event);
}
void InternalPulseChart::mousePressEvent(QGraphicsSceneMouseEvent *event){
   qDebug()<<"InternalPulseChart-- mousePressEvent";
}
void InternalPulseChart::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    qDebug()<<"InternalPulseChart--mouseReleaseEvent";
}
void InternalPulseChart:: mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event){
    if(event->button() == Qt::RightButton){
        if(!doubleclick_){
            doubleclick_ = true;
            emit InternalChartFullScreen();
            qDebug() << "InternalChart-doubleClick-fullScreen";
        }else{
            doubleclick_ = false;
            emit InternalChartQuitFullScreen();
            qDebug() <<"InternalChart-doubleClick-quitFullScreen";
        }
    }

}
