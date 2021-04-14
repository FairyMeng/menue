#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qwidget.h"
#include <QDebug>
#include "pulsemapper.h"
#include <QScatterSeries>
#include <QThread>
MainWindow::MainWindow(QWidget *parent) :
    BaseWindow(parent),
    ui(new Ui::MainWindow)
{
    //设置qss样式
        {
        QFile file(":/qss/mainwindow.css");
        file.open(QFile::ReadOnly);
        QTextStream filetext(&file);
        QString stylesheet = filetext.readAll();
        this->setStyleSheet(stylesheet);
        file.close();
        }

    ui->setupUi(this);
    initTitleBar();
    setSupportStretch(true);

    pulse_window_flag=false;
    internal_pulse_window_flag=false;
    impulse_clustering_flag=false;

    tab_tool_hide_flag=false;
    imp_form_show_flag=false;


    InterPulseFileIsLoadedFinish=false;

    windowStackedLayer[0] = NULL;
    windowStackedLayer[1] = NULL;
    windowStackedLayer[2] = NULL;
    windowStackedLayer[3] = NULL;

    //对toolbutton设置提示框，显示提示内容
        ui->toolButton_cgb->setToolTip("草稿本");
        ui->toolButton_mcjrcc->setToolTip("根据复制进入还是隐藏<br>进入选项，将选中脉冲<br>放入次窗口图表中。首<br>次进入就新建次窗口，<br>非首次则在已有数据基<br>础上加入本次选中脉冲<br>显示。");
        ui->toolButton_mcfhzc->setToolTip("如果之前是‘复制进入’<br>则直接在次窗口图表中<br>删除选中脉冲，如果之前<br>是‘隐藏进入’，则在次窗<br>口图表中删除选中脉冲<br>并在主窗口中将本次返<br>回的选中脉冲恢复显示");
        ui->toolButton_mcfx->setToolTip("拉框选中脉冲后，点击<br>脉冲反选后，未被选中的脉<br>冲显示为选中，选中的显示<br>为未选中。");
        ui->toolButton_mctc->setToolTip("剔除选中脉冲。");
        ui->toolButton_mcdb->setToolTip("脉冲对比");
        ui->toolButton_mcyc->setToolTip("对脉间时序特征中的脉<br>冲进行隐藏");
        ui->toolButton_hfxs->setToolTip("针对脉冲隐藏，点击后<br>恢复显示所有隐藏脉冲。");
        ui->toolButton_ppss->setToolTip("选中需要匹配的脉冲序<br>列后点击匹配搜索出现<br>匹配搜索界面");
        ui->toolButton_glsz->setToolTip("过滤设置");
        ui->toolButton_mccc->setToolTip("脉冲存储");
        ui->toolButton_mjjjc->setToolTip("选中脉冲后点击即将其<br>按照显示要求放入脉间<br>距窗口，可反复加入。");
        ui->toolButton_txxlzbjc->setToolTip("启动精测频窗口，并将<br>选中脉冲复制入窗口内，<br>当已经启动后，可通过在<br>选中脉冲后再次点击加<br>入新脉冲，已经加入了<br>的不再重复加入。");
        ui->toolButton_xlgdprijc->setToolTip("选中脉冲后点击即将其<br>按照显示要求放入精测PRI<br>窗口，可反复加入");
        ui->toolButton_xlppfx->setToolTip("选中脉冲后点击即将<br>放入序列频谱分析窗口，可<br>反复加入");
        ui->toolButton_ldxngj->setToolTip("雷达性能估计");
        ui->toolButton_szjs->setToolTip("数值计算");
        ui->toolButton_sjnh->setToolTip("数据拟合");
        ui->toolButton_cldx->setToolTip("测量点/线");
        ui->toolButton_qttd->setToolTip("全图拖动");
        ui->toolButton_zzgj->setToolTip("指针工具");
        ui->toolButton_pmjt->setToolTip("屏幕截图");
        ui->toolButton_wdmk->setToolTip("文档模板");
        ui->toolButton_cgb->setToolTip("草稿本");
        ui->toolButton_zfxc->setToolTip("主信息窗口");
        ui->toolButton_cfxc->setToolTip("次分析窗口");
        ui->toolButton_PulseWindow->setToolTip("脉间时序特征");
        ui->toolButton_InternalPulseWindow->setToolTip("脉冲内部特征");
        ui->toolButton_Impulse_Clustering->setToolTip("信号列表");

        //设置按钮选中后被标记
        ui->toolButton_PulseWindow->setCheckable(true);
        ui->toolButton_zfxc->setCheckable(true);
        ui->toolButton_cfxc->setCheckable(true);
        ui->toolButton_InternalPulseWindow->setCheckable(true);
        ui->toolButton_Impulse_Clustering->setCheckable(true);




    //设置tabWidget的默认标签为第一个
        ui->tabWidget->setCurrentIndex(0);

    clientRect = QApplication::desktop()->availableGeometry(); //用户可用窗口大小
    timerForLoadInternalPulseFile=new QTimer(this);
    //载入文件子界面
    QObject::connect(ui->toolButton_zrwjzs,SIGNAL(clicked()),this,SLOT(openwindow_childSlots()));
    //同行序列载频精测界面
    sequence=new  SequenceFrequency();
    QObject::connect(ui->toolButton_txxlzbjc,SIGNAL(clicked()),this,SLOT(openSequenceFrequency()));
    //雷达性能估计界面
    radarperform =new RadarPerform();
    QObject::connect(ui->toolButton_ldxngj,SIGNAL(clicked()),this,SLOT(openRadarPerformSlots()));
    //数据整合界面
    datafit=new Datafit();
    QObject::connect(ui->toolButton_sjnh,SIGNAL(clicked()),this,SLOT(openDataFitting()));
    //测量点线界面
    pointline=new PiontLineMeasuring();
    QObject::connect(ui->toolButton_cldx,SIGNAL(clicked()),this,SLOT(openPointLineMeasuring()));
    //数值计算界面
    shuzhi1=new shuzhijisuan1();
    QObject::connect(ui->toolButton_szjs,SIGNAL(clicked()),this,SLOT(openshuzhijisuan1()));
    //频谱分析
    pingpufenxi=new  xvliepingpufenxi();
    QObject::connect(ui->toolButton_xlppfx,SIGNAL(clicked()),this,SLOT(openxvliepingpufenxiSlots()));
    //数据处理
    datahanding=new  shujuchuli();
    QObject::connect(ui->toolButton_sjcl,SIGNAL(clicked()),this,SLOT(openshujuchuliSlots()));
    pulse_storage=new  maichongcunchu();
    //脉冲存储
    QObject::connect(ui->toolButton_mccc,SIGNAL(clicked()),this,SLOT(openmaichongcunchuSlots()));
    pulse_spacing_measurement=new  maijianjujingce();
    //脉间距精测
    QObject::connect(ui->toolButton_mjjjc,SIGNAL(clicked()),this,SLOT(openmaijianjujingceSlots()));
    PRI_measurement=new PRIjingce();
    //序列固定PRI精测
    QObject::connect(ui->toolButton_xlgdprijc,SIGNAL(clicked()),this,SLOT(openprijingceSlots()));
    //过滤设置
    guolv =new guolvshezhi();
    QObject::connect(ui->toolButton_glsz,SIGNAL(clicked()),this,SLOT(openguolvshezhiSlots()));
    //草稿本
    caogao=new caogaoben();
    QObject::connect(ui->toolButton_cgb,SIGNAL(clicked()),this,SLOT(openCaoGaoBen()));
    //脉冲对比
    imcon=new Imcontrast();
    QObject::connect(ui->toolButton_mcdb,SIGNAL(clicked()),this,SLOT(openImpluseContrast()));
    //匹配搜索
    match=new matchsearch();
    QObject::connect(ui->toolButton_ppss,SIGNAL(clicked()),this,SLOT(openMatchSearch()));

    //选择载入文件，弹出载入文件窗口
    mywin=new mywindow();
    QObject::connect(mywin, SIGNAL(openFile(QFileInfo)), this, SLOT(openFileSlots(QFileInfo)));
    //直接在mainwindow界面根据文件序号，快捷载入文件
    QObject::connect(this,SIGNAL(openFile(QFileInfo)), this, SLOT(openFileSlots(QFileInfo)));
    //使用chart上的reset按钮实现还原到初始数据
    QObject::connect(&imp_s.view_,SIGNAL(resetPulseGraph()),this,SLOT(chart_data_reset()));

    //将鼠标选中脉冲的信号与添加选中脉冲信息入列表的槽函数连接
//    for(int i = 0; i < 5; i++){
//        QObject::connect(imp_s.view_.charts_list_->at(i)->series1_, SIGNAL(clicked(QPointF)),&imp_Clu, SLOT(AddMouseSelectedPulseInfoIntoListSlots(QPointF)));
//    }
    QObject::connect(&imp_s.view_, SIGNAL(Series1Clicked_List(InterPulse)), &imp_Clu, SLOT(AddMouseSelectedPulseInfoIntoListSlots(InterPulse)));
    /*将键盘选中脉冲的信号与添加选中脉冲信息入列表的槽函数连接
     将框选脉冲的信号与添加选中脉冲信息入列表的槽函数连接*/
    for(int i = 0; i <imp_s.view_.charts_list_->size(); i++){
        QObject::connect(imp_s.view_.charts_list_->at(i), SIGNAL(AddKeyPressSelectedPulseInfoIntoList(InterPulse)),&imp_Clu, SLOT(AddKeyPressSelectedPulseInfoIntoListSlots(InterPulse)));
        QObject::connect(imp_s.view_.charts_list_->at(i), SIGNAL(AddRubberBandSelectedPulseInfoIntoList(Chart*,QVector<int>)),&imp_Clu, SLOT(AddRubberBandSelectedPulseInfoIntoListSlots(Chart*,QVector<int>)));
    }
    //关闭其中的某个窗口，其他窗口扩展显示
    connect(&imp_s,SIGNAL(PulseWindow_close()),this,SLOT(PulseWindow_closeSlots()));
    connect(&interWindow,SIGNAL(InternalPulseWindow_close()),this,SLOT(InternalPulseWindow_closeSlots()));
    connect(&imp_Clu,SIGNAL(impulse_Clustering_close()),this,SLOT(impulse_Clustering_closeSlots()));

    //关闭mywindow
    connect(mywin,SIGNAL(closeMyWindow()),this,SLOT(mywindow_CloseSlot()));

    //QSpinBox中文件序号发生改变,将相应的文件信息显示到mainwindow界面上方
    connect(ui->spinBox_fileNumber,SIGNAL(valueChanged(int)),this,SLOT(spinBox_setValue(int)));

    //mainwindow界面快速载入文件
    connect(ui->pushButton_quickLoadFile,SIGNAL(clicked()),this,SLOT(pushButton_QuickLoadFileSlot()));
    //载入脉内文件
    //connect(this,SIGNAL(openInternalPulseFile()),this,SLOT(openInternalPulseFileSlot()));

    //鼠标点击显示脉内图像
    connect(&(imp_s.view_), SIGNAL(Series1Clicked(int, int)), &(interWindow.internalView), SLOT(UpdateInternalChart(int, int)));
    /*框选显示第一条脉冲的脉内图像*
     * 取消全部选择，清空脉冲聚类列表*/
    for(int i = 0; i <imp_s.view_.charts_list_->size(); i++){
        connect(imp_s.view_.charts_list_->at(i), SIGNAL(UpdateInternalPulseChartWithRBFirstPulse(int, int)), &(interWindow.internalView), SLOT(UpdateInternalChart(int,int)));
        connect(imp_s.view_.charts_list_->at(i), SIGNAL(ClearPulseList()), &imp_Clu, SLOT(ClearPulseListSlot()));
        connect(imp_s.view_.charts_list_->at(i), SIGNAL(UpdateInternalPulseChartWithKeyPressPulse(int,int)),&(interWindow.internalView), SLOT(UpdateInternalChart(int,int)));
        connect(imp_s.view_.charts_list_->at(i), SIGNAL(AddAllPulseInfoIntoList(QVector<InterPulse>)), &imp_Clu, SLOT(AddAllPulseInfoIntoListSlot(QVector<InterPulse>)));
    }

    //安装事件过滤器
    this->installEventFilter(this);
    connect(this, SIGNAL(AllWindowActivate(QObject *)), this, SLOT(AllWindowRaiseSlot(QObject *)));
    connect(&imp_s, SIGNAL(AllWindowActivate(QObject *)), this, SLOT(AllWindowRaiseSlot(QObject *)));
    connect(&imp_Clu, SIGNAL(AllWindowActivate(QObject *)), this, SLOT(AllWindowRaiseSlot(QObject *)));
    connect(&interWindow, SIGNAL(AllWindowActivate(QObject *)), this, SLOT(AllWindowRaiseSlot(QObject *)));
//    qDebug()<<this->windowState()<<this->imp_s.windowState();

    connect(&imp_s, SIGNAL(MouseRelease(QObject*)), this, SLOT(AutoSetWindowPosSlot(QObject*)));
    connect(&interWindow, SIGNAL(MouseRelease(QObject*)), this, SLOT(AutoSetWindowPosSlot(QObject*)));
    connect(&imp_Clu, SIGNAL(MouseRelease(QObject*)), this, SLOT(AutoSetWindowPosSlot(QObject*)));

    //脉间窗口全屏、退出全屏信号与槽函数连接
    connect(&imp_s.view_,SIGNAL(FullScreen1()),this,SLOT(PulseWindowFullScreenSlot()));
    connect(&imp_s.view_,SIGNAL(QuitFullScreen1()),this,SLOT(PulseWindowQuitFullScreenSlot()));
    //脉内窗口全屏、退出全屏信号与槽函数连接
    connect(&interWindow.internalView,SIGNAL(InternalPulseWindowFullScreen()),this,SLOT(InternalPulseWindowFullScreenSlot()));
    connect(&interWindow.internalView,SIGNAL(InternalPulseWindowQuitFullScreen()),this,SLOT(InternalPulseWindowQuitFullScreenSlot()));
}
MainWindow::~MainWindow()
{
    delete ui;
    delete mywin;

}
void MainWindow::initTitleBar()
{
    // 设置标题栏跑马灯效果，可以不设置;
    m_titleBar->setTitleRoll();

    m_titleBar->setTitleIcon(":/new/icon/images/app_icon.png");
    m_titleBar->setButtonType(MIN_BUTTON);
    m_titleBar->setTitleWidth(this->width());
}
void MainWindow::openFileSlots(QFileInfo fileInfo){
    QFile *file = new QFile();
    file->setFileName(fileInfo.filePath());
    /*多线程处理连接数据库，加载文件到数据库*/
//    SignalWorker *signal_worker=new SignalWorker();
//    SlotWorker *slot_work_LoadPulseFileToDatabase=new SlotWorker();

//    QThread *thread=new QThread;
//    qDebug()<<"the current thread id:"<<QThread::currentThreadId();
//    slot_work_LoadPulseFileToDatabase->moveToThread(thread);
//    QObject::connect(signal_worker,SIGNAL(Sig_GetDatabasesName(QFileInfo)),slot_work_LoadPulseFileToDatabase,SLOT(Slot_GetDatabasesName(QFileInfo)));
//    thread->start();
//    signal_worker->emitSig_GetDatabasesName(fileInfo);


    //加载脉间文件，并完成图像的初始化
    million_data_vector = PulseFileManager::LoadInterPulseFileDataIntoMemVector(file);
    if(million_data_vector.size() != 0){
        this->imp_s.view_.scene_->chart1_->million_data_vector_ = million_data_vector;
        this->imp_s.view_.scene_->chart2_->million_data_vector_ = million_data_vector;
        this->imp_s.view_.scene_->chart3_->million_data_vector_ = million_data_vector;
        this->imp_s.view_.scene_->chart4_->million_data_vector_ = million_data_vector;
        this->imp_s.view_.scene_->chart5_->million_data_vector_ = million_data_vector;
        this->imp_Clu.million_data_vector_ = million_data_vector;

        AverageVector = PulseMapper::MapPulseTobePlotted(million_data_vector, 0, million_data_vector.size() - 1);
        this->imp_s.view_.scene_->chart1_->average_data_vector=AverageVector;

        this->imp_s.view_.scene_->chart1_->axisx1_->setRange(0, million_data_vector.size());
        this->imp_s.view_.scene_->chart2_->axisx1_->setRange(0, million_data_vector.size());
        this->imp_s.view_.scene_->chart3_->axisx1_->setRange(0, million_data_vector.size());
        this->imp_s.view_.scene_->chart4_->axisx1_->setRange(0, million_data_vector.size());
        this->imp_s.view_.scene_->chart5_->axisx1_->setRange(0, million_data_vector.size());

        int average_number = million_data_vector.size() / POINTNUMBER+1;//+1为了补全显示不全的问题
        this->imp_s.view_.scene_->chart1_->average_number=average_number;
        PulseGraphDisplayer::displayInterPulseGraph(AverageVector, average_number, 0,imp_s.view_.charts_list_);
        qDebug()<<"View update";
        timerForLoadInternalPulseFile->start(2000);
        connect(timerForLoadInternalPulseFile, SIGNAL(timeout()), this, SLOT(openInternalPulseFileSlot()));
    }

//     internalVector=PulseFileManager::LoadInternalPulseFileDataIntoMemVector(file);
//     qDebug()<<"Mainwindow:"<<internalVector.size();
//     this->interWindow.internalView.internalScene->interChart1_->axisx1_->setRange(1091,2268);
//     PulseGraphDisplayer::disPlayInternalPulseGraph(internalVector,this->interWindow.internalView.internalChartsList->at(0));
//     qDebug()<<"Mainwindow:InternalView update";

}
//载入脉内文件
void MainWindow::openInternalPulseFileSlot(){
    timerForLoadInternalPulseFile->stop();
    //QDir::setCurrent("E:/Shared/Win10Pro/Current Project/build-menue-Desktop_Qt_5_7_1_MSVC2013_64bit-Release");
    qDebug()<<"applicationDirPath"<<QCoreApplication::applicationDirPath()<<
              "currentPath"<<QDir::currentPath();
    qDebug()<<"MainWindow-openInternalPulseFileSlot-选择脉内文件:";
    QFile *file=new QFile("TestData_InternalPulse.txt");
    if(file->exists()){
        qDebug()<<"MainWindow-openInternalPulseFileSlot: file exist";
        //内存映射文件
        if(!file->open(QIODevice::ReadOnly)){
            qDebug()<<"MainWindow-openInternalPulseFileSlot:open file error";
        }else{
            uchar *fpr = file->map(0, file->size());
            if(fpr){
                interWindow.internalView.fpr = file->map(0, file->size());
            }else{
                qDebug()<<"file.map error";
            }
        }
//        interWindow.internalView.file = file;
    }else{
        qDebug()<<"MainWindow-openInternalPulseFileSlot: file not exist";
    }
//        internalVector=PulseFileManager::LoadInternalPulseFileDataIntoMemVector(file);
//        qDebug()<<"Mainwindow:"<<internalVector.size();
//        this->interWindow.internalView.internalScene->interChart1_->axisx1_->setRange(1091,2268);
//        PulseGraphDisplayer::disPlayInternalPulseGraph(internalVector,this->interWindow.internalView.internalChartsList->at(0));
//        qDebug()<<"Mainwindow:InternalView update";
}

