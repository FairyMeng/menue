#include "pulsefilemanager.h"
#include <QTime>

PulseFileManager::PulseFileManager()
{

}

void PulseFileManager::LoadInterPulseToDatabase(QFileInfo file_info){
    //判断文本文件名，如不存在同名数据库，则建立同名数据库，interpulse数据表，将文本数据导入数据库
//    qDebug()<<file->fileName();
//    QString file_path = file_info->filePath();
//    QString file_name = file_info->fileName();
    DatabaseOperator::insertData(file_info);
}
/*加载脉间文件:TestData_InterPulse到内存结构体数组*/
QVector<InterPulse> PulseFileManager::LoadInterPulseFileDataIntoMemVector(QFile *file){
    QTime time;
    time.start();
    qDebug()<<"the file's size is"<<file->size();
    InterPulse inter_pulse;
    QVector<InterPulse> vector;

    if(file->open(QIODevice::ReadOnly)){
        qDebug()<<"PulseFileManager-LoadInterPulseFileDataIntoMemVector:文件打开成功";
        //in.seek(0);
        QStringList list1=QString(file->readAll()).trimmed().split('\n');
        QStringList list2;
        QStringList list3;
        qDebug()<<"list1.size() = "<<list1.size();
        for(int i=0;i<list1.size();i++){
            list2 = list1[i].trimmed().split('\t');//一行文件有2个tab,所以list2的容量为2

            QString strOfList2=list2.at(list2.size()-1);
            //qDebug()<<list2.size()<<list2.at(list2.size()-1);
            list3=strOfList2.trimmed().split(QRegExp("\\s+")); //把所有的多余的空格转为一个空格,并进行分割

            //保存为结构体数组
            inter_pulse.pulse_id=list2.at(0).toInt();//此处为list2.at(0)，即id

            inter_pulse.frequency=list3.at(1).toDouble();
            inter_pulse.dtoa=list3.at(6).toDouble();
            inter_pulse.pulse_width=list3.at(2).toDouble();
            inter_pulse.pulse_amp=list3.at(3).toDouble();
            inter_pulse.toa=list3.at(0).toDouble();
            inter_pulse.midFrequencyInitialPoint=list3.at(4).toInt();
            inter_pulse.midFrequencyEndPoint=list3.at(5).toInt();
            vector.append(inter_pulse);
        }

    }else
         qDebug()<<"PulseFileManager-LoadInterPulseFileDataIntoMemVector:文件打开失败";

    qDebug()<<"vector.size = "<<vector.size();
    qDebug()<<"load file to Memory vector use time is:"<<time.elapsed() / 1000<<"s";
    return vector;
    /*读入第一次给定数据millonPDW
    QByteArray bytes;
    bytes = file->read(file->bytesAvailable());
    QString str = QString(bytes);
    //qDebug()<<"read file for QString  ok!!";
    //存为QStringlist
    QStringList list1 = str.trimmed().split('\n');
    QStringList list2;
    //qDebug()<<"read file for QStringlist ok!!";
    for(int i = 1; i < list1.size(); i++){
        list2 = list1[i].trimmed().split('\t');
       // qDebug()<<"the file is:"<<list2;
        inter_pulse.pulse_id = list2.at(0).toInt();
        inter_pulse.pulse_direction = list2.at(1).toInt();
        inter_pulse.frequency = list2.at(2).toDouble();
        inter_pulse.dtoa = list2.at(3).toInt();
        inter_pulse.pulse_width = list2.at(4).toDouble();
        inter_pulse.pulse_amp = list2.at(5).toDouble();
        inter_pulse.toa = list2.at(6).toDouble();
        vector.append(inter_pulse);
    }*/
}

/*加载脉内文件：TestData_InternalPulse*/
QVector<int> PulseFileManager::LoadInternalPulseFileDataIntoMemVector(QFile *file){
    QVector<int> vector;
    QTime time;
    time.start();
    if(file->open(QIODevice::ReadOnly)){
        //qDebug()<<"PulseFileManager-LoadInternalPulseFileDataIntoMemVector:文件打开成功";
        qDebug()<<file->readAll().size();
            QStringList list1=QString(file->readAll()).trimmed().split('\n');
            for(int i=0;i<list1.size();i++){
                QStringList str1=list1.at(i).split('\r');
                vector.append(str1.at(0).toInt());
            }
    }
    qDebug()<<"PulseFileManager-LoadInternalPulseFileDataIntoMemVector：将脉内文件保存为数组，数组大小为："<<vector.size();
//    for(int i=1000;i<2000;i++){
//        qDebug()<<vector.at(i);
//    }
    //file->close();
    qDebug()<<"PulseFileManager-LoadInternalPulseFileDataIntoMemVector:加载脉内文件耗时："<<time.elapsed()/1000<<"s";
    return vector;
}










