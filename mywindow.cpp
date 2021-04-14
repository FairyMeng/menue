#include "mywindow.h"
#include "ui_mywindow.h"
mywindow::mywindow(QWidget *parent) :
   BaseWindow(parent),
    ui(new Ui::mywindow)
{
    ui->setupUi(this);
    // 初始化标题栏;
        initTitleBar();
        setSupportStretch(false);

        //设置qss样式
        {
        QFile file(":/qss/mywindow.css");
        file.open(QFile::ReadOnly);
        QTextStream filetext(&file);
        QString stylesheet = filetext.readAll();
        this->setStyleSheet(stylesheet);
        file.close();
        }


    //设置tabwidget_2的标签属性为拉伸
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    m_cancel=new Message_cancel();
    model = new QDirModel();
    tree = new QTreeView();

    isCtrlSelected=false;
    itemSelected=false;
    //设置tablewidget,tablewidget2属性,待改，不管用
     setTableAttributes();

    //硬盘列表 添加到combobox
    AddHardDiskItemsToCombobox();
    //设置tabwidget列宽根据内容自适应
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);


    //不在显示栏显示
    this->setWindowFlags(Qt::FramelessWindowHint |Qt::SubWindow);

    //将载入文件的信号与槽关联
    //QObject::connect(ui->openAction,SIGNAL(triggered()),this,SLOT( openPulseFileSlots()));
    QObject::connect(ui->pushButton_cancel,SIGNAL(clicked()),this,SLOT(openMessage_cancelSlots()));

    /*1.在缺省状态下显示当前目录的所有文件
    */
    QObject::connect(ui->comboBox,SIGNAL(currentTextChanged(QString)),this,SLOT(currentTextChangedSLotInComboBox(QString)));
    /*1.完成文件查找，有2种方式（直接输入文件回车键，或者点击查找按钮）
      2.在显示文件时，将上次查找记录在表中删除*/
    QObject::connect(ui->pushButton_findFile,SIGNAL(clicked()),this,SLOT(pushButton_findFileClicked()));
    QObject::connect(ui->lineEdit_inPutFileName,SIGNAL(returnPressed()),this,SLOT(ClearContentsSLot()));
    QObject::connect(ui->lineEdit_inPutFileName,SIGNAL(returnPressed()),this,SLOT(pushButton_findFileClicked()));

    /*1.单击tablewidget,并获得单击的内容
      2.单击tablewidget2,并获得单击的内容*/
    QObject::connect(ui->tableWidget,SIGNAL(itemClicked(QTableWidgetItem *)),this,SLOT(TableWidget_clickedItem(QTableWidgetItem*)));
    QObject::connect(ui->tableWidget_2,SIGNAL(itemClicked(QTableWidgetItem *)),this,SLOT(TableWidget_clickedItem(QTableWidgetItem*)));

    /*1.双击tablewidget，添加文件到tablewidget2
      2.双击tablewidget2，移除文件*/
    QObject::connect(ui->tableWidget,SIGNAL(itemDoubleClicked(QTableWidgetItem *)),this,SLOT(TableWidget_doubleClickedItem(QTableWidgetItem*)));
    QObject::connect(ui->tableWidget_2,SIGNAL(itemDoubleClicked(QTableWidgetItem *)),this,SLOT(TableWidget2_doubleClickedItem(QTableWidgetItem*)));


    /*1.点击添加文件，将选中的文件显示到右边的tablewidget中
      2.点击移除文件，将选中的tablewidget2中的文件移除*/
    QObject::connect(ui->pushButton_choosen,SIGNAL(clicked()),this,SLOT(pushButton_ChoosenSlot()));
    QObject::connect(ui->pushButton_remove,SIGNAL(clicked()),this,SLOT(pushButton_RemoveSlot()));

    //将tableWidget2中的文件全部移除
    QObject::connect(ui->pushButton_removeAll,SIGNAL(clicked()),this,SLOT(pushButton_RemoveAllSlot()));

    //点击加载文件，进行图像初始化
    QObject::connect(ui->pushButton_loadFile,SIGNAL(clicked()),this,SLOT(pushButton_loadFileSlot()));
    //浏览目录
    //QObject::connect(ui->pushButton_onBrowse,SIGNAL(clicked()),this,SLOT(on_pushButton_onBrowse_clicked()));
}
mywindow::~mywindow()
{
    delete ui;
    delete m_cancel;
    delete model;
    delete tree;
}