//mywindow关闭槽函数
void MainWindow::mywindow_CloseSlot(){
    //qDebug()<<"Mainwindow connected";
    QString fileCounts=QString::number(mywin->fileCounts);
      ui->lineEdit_showFilesCounts->setText(fileCounts);
      ui->spinBox_fileNumber->setRange(1,mywin->fileCounts);
      ui->spinBox_fileNumber->setValue(mywin->pulseFileNumber+1);

      QString totalFileCounts=QString::number(mywin->fileCounts);//文件总数
      QString currentPulseFileNumber=QString::number(mywin->pulseFileNumber+1);//当前文件序号
      QString str="                                                                       "+currentPulseFileNumber+"/"+totalFileCounts;

      QString strFilePath="  "+mywin->pulseFilePath;
      m_titleBar->setTitleContent(QString(str.append(strFilePath)));
    
}
//随着spinBox中文件序号的改变，将相应的文件信息显示到mainwindow界面上方
void MainWindow::spinBox_setValue(int value){
    int fileTotalCounts=ui->lineEdit_showFilesCounts->text().toInt();
    if(fileTotalCounts==0){
        QMessageBox msgBox;
        msgBox.setText("当前无文件");
        msgBox.exec();
        qDebug()<<"Mainwindow-spinBoxSetValue:当前没有选中的文件";
    }else{
       QString strCurrentFileNumber=QString::number(value);
       QString strFileCounts=QString::number(fileTotalCounts);
       QString str="                                                                             "+strCurrentFileNumber+"/"+strFileCounts;

       QString pulseFilePath=mywin->filePathForQuickLoadFile(value-1);
       QString str1=" "+pulseFilePath;

       m_titleBar->setTitleContent(QString(str.append(str1)));
    }
}
//从mainwindow界面快速载入文件
void MainWindow::pushButton_QuickLoadFileSlot(){
    //qDebug()<<"MainWindow-pushButton_QuickLoadFileSlot:mainwindow快速载入文件";
     int fileTotalCounts=ui->lineEdit_showFilesCounts->text().toInt();
     //若显示的文件总数为0，则无法在mainwindow界面快速加载
     if(fileTotalCounts==0){
         QMessageBox msgBox;
         msgBox.setText("当前无任何文件可加载");
         msgBox.exec();
     }else{
         int fileNumber=ui->spinBox_fileNumber->text().toInt();
         //qDebug()<<"当前文件序号"<<fileNumber;
         QString pulseFilePath=mywin->filePathForQuickLoadFile(fileNumber-1);
         //qDebug()<<"mainwindow:"<<fileNumber<<pulseFilePath;
         QFileInfo fileInfo(pulseFilePath);
         bool success=mywin->pulseFileTypeDeterminite(fileInfo);
         if(success){
              emit openFile(fileInfo);
         }
     }
}
//各类界面的槽函数实现
void MainWindow::openwindow_childSlots(){
    mywin->show();
    mywin->raise();
   // mywin->setFixedSize(1200,590);
    mywin->setMinimumSize(1080,530);
}
void MainWindow::openSequenceFrequency(){
    sequence->show();
}
void MainWindow::openRadarPerformSlots(){
    radarperform->show();
}
void  MainWindow::openDataFitting(){
    datafit->show();
}
void MainWindow::openPointLineMeasuring(){
    pointline->show();
}
void MainWindow::openshuzhijisuan1(){
    shuzhi1->show();
}
void MainWindow::openxvliepingpufenxiSlots(){
    pingpufenxi->show();
}
void MainWindow::openshujuchuliSlots(){
    datahanding->show();
}
void MainWindow::openmaichongcunchuSlots(){
    pulse_storage->show();
}
void MainWindow::openmaijianjujingceSlots(){
    pulse_spacing_measurement->show();
}
void MainWindow::openprijingceSlots(){
    PRI_measurement->show();
}
void MainWindow::openguolvshezhiSlots(){
    guolv->show();
}
void MainWindow::openCaoGaoBen(){
    caogao->show();
}
void MainWindow::openImpluseContrast(){
    imcon->show();

}
void MainWindow::openMatchSearch(){
    match->show();
}
void MainWindow::SetAllWindowsSizeAndShow(){

        this->setFixedWidth(clientRect.width());
        this->setGeometry(0,0,clientRect.width(),clientRect.height()*0.124);
        this->setMaximumHeight(clientRect.height()*0.124);

        interWindow.setGeometry(clientRect.width()*0.5,this->frameGeometry().height(),clientRect.width()*0.5,clientRect.height()*0.716);
        interWindow.setMinimumSize(clientRect.width()*0.5,clientRect.height()*0.716);

        imp_s.setGeometry(0,this->frameGeometry().height(),clientRect.width()*0.4995,clientRect.height()*0.716);
        imp_s.setMinimumSize(clientRect.width()*0.499,clientRect.height()*0.716);

        imp_Clu.setGeometry(0,this->frameGeometry().height()+imp_s.frameGeometry().height(),clientRect.width(),clientRect.height()*0.16);
        imp_Clu.setMinimumSize(clientRect.width(),clientRect.height()*0.16);


        this->setAttribute(Qt::WA_QuitOnClose,true);
        imp_s.setAttribute(Qt::WA_QuitOnClose,false);
        imp_Clu.setAttribute(Qt::WA_QuitOnClose,false);
        interWindow.setAttribute(Qt::WA_QuitOnClose,false);

        //不在显示栏显示
        imp_Clu.setWindowFlags(Qt::Tool|Qt::FramelessWindowHint);
        interWindow.setWindowFlags(Qt::Tool|Qt::FramelessWindowHint);
        imp_s.setWindowFlags(Qt::Tool|Qt::FramelessWindowHint);

    //按调用show()方法的顺序设置堆叠层次
    windowStackedLayer[0] = &(this->imp_Clu);
    windowStackedLayer[1] = &(this->interWindow);
    windowStackedLayer[2] = &(this->imp_s);
    windowStackedLayer[3] = this;

    this->show();
    imp_s.show();
    interWindow.show();
    imp_Clu.show();
}

