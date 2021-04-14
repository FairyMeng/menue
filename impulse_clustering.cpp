#include "impulse_clustering.h"
#include "ui_impulse_clustering.h"
impulse_Clustering::impulse_Clustering(QWidget *parent) :
    BaseWindow(parent),
    ui(new Ui::impulse_Clustering)
{
    //设置qss样式
        {
        QFile file(":/qss/impulse_Clustering.css");
        file.open(QFile::ReadOnly);
        QTextStream filetext(&file);
        QString stylesheet = filetext.readAll();
        this->setStyleSheet(stylesheet);
        file.close();
        }
        ui->setupUi(this);
        // 初始化标题栏;
        initTitleBar();
        setSupportStretch(true);
        //脉冲聚类列表布局
        ui->tableWidget_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->tableWidget_4->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        clientRect = QApplication::desktop()->availableGeometry(); //用户可用窗口大小
        installEventFilter(this);
}

impulse_Clustering::~impulse_Clustering()
{
    delete ui;
}

void impulse_Clustering::initTitleBar()
{
    // 设置标题栏跑马灯效果，可以不设置;
    m_titleBar->setTitleRoll();

    m_titleBar->setTitleIcon(":/new/icon/images/app_icon.jpg");
    m_titleBar->setTitleContent(QString("  脉冲主窗"));
    m_titleBar->setButtonType(MIN_MAX_BUTTON);
    m_titleBar->setTitleWidth(this->width());

}

//鼠标点选点加入脉冲聚类列表
void impulse_Clustering::AddMouseSelectedPulseInfoIntoListSlots(InterPulse proximalOriginalPulse){
//    InterPulse pulse;
//    foreach(InterPulse one_pulse, this->million_data_vector_){
//        if(one_pulse.pulse_id == point_x){
//            pulse = one_pulse;
//            break;
//        }
//    }
//    QVector<InterPulse> originalPulseVector = PulseMapper::GetOriginalPulseData(chart, point_x, average_number);
//    InterPulse pulse = PulseMapper::GetProximalOriginalPulseData(chart, point_x, point_y, average_number);

    InterPulse pulse = proximalOriginalPulse;
    if(QApplication::keyboardModifiers()&Qt::ControlModifier){

    }else{
        ui->tableWidget_4->setRowCount(0);
    }
    ui->tableWidget_4->setRowCount(ui->tableWidget_4->rowCount() + 1);
    ui->tableWidget_4->setItem(ui->tableWidget_4->rowCount() - 1, 0, new QTableWidgetItem(QString::number(pulse.pulse_id)));
    ui->tableWidget_4->setItem(ui->tableWidget_4->rowCount() - 1, 1, new QTableWidgetItem(QString::number(0)));
    ui->tableWidget_4->setItem(ui->tableWidget_4->rowCount() - 1, 2, new QTableWidgetItem(QString::number(pulse.frequency,'g',8)));
    ui->tableWidget_4->setItem(ui->tableWidget_4->rowCount() - 1, 3, new QTableWidgetItem(QString::number(pulse.pulse_width,'g',8)));
    ui->tableWidget_4->setItem(ui->tableWidget_4->rowCount() - 1, 4, new QTableWidgetItem(QString::number(pulse.pulse_amp,'g',8)));
    ui->tableWidget_4->setItem(ui->tableWidget_4->rowCount() - 1, 5, new QTableWidgetItem(QString::number(pulse.toa,'g',8)));
    ui->tableWidget_4->setItem(ui->tableWidget_4->rowCount() - 1, 6, new QTableWidgetItem(QString::number(pulse.dtoa,'g',8)));
}
//键盘选点加入脉冲聚类列表
void impulse_Clustering::AddKeyPressSelectedPulseInfoIntoListSlots(InterPulse pulse){
    if(QApplication::keyboardModifiers()&Qt::ControlModifier){

    }else{
        ui->tableWidget_4->setRowCount(0);
    }
    ui->tableWidget_4->setRowCount(ui->tableWidget_4->rowCount() + 1);
    ui->tableWidget_4->setItem(ui->tableWidget_4->rowCount() - 1, 0, new QTableWidgetItem(QString::number(pulse.pulse_id)));
    ui->tableWidget_4->setItem(ui->tableWidget_4->rowCount() - 1, 1, new QTableWidgetItem(QString::number(0)));
    ui->tableWidget_4->setItem(ui->tableWidget_4->rowCount() - 1, 2, new QTableWidgetItem(QString::number(pulse.frequency,'g',8)));
    ui->tableWidget_4->setItem(ui->tableWidget_4->rowCount() - 1, 3, new QTableWidgetItem(QString::number(pulse.pulse_width,'g',8)));
    ui->tableWidget_4->setItem(ui->tableWidget_4->rowCount() - 1, 4, new QTableWidgetItem(QString::number(pulse.pulse_amp,'g',8)));
    ui->tableWidget_4->setItem(ui->tableWidget_4->rowCount() - 1, 5, new QTableWidgetItem(QString::number(pulse.toa,'g',8)));
    ui->tableWidget_4->setItem(ui->tableWidget_4->rowCount() - 1, 6, new QTableWidgetItem(QString::number(pulse.dtoa,'g',8)));
    //ui->tableWidget_4->setItem(ui->tableWidget_4->rowCount() - 1, 7, new QTableWidgetItem(QString::number(pulse.pulse_direction)));
}

