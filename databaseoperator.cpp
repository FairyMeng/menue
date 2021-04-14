#include "databaseoperator.h"

DatabaseOperator::DatabaseOperator()
{

}
void DatabaseOperator::insertData(QFileInfo fileInfo){
    QString file_path = fileInfo.filePath();
    QString db_name = fileInfo.fileName().split(".").at(0);
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName(db_name);
    db.setUserName("root");
    db.setPassword("1111");
    if(db.open()){
        qDebug()<<"insert data:database already open success!";
        QSqlQuery query;
        //根据文件路径、文件名拼接Sql语句
        QString sql = "load data local infile ";
        sql = sql + "'" + file_path + "'" + "into table interpulse "
                                            "fields terminated by '\t' lines terminated by '\n' ignore 1 lines";
        QTime useTime;
        useTime.start();
        db.transaction();//开启事务
        bool success = query.exec(sql);
        if(!success){
            QSqlError lastError = query.lastError();
            qDebug()<<lastError.databaseText()<<"Insert data failed";
        }else{
            qDebug()<<"Insert data success";
        }
        db.commit();//提交事务
        qDebug()<<"load PulseFile to database USE TIME:"<<useTime.elapsed()/1000<<"s";
    }
    db.close();
}

QVector<long long> DatabaseOperator::findRowTOA(){
    //DatabaseConnector::ConnectDatabase();//获取MySQL数据库的链接
    QSqlQuery query;
    QVector<long long> RowTOA;
    bool success = query.exec("SELECT TOA FROM dbinfo.interpulse where PulseID % 100 = '0'");
    if(!success){
        QSqlError lastError = query.lastError();
        qDebug()<<lastError.databaseText()<<"查询数据失败";
    }else
        qDebug()<<"查询数据TOA成功";
    while(query.next()){
        RowTOA.append(query.value(0).toLongLong());
    }
    return RowTOA;
}

QVector<double> DatabaseOperator::findRowFrequency(){
    DatabaseConnector::ConnectDatabase();//获取MySQL数据库的链接
    QSqlQuery query;
    QVector<double> RowFrequency;
//    int temp = 100;
//    int avgStart = 0;
//    int avgEnd = 0;
//    QString sql = "SELECT AVG(Frequency) FROM dbinfo.interpulse where PulseID";
//    for(int i = 0; i < 10000; i++){
//        avgStart = temp * i;
//        avgEnd = temp * i + 100;
//        sql += ">=" + avgStart;
//        sql += "AND PulseID <" + avgEnd;
//        query.exec(sql);
//        while(query.next()){
//            RowFrequency.append(query.value(0).toDouble());
//        }
//    }
    bool success = query.exec("SELECT Frequency FROM dbinfo.interpulse");
    if(!success){
        QSqlError lastError = query.lastError();
        qDebug()<<lastError.databaseText()<<"查询数据失败";
    }else
        qDebug()<<"查询数据列Frequency成功";
    while(query.next()){
        RowFrequency.append(query.value(0).toDouble());
    }
    return RowFrequency;
}

QVector<int> DatabaseOperator::findRowDTOA(){
    DatabaseConnector::ConnectDatabase();//获取MySQL数据库的链接
    QSqlQuery query;
    QVector<int> RowDTOA;
    bool success = query.exec("SELECT DTOA FROM dbinfo.interpulse");
    if(!success){
        QSqlError lastError = query.lastError();
        qDebug()<<lastError.databaseText()<<"查询数据失败";
    }else
        qDebug()<<"查询数据列DTOA成功";
    while(query.next()){
        RowDTOA.append(query.value(0).toInt());
    }
    return RowDTOA;
}

QVector<double> DatabaseOperator::findRowPulseWidth(){
    DatabaseConnector::ConnectDatabase();//获取MySQL数据库的链接
    QSqlQuery query;
    QVector<double> RowPulseWidth;
    bool success = query.exec("SELECT PulseWidth FROM dbinfo.interpulse");
    if(!success){
        QSqlError lastError = query.lastError();
        qDebug()<<lastError.databaseText()<<"查询数据失败";
    }else
        qDebug()<<"查询数据列PulseWidth成功";
    while(query.next()){
        RowPulseWidth.append(query.value(0).toDouble());
    }
    return RowPulseWidth;
}

QVector<double> DatabaseOperator::findRowPulseAmp(){
    DatabaseConnector::ConnectDatabase();//获取MySQL数据库的链接
    QSqlQuery query;
    QVector<double> RowPulseAmp;
    bool success = query.exec("SELECT PulseAmp FROM dbinfo.interpulse");
    if(!success){
        QSqlError lastError = query.lastError();
        qDebug()<<lastError.databaseText()<<"查询数据失败";
    }else
        qDebug()<<"查询数据列PulseAmp成功";
    while(query.next()){
        RowPulseAmp.append(query.value(0).toDouble());
    }
    return RowPulseAmp;
}

QVector<int> DatabaseOperator::findRowPulseDirection(){
    DatabaseConnector::ConnectDatabase();//获取MySQL数据库的链接
    QSqlQuery query;
    QVector<int> RowPulseDirection;
    bool success = query.exec("SELECT PulseDirection FROM dbinfo.interpulse");
    if(!success){
        QSqlError lastError = query.lastError();
        qDebug()<<lastError.databaseText()<<"查询数据失败";
    }else
        qDebug()<<"查询数据列PulseDirection成功";
    while(query.next()){
        RowPulseDirection.append(query.value(0).toInt());
    }
    return RowPulseDirection;
}

QVector<InterPulse> DatabaseOperator::findInterPulseData(){
    DatabaseConnector::ConnectDatabase();//获取MySQL数据库的链接
    QSqlQuery query;
    QVector<InterPulse> millionData;
    QTime time;
    time.start();
    bool success = query.exec("SELECT * FROM dbinfo.interpulse");
    if(!success){
        QSqlError lastError = query.lastError();
        qDebug()<<lastError.databaseText()<<"查询数据失败";
    }else
        qDebug()<<"查询数据表interpulse成功";
    InterPulse pulse;
    while(query.next()){
        pulse.pulse_id = query.value(0).toInt();
        //pulse.pulse_direction = query.value(1).toInt();
        pulse.frequency = query.value(2).toDouble();
        pulse.dtoa = query.value(3).toInt();
        pulse.pulse_width = query.value(4).toDouble();
        pulse.pulse_amp = query.value(5).toDouble();
        pulse.toa = 0;
        millionData.append(pulse);
    }
    qDebug()<<"select all data time :"<<time.elapsed();
    return millionData;
}



