void mywindow::initTitleBar()
{
    // 设置标题栏跑马灯效果，可以不设置;
    m_titleBar->setTitleRoll();
    m_titleBar->setTitleIcon(":/new/icon/images/app_icon.jpg");
    //m_titleBar->setTitleContent(QStringLiteral("春水初生，春林初盛,春风十里，不如你。春水初生，春林初盛,春风十里，不如你。"));
    m_titleBar->setTitleContent(QString("  文件加载"));
    m_titleBar->setButtonType(MIN_MAX_BUTTON);
    m_titleBar->setTitleWidth(this->width());

}
//设置表格属性
void mywindow::setTableAttributes(){
    //ui->tableWidget->verticalHeader()->setVisible(true);
    ui->tableWidget_2->verticalHeader()->setHidden(false);//tablewidget2显示表格行号
    ui->tableWidget->verticalHeader()->setHidden(false);//tablewidget显示表格行号
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget_2->setColumnCount(3);

    //左右列表不可被编辑
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
   // ui->tableWidget->setSelectionBehavior(QAbstractItemView::NoSelection);
}
//硬盘列表 添加到combobox
void mywindow::AddHardDiskItemsToCombobox(){
    // 指定要监视的目录
    tree->setModel(model);
    tree->setRootIndex(model->index(QDir::currentPath()));
    ui->comboBox->setView(tree);
    ui->comboBox->setModel(model);
    getFileNamesInDiskInDefault("D:/");
}

//下拉选目录发生改变，显示当前目录下所有的
void mywindow::currentTextChangedSLotInComboBox(QString str){

     str=ui->comboBox->currentText()+"/";
     if(ui->tableWidget->rowCount()>0){
          //清空上次查询记录
         ui->tableWidget->clearContents();
         ui->tableWidget->setRowCount(0);

         getFileNamesInDiskInDefault(str);

     }else{
         getFileNamesInDiskInDefault(str);
     }
}
//显示TXT文件
void mywindow::getFileNamesInDiskInDefault(QString path){
    QStringList fileLists;
    QDir dir(path);
    dir.setFilter(QDir::AllEntries|QDir::NoDotAndDotDot);
    QFileInfoList currentDirList=dir.entryInfoList();
    foreach (QFileInfo fileInfo, currentDirList) {
        if(fileInfo.isDir()==true){
            QString strFilePath=fileInfo.filePath();
            //qDebug()<<"文件目录:"<<strFilePath;
            getFileNamesInDiskInDefault(strFilePath);
        }
        if(fileInfo.isFile()==true){
            //qDebug()<<fileInfo.suffix();
            if(fileInfo.suffix()=="txt"){
                fileLists.append(fileInfo.fileName());
            }
        }
    }
    ShowFiles(dir,fileLists);
}
//显示当前目录下的子目录
void mywindow::SearchDirs(QDir dir){
    QStringList strDirList;//存储获得的子目录
    QString strDir;//目录名称
    QDir currentDir(dir);
    currentDir.setFilter(QDir::AllEntries|QDir::NoDotAndDotDot);
    QFileInfoList currentDirList=currentDir.entryInfoList();

    foreach (QFileInfo fileInfo, currentDirList) {
        if(fileInfo.isDir()==true){
           strDir=fileInfo.fileName();
           strDirList.append(strDir);
        }
    }
    //将目录显示出来
    ShowFiles(dir,strDirList);
}
//浏览查找位置
//void mywindow::on_pushButton_onBrowse_clicked()
//{
//    QFileInfo fileInfo = QFileDialog::getOpenFileName(this,"mywindow:open file",QDir::currentPath());
////    QString db_name=fileInfo.fileName().split(".").at(0);
////    qDebug()<<"PulseFile path is :"<<fileInfo.filePath()<<endl<<"the Pulse file name is:"<<db_name;
//    emit openFile(fileInfo);
//    this->close();