//把框选选中的点加入到脉冲列表中
void impulse_Clustering::AddRubberBandSelectedPulseInfoIntoListSlots(Chart *chart, QVector<int> rubberband_points_x){
//    QVector<InterPulse> pulse_vector;
    qDebug()<<"rubberbandvector"<<rubberband_points_x.size();
//    for(int i = 0; i < rubberband_points_x.size(); i++){
//        foreach(InterPulse one_pulse, this->million_data_vector_){
//            if(one_pulse.pulse_id == rubberband_points_x[i]){
//                pulse_vector.append(one_pulse);
//                break;
//            }
//        }
//    }
//    ui->tableWidget_4->setRowCount(0);
    if(QApplication::keyboardModifiers()&Qt::ControlModifier){

    }else{
        ui->tableWidget_4->setRowCount(0);
    }
    //qDebug()<<"im_clustering-pulsevector"<<pulse_vector.size();
    int average_number = (chart->axisx1_->max() - chart->axisx1_->min()) / POINTNUMBER + 1;//横坐标每两个点相距average_number
    for(int i = 0; i < rubberband_points_x.size(); i++){
        QVector<InterPulse> originalPulseVector = PulseMapper::GetOriginalPulseData(chart, rubberband_points_x[i], average_number);
        foreach(InterPulse pulse, originalPulseVector){
            ui->tableWidget_4->setRowCount(ui->tableWidget_4->rowCount() + 1);
            ui->tableWidget_4->setItem(ui->tableWidget_4->rowCount() - 1, 0, new QTableWidgetItem(QString::number(pulse.pulse_id)));
            ui->tableWidget_4->setItem(ui->tableWidget_4->rowCount() - 1, 1, new QTableWidgetItem(QString::number(0)));
            ui->tableWidget_4->setItem(ui->tableWidget_4->rowCount() - 1, 2, new QTableWidgetItem(QString::number(pulse.frequency,'g',8)));
            ui->tableWidget_4->setItem(ui->tableWidget_4->rowCount() - 1, 3, new QTableWidgetItem(QString::number(pulse.pulse_width,'g',8)));
            ui->tableWidget_4->setItem(ui->tableWidget_4->rowCount() - 1, 4, new QTableWidgetItem(QString::number(pulse.pulse_amp,'g',8)));
            ui->tableWidget_4->setItem(ui->tableWidget_4->rowCount() - 1, 5, new QTableWidgetItem(QString::number(pulse.toa,'g',8)));
            ui->tableWidget_4->setItem(ui->tableWidget_4->rowCount() - 1, 6, new QTableWidgetItem(QString::number(pulse.dtoa,'g',8)));
            //ui->tableWidget_4->setItem(ui->tableWidget_4->rowCount() - 1, 6, new QTableWidgetItem(QString::number(pulse.pulse_direction,'g',8)));
        }
    }
}

//清空脉冲列表
void impulse_Clustering::ClearPulseListSlot(){
    ui->tableWidget_4->setRowCount(0);
}

//将所有脉冲添加到脉冲列表
void impulse_Clustering::AddAllPulseInfoIntoListSlot(QVector<InterPulse> allPulseVector){
    ui->tableWidget_4->setRowCount(0);
    int i = 0;
    foreach(InterPulse pulse, allPulseVector){
        i++;
        if(i % 10000 == 0){
            qDebug()<<"alerady add"<<i<<"pulse into list";
        }
        ui->tableWidget_4->setRowCount(ui->tableWidget_4->rowCount() + 1);
        ui->tableWidget_4->setItem(ui->tableWidget_4->rowCount() - 1, 0, new QTableWidgetItem(QString::number(pulse.pulse_id)));
        ui->tableWidget_4->setItem(ui->tableWidget_4->rowCount() - 1, 1, new QTableWidgetItem(QString::number(0)));
        ui->tableWidget_4->setItem(ui->tableWidget_4->rowCount() - 1, 2, new QTableWidgetItem(QString::number(pulse.frequency,'g',8)));
        ui->tableWidget_4->setItem(ui->tableWidget_4->rowCount() - 1, 3, new QTableWidgetItem(QString::number(pulse.pulse_width,'g',8)));
        ui->tableWidget_4->setItem(ui->tableWidget_4->rowCount() - 1, 4, new QTableWidgetItem(QString::number(pulse.pulse_amp,'g',8)));
        ui->tableWidget_4->setItem(ui->tableWidget_4->rowCount() - 1, 5, new QTableWidgetItem(QString::number(pulse.toa,'g',8)));
        ui->tableWidget_4->setItem(ui->tableWidget_4->rowCount() - 1, 6, new QTableWidgetItem(QString::number(pulse.dtoa,'g',8)));
    }
}

bool impulse_Clustering::eventFilter(QObject *watched, QEvent *event){
    //当窗口状态变为激活状态时，发射信号，槽函数将所有窗口提升到堆叠层次最上面
    if(watched == this){
        //过滤窗口激活事件
        if(event->type() == QEvent::WindowActivate){
            qDebug()<<"Impulse_ClusteringWindow activate";
            emit AllWindowActivate(this);
        }
        //过滤窗口的鼠标松开事件
        if(QEvent::MouseButtonRelease == event->type()){
            emit MouseRelease(this);
        }
    }
    return BaseWindow::eventFilter(watched, event);
}

void impulse_Clustering::closeEvent(QCloseEvent *event)
{
    emit impulse_Clustering_close();
}