//隐藏和关闭3个窗口
void MainWindow::on_toolButton_PulseWindow_clicked()
{
    pulse_window_flag=!pulse_window_flag;
    imp_s.setHidden(pulse_window_flag);
    if(pulse_window_flag)//执行关闭的方法
    {
        if(!internal_pulse_window_flag)//B在
        {
            if(!impulse_clustering_flag)//C在
                interWindow.setGeometry(0,this->frameGeometry().height(),clientRect.width(),clientRect.height()*0.716);
            else//C不在
                interWindow.setGeometry(0,this->frameGeometry().height(),clientRect.width(),clientRect.height()*0.876);
        }
        else
        {
            if(!impulse_clustering_flag)
              imp_Clu.setGeometry(0,this->frameGeometry().height(),clientRect.width(),clientRect.height()*0.876);
        }
    }
    else
    {
        if(!internal_pulse_window_flag)//B在
        {
            if(!impulse_clustering_flag)//C在
            {
                imp_s.setGeometry(0,this->frameGeometry().height(),clientRect.width()*0.5,clientRect.height()*0.716);
                interWindow.setGeometry(clientRect.width()*0.5,this->frameGeometry().height(),clientRect.width()*0.5,clientRect.height()*0.716);
            }else//C不在
             {
                imp_s.setGeometry(0,this->frameGeometry().height(),clientRect.width()*0.5,clientRect.height()*0.876);
                interWindow.setGeometry(clientRect.width()*0.5,this->frameGeometry().height(),clientRect.width()*0.5,clientRect.height()*0.876);
             }
        }else
        {
            if(!impulse_clustering_flag)
            {
                imp_s.setGeometry(0,this->frameGeometry().height(),clientRect.width(),clientRect.height()*0.716);
                imp_Clu.setGeometry(0,this->frameGeometry().height()+imp_s.frameGeometry().height(),clientRect.width(),clientRect.height()*0.16);
            }else
            {
                imp_s.setGeometry(0,this->frameGeometry().height(),clientRect.width(),clientRect.height()*0.876);
            }
        }
    }

}
void MainWindow::on_toolButton_InternalPulseWindow_clicked()
{
    internal_pulse_window_flag=!internal_pulse_window_flag;
    interWindow.setHidden(internal_pulse_window_flag);
    if(internal_pulse_window_flag)
    {
        if(!pulse_window_flag)
        {
            if(!impulse_clustering_flag)
                imp_s.setGeometry(0,this->frameGeometry().height(),clientRect.width(),clientRect.height()*0.716);
            else
                imp_s.setGeometry(0,this->frameGeometry().height(),clientRect.width(),clientRect.height()*0.876);
        }
        else
        {
            if(!impulse_clustering_flag)
                imp_Clu.setGeometry(0,this->frameGeometry().height(),clientRect.width(),clientRect.height()*0.876);
        }
    }else
    {
        if(!pulse_window_flag)
        {
            if(!impulse_clustering_flag)
            {
                imp_s.setGeometry(0,this->frameGeometry().height(),clientRect.width()*0.5,clientRect.height()*0.716);
                interWindow.setGeometry(clientRect.width()*0.5,this->frameGeometry().height(),clientRect.width()*0.5,clientRect.height()*0.716);
            }
            else
            {
                imp_s.setGeometry(0,this->frameGeometry().height(),clientRect.width()*0.5,clientRect.height()*0.876);
                interWindow.setGeometry(clientRect.width()*0.5,this->frameGeometry().height(),clientRect.width()*0.5,clientRect.height()*0.876);
            }
        }
        else
        {
            if(!impulse_clustering_flag)
            {
                interWindow.setGeometry(0,this->frameGeometry().height(),clientRect.width(),clientRect.height()*0.716);
                imp_Clu.setGeometry(0,this->frameGeometry().height()+imp_s.frameGeometry().height(),clientRect.width(),clientRect.height()*0.23);
            }
            else
            {
                interWindow.setGeometry(0,this->frameGeometry().height(),clientRect.width(),clientRect.height()*0.876);
            }
        }
    }

}
void MainWindow::on_toolButton_Impulse_Clustering_clicked()
{
    impulse_clustering_flag=!impulse_clustering_flag;
    imp_Clu.setHidden(impulse_clustering_flag);
    if(impulse_clustering_flag)
    {
        if((!pulse_window_flag)&&(!internal_pulse_window_flag))
        {
            imp_s.setGeometry(0,this->frameGeometry().height(),clientRect.width()*0.5,clientRect.height()*0.876);
            interWindow.setGeometry(clientRect.width()*0.5,this->frameGeometry().height(),clientRect.width()*0.5,clientRect.height()*0.876);
        }
        else
        {
            if(!pulse_window_flag)
                imp_s.setGeometry(0,this->frameGeometry().height(),clientRect.width(),clientRect.height()*0.876);
            if(!internal_pulse_window_flag)
                interWindow.setGeometry(0,this->frameGeometry().height(),clientRect.width(),clientRect.height()*0.876);
        }
    }else
    {
        if((!pulse_window_flag)&&(!internal_pulse_window_flag))
        {
            imp_s.setGeometry(0,this->frameGeometry().height(),clientRect.width()*0.5,clientRect.height()*0.716);
            interWindow.setGeometry(clientRect.width()*0.5,this->frameGeometry().height(),clientRect.width()*0.5,clientRect.height()*0.716);
            imp_Clu.setGeometry(0,this->frameGeometry().height()+imp_s.frameGeometry().height(),clientRect.width(),clientRect.height()*0.23);
        }
        else
        {
            if(!pulse_window_flag)
            {
               imp_s.setGeometry(0,this->frameGeometry().height(),clientRect.width(),clientRect.height()*0.716);
               imp_Clu.setGeometry(0,this->frameGeometry().height()+imp_s.frameGeometry().height(),clientRect.width(),clientRect.height()*0.23);
            }
            if(!internal_pulse_window_flag)
            {
               interWindow.setGeometry(0,this->frameGeometry().height(),clientRect.width(),clientRect.height()*0.716);
               imp_Clu.setGeometry(0,this->frameGeometry().height()+imp_s.frameGeometry().height(),clientRect.width(),clientRect.height()*0.23);
            }
        }

    }
}
void MainWindow::PulseWindow_closeSlots()
{
    //将对应按钮设置为被选中状态
    ui->toolButton_PulseWindow->setChecked(true);

    if(!internal_pulse_window_flag)//B在
    {
        if(!impulse_clustering_flag)//C在
            interWindow.setGeometry(0,this->frameGeometry().height(),clientRect.width(),clientRect.height()*0.716);
        else//C不在
            interWindow.setGeometry(0,this->frameGeometry().height(),clientRect.width(),clientRect.height()*0.876);
    }
    else
    {
        if(!impulse_clustering_flag)
          imp_Clu.setGeometry(0,this->frameGeometry().height(),clientRect.width(),clientRect.height()*0.876);
    }
   pulse_window_flag=!pulse_window_flag;
}
void MainWindow::InternalPulseWindow_closeSlots()
{

    //将对应按钮设置为被选中状态
    ui->toolButton_InternalPulseWindow->setChecked(true);

    if(!pulse_window_flag)
    {
        if(!impulse_clustering_flag)
            imp_s.setGeometry(0,this->frameGeometry().height(),clientRect.width(),clientRect.height()*0.716);
        else
            imp_s.setGeometry(0,this->frameGeometry().height(),clientRect.width(),clientRect.height()*0.876);
    }
    else
    {
        if(!impulse_clustering_flag)
            imp_Clu.setGeometry(0,this->frameGeometry().height(),clientRect.width(),clientRect.height()*0.876);
    }
    internal_pulse_window_flag=!internal_pulse_window_flag;
}
void MainWindow::impulse_Clustering_closeSlots()
{

    //将对应按钮设置为被选中状态
    ui->toolButton_Impulse_Clustering->setChecked(true);

   if((!pulse_window_flag)&&(!internal_pulse_window_flag))
   {
       imp_s.setGeometry(0,this->frameGeometry().height(),clientRect.width()*0.5,clientRect.height()*0.876);
       interWindow.setGeometry(clientRect.width()*0.5,this->frameGeometry().height(),clientRect.width()*0.5,clientRect.height()*0.876);
   }
   else
   {
       if(!pulse_window_flag)
           imp_s.setGeometry(0,this->frameGeometry().height(),clientRect.width(),clientRect.height()*0.876);
       if(!internal_pulse_window_flag)
           interWindow.setGeometry(0,this->frameGeometry().height(),clientRect.width(),clientRect.height()*0.876);
   }
   impulse_clustering_flag=!impulse_clustering_flag;
}
void MainWindow::chart_data_reset()
{
    this->imp_s.view_.scene_->chart1_->million_data_vector_ = million_data_vector;
    this->imp_s.view_.scene_->chart2_->million_data_vector_ = million_data_vector;
    this->imp_s.view_.scene_->chart3_->million_data_vector_ = million_data_vector;
    this->imp_s.view_.scene_->chart4_->million_data_vector_ = million_data_vector;
    this->imp_s.view_.scene_->chart5_->million_data_vector_ = million_data_vector;
    this->imp_Clu.million_data_vector_ = million_data_vector;

    this->imp_s.view_.scene_->chart1_->average_data_vector=AverageVector;

    this->imp_s.view_.scene_->chart1_->axisx1_->setRange(0, million_data_vector.size());
    this->imp_s.view_.scene_->chart2_->axisx1_->setRange(0, million_data_vector.size());
    this->imp_s.view_.scene_->chart3_->axisx1_->setRange(0, million_data_vector.size());
    this->imp_s.view_.scene_->chart4_->axisx1_->setRange(0, million_data_vector.size());
    this->imp_s.view_.scene_->chart5_->axisx1_->setRange(0, million_data_vector.size());

    int average_number = million_data_vector.size() / POINTNUMBER+1;//+1为了补全显示不全的问题
    this->imp_s.view_.scene_->chart1_->average_number=average_number;
    PulseGraphDisplayer::displayInterPulseGraph(AverageVector, average_number, 0,imp_s.view_.charts_list_);
    qDebug()<<" reset View update";
}