//}
//查找按钮槽函数
void mywindow::pushButton_findFileClicked()
{
//    ui->tableWidget->clear();
//    ui->tableWidget->setRowCount(0);
    qDebug()<<"mywindow-pushButton_FindFileClicked:点击查找按钮（输入文件名回车）";
    QTime time;
    time.start();

    QString strFileName=ui->lineEdit_inPutFileName->text();
    if(strFileName==NULL){
        QMessageBox msgBox;
        msgBox.setText("请输入文件名");
        msgBox.exec();
    }else{
        int rowCounts=ui->tableWidget->rowCount();
        if(rowCounts>0){
            ui->tableWidget->clearContents();
            ui->tableWidget->setRowCount(0);
            QString pulseFileName=ui->lineEdit_inPutFileName->text();
            QString dir=ui->comboBox->currentText()+"/";
            qDebug()<<"mywindow-pushButton_findFileClicked:当前查找目录为"<<dir;
            ExecuteSearchFile(QDir(dir),pulseFileName);
            qDebug()<<"mywindow-pushButton_FindFileClicked：查找文件时间为:"<<time.elapsed()/1000<<"s";
        }else{
            QString pulseFileName=ui->lineEdit_inPutFileName->text();
            QString dir=ui->comboBox->currentText()+"/";
            qDebug()<<"mywindow-pushButton_findFileClicked:当前查找目录为"<<dir;
            ExecuteSearchFile(QDir(dir),pulseFileName);
            qDebug()<<"mywindow-pushButton_FindFileClicked：查找文件时间为:"<<time.elapsed()/1000<<"s";
        }

    }

}
//查找文件过程
void mywindow::ExecuteSearchFile(const QDir &dir,const QString &pulseFileName){
    QStringList strFileNames;//将获得的文件名存储
    QDir currentDir(dir);
    currentDir.setFilter(QDir::AllEntries|QDir::NoDotAndDotDot);
    QFileInfoList currentDirList=currentDir.entryInfoList();
    //获取目录及子目录下的文件
    foreach (QFileInfo fileInfo, currentDirList) {
        if(fileInfo.isDir()==true){
            QString strFilePath=fileInfo.filePath();
            ExecuteSearchFile(strFilePath,pulseFileName);
        }
        if(fileInfo.isFile()==true){
            QString strFileName=fileInfo.fileName();
            if(strFileName.contains(ui->lineEdit_inPutFileName->text(),Qt::CaseInsensitive))
                 strFileNames.append(strFileName);
        }
    }
    ShowFiles(dir,strFileNames);
//    for(int i=0;i<strFileNames.size();i++){
//        qDebug()<<"mywindow-ExecuteSearchFile：执行查找过程:"<<strFileNames[i];
//    }
}
//将查找的文件显示到列表中
void mywindow::ShowFiles(const QDir &dir,const QStringList &files){
    for(int i = 0 ; i < files.size();i++)
       {
           QFile file(dir.absoluteFilePath(files[i]));
           QFileInfo fileInfo(file);
           if(fileInfo.isDir()){

               QString name = fileInfo.fileName();
               QString time =  fileInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss");
               QString filePath=fileInfo.filePath();
               //qint64  sizes = fileInfo.size();

               ui->tableWidget->setRowCount(ui->tableWidget->rowCount() + 1);
               ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,new QTableWidgetItem(name));
               ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,1,new QTableWidgetItem(time));
               ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,2,new QTableWidgetItem(filePath));
               ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,3,new QTableWidgetItem(tr("   ")));
               //qDebug()<<"是目录";
           }
           if(fileInfo.isFile()){
               QString name = fileInfo.fileName();
               QString time =  fileInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss");
               QString filePath=fileInfo.filePath();
               qint64  sizes = fileInfo.size();

               ui->tableWidget->setRowCount(ui->tableWidget->rowCount() + 1);
               ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,new QTableWidgetItem(name));
               ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,1,new QTableWidgetItem(time));
               ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,2,new QTableWidgetItem(filePath));
               ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,3,new QTableWidgetItem(tr("%1KB").arg(int(sizes+1023)/1024)));
           }
       }
}
//清除上次的查询记录
void mywindow::ClearContentsSLot(){
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
}
//点击tableWidget，并获得相对应的内容
void mywindow::TableWidget_clickedItem(QTableWidgetItem *item){
    itemSelected=true;
    if(QApplication::keyboardModifiers()&Qt::ControlModifier)
        isCtrlSelected=true;
    else
       isCtrlSelected=false;
}
//双击tablewidget
void mywindow::TableWidget_doubleClickedItem(QTableWidgetItem *item){
    bool focus=ui->tableWidget->isItemSelected(ui->tableWidget->currentItem());//判断左侧列表是否被点击
    int tableWidget2_rowCounts=ui->tableWidget_2->rowCount();
    if(tableWidget2_rowCounts==0){//右侧无文件
        qDebug()<<"右侧无文件";
        if(focus){
            if(isCtrlSelected==false){
               AddFileInTablewidget2WithClick();
               fileNameList.append(ui->tableWidget->item(ui->tableWidget->currentItem()->row(),0)->text());
               filePathList.append(ui->tableWidget->item(ui->tableWidget->currentItem()->row(),2)->text());
            }
        }else{
            QMessageBox msgBox;
            msgBox.setText("请选择需要添加的文件");
            msgBox.exec();
        }
    }else{
       qDebug()<<"右侧有文件";
       if(focus){
           fileName = ui->tableWidget->item(ui->tableWidget->currentItem()->row(),0)->text();//左侧列表当前文件名
           filePath = ui->tableWidget->item(ui->tableWidget->currentItem()->row(),2)->text();//左侧列表当前文件路径
           qDebug()<<fileName;
           qDebug()<<"filenameliset:"<<fileNameList;

           bool flag = false;
           for(int i=0;i<fileNameList.size();i++){
//               qDebug()<<fileNameList.at(i);
               if(fileNameList.at(i)==fileName && filePathList.at(i)==filePath){
                   QMessageBox msgBox;
                   msgBox.setText("右侧列表已经包含所选文件，请重新选择！！！");
                   msgBox.exec();
                   flag = true;
                   break;
               }
           }
           if(!flag){
               qDebug()<<"flag = "<<flag;
               qDebug()<<"添加文件到右边列表";
               AddFileInTablewidget2WithClick();
               fileNameList.append(ui->tableWidget->item(ui->tableWidget->currentItem()->row(),0)->text());
               filePathList.append(ui->tableWidget->item(ui->tableWidget->currentItem()->row(),2)->text());

           }
           qDebug()<<"文件列表的大小"<<fileNameList.size();
       }
    }

}

