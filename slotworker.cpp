#include "slotworker.h"
#include <QDebug>
#include <QThread>
#include <QSplashScreen>
#include "databaseconnector.h"
#include "pulsefilemanager.h"
SlotWorker::SlotWorker():QObject(){
    qRegisterMetaType<QFileInfo>("QFileInfo");
    //qRegisterMetaType<QFile>("QFile");
}
void SlotWorker::Slot_GetDatabasesName(QFileInfo fileInfo){
    qDebug()<<"from Slot_GetDatabaseName thread id  is"<<QThread::currentThreadId();
    QString PulseFile_Name=fileInfo.fileName().split(".").at(0);//获得脉冲文件名字不带后缀
    QVector<QString> databases_Name=DatabaseConnector::getDatabaseName();
    for(int i = 0; i < databases_Name.size(); i++){
            qDebug()<<"the databases are:"<<databases_Name.at(i);
        }
    bool flag = false;
       for(int i = 0; i < databases_Name.size(); i++){
           if(databases_Name[i] ==PulseFile_Name.toLower()){
               flag = true;
           }
       }
       if(flag){
           qDebug()<<"already have database same with file_name";
               //PulseFileManager::LoadInterPulseToDatabase(fileInfo);
       }else{
           DatabaseConnector::creatingDatabase(PulseFile_Name);
           DatabaseConnector::TableCreated(PulseFile_Name);
          // PulseFileManager::LoadInterPulseToDatabase(fileInfo);
       }
}