void MainWindow::on_tabWidget_tabBarDoubleClicked(int index)
{
    if(!tab_tool_hide_flag)
    {
        //标题栏从显示状态变成隐藏状态
        this->setFixedHeight(clientRect.height()*0.031);
        this->setGeometry(0,0,clientRect.width(),clientRect.height()*0.031);
        if(impulse_clustering_flag){
            //脉冲列表隐藏时
            if(interWindow.frameGeometry().width() > clientRect.width() * 0.999){
                //脉内窗口全屏时
                interWindow.setGeometry(0,this->frameGeometry().height(),interWindow.frameGeometry().width(),clientRect.height()*0.969);
            }else{
                //脉内窗口非全屏时
                interWindow.setGeometry(clientRect.width()*0.5,this->frameGeometry().height(),clientRect.width() * 0.5 ,clientRect.height()*0.969);
            }
            imp_s.setGeometry(0,this->frameGeometry().height(),imp_s.frameGeometry().width(),clientRect.height()*0.969);
        }else{
            //脉冲列表显示时
            if(interWindow.frameGeometry().width() > clientRect.width() * 0.999){
                //脉内窗口全屏时
                interWindow.setGeometry(0,this->frameGeometry().height(),interWindow.frameGeometry().width(),clientRect.height()*0.809);
            }else{
                //脉内窗口非全屏时
                interWindow.setGeometry(clientRect.width()*0.5,this->frameGeometry().height(),clientRect.width() * 0.5 ,clientRect.height()*0.809);
            }
            imp_s.setGeometry(0,this->frameGeometry().height(),imp_s.frameGeometry().width(),clientRect.height()*0.809);
        }
        tab_tool_hide_flag=!tab_tool_hide_flag;
    }
    else
    {
        //标题栏从隐藏状态变成显示状态
        this->setGeometry(0,0,clientRect.width(),clientRect.height()*0.124);
        this->setFixedHeight(clientRect.height()*0.124);
        if(impulse_clustering_flag){
            //脉冲列表隐藏时
            if(interWindow.frameGeometry().width() > clientRect.width() * 0.999){
                //脉内窗口全屏时
                interWindow.setGeometry(0,this->frameGeometry().height(),interWindow.frameGeometry().width(),clientRect.height() * 0.875);
            }else{
                //脉内窗口非全屏时
                interWindow.setGeometry(clientRect.width()*0.5,this->frameGeometry().height(),clientRect.width() * 0.5,clientRect.height() * 0.875);
            }
            imp_s.setGeometry(0,this->frameGeometry().height(),imp_s.frameGeometry().width(),clientRect.height() * 0.875);
        }else{
            //脉冲列表显示时
            if(interWindow.frameGeometry().width() > clientRect.width() * 0.999){
                //脉内窗口全屏时
                interWindow.setGeometry(0,this->frameGeometry().height(),interWindow.frameGeometry().width(),clientRect.height() * 0.716);
            }else{
                //脉内窗口非全屏时
                interWindow.setGeometry(clientRect.width()*0.5,this->frameGeometry().height(),clientRect.width() * 0.5,clientRect.height() * 0.716);
            }
            imp_s.setGeometry(0,this->frameGeometry().height(),imp_s.frameGeometry().width(),clientRect.height() * 0.716);
        }

        tab_tool_hide_flag=!tab_tool_hide_flag;
    }
}