//将左侧列表选中的文件添加到右侧
void mywindow::pushButton_ChoosenSlot(){
     //单选选中的文件添加到右侧tableWidget显示
     bool focus;
     if((ui->tableWidget->selectedItems().count())>0)
         focus=true;
     else
         focus=false;
     int tableWidget2_rowCounts=ui->tableWidget_2->rowCount();
     if(tableWidget2_rowCounts==0){//右侧无文件
         qDebug()<<"mywindow-choosenSlot:右侧无文件";
         if(focus){
             if(isCtrlSelected==false){
                 QList<QTableWidgetItem*> ColumnItems;//用来保存多选获得的tablewidget内容
                 QList<int> rowsList;//存放当前左侧列表点击行数的值，比如1，3，5

                 ColumnItems=ui->tableWidget->selectedItems();
                 for(int i=0;i<ColumnItems.count();i++){
                     rowsList.append(ui->tableWidget->row(ColumnItems.at(i)));
                 }
                 for(int i=0;i<rowsList.size();i++){
                     QStringList fileInfoList;
                     QString name=ui->tableWidget->item(rowsList.at(i),0)->text();
                     QString size=ui->tableWidget->item(rowsList.at(i),1)->text();
                     QString path=ui->tableWidget->item(rowsList.at(i),2)->text();
                     fileInfoList<<name<<size<<path;

                     if(fileInfoList.size()>0){
                         ui->tableWidget_2->setRowCount(ui->tableWidget_2->rowCount()+1);
                         ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1,0,new QTableWidgetItem(name));
                         ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1,1,new QTableWidgetItem(size));
                         ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1,2,new QTableWidgetItem(path));
                      }
                     fileNameList.append(ui->tableWidget->item(rowsList.at(i),0)->text());
                     filePathList.append(ui->tableWidget->item(rowsList.at(i),2)->text());
                 }
                 //AddFileInTablewidget2WithClick();
             }
             if(isCtrlSelected==true){
                 AddFileInTablewidget2WithCtrl();
             }
         }
         else{
             QMessageBox msgBox;
             msgBox.setText("请选择需要添加的文件");
             msgBox.exec();
         }
     }else{
        qDebug()<<"mywindow-choosenSlot-右侧有文件";
        if(focus){
            if(isCtrlSelected==false){
                    QList<int> fileNameIndex;//左侧列表所选文件名的行号，这些文件没有被添加至右侧
                    QList<int> rowsList;//存放当前点击行数的值，比如1，3，5
                    QList<QTableWidgetItem*> ColumnItems;//用来保存多选获得的tablewidget2内容
                    QList<int> list;//存放不同下标文件
                    ColumnItems=ui->tableWidget->selectedItems();
                    int rowCounts=ColumnItems.count();

                    for(int i=0;i<rowCounts;i++){
                        int row=ui->tableWidget->row(ColumnItems.at(i));
                        rowsList.append(row);
                    }
                    for(int i=0;i<rowsList.size();i++){
                        fileName = ui->tableWidget->item(rowsList.at(i),0)->text();//左侧列表当前文件名
                        filePath = ui->tableWidget->item(rowsList.at(i),2)->text();//左侧列表当前文件路径
                        list.append(saveDifferentFileName(rowsList.at(i),fileName,filePath,fileNameList,filePathList));
                    }
                    if(list.isEmpty()){//所选文件在右侧列表均已存在
                        QMessageBox msgBox;
                        msgBox.setText("右侧列表已包含左侧选中文件，请重新选择！！！");
                        msgBox.exec();
                    }else{//所选文件在右侧列表部分存在
                        //qDebug()<<"list.size="<<list.size();
                        for(int i=0;i<list.size();i++){
                           // qDebug()<<"mywindow-choosenSlot:不同文件名行数是"<<list.at(i);
                            fileNameIndex.append(list.at(i));

                            fileNameList.append(ui->tableWidget->item(list.at(i),0)->text());
                            filePathList.append(ui->tableWidget->item(list.at(i),2)->text());
                        }
                    }
                    for(int i=0;i<fileNameIndex.size();i++){
                         QStringList specifiedColumnItems;

                         QString name= ui->tableWidget->item(fileNameIndex.at(i),0)->text();
                         QString size=ui->tableWidget->item(fileNameIndex.at(i),1)->text();
                         QString path=ui->tableWidget->item(fileNameIndex.at(i),2)->text();

                         specifiedColumnItems.append(name);
                         specifiedColumnItems.append(size);
                         specifiedColumnItems.append(path);

                         ui->tableWidget_2->setRowCount(ui->tableWidget_2->rowCount()+1);
                         ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1,0,new QTableWidgetItem(name));
                         ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1,1,new QTableWidgetItem(size));
                         ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1,2,new QTableWidgetItem(path));
                    }
