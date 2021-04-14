#include "databaseconnector.h"
#include "QDebug"

DatabaseConnector::DatabaseConnector()
{

}

bool DatabaseConnector::ConnectDatabase(){
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setUserName("root");
    db.setPassword("1111");
    if(db.open()){
        qDebug()<<"database open success!";
        return true;
    }else{
        qDebug()<<"database open error";
        return false;
    }
}

bool DatabaseConnector::creatingDatabase(QString db_name){
    bool success=DatabaseConnector::ConnectDatabase();
    if(success){
        qDebug()<<"connect success";
        QSqlQuery query;
        QString strsql="create database";
        strsql+= " " + db_name;
        bool success = query.exec(strsql);
        if(success){
            qDebug()<<"create database success";
        }else{
            qDebug()<<"create database failed";
        }
        return true;
    }else{
        qDebug()<<"connect failed";
        return false;
    }
}

QVector<QString> DatabaseConnector::getDatabaseName(){
    bool success=DatabaseConnector::ConnectDatabase();
    if(success){
        QSqlQuery query;
        QVector<QString> result;
        query.exec("show databases");
        while(query.next()){
          result.append(query.value(0).toString());
        }
        return result;
    }
}

bool DatabaseConnector::TableCreated(QString db_name){
     QSqlQuery query;
     QString str = "use";
     query.exec(str + " " + db_name);
     bool success=query.exec("create table interpulse(PulseID int, PulseDirection int, "
                             "Frequency double, DTOA int, PulseWidth float, PulseAmp float, "
                             "TOA bigint)");
     if(success){
        qDebug()<<"create table success";
        return true;
     }else{
         qDebug()<<"create table failed";
         return false;
     }
}



