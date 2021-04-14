#include "scene.h"
//#include "databaseoperator.h"

Scene::Scene(QGraphicsScene *parent)
    :QGraphicsScene(parent)
{
    //QVector<interpulse> millionData = DB_T_InterPulse::findInterPulseData();
    //QVector<InterPulse> million_data_vector;
    setBackgroundBrush(Qt::gray);
    //脉间chart
    chart1_ = new Chart();
    chart2_ = new Chart();
    chart3_ = new Chart();
    chart4_ = new Chart();
    chart5_ = new Chart();
    //脉内chart
    interChart1_=new InternalPulseChart();
    interChart2_=new InternalPulseChart();
    interChart3_=new InternalPulseChart();
    interChart4_=new InternalPulseChart();
    //设置脉间，脉内chart属性
    setAllChartProperties();
    //给所有chart(脉间，脉内)设置边距
    setAllChartsMargins();
}
//设置脉间，脉内chart属性
void Scene::setAllChartProperties(){
    //脉间
    chart1_->setTitle("Frequency");
    chart2_->setTitle("DTOA");
    chart3_->setTitle("Width");
    chart4_->setTitle("Amp");
    chart5_->setTitle("Direction");

    chart1_->setTitleBrush(QColor::fromRgb(0, 204, 204));
    chart2_->setTitleBrush(QColor::fromRgb(0, 204, 204));
    chart3_->setTitleBrush(QColor::fromRgb(0, 204, 204));
    chart4_->setTitleBrush(QColor::fromRgb(0, 204, 204));
    chart5_->setTitleBrush(QColor::fromRgb(0, 204, 204));

    chart1_->setBackgroundBrush(QColor::fromRgb(55, 55, 55));
    chart2_->setBackgroundBrush(QColor::fromRgb(55, 55, 55));
    chart3_->setBackgroundBrush(QColor::fromRgb(55, 55, 55));
    chart4_->setBackgroundBrush(QColor::fromRgb(55, 55, 55));
    chart5_->setBackgroundBrush(QColor::fromRgb(55, 55, 55));

    //脉内
    interChart1_->setTitle("Intermediate frequency waveform");
    interChart2_->setTitle("frequency spectrum");
    interChart3_->setTitle("time-frequency");
    interChart4_->setTitle("phase difference");

    interChart1_->setBackgroundBrush(QColor::fromRgb(55, 55, 55));
    interChart2_->setBackgroundBrush(QColor::fromRgb(55, 55, 55));
    interChart3_->setBackgroundBrush(QColor::fromRgb(55, 55, 55));
    interChart4_->setBackgroundBrush(QColor::fromRgb(55, 55, 55));

    interChart1_->setTitleBrush(QColor::fromRgb(0, 204, 204));
    interChart2_->setTitleBrush(QColor::fromRgb(0, 204, 204));
    interChart3_->setTitleBrush(QColor::fromRgb(0, 204, 204));
    interChart4_->setTitleBrush(QColor::fromRgb(0, 204, 204));
}
//给脉间chart设置边距，使得最大显示
void Scene::setAllChartsMargins(){
    //脉间
    chart1_->setMargins(QMargins(0,0,0,0));
    chart2_->setMargins(QMargins(0,0,0,0));
    chart3_->setMargins(QMargins(0,0,0,0));
    chart4_->setMargins(QMargins(0,0,0,0));
    chart5_->setMargins(QMargins(0,0,0,0));
    //脉内
    interChart1_->setMargins(QMargins(0,0,0,0));
    interChart2_->setMargins(QMargins(0,0,0,0));
    interChart3_->setMargins(QMargins(0,0,0,0));
    interChart4_->setMargins(QMargins(0,0,0,0));
}
void Scene::keyPressEvent(QKeyEvent *event){
    qDebug()<<"scene--keyPressEvent";
    QGraphicsScene::keyPressEvent(event);
}
void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event){
    qDebug()<<"scene--mousePressEvent";
    QGraphicsScene::mousePressEvent(event);
}
void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    qDebug()<<"scene--mouseReleaseEvent";
    QGraphicsScene::mouseReleaseEvent(event);
}

void Scene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event){
    QGraphicsScene::mouseDoubleClickEvent(event);
}
void Scene::wheelEvent(QGraphicsSceneWheelEvent *event){
    QGraphicsScene::wheelEvent(event);
}
//Scene::~Scene(){
//    delete chart1_;
//    delete chart2_;
//    delete chart3_;
//    delete chart4_;
//    delete chart5_;
//}

















