//                fileName = ui->tableWidget->item(ui->tableWidget->currentItem()->row(),0)->text();//左侧列表当前文件名
//                filePath = ui->tableWidget->item(ui->tableWidget->currentItem()->row(),2)->text();//左侧列表当前文件路径
//                qDebug()<<fileName;
//                qDebug()<<"filenameliset:"<<fileNameList;
//                bool flag = false;
//                for(int i=0;i<fileNameList.size();i++){
//                    qDebug()<<fileNameList.at(i);
//                    if(fileNameList.at(i)==fileName && filePathList.at(i)==filePath){
//                        QMessageBox msgBox;
//                        msgBox.setText("该文件已存在");
//                        msgBox.exec();
//                        flag = true;
//                        break;
//                    }
//                }
//                if(!flag){
//                    qDebug()<<"flag = "<<flag;
//                    qDebug()<<"添加文件到右边列表";
//                    AddFileInTablewidget2WithClick();
//                    fileNameList.append(ui->tableWidget->item(ui->tableWidget->currentItem()->row(),0)->text());
//                    filePathList.append(ui->tableWidget->item(ui->tableWidget->currentItem()->row(),2)->text());
//                }
//                qDebug()<<"文件列表的大小"<<fileNameList.size();
            }
            if(isCtrlSelected==true){
                QList<int> fileNameIndex;//左侧列表所选文件名的行号，这些文件没有被添加至右侧
                QList<int>list;//用来存储左侧被比较文件的下标
                QList<QTableWidgetItem*> ColumnItems;//用来保存多选获得的tablewidget内容
                QList<int> rowsList;//存放当前左侧列表点击行数的值，比如1，3，5

                ColumnItems=ui->tableWidget->selectedItems();
                for(int i=0;i<ColumnItems.count();i++){
                    rowsList.append(ui->tableWidget->row(ColumnItems.at(i)));
                }
                for(int i=0;i<rowsList.size();i++){
                   // qDebug()<<"before compare,左侧当前选中的行数是："<<rowsList.at(i);
                    QString fileName=ui->tableWidget->item(rowsList.at(i),0)->text();
                    QString filePath=ui->tableWidget->item(rowsList.at(i),2)->text();
                    list.append(saveDifferentFileName(rowsList.at(i),fileName,filePath,fileNameList,filePathList));
                }
                if(list.isEmpty()){//所选文件在右侧列表均已存在
                    QMessageBox msgBox;
                    msgBox.setText("右侧列表已包含左侧选中文件，请重新选择！！！");
                    msgBox.exec();
                }else{//所选文件在右侧列表部分存在
                    //qDebug()<<"list.size="<<list.size();
                    for(int i=0;i<list.size();i++){
                       // qDebug()<<"mywindow-choosenSlot:不同文件名行数是"<<list.at(i);
                        fileNameIndex.append(list.at(i));

                        fileNameList.append(ui->tableWidget->item(list.at(i),0)->text());
                        filePathList.append(ui->tableWidget->item(list.at(i),2)->text());
                    }
                }
               // qDebug()<<"当前右侧文件个数为"<<fileNameList.size()<<",文件为"<<fileNameList;
                for(int i=0;i<fileNameIndex.size();i++){
                    QStringList specifiedColumnItems;
                     qDebug()<<"行号为"<<fileNameIndex.at(i);

                     QString name= ui->tableWidget->item(fileNameIndex.at(i),0)->text();
                     QString size=ui->tableWidget->item(fileNameIndex.at(i),1)->text();
                     QString path=ui->tableWidget->item(fileNameIndex.at(i),2)->text();

                     specifiedColumnItems.append(name);
                     specifiedColumnItems.append(size);
                     specifiedColumnItems.append(path);

                     ui->tableWidget_2->setRowCount(ui->tableWidget_2->rowCount()+1);
                     ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1,0,new QTableWidgetItem(name));
                     ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1,1,new QTableWidgetItem(size));
                     ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1,2,new QTableWidgetItem(path));
                }
            }
        }
        else{
            QMessageBox msgBox;
            msgBox.setText("请选择需要添加的文件");
            msgBox.exec();
        }
     }
}
//将单击选中的文件添加到右侧列表
void mywindow::AddFileInTablewidget2WithClick(){
    QString name=ui->tableWidget->item(ui->tableWidget->currentItem()->row(),0)->text();
    QString size=ui->tableWidget->item(ui->tableWidget->currentItem()->row(),1)->text();
    QString path=ui->tableWidget->item(ui->tableWidget->currentItem()->row(),2)->text();
    specifiedColumnItem<<name<<size<<path;

    if(specifiedColumnItem.size()>0){
        ui->tableWidget_2->setRowCount(ui->tableWidget_2->rowCount()+1);
        ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1,0,new QTableWidgetItem(name));
        ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1,1,new QTableWidgetItem(size));
        ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1,2,new QTableWidgetItem(path));
     }
 }