void MainWindow::on_toolButton_cfxc_clicked()
{

    if(!imp_form_show_flag)
    {
        imp_Clu.hide();
        imp_form.show();
        imp_form.setGeometry(0,this->frameGeometry().height()+imp_s.frameGeometry().height(),imp_Clu.width(),imp_Clu.height());
        imp_form_show_flag=!imp_form_show_flag;
    }
    else
    {
        imp_form.hide();
        imp_Clu.show();
        imp_form_show_flag=!imp_form_show_flag;
    }

}

//SLOT:处理view发出的fullScreen()信号
void MainWindow::PulseWindowFullScreenSlot(){
    if(this->imp_s.pos().y() < clientRect.height() * 0.1){
        imp_s.setGeometry(0, this->frameGeometry().height(), clientRect.width(),clientRect.height()*0.969);
    }else{
        imp_s.setGeometry(0,this->frameGeometry().height(),clientRect.width(),clientRect.height()*0.875);
    }
    qDebug()<<"MainWindow::PulseWindowFullScreenSlot: full screen";

}

//SLOT:处理view发出的quitFullScreen()信号
void MainWindow::PulseWindowQuitFullScreenSlot(){
    if(!internal_pulse_window_flag && !impulse_clustering_flag){
        if(imp_s.pos().y() < clientRect.height() * 0.1){
            imp_s.setGeometry(0, this->frameGeometry().height(), clientRect.height() * 0.4995, clientRect.height() * 0.809);
        }else{
            imp_s.setGeometry(0, this->frameGeometry().height(), clientRect.height() * 0.4995, clientRect.height() * 0.716);
        }
    }else if(internal_pulse_window_flag && impulse_clustering_flag){
        if(imp_s.pos().y() < clientRect.height() * 0.1){
            qDebug()<<"imp_s.pos().y() < clientRect.height() * 0.1";
            imp_s.setGeometry(0, this->frameGeometry().height(), clientRect.width() * 0.9999, clientRect.height() * 0.969);
        }else{
            imp_s.setGeometry(0, this->frameGeometry().height(), clientRect.width() * 0.9999, clientRect.height() * 0.875);
        }
    }else if(internal_pulse_window_flag && !impulse_clustering_flag){
        if(imp_s.pos().y() < clientRect.height() * 0.1){
            qDebug()<<"imp_s.pos().y() < clientRect.height() * 0.1";
            imp_s.setGeometry(0, this->frameGeometry().height(), clientRect.width() * 0.9998, clientRect.height() * 0.809);
        }else{
            imp_s.setGeometry(0, this->frameGeometry().height(), clientRect.width() * 0.9998, clientRect.height() * 0.716);
        }
    }else if(!internal_pulse_window_flag && impulse_clustering_flag){
        if(imp_s.pos().y() < clientRect.height() * 0.1){
            qDebug()<<"imp_s.pos().y() < clientRect.height() * 0.1";
            imp_s.setGeometry(0, this->frameGeometry().height(), clientRect.width() * 0.4995, clientRect.height() * 0.969);
        }else{
            imp_s.setGeometry(0, this->frameGeometry().height(), clientRect.width() * 0.4995, clientRect.height() * 0.875);
        }
    }
    qDebug()<<"MainWindow::PulseWindowQuitFullScreenSlot: quitscreen";
}

