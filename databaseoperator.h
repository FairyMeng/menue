#ifndef DATABASEOPERATOR_H
#define DATABASEOPERATOR_H

#include "databaseconnector.h"
#include "structdefine.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QTime>
#include <QDebug>
#include <QFileInfo>
//解决中文乱码
#pragma execution_character_set("utf-8")
/*
*此类提供对数据库InterPulse表进行增删改查的方法
*/
class DatabaseOperator
{
public:
    DatabaseOperator();
    static void insertData(QFileInfo fileInfo);
    static QVector<long long> findRowTOA();
    static QVector<double> findRowFrequency();
    static QVector<int> findRowDTOA();
    static QVector<double> findRowPulseWidth();
    static QVector<double> findRowPulseAmp();
    static QVector<int> findRowPulseDirection();
    static QVector<InterPulse> findInterPulseData();
};

#endif // DATABASEOPERATOR_H