//将多选选中的文件添加到右侧列表
void mywindow:: AddFileInTablewidget2WithCtrl(){
    QList<QTableWidgetItem*> ColumnItems;//用来保存多选获得的tablewidget内容
    QStringList specifiedColumnItems;//多选行保存对应的指定的列的内容
    ColumnItems=ui->tableWidget->selectedItems();
    int counts=ColumnItems.count();
    for(int i=0;i<counts;i++){
         fileNameList.append(ui->tableWidget->item(i,0)->text());
         filePathList.append(ui->tableWidget->item(i,2)->text());
     }
    for(int i=0;i<counts;i++){
        QList<int> rowsList;//存放当前点击行数的值，比如1，3，5
        int row=ui->tableWidget->row(ColumnItems.at(i));
        //qDebug()<<"当前行数是："<<row;
        rowsList.append(row);
        for(int i=0;i<rowsList.size();i++){
            QString name= ui->tableWidget->item(rowsList.at(i),0)->text();
            QString size=ui->tableWidget->item(rowsList.at(i),1)->text();
            QString path=ui->tableWidget->item(rowsList.at(i),2)->text();

            specifiedColumnItems.append(name);
            specifiedColumnItems.append(size);
            specifiedColumnItems.append(path);

            ui->tableWidget_2->setRowCount(ui->tableWidget_2->rowCount()+1);
            ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1,0,new QTableWidgetItem(name));
            ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1,1,new QTableWidgetItem(size));
            ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1,2,new QTableWidgetItem(path));
        }
    }
}
//返回不同的文件名在tablewidget的行号
QList<int> mywindow::saveDifferentFileName(int index,QString strFileName,QString filePath,QStringList fileList,QStringList pathList){
    QList<int> sameIndex;//表示左侧文件行号列表，但是该文件已在右侧存在
    QList<int> diffIndex;
    bool flag=false;
    //bool flag = false;
    for(int i=0;i<fileList.size();i++){
        if((fileList.at(i)==strFileName) && (pathList.at(i)==filePath)){
           //qDebug()<<"存在相同文件";
           flag=true;
           return sameIndex;
        }
    }
    if(!flag){
        //qDebug()<<"不同文件"<<diffIndex.size();
        diffIndex.append(index);
        return diffIndex;
    }
}
//将tableWidget2选中的文件移除
void mywindow::pushButton_RemoveSlot(){

    int rowCounts=ui->tableWidget_2->rowCount();//判断当前列表是否为空，并给出提示

    if(rowCounts>1){
         bool focus;
        if((ui->tableWidget_2->selectedItems().count())>0)
          focus=true;
        else
            focus=false;
        if(focus){
            if(isCtrlSelected){
                QList<QTableWidgetItem*> ColumnItems;//用来保存多选获得的tablewidget2内容
                ColumnItems=ui->tableWidget_2->selectedItems();
                int rowCounts=ColumnItems.count();
                QList<int> rowsList;//存放当前点击行数的值，比如1，3，5
                for(int i=0;i<rowCounts;i++){
                    int row=ui->tableWidget_2->row(ColumnItems.at(i));
                    rowsList.append(row);  
                }
                for(int i=0;i<rowsList.size();i++){
                    //qDebug()<<"mywindow-pushButton_RemoveSlot"<<rowsList.at(i);
                    ui->tableWidget_2->removeRow(rowsList.at(i));
                    fileNameList.removeAt(rowsList.at(i));
                    filePathList.removeAt(rowsList.at(i));
                }
                //qDebug()<<"mywindow-pushButton_RemoveSlot:选中多个文件移除";
            }
            else{
                QTableWidgetItem *item=ui->tableWidget_2->currentItem();
                fileNameList.removeAt(item->row());
                filePathList.removeAt(item->row());
                ui->tableWidget_2->removeRow(item->row());
                //qDebug()<<"mywindow-pushButton_RemoveSlot:移除文件";
            }
        }
        else{
            QMessageBox msgBox;
            msgBox.setText("请选择需要移除的文件!!!");
            msgBox.exec();
        }
    }else if(rowCounts == 1){
        bool focus=ui->tableWidget_2->isItemSelected(ui->tableWidget_2->currentItem());
        if(focus){
            QMessageBox msgBox;
            msgBox.setText("移除后当前列表无文件");
            msgBox.setInformativeText("您确定执行此操作吗？");
            msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
            int ret=msgBox.exec();
            switch (ret) {
            case QMessageBox::Ok:{
                QTableWidgetItem *item=ui->tableWidget_2->currentItem();
                fileNameList.removeAt(item->row());
                filePathList.removeAt(item->row());
                ui->tableWidget_2->removeRow(item->row());
            }
            case QMessageBox::Cancel:
                break;
            }
        }else{
            QMessageBox msgBox;
            msgBox.setText("请选择需要移除的文件!!!");
            msgBox.exec();
        }
    }else{
        QMessageBox msgBox;
        msgBox.setText("当前列表无文件!!!");
        msgBox.exec();
    }
}
//鼠标双击tablewidget2列表，移除文件
void mywindow::TableWidget2_doubleClickedItem(QTableWidgetItem *item){
    //fileNameList.clear();
    //filePathList.clear();
    //判断当前列表是否为空
    int rowCounts=ui->tableWidget_2->rowCount();
    if(rowCounts>1){
        item=ui->tableWidget_2->currentItem();        
        qDebug()<<"要删除的右侧item是第"<<item->row()<<"行";
        qDebug()<<"filenamelist中有"<<fileNameList.count()<<"行";
        fileNameList.removeAt(item->row());
        filePathList.removeAt(item->row());

        ui->tableWidget_2->removeRow(item->row());


    }else{
        QMessageBox msgBox;
        msgBox.setText("移除后当前列表无文件");
        msgBox.setInformativeText("您确定执行此操作吗？");
        msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
        int ret=msgBox.exec();
        switch (ret) {
        case QMessageBox::Ok:
            fileNameList.removeAt(item->row());
            filePathList.removeAt(item->row());
            ui->tableWidget_2->removeRow(item->row());
        case QMessageBox::Cancel:
            break;
        }
       // qDebug()<<"mywindow-TableWidget2:当前列表无文件";
    }
}
//将tableWidget2中的文件全部移除
void mywindow::pushButton_RemoveAllSlot(){
    fileNameList.clear();
    filePathList.clear();

    int rowCounts=ui->tableWidget_2->rowCount();//判断当前列表是否为空，并给出提示
    if(rowCounts>0){
        QMessageBox msgBox;
        msgBox.setText("移除后当前列表无文件");
        msgBox.setInformativeText("您确定执行此操作吗？");
        msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
        int ret=msgBox.exec();
        switch (ret) {
        case QMessageBox::Ok:
            ui->tableWidget_2->setRowCount(0);
            ui->tableWidget_2->clearContents();
        case QMessageBox::Cancel:
            break;
        }
    }else{
        QMessageBox msgBox;
        msgBox.setText("当前列表无文件！！！");
        msgBox.exec();
    }
    //qDebug()<<"mywindow-pushButton_RemoveAllSlot:全部文件移除";
}
//点击加载文件
void mywindow::pushButton_loadFileSlot(){
   // qDebug()<<"mywindow-pushButton_LoadFile:点击加载文件按钮，加载文件";
    int rowCounts=ui->tableWidget_2->rowCount();
    if(rowCounts>0){
        bool focus = ui->tableWidget_2->isItemSelected(ui->tableWidget_2->currentItem());//判断是否选中一行
        if(focus){
            QString ColumnItem;
            QTableWidgetItem *item=ui->tableWidget_2->currentItem();
            ColumnItem=ui->tableWidget_2->item(item->row(),2)->text();
            QFileInfo fileInfo(ColumnItem);
            bool success=pulseFileTypeDeterminite(fileInfo);
            if(success){
                emit openFile(fileInfo);
                this->close();
            }
        }else{
           QString columnItem;
           columnItem=ui->tableWidget_2->item(0,2)->text();
           QFile fileInfo(columnItem);
           bool success=pulseFileTypeDeterminite(fileInfo);
           if(success){
               emit openFile(fileInfo);
               this->close();
           }
        }
    }else{
        QMessageBox msgBox;
        msgBox.setText("当前列表无文件，请选择需要加载的文件！！！");
        msgBox.exec();
    }
}
//取消加载文件
void mywindow::openMessage_cancelSlots(){
    this->close();
}
//mywindow关闭事件
void mywindow::closeEvent(QCloseEvent *event){
    int rowCounts=ui->tableWidget_2->rowCount();
    if(rowCounts>0){
        bool focus=ui->tableWidget_2->isItemSelected(ui->tableWidget_2->currentItem());
        if(focus){
            fileCounts=ui->tableWidget_2->rowCount();
            pulseFileNumber=ui->tableWidget_2->currentItem()->row();
            pulseFileName=ui->tableWidget_2->item(pulseFileNumber,0)->text();
            pulseFilePath=ui->tableWidget_2->item(pulseFileNumber,2)->text();
             emit closeMyWindow();
            //qDebug()<<"mywindow -closeEvent:current file row:"<<pulseFileNumber<<"pulseFileName:"<<pulseFileName;
        }else{
            pulseFileNumber=0;
            fileCounts=ui->tableWidget_2->rowCount();
            pulseFilePath=ui->tableWidget_2->item(0,2)->text();
             emit closeMyWindow();
           // qDebug()<<"mywindow- closeEvent"<<pulseFilePath;
        }
    }else{
        this->close();
    }
}
//mainwindow界面快速载入文件,获得文件路径
QString mywindow::filePathForQuickLoadFile(int row){
    QString filePath=ui->tableWidget_2->item(row,2)->text();
    return filePath;
    qDebug()<<"mywindow:"<<filePath;
}