//处理InternalPulseView发出的fullScreen()信号
void MainWindow::InternalPulseWindowFullScreenSlot(){
//     this->setGeometry(0,clientRect.height()*0.125,clientRect.width(),clientRect.height()*0.875);
    if(interWindow.pos().y() < clientRect.height() * 0.1){
        interWindow.setGeometry(0, this->frameGeometry().height(), clientRect.width(),clientRect.height()*0.969);
    }else{
        interWindow.setGeometry(0,this->frameGeometry().height(),clientRect.width(),clientRect.height()*0.875);
    }

//     interWindow.ui->tableWidget->hide();
     qDebug()<<"MainWindow::InternalPulseWindowFullScreenSlot: full screen";

}

//SLOT:处理internalView发出的quitFullScreen()信号
void MainWindow::InternalPulseWindowQuitFullScreenSlot(){
//     this->setGeometry(clientRect.width()*0.5,clientRect.height()*0.125,clientRect.width()*0.4995,clientRect.height()*0.716);
    if(!pulse_window_flag && !impulse_clustering_flag){
        if(interWindow.pos().y() < clientRect.height() * 0.1){
            interWindow.setGeometry(clientRect.width()*0.5, this->frameGeometry().height(), clientRect.height() * 0.4995, clientRect.height() * 0.809);
        }else{
            interWindow.setGeometry(clientRect.width()*0.5, this->frameGeometry().height(), clientRect.height() * 0.4995, clientRect.height() * 0.716);
        }
    }else if(pulse_window_flag && impulse_clustering_flag){
        if(interWindow.pos().y() < clientRect.height() * 0.1){
            qDebug()<<"imp_s.pos().y() < clientRect.height() * 0.1";
            interWindow.setGeometry(0, this->frameGeometry().height(), clientRect.width() * 0.9999, clientRect.height() * 0.969);
        }else{
            interWindow.setGeometry(0, this->frameGeometry().height(), clientRect.width() * 0.9999, clientRect.height() * 0.875);
        }
    }else if(pulse_window_flag && !impulse_clustering_flag){
        if(interWindow.pos().y() < clientRect.height() * 0.1){
            qDebug()<<"imp_s.pos().y() < clientRect.height() * 0.1";
            interWindow.setGeometry(0, this->frameGeometry().height(), clientRect.width() * 0.9998, clientRect.height() * 0.809);
        }else{
            interWindow.setGeometry(0, this->frameGeometry().height(), clientRect.width() * 0.9998, clientRect.height() * 0.716);
        }
    }else if(!pulse_window_flag && impulse_clustering_flag){
        if(interWindow.pos().y() < clientRect.height() * 0.1){
            qDebug()<<"imp_s.pos().y() < clientRect.height() * 0.1";
            interWindow.setGeometry(clientRect.width()*0.5, this->frameGeometry().height(), clientRect.width() * 0.4995, clientRect.height() * 0.969);
        }else{
            interWindow.setGeometry(clientRect.width()*0.5, this->frameGeometry().height(), clientRect.width() * 0.4995, clientRect.height() * 0.875);
        }
    }
//    interWindow.ui->tableWidget->show();
     qDebug()<<"MainWindow::InternalPulseWindowQuitFullScreenSlot:quit full screen";

}

void MainWindow::changeEvent(QEvent *event)
{
    if(event->type()!=QEvent::WindowStateChange)
        return;
    //最小化按钮实现主窗口与子窗口均最小化
    if(this->windowState()==Qt::WindowMinimized)
    {
            imp_s.hide();
            imp_Clu.hide();
            interWindow.hide();
    }
    //点击任务栏应用程序图标可以实现主窗口与子窗口均恢复显示
    if(this->windowState()==Qt::WindowNoState)
    {
            imp_s.show();
            imp_Clu.show();
            interWindow.show();

    }
}

//当一个窗口激活时，所有窗口提升到堆叠层次最上面
void MainWindow::AllWindowRaiseSlot(QObject *obj){
    QObject* tempWindow = NULL;
    int index = 0;
    QVector<QObject*> tempWindowVector;
    for(int i = 0; i < 4; i++){
        if(windowStackedLayer[i] == obj){
            //qDebug()<<"the activate window is"<<i;
            tempWindow = windowStackedLayer[i];
            //qDebug()<<"tempWindow is"<<tempWindow;
            index = i;
        }
    }
    for(int i = 0; i < 4; i++){
        if(i != index){
            tempWindowVector.append(windowStackedLayer[i]);
        }
    }
    //qDebug()<<"tempWindowVector.size = "<<tempWindowVector.size();

    windowStackedLayer[0] = tempWindow;
    for(int i = 0; i < tempWindowVector.size(); i++){
        windowStackedLayer[i+1] = tempWindowVector[i];
    }
    tempWindowVector.clear();

    for(int i = 3; i >= 0; i--){
        if(windowStackedLayer[i] == this){
            this->raise();
        }else if(windowStackedLayer[i] == &(this->imp_s)){
            this->imp_s.raise();
        }else if(windowStackedLayer[i] == &(this->imp_Clu)){
            this->imp_Clu.raise();
        }else if(windowStackedLayer[i] == &(this->interWindow)){
            this->interWindow.raise();
        }
        //qDebug()<<"windowStackedLayer["<<i<<"] = "<<windowStackedLayer[i];
    }

    qDebug()<<"MainWindow::AllWindowRaiseSlot-All windows raise";
//    qDebug()<<this->windowState()<<this->imp_s.windowState();
}