//判断文件类型
bool mywindow::pulseFileTypeDeterminite(QFileInfo fileInfo){
    QString fileName=fileInfo.fileName();
    QFile file(fileInfo.filePath());
    /*判断文件名后缀是不是txt
     * 如果文件名中包含'.'，就应该判断分割后的最后一项
     */
    QStringList fileType=fileName.split(".");
    QString suffix=fileType.at(1);

//    QString file_suffix=fileInfo.suffix();
    qDebug()<<"文件后缀名："<<suffix;
    if(suffix!="txt"){
        QMessageBox msgBox;
        msgBox.setText("当前文件类型有误，请重新选择文件！！！");
        msgBox.exec();
        return false;
    }else{
        qDebug()<<"文件类型符合"; 
        if(file.open(QIODevice::ReadOnly)){
            //获取文件的第一行
            QTextStream in(&file);
            QString line=in.readLine();
            QStringList strLine=line.trimmed().split(QRegExp("\\s+"));
            qDebug()<<line<<"数组大小为"<<strLine.size();
            if(strLine.size()==8){
                //符合要求则读取文件
                return true;
            }else{
                //不符合
                QMessageBox msgBox;
                msgBox.setText("当前文件不符合格式，请重新选择文件！！！");
                msgBox.exec();
                return false;
            }
        }else{
            qDebug()<<"文件打开失败";
            return false;
        }
    }
}