//移动窗口到默认位置时自动设置窗口位置对齐
void MainWindow::AutoSetWindowPosSlot(QObject *obj){
    double MainWindowLeftBottomPos_x = this->pos().x();
    double MainWindowLeftBottomPos_y = this->pos().y() + this->frameGeometry().height();
    double MainWindowRightBottom_x = this->pos().x() + this->frameGeometry().width();
    double MainWindowRightBottom_y = this->pos().y() + this->frameGeometry().height();
    if(obj == &(this->imp_s)){
        double objLeftTop_x = this->imp_s.pos().x();
        double objLeftTop_y = this->imp_s.pos().y();
        double distance_x = objLeftTop_x - MainWindowLeftBottomPos_x;
        double distance_y = objLeftTop_y - MainWindowLeftBottomPos_y;
        if(objLeftTop_y < MainWindowLeftBottomPos_y || (distance_x < 50 && distance_y < 50)){
            this->imp_s.setGeometry(0,this->frameGeometry().height(),this->imp_s.frameGeometry().width(), this->imp_s.frameGeometry().height());
        }
    }else if(obj == &(this->interWindow)){
        double objRightTop_x = this->interWindow.pos().x() + this->interWindow.frameGeometry().width();
        double objRightTop_y = this->interWindow.pos().y();
        double distance_x = MainWindowRightBottom_x - objRightTop_x;
        double distance_y = objRightTop_y - MainWindowRightBottom_y;
        if(objRightTop_y < MainWindowRightBottom_y || (distance_x < 50 && distance_y < 50)){
            this->interWindow.setGeometry(MainWindowRightBottom_x - this->interWindow.frameGeometry().width(), this->frameGeometry().height(), this->interWindow.frameGeometry().width(), this->interWindow.frameGeometry().height());
        }
    }else if(obj == &(this->imp_Clu)){
        double objBottom_y = this->imp_Clu.pos().y() + this->imp_Clu.frameGeometry().height();
        if(objBottom_y > clientRect.height() || (clientRect.height() - objBottom_y) < 50){
            this->imp_Clu.setGeometry(0, clientRect.height() - this->imp_Clu.frameGeometry().height(), this->imp_Clu.frameGeometry().width(), this->imp_Clu.frameGeometry().height());
        }
    }
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event){
    //当窗口状态变为激活状态时，发射信号，槽函数将所有窗口提升到堆叠层次最上面
    if(watched == this){
        //过滤窗口激活事件
        if(QEvent::WindowActivate == event->type()){
            qDebug()<<"MainWindow activate";
            emit AllWindowActivate(this);
        }
    }
    return BaseWindow::eventFilter(watched, event);
}






